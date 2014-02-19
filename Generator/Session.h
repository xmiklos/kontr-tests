#ifndef GENERATOR_SESSION_H
#define GENERATOR_SESSION_H

#include "../kontr-internal.h"
#include <tuple>

namespace kontr {
namespace Generator {

template<typename T>
class Session : public ::kontr::Session::Data<T> {
    using ::kontr::Session::Data<T>::scripts_dir;
    using ::kontr::Session::Data<T>::files_dir;
    using ::kontr::Session::Data<T>::nanecisto;
    using ::kontr::Session::Data<T>::naostro;
    using ::kontr::Session::Data<T>::post;
    using ::kontr::Session::Data<T>::valgrind;
    using ::kontr::Session::Data<T>::bonus;
    std::ofstream out;
public:
    using typename ::kontr::Session::Interface<T>::TMasterTests;
    using typename ::kontr::Session::Interface<T>::TPost;

    Session(const char* scripts_dir, const char* files_dir,
            TMasterTests nanecisto, TMasterTests naostro,
            TPost post) :
        ::kontr::Session::Data<T>(scripts_dir, files_dir, nanecisto, naostro, post),
        out(std::string(scripts_dir) + "/" + T::instance().storage.nextFileName)
    {
        if (!out.good()) {
            T::instance().report.create(::kontr::Report::ERROR, "Could not create output file");
        }
    }

    Session(const char* scripts_dir, const char* files_dir,
            TMasterTests nanecisto, TMasterTests naostro,
            bool valgrind, bool bonus) :
        ::kontr::Session::Data<T>(scripts_dir, files_dir, nanecisto, naostro, valgrind, bonus),
        out(std::string(scripts_dir) + "/" + T::instance().storage.nextFileName)
    {
        if (!out.good()) {
            T::instance().report.create(::kontr::Report::ERROR, "Could not create output file");
        }
    }

    virtual void pre_test() {
        using Variable = const typename ::kontr::Variable::Delegator<T>&;
        auto &s = T::instance().storage; //::kontr::Generator::Storage
        s.names.currentMasterIndex = 0;

        out << "sub pre_test {" << std::endl;
        for (auto i : nanecisto) {
            auto names = s.names.masterTests[s.names.currentMasterIndex]; //tuple<string, string>
            const std::string& strName = std::get<1>(names);
            s.nextFileName = strName.c_str();
            typename T::MasterDelegatorInstance instance = T::MasterTestInstance(i);
            if (instance->__getClassName() != std::get<0>(names)) {
                T::instance().report.create(Report::ERROR, "Invalid master test class name");
            }
            Variable name = strName.c_str();
            out << "\t$session->register_master(" << name << ");" << std::endl;
            instance->execute();
            ++ s.names.currentMasterIndex;
        }

        if (naostro.size()) {
            out << "\tif($session->run_type eq 'teacher') {" << std::endl;
            for (auto i : naostro) {
                auto names = s.names.masterTests[s.names.currentMasterIndex]; //tuple<string, string>
                const std::string& strName = std::get<1>(names);
                s.nextFileName = strName.c_str();
                typename T::MasterDelegatorInstance instance = T::MasterTestInstance(i);
                if (instance->__getClassName() != std::get<0>(names)) {
                    T::instance().report.create(Report::ERROR, "Invalid master test class name");
                }
                Variable name = strName.c_str();
                out << "\t\t$session->register_master(" << name << ");" << std::endl;
                instance->execute();
                ++ s.names.currentMasterIndex;
            }
            out << "\t}" << std::endl;
        }
        out << "}" << std::endl;
    }

