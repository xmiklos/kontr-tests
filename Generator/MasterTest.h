#ifndef GENERATOR_MASTERTEST_H
#define GENERATOR_MASTERTEST_H

#include "../kontr.h"

#include <iostream>
#include <memory>

namespace kontr {
namespace Generator {

template <typename T>
class MasterTest : public ::kontr::IMasterTest  {
    const char* variable = "$master_test";
    std::ostream& out = std::cout;
public:
    MasterTest() {}

    virtual void name(const char *name) {
        out << variable << "->name('" << name << "');" << std::endl;
    }

    virtual void register_unit(const char *unit) {
        out << variable << "->register_unit('" << unit << "');" << std::endl;
    }

    virtual void stage_file(const char* filename) {
        out << variable << "->stage_file('" << filename << "');" << std::endl;
    }

    virtual void stage_compiled_file(const char* filename) {
        out << variable << "->stage_compiled_file('" << filename << "');" << std::endl;
    }

    virtual void stage_student_file(const char* filename) {
        out << variable << "->stage_student_file('" << filename << "');" << std::endl;
    }

    virtual void stage_compiled_student_file(const char* filename) {
        out << variable << "->stage_compiled_student_file('" << filename << "');" << std::endl;
    }
};

}
}

#endif // GENERATOR_MASTERTEST_H
