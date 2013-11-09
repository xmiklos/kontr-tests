#include "MasterTest.h"

namespace kontr {
namespace Generator {

MasterTest::MasterTest() {}

void MasterTest::name(const char *name) {
    out << variable << "->name('" << name << "');" << std::endl;
}

void MasterTest::register_unit(const char *unit) {
    out << variable << "->register_unit('" << unit << "');" << std::endl;
}

void MasterTest::stage_file(const char *filename) {
    out << variable << "->stage_file('" << filename << "');" << std::endl;
}

void MasterTest::stage_compiled_file(const char *filename) {
    out << variable << "->stage_compiled_file('" << filename << "');" << std::endl;
}

void MasterTest::stage_student_file(const char *filename) {
    out << variable << "->stage_student_file('" << filename << "');" << std::endl;
}

void MasterTest::stage_compiled_student_file(const char *filename) {
    out << variable << "->stage_compiled_student_file('" << filename << "');" << std::endl;
}

}
}
