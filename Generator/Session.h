#ifndef GENERATOR_SESSION_H
#define GENERATOR_SESSION_H

#include "../kontr.h"
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
    std::ofstream out;
public:
    using typename ::kontr::Session::Interface<T>::TMasterTests;
    using typename ::kontr::Session::Interface<T>::TPost;

    Session(const char* scripts_dir, const char* files_dir,
            TMasterTests nanecisto, TMasterTests naostro,
            TPost post = nullptr) :
        ::kontr::Session::Data<T>(scripts_dir, files_dir, nanecisto, naostro, post),
        out(std::string(scripts_dir) + "/" + T::instance().storage.nextFileName + ".pl")
    {
        if (!out.good()) {
            T::instance().report.create(::kontr::Report::ERROR, "Could not create output file");
        }
    }

    virtual void pre_test() {
        using Variable = const typename T::VariableDelegator&;
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
            Variable name = (strName + ".pl").c_str();
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
                Variable name = (strName + ".pl").c_str();
                out << "\t\t$session->register_master(" << name << ");" << std::endl;
                instance->execute();
                ++ s.names.currentMasterIndex;
            }
            out << "\t}" << std::endl;
        }
        out << "}" << std::endl;
    }

    virtual void post_test() {
    }

    using typename ::kontr::Session::Interface<T>::Variable;

    //Getter
    virtual Variable has_tag(Variable tag) override {
        std::stringstream ss;
        ss << tag;
        return T::VariableDelegator::__create(
                    ::kontr::Generator::Variable<T>(::kontr::Variable::DataType::Bool, "$session->has_tag(" + ss.str() + ")")
                    );
    }

    //Getter
    virtual Variable run_type() override {
        return T::VariableDelegator::__create(
                        ::kontr::Generator::Variable<T>(::kontr::Variable::DataType::String, "$session->run_type")
                    );
    }

    //This methods prints to out in instance
    virtual void add_summary(Variable message) override {
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << "$session->add_summary(" << message << ");" << std::endl;
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
