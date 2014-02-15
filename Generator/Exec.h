#ifndef GENERATOR_EXEC_H
#define GENERATOR_EXEC_H

#include "../kontr-internal.h"

namespace kontr {
namespace Generator {

template<typename T>
class Execution : public ::kontr::Exec::Execution::Interface<T> {
    std::string prefix;
public:
    Execution(std::string prefix) : prefix(prefix) {}

    using typename ::kontr::Exec::Execution::Interface<T>::Variable;

#define GETTER(NAME, TYPE) virtual Variable NAME() override {\
        return ::kontr::Variable::Delegator<T>::__create(::kontr::Generator::Variable<T>(::kontr::Variable::DataType::TYPE, prefix + "->" #NAME));\
    }
    GETTER(cmd, String)
    GETTER(output_path, String)
    GETTER(stdin_path, String)
    GETTER(stdout_path, String)
    GETTER(stderr_path, String)
    GETTER(work_path, String)
    GETTER(success, Bool)
    GETTER(exit_type, String)
    GETTER(exit_value, Int)


    virtual void log_stdout(Variable type = "both") override {
        std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent;
        out << indent << prefix << "->log_stdout(" << type << ");" << std::endl;
    }

    virtual void log_stderr(Variable type = "both") override {
        std::ostream& out = *(T::instance().storage.out_ptr);
        std::string& indent = T::instance().storage.indent;
        out << indent << prefix << "->log_stderr(" << type << ");" << std::endl;
    }
};

template<typename T>
class Compilation : public ::kontr::Exec::Compilation::Interface<T> {
    std::string prefix;
    Execution<T> exec;
public:
    Compilation(std::string prefix) : prefix(prefix), exec(prefix) {}

    using typename ::kontr::Exec::Execution::Interface<T>::Variable;

    /* //When compilator comes to senses, this should work (works in Empty, but not here)
    using Execution<T>::cmd;
    using Execution<T>::output_path;
    using Execution<T>::stdin_path;
    using Execution<T>::stdout_path;
    using Execution<T>::stderr_path;
    using Execution<T>::work_path;
    using Execution<T>::success;
    using Execution<T>::exit_value;
    using Execution<T>::exit_type;
    using Execution<T>::log_stdout;
    using Execution<T>::log_stderr;
    */
#define DELEGATE(NAME) virtual Variable NAME() override { return exec.NAME(); }
    DELEGATE(cmd)
    DELEGATE(output_path)
    DELEGATE(stdin_path)
    DELEGATE(stdout_path)
    DELEGATE(stderr_path)
    DELEGATE(work_path)
    DELEGATE(success)
    DELEGATE(exit_value)
    DELEGATE(exit_type)
#undef DELEGATE
    virtual void log_stdout(Variable type = "both") override { exec.log_stdout(type); }
    virtual void log_stderr(Variable type = "both") override { exec.log_stderr(type); }

    GETTER(result, String)
};

template<typename T>
class Valgrind: public ::kontr::Exec::Valgrind::Interface<T> {
    std::string prefix;
    Execution<T> exec;
public:
    Valgrind(std::string prefix) : prefix(prefix), exec(prefix) {}

    using typename ::kontr::Exec::Execution::Interface<T>::Variable;

    /* //When compilator comes to senses, this should work (works in Empty, but not here)
    using Execution<T>::cmd;
    using Execution<T>::output_path;
    using Execution<T>::stdin_path;
    using Execution<T>::stdout_path;
    using Execution<T>::stderr_path;
    using Execution<T>::work_path;
    using Execution<T>::success;
    using Execution<T>::exit_value;
    using Execution<T>::exit_type;
    using Execution<T>::log_stdout;
    using Execution<T>::log_stderr;
    */
#define DELEGATE(NAME) virtual Variable NAME() override { return exec.NAME(); }
    DELEGATE(cmd)
    DELEGATE(output_path)
    DELEGATE(stdin_path)
    DELEGATE(stdout_path)
    DELEGATE(stderr_path)
    DELEGATE(work_path)
    DELEGATE(success)
    DELEGATE(exit_value)
    DELEGATE(exit_type)
#undef DELEGATE
    virtual void log_stdout(Variable type = "both") override { exec.log_stdout(type); }
    virtual void log_stderr(Variable type = "both") override { exec.log_stderr(type); }

    GETTER(grind_errors, Bool)
    GETTER(grind_data, String)
    GETTER(grind_user, String)
    GETTER(grind_path, String)
#undef GETTER
};

template<typename T>
struct Exec {
    using Execution = ::kontr::Generator::Execution<T>;
    using Compilation = ::kontr::Generator::Compilation<T>;
    using Valgrind = ::kontr::Generator::Valgrind<T>;
};

} //Generator
} //kontr
#endif // EXEC_H
