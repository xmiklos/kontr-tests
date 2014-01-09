#ifndef EXEC_H
#define EXEC_H

#include "kontr-internal.h"

namespace kontr {
namespace Exec {

namespace Execution {

template<typename T>
class Interface {
public:
    using Variable = typename ::kontr::Variable::Delegator<T>;

    /**
      Return cmd
     * @brief cmd
     * @return Variable(string)
     */
    virtual Variable cmd() = 0;

    virtual Variable output_path() = 0;
    virtual Variable stdin_path() = 0;
    virtual Variable stdout_path() = 0;
    virtual Variable stderr_path() = 0;
    virtual Variable work_path() = 0;

    //limit_runtime, limit_output

    virtual Variable success() = 0; //bool
    virtual Variable exit_type() = 0; //string
    virtual Variable exit_value() = 0; //int

    virtual void log_stdout() = 0;
    virtual void log_stderr() = 0;
};

template<typename T>
class Empty : public Interface<T> {
public:
    using typename Interface<T>::Variable;

    virtual Variable cmd() override { return ""; }
    virtual Variable output_path() override { return ""; }
    virtual Variable stdin_path() override { return ""; }
    virtual Variable stdout_path() override { return ""; }
    virtual Variable stderr_path() override { return ""; }
    virtual Variable work_path() override { return ""; }

    virtual Variable success() override { return false; }
    virtual Variable exit_type() override { return ""; }
    virtual Variable exit_value() override { return 0; }

    virtual void log_stdout() override {}
    virtual void log_stderr() override {}
};

template<typename T>
class Delegator : public Interface<T> {
    typename T::Exec delegate;
public:
    Delegator(typename T::Exec&& instance) : delegate(instance) {}

    using typename Interface<T>::Variable;

    virtual Variable cmd() override { return delegate.cmd(); }
    virtual Variable output_path() override { return delegate.output_path(); }
    virtual Variable stdin_path() override { return delegate.stdin_path(); }
    virtual Variable stdout_path() override { return delegate.stdout_path(); }
    virtual Variable stderr_path() override { return delegate.stderr_path(); }
    virtual Variable work_path() override { return delegate.work_path(); }

    virtual Variable success() override { return delegate.success(); }
    virtual Variable exit_value() override { return delegate.exit_value(); }
    virtual Variable exit_type() override { return delegate.exit_type(); }

    virtual void log_stdout() override {
        delegate.log_stdout();
    }
    virtual void log_stderr() override {
        delegate.log_stderr();
    }
};

} //Execution

namespace Compilation {

template<typename T>
class Interface : public ::kontr::Exec::Execution::Interface<T> {
public:
    using typename ::kontr::Exec::Execution::Interface<T>::Variable;

    /**
      Result of compilation (string = clean, warning, errors)
     * @brief result
     * @return
     */
    virtual Variable result() = 0;
};

template<typename T>
class Empty : public ::kontr::Exec::Execution::Empty<T>, public Interface<T> {
public:
    using typename ::kontr::Exec::Execution::Interface<T>::Variable;
    /* When compiler comes to senses
    using ::kontr::Exec::Execution::Empty<T>::cmd;
    using ::kontr::Exec::Execution::Empty<T>::stdin_path;
    using ::kontr::Exec::Execution::Empty<T>::stdout_path;
    using ::kontr::Exec::Execution::Empty<T>::stderr_path;
    using ::kontr::Exec::Execution::Empty<T>::work_path;
    using ::kontr::Exec::Execution::Empty<T>::success;
    using ::kontr::Exec::Execution::Empty<T>::exit_value;
    using ::kontr::Exec::Execution::Empty<T>::exit_type;
    using ::kontr::Exec::Execution::Empty<T>::log_stdout;
    using ::kontr::Exec::Execution::Empty<T>::log_stderr;
    */
    virtual Variable cmd() override { return ""; }
    virtual Variable output_path() override { return ""; }
    virtual Variable stdin_path() override { return ""; }
    virtual Variable stdout_path() override { return ""; }
    virtual Variable stderr_path() override { return ""; }
    virtual Variable work_path() override { return ""; }

    virtual Variable success() override { return false; }
    virtual Variable exit_type() override { return ""; }
    virtual Variable exit_value() override { return 0; }

    virtual void log_stdout() override {}
    virtual void log_stderr() override {}

    /**
      Result of compilation (string = clean, warning, errors)
     * @brief result
     * @return
     */
    virtual Variable result() { return ""; }
};

template<typename T>
class Delegator : public Interface<T> {
    typename T::Compile delegate;
public:
    Delegator(typename T::Compile&& instance) : delegate(instance) {}

    using typename Interface<T>::Variable;

    virtual Variable cmd() override { return delegate.cmd(); }
    virtual Variable output_path() override { return delegate.output_path(); }
    virtual Variable stdin_path() override { return delegate.stdin_path(); }
    virtual Variable stdout_path() override { return delegate.stdout_path(); }
    virtual Variable stderr_path() override { return delegate.stderr_path(); }
    virtual Variable work_path() override { return delegate.work_path(); }

    virtual Variable success() override { return delegate.success(); }
    virtual Variable exit_value() override { return delegate.exit_value(); }
    virtual Variable exit_type() override { return delegate.exit_type(); }

    virtual void log_stdout() override {
        delegate.log_stdout();
    }
    virtual void log_stderr() override {
        delegate.log_stderr();
    }

    virtual Variable result() override { return delegate.result(); }
};

} //Compilation

template<typename T>
struct Empty {
    using Execution = ::kontr::Exec::Execution::Empty<T>;
    using Compilation = ::kontr::Exec::Compilation::Empty<T>;
};

} //Exec
} //kontr
#endif // EXEC_H
