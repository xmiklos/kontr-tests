#ifndef MASTERTEST_H
#define MASTERTEST_H

#include "kontr.h"

namespace kontr {

template<typename T>
class IMasterTest {
protected:
    T& instance;
public:
    IMasterTest(T& instance) : instance(instance) {}
    virtual ~IMasterTest() {}

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
    virtual void register_unit(const char* unit) = 0;

    /**
     * Add a file from _files_
     * @brief stage_file
     * @param filename
     */
    virtual void stage_file(const char* filename) = 0;
    /**
     * Add a compiled file from _file_
     * @brief stage_compiled_file
     * @param filename
     */
    virtual void stage_compiled_file(const char* filename) = 0;

    /**
     * Add a file from student repository
     * @brief stage_student_file
     * @param filename
     */
    virtual void stage_student_file(const char* filename) = 0;

    /**
     * Add a compiled file from student repository
     * @brief stage_compiled_student_file
     * @param filename
     */
    virtual void stage_compiled_student_file(const char* filename) = 0;
};

template <typename T>
class MasterTestDelegator : public ::kontr::IMasterTest<T>
{
    typename T::Master delegate;
    const char* className;
protected:
    MasterTestDelegator<T>(T& instance, const char* className) :
        ::kontr::IMasterTest<T>(instance), delegate(instance), className(className) {}
public:
    /// Type of a function returning pointer to this delegator type
    using Function = MasterTestDelegator*(*)(T&);

    virtual void name(const char* name) {
        delegate.name(name);
    }

    virtual void register_unit(const char* unit) {
        delegate.register_unit(unit);
    }

    virtual void stage_file(const char* filename) {
        delegate.stage_file(filename);
    }

    virtual void stage_compiled_file(const char* filename) {
        delegate.stage_compiled_file(filename);
    }

    virtual void stage_student_file(const char* filename) {
        delegate.stage_student_file(filename);
    }

    virtual void stage_compiled_student_file(const char* filename) {
        delegate.stage_compiled_student_file(filename);
    }

    const char* getClassName() const {
        return className;
    }

    virtual void execute () = 0;
};

}
#endif // MASTERTEST_H
