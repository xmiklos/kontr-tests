#ifndef MASTERTEST_H
#define MASTERTEST_H

namespace kontr {

class IMasterTest {
public:
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

class MasterTestDelegator : public ::kontr::IMasterTest
{
    IMasterTest& delegate;
    const char* className;
protected:
    MasterTestDelegator(IMasterTest& delegate, const char* className) : delegate(delegate), className(className) {}
public:
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
};

}
#endif // MASTERTEST_H
