#ifndef GENERATOR_EXEC_H
#define GENERATOR_EXEC_H

#include "../kontr.h"

namespace kontr {
namespace Generator {

template<typename T>
class Exec : public ::kontr::Exec::Interface<T> {
    std::string prefix;
public:
    Exec(std::string prefix) : prefix(prefix) {}

    using typename ::kontr::Exec::Interface<T>::Variable;

#define GETTER(NAME, TYPE) virtual Variable NAME() override {\
        return T::VariableDelegator::__create(::kontr::Generator::Variable<T>(::kontr::Variable::DataType::TYPE, prefix + "->" #NAME));\
    }
    GETTER(cmd, String)
    GETTER(output_path, String)
    GETTER(stdin_path, String)
    GETTER(stdout_path, String)
    GETTER(stderr_path, String)
    GETTER(work_path, String)
    GETTER(success, Bool)
    GETTER(exit_type, Int)
#undef GETTER

    virtual void log_stdout() override {
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << prefix << "->log_stdout();" << std::endl;
    }

    virtual void log_stderr() override {
        std::ostream& out = *(T::instance().storage.out_ptr);
        out << prefix << "->log_stderr();" << std::endl;
    }
};

} //Generator
} //kontr
#endif // EXEC_H
