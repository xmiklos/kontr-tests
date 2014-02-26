#ifndef UNITTEST_H
#define UNITTEST_H

#include "kontr-internal.h"

namespace kontr {
namespace UnitTest {

/// Pure interface
template<typename T>
class Interface {
public:
    /// Parameter types
    using Variable = typename ::kontr::Variable::Delegator<T>;
    using Compilation = typename ::kontr::Exec::Compilation::Delegator<T>;
    using Execution = typename ::kontr::Exec::Execution::Delegator<T>;
    using Valgrind = typename ::kontr::Exec::Valgrind::Delegator<T>;

    virtual ~Interface() {}

    /**
     * Set name to the unit test
     * @brief name
     * @param name
     */
    virtual void name(const char* name) = 0;

    /**
     * Add a file from _files_
     * @brief stage_file
     * @param filename
     */
    virtual void stage_file(Variable filename) = 0;
    /**
     * Add a compiled file from _file_
     * @brief stage_compiled_file
     * @param filename
     */
    virtual void stage_compiled_file(Variable filename) = 0;

    /**
     * Add a file from student repository
     * @brief stage_student_file
     * @param filename
     */
    virtual void stage_student_file(Variable filename) = 0;

    /**
     * Add a compiled file from student repository
     * @brief stage_compiled_student_file
     * @param filename
     */
    virtual void stage_compiled_student_file(Variable filename) = 0;

    virtual void add_tag(Variable tag) = 0;
    virtual void add_points(Variable name, Variable points) = 0;
    virtual Variable work_path() = 0; //string
    virtual Variable file_path() = 0; //string

    virtual Compilation* compilation() = 0;
    virtual void extra_compiler_flags(Variable flags) = 0;
    virtual Variable compilation_log_errors() = 0; //bool
    virtual Execution* execution() = 0;
    virtual Execution* analysis() = 0;
    virtual Execution* difference() = 0;
    virtual Valgrind* valgrind() = 0;

    virtual void compile() = 0;
    //TODO - not an array?
    virtual void run(Variable input, std::vector<Variable> args = {}) = 0;
    virtual void run_grind(Variable input, std::vector<Variable> args = {}) = 0;
    virtual void run_split(Variable input, Variable arg_str) = 0;
    virtual void run_grind_split(Variable input, Variable arg_str) = 0;

    virtual void diff_stdout(Variable mode, Variable file) = 0;
    virtual void diff_stderr(Variable mode, Variable file) = 0;
    virtual void diff_generic(Variable mode, Variable file1, Variable file2) = 0;

    virtual void analyze_stdout(Variable desc, Variable cmd) = 0;
    virtual void analyze_stderr(Variable desc, Variable cmd) = 0;
    virtual void analyze(Variable desc, Variable input, Variable cmd) = 0;

    virtual void log(Variable text, Variable type = "both") = 0;
    virtual void log_file(Variable filename, Variable type = "both") = 0;
    virtual void add_attachment(Variable filename, Variable type = "both") = 0;

    //UNUSED methods:
    //virtual void log_comit() = 0;
    //virtual void log_purge() = 0;
    //virtual void log_mode(Variable comit) = 0;

    virtual void log_run_fail(Variable message) = 0;
    virtual void log_tag(Variable tag, Variable text, Variable type = "both") = 0;
    virtual void log_valgrind(Variable tag, Variable text) = 0;
    virtual void subtest(Variable name) = 0;    
};

/// Empty class (all virtual methods implemented as empty)
template <typename T>
class Empty : public Interface<T> {
public:
    using typename Interface<T>::Execution;
    using typename Interface<T>::Compilation;
    using typename Interface<T>::Valgrind;
    using typename Interface<T>::Variable;

    virtual void name(const char * name) override { kontr::unused(name); }
    virtual void stage_compiled_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_compiled_student_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_student_file(Variable filename) override { kontr::unused(filename); }

    virtual void add_tag(Variable tag) override { kontr::unused(tag); }
    virtual void add_points(Variable name, Variable points) override { kontr::unused(name, points); }
    virtual Variable work_path() override { return ""; }
    virtual Variable file_path() override { return ""; }
    virtual Compilation* compilation() override { return nullptr; }
    virtual void extra_compiler_flags(Variable flags) override { kontr::unused(flags); }
    virtual Variable compilation_log_errors() override { return false; }
    virtual Execution* execution() override { return nullptr; }
    virtual Execution* analysis() override { return nullptr; }
    virtual Execution* difference() override { return nullptr; }
    virtual Valgrind* valgrind() override { return nullptr; }

    virtual void compile() override {}

    virtual void run(Variable input, std::vector<Variable> args = {}) override { kontr::unused(input, args); }
    virtual void run_grind(Variable input, std::vector<Variable> args = {}) override { kontr::unused(input, args); }
    virtual void run_split(Variable input, Variable arg_str) override { kontr::unused(input, arg_str); }
    virtual void run_grind_split(Variable input, Variable arg_str) override { kontr::unused(input, arg_str); }

    virtual void diff_stdout(Variable mode, Variable file) override { kontr::unused(mode, file); }
    virtual void diff_stderr(Variable mode, Variable file) override { kontr::unused(mode, file); }
    virtual void diff_generic(Variable mode, Variable file1, Variable file2) override { kontr::unused(mode, file1, file2); }

