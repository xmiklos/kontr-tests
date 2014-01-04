#ifndef UNITTEST_H
#define UNITTEST_H

namespace kontr {
namespace UnitTest {

/// Pure interface
template<typename T>
class Interface {
public:
    /// Parameter type
    using Variable = typename T::VariableDelegator;

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

    //virtual void addTag(Variable tag) = 0;
    //virtual void addPoints(Variable name, Variable points) = 0;
    //virtual Variable work_path() = 0; //string
    //virtual Variable file_path() = 0; //string
    using Compilation = typename T::CompileDelegator;
    virtual Compilation* compilation() = 0;
    //virtual Variable extra_compiler_flags() = 0; //string
    using Execution = typename T::ExecDelegator;
    virtual Execution* execution() = 0;
};

/// Empty class (all virtual methods implemented as empty)
template <typename T>
class Empty : public Interface<T> {
public:
    using typename Interface<T>::Execution;
    using typename Interface<T>::Compilation;
    using typename Interface<T>::Variable;

    virtual void name(const char * name) override { kontr::unused(name); }
    virtual void stage_compiled_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_compiled_student_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_student_file(Variable filename) override { kontr::unused(filename); }

    virtual Execution* execution() override { return nullptr; }
    virtual Compilation* compilation() override { return nullptr; }
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
    using Function = std::unique_ptr<Delegator> (*)();

    using Variable = typename Interface<T>::Variable;
    using typename Interface<T>::Execution;
    using typename Interface<T>::Compilation;

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

    virtual Execution* execution() override {
        return delegate.execution();
    }

    virtual Compilation* compilation() override {
        return delegate.compilation();
    }

    /// Get class name of the UnitTest
    const char* __getClassName() const {
        return className;
    }

    /// Code of the unit test will be in this method
    virtual void execute () = 0;
};


} //UnitTest
} //kontr
#endif // UNITTEST_H
