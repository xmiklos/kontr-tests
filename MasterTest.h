#ifndef MASTERTEST_H
#define MASTERTEST_H

#include "kontr-internal.h"

namespace kontr {
namespace MasterTest {

/// Pure interface
template<typename T>
class Interface {
public:
    /// Parameter type
    using Variable = typename ::kontr::Variable::Delegator<T>;
    using Unit = typename ::kontr::UnitTest::Delegator<T>::Function;

    virtual ~Interface() {}

    /**
     * Set name to the master test
     * @brief name
     * @param name
     */
    virtual void name(const char* name) = 0;

    /**
     * Register unit test
     * @brief register_unit
     * @param unit
     */
    virtual void register_unit(Unit unit) = 0;

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
    using typename Interface<T>::Unit;

    virtual void name(const char * name) override { kontr::unused(name); }
    virtual void register_unit(Unit unit) override { kontr::unused(unit); }
    virtual void stage_compiled_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_compiled_student_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_file(Variable filename) override { kontr::unused(filename); }
    virtual void stage_student_file(Variable filename) override { kontr::unused(filename); }
};

/// Delegator class
template <typename T>
class Delegator : public Interface<T>
{
    typename T::Master delegate;
    const char* className;
protected:
    Delegator(const char* className) :
        Interface<T>(), delegate(), className(className) {}

public:
    /// Type of a function returning unique pointer to this delegator type
    using Function = void (*)(std::unique_ptr<Delegator>&);

    using Variable = typename Interface<T>::Variable;
    using Unit = typename Interface<T>::Unit;

    virtual void name(const char* name) {
        delegate.name(name);
    }

    virtual void register_unit(Unit unit) {
        delegate.register_unit(unit);
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

} //MasterTest
} //kontr
#endif // MASTERTEST_H