    virtual void post_test() {
        out << "sub post_test {" << std::endl;
        T::instance().storage.out_ptr = &out;
        if (post != nullptr) {
            T::instance().storage.addIndent();
            post();
            T::instance().storage.removeIndent();
        }
        else {
            T::instance().storage.addIndent();
            IF(run_type() == "student", {
                IF(has_tag("nanecisto"), {
                   add_summary("* test nanecisto neprosel");
                }, {
                   add_summary("* test nanecisto prosel");
                });
            },
            {
                   VAR(points, get_points("points"));
                   IF (has_tag("nanecisto") || has_tag("naostro"),
                   {
                       add_summary("* v testu byla nalezena chyba\n");
                       add_summary("* pocet bodu je: " + points + "\n");
                   },
                   {
                       add_summary("* test prosel kompletne spravne\n");
                       add_summary("* pocet bodu za funcionalitu je: " + points + "\n");
                       if (bonus) {
                           VAR(bonus, get_points("bonus"));
                           add_summary("* pocet bodu za bonus je: " + bonus + "\n");
                       }
                   });
                   if (valgrind) {
                       IF(has_tag("valgrind"),
                        { add_summary("NEPROSLA kontrola Valgrindem, -1 bod\n"); },
                        { add_summary("Prosla kontrola Valgrindem.\n"); }
                       );
                   }
            });
            T::instance().storage.removeIndent();
        }
        out << "}" << std::endl;
    }

    using typename ::kontr::Session::Interface<T>::Variable;

    //Getter
    virtual Variable has_tag(Variable tag) override {
        std::stringstream ss;
        ss << tag;
        return ::kontr::Variable::Delegator<T>::__create(
                    ::kontr::Generator::Variable<T>(::kontr::Variable::DataType::Bool, "$session->has_tag(" + ss.str() + ")")
                    );
    }

    //Getter
    virtual Variable run_type() override {
        return ::kontr::Variable::Delegator<T>::__create(
                        ::kontr::Generator::Variable<T>(::kontr::Variable::DataType::String, "$session->run_type")
                    );
    }

    //This methods prints to out in instance
    virtual void add_summary(Variable message) override {
        std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent; 
        out << indent << "$session->add_summary(" << message << ");" << std::endl;
    }

    //Getter
    virtual Variable get_points(Variable points) override {
        std::stringstream ss;
        ss << points;
        return ::kontr::Variable::Delegator<T>::__create(
                    ::kontr::Generator::Variable<T>(::kontr::Variable::DataType::Bool, "$session->get_points(" + ss.str() + ")")
                    );
    }

    virtual Variable available_file(const char* name) override {
        std::stringstream ss;
        std::set<char> quote = {'\\', '/', '.', '+', '*', '^', '$', '?', '(', ')', '[', ']', '{', '}', '|'};
        for(unsigned i = 0; name[i] != 0; ++i) {
            if (quote.find(name[i]) != quote.end()) {
                ss << '\\';
            }
            ss << name[i];
        }
        return ::kontr::Variable::Delegator<T>::__create(
                    ::kontr::Generator::Variable<T>(::kontr::Variable::DataType::String, "$session->available_file( sub { /" + ss.str() + "/ } )")
                    );
    }

    virtual void set_value(Variable name, Variable value) override {
        std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent;
        out << indent << "$session->set_value(" << name << ", " << value << ");" << std::endl;
    }

    virtual Variable get_value(Variable name) override {
        std::stringstream ss;
        ss << name;
        return ::kontr::Variable::Delegator<T>::__create(
                    ::kontr::Generator::Variable<T>(::kontr::Variable::DataType::String, "$session->get_value(" + ss.str() + ")")
                    );
    }

    virtual Variable got_value(Variable name) override {
        std::stringstream ss;
        ss << name;
        return ::kontr::Variable::Delegator<T>::__create(
                    ::kontr::Generator::Variable<T>(::kontr::Variable::DataType::Bool, "$session->got_value(" + ss.str() + ")")
                    );
    }

    virtual void remove_value(Variable name) override {
        std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent;
        out << indent << "$session->remove_value(" << name << ");" << std::endl;
    }
};

#if 0
template<typename T>
void SessionGenerator<T>::configure(const char *script_dir, const char *files_dir, TMasterTests nanecisto, TMasterTests naostro, TPost post)
{
    this->script_dir = script_dir;
    this->files_dir = files_dir;
    this->nanecisto = nanecisto;
    this->naostro = naostro;
    this->post = post;
}
#endif

} //Generator
} //kontr
#endif // GENERATOR_SESSION_H
