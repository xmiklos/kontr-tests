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
};

/// Empty class (all virtual methods implemented as empty)
template <typename T>
class Empty : public Interface<T> {
public:
    using typename Interface<T>::Variable;

    virtual void name(const char * name) override { kontr::unused(name); }
    virtual void stage_compiled_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_compiled_student_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_student_file(Variable filename) override { kontr::unused(filename); }
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

    /// Get class name of the MasterTest
    const char* __getClassName() const {
        return className;
    }

    /// Code of the master test will be in this method
    virtual void execute () = 0;
};


} //UnitTest
} //kontr
#endif // UNITTEST_H
