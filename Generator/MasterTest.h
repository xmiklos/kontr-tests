#ifndef GENERATOR_MASTERTEST_H
#define GENERATOR_MASTERTEST_H

#include <iostream>

#include "../MasterTest.h"

namespace kontr {
namespace Generator {

class MasterTest : public ::kontr::IMasterTest  {
    const char* variable = "$master_test";
    std::ostream& out = std::cout;
public:
    MasterTest();

    virtual void name(const char *name);

    virtual void register_unit(const char *unit);

    virtual void stage_file(const char* filename);

    virtual void stage_compiled_file(const char* filename);

    virtual void stage_student_file(const char* filename);

    virtual void stage_compiled_student_file(const char* filename);
};

}
}

#endif // GENERATOR_MASTERTEST_H