    virtual void analyze_stdout(Variable desc, Variable cmd) override { kontr::unused(desc, cmd); }
    virtual void analyze_stderr(Variable desc, Variable cmd) override { kontr::unused(desc, cmd); }
    virtual void analyze(Variable desc, Variable input, Variable cmd) override { kontr::unused(desc, input, cmd); }

    virtual void log(Variable text, Variable type = "both") override { kontr::unused(text, type); }
    virtual void log_file(Variable filename, Variable type = "both") override { kontr::unused(filename, type); }
    virtual void add_attachment(Variable filename, Variable type = "both") override { kontr::unused(filename, type); }

    virtual void log_run_fail(Variable message) override { kontr::unused(message); }
    virtual void log_tag(Variable tag, Variable text, Variable type = "both") override { kontr::unused(tag, text, type); }
    virtual void log_valgrind(Variable tag, Variable text) override { kontr::unused(tag, text); }
    virtual void subtest(Variable name) override { kontr::unused(name); }
};

/// Delegator class
template <typename T>
class Delegator : public Interface<T>
{
    typename T::Unit delegate;
    const char* className;
protected:
    Delegator(const char* className) :
        Interface<T>(), delegate(), className(className) {}

public:
    /// Type of a function returning unique pointer to this delegator type
    using Function = void (*)(std::unique_ptr<Delegator>&);

    using Variable = typename Interface<T>::Variable;
    using typename Interface<T>::Execution;
    using typename Interface<T>::Compilation;
    using typename Interface<T>::Valgrind;

    virtual void name(const char* name) {
        delegate.name(name);
    }

    virtual void stage_file(Variable filename) {
        delegate.stage_file(filename);
    }

    virtual void stage_compiled_file(Variable filename) {
        delegate.stage_compiled_file(filename);
    }

    virtual void stage_student_file(Variable filename) {
        delegate.stage_student_file(filename);
    }

    virtual void stage_compiled_student_file(Variable filename) {
        delegate.stage_compiled_student_file(filename);
    }

    virtual void add_tag(Variable tag) override { delegate.add_tag(tag); }
    virtual void add_points(Variable name, Variable points) override { delegate.add_points(name, points); }
    virtual Variable work_path() override { return delegate.work_path(); }
    virtual Variable file_path() override { return delegate.file_path(); }
    virtual Compilation* compilation() override { return delegate.compilation(); }
    virtual void extra_compiler_flags(Variable flags) override { delegate.extra_compiler_flags(flags); }
    virtual Variable compilation_log_errors() override { return delegate.compilation_log_errors(); }
    virtual Execution* execution() override { return delegate.execution(); }
    virtual Execution* analysis() override { return delegate.analysis(); }
    virtual Execution* difference() override { return delegate.difference(); }
    virtual Valgrind* valgrind() override { return delegate.valgrind(); }

    virtual void compile() override { delegate.compile(); }

    virtual void run(Variable input, std::vector<Variable> args = {}) override { delegate.run(input, args); }
    virtual void run_grind(Variable input, std::vector<Variable> args = {}) override { delegate.run_grind(input, args); }
    virtual void run_split(Variable input, Variable arg_str) override { delegate.run_split(input, arg_str); }
    virtual void run_grind_split(Variable input, Variable arg_str) override { delegate.run_grind_split(input, arg_str); }

    virtual void diff_stdout(Variable mode, Variable file) override { delegate.diff_stdout(mode, file); }
    virtual void diff_stderr(Variable mode, Variable file) override { delegate.diff_stderr(mode, file); }
    virtual void diff_generic(Variable mode, Variable file1, Variable file2) override { delegate.diff_generic(mode, file1, file2); }

    virtual void analyze_stdout(Variable desc, Variable cmd) override { delegate.analyze_stdout(desc, cmd); }
    virtual void analyze_stderr(Variable desc, Variable cmd) override { delegate.analyze_stderr(desc, cmd); }
    virtual void analyze(Variable desc, Variable input, Variable cmd) override { delegate.analyze(desc, input, cmd); }

    virtual void log(Variable text, Variable type = "both") override { delegate.log(text, type); }
    virtual void log_file(Variable filename, Variable type = "both") override { delegate.log_file(filename, type); }
    virtual void add_attachment(Variable filename, Variable type = "both") override { delegate.add_attachment(filename, type); }

    virtual void log_run_fail(Variable message) override { delegate.log_run_fail(message); }
    virtual void log_tag(Variable tag, Variable text, Variable type = "both") override { delegate.log_tag(tag, text, type); }
    virtual void log_valgrind(Variable tag, Variable text) override { delegate.log_valgrind(tag, text); }
    virtual void subtest(Variable name) override { delegate.subtest(name); }

    /// Get class name of the UnitTest
    const char* __getClassName() const {
        return className;
    }

    /// Code of the unit test will be in this method
    virtual void execute () = 0;

    typename std::unique_ptr<::kontr::Session::Delegator<T>>& session() const {
        return T::instance().session;
    }
};


} //UnitTest
} //kontr
#endif // UNITTEST_H
