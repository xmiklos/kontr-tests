#ifndef GENERATOR_MASTERTEST_H
#define GENERATOR_MASTERTEST_H

#include "../kontr.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <cassert>

namespace kontr {
namespace Generator {

template <typename T>
class MasterTest : public ::kontr::MasterTest::Interface<T>  {
    const char* variable = "$master_test";
    std::ofstream out;
    using Variable = typename ::kontr::MasterTest::Interface<T>::Variable;

public:
    MasterTest() : out(T::instance().session->__getScriptsDir() + "/" +
                       T::instance().storage.nextFileName + ".pl") {
        if (!out.good()) {
            T::instance().report.create(Report::ERROR, "Could not open file for writing");
        }
        T::instance().storage.out_ptr = &out;
    }

    virtual void name(const char* name) {
        Variable print = name;
        out << variable << "->name(" << print << ");" << std::endl;
    }

    virtual void register_unit(Variable unit) {
        out << variable << "->register_unit(" << unit << ");" << std::endl;
    }

    virtual void stage_file(Variable filename) {
        out << variable << "->stage_file(" << filename << ");" << std::endl;
    }

    virtual void stage_compiled_file(Variable filename) {
        out << variable << "->stage_compiled_file(" << filename << ");" << std::endl;
    }

    virtual void stage_student_file(Variable filename) {
        out << variable << "->stage_student_file(" << filename << ");" << std::endl;
    }

    virtual void stage_compiled_student_file(Variable filename) {
        out << variable << "->stage_compiled_student_file(" << filename << ");" << std::endl;
    }

    virtual ~MasterTest() {
        if (T::instance().storage.out_ptr == &out) {
            T::instance().storage.out_ptr = nullptr;
        }
    }
};

} //Generator
} //kontr
#endif // GENERATOR_MASTERTEST_H
