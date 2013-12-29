#ifndef MASTERTEST_H
#define MASTERTEST_H

#include "kontr.h"
#include <memory>

namespace kontr {

template<typename T>
class IMasterTest {
public:
    /// Parameter type
    using Variable = const typename T::VariableDelegator&;

    virtual ~IMasterTest() {}

    /**
     * Set name to the master test
     * @brief name
     * @param name
     */
    virtual void name(Variable name) = 0;

    /**
     * Register unit test
     * @brief register_unit
     * @param unit
     */
    virtual void register_unit(Variable unit) = 0;

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

template <typename T>
class MasterTestDelegator : public ::kontr::IMasterTest<T>
{
    typename T::Master delegate;
    const char* className;
    using Variable = typename ::kontr::IMasterTest<T>::Variable;

protected:
    MasterTestDelegator<T>(const char* className) :
        ::kontr::IMasterTest<T>(), delegate(), className(className) {}
public:
    /// Type of a function returning unique pointer to this delegator type
    using Function = std::unique_ptr<MasterTestDelegator> (*)();

    virtual void name(Variable name) {
        delegate.name(name);
    }

    virtual void register_unit(Variable unit) {
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

    const char* getClassName() const {
        return className;
    }

    virtual void execute () = 0;
};

}
#endif // MASTERTEST_H
