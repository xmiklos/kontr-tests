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
    using Unit = typename ::kontr::MasterTest::Interface<T>::Unit;
    unsigned unit_order = 0;
    std::ostream* prevOutPtr = nullptr;

public:
    MasterTest() : out(T::instance().session->__getScriptsDir() + "/" +
                       T::instance().storage.nextFileName + ".pl") {
        if (!out.good()) {
            T::instance().report.create(Report::ERROR, "Could not open file for writing");
        }
        prevOutPtr = T::instance().storage.out_ptr;
        T::instance().storage.out_ptr = &out;
    }

    virtual void name(const char* name) {
        Variable print = name;
        out << variable << "->name(" << print << ");" << std::endl;
    }

    virtual void register_unit(Unit unit) {
        const Names::All& names = T::instance().storage.names;
        auto current = names.unitTests.find(names.currentMasterIndex)->second[unit_order++]; //tuple<string, string>
        T::instance().storage.nextFileName = std::get<1>(current).c_str();
        //Check if this is correct unit test
        typename T::UnitDelegatorInstance instance = T::UnitTestInstance(unit);
        if (instance->__getClassName() != std::get<0>(current)) {
            T::instance().report.create(Report::ERROR, "Invalid unit test class name");
        }

        Variable print = (std::get<1>(current) + ".pl").c_str();
        out << variable << "->register_unit(" << print << ");" << std::endl;

        //Generate code for unit test
        instance->execute();
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
            T::instance().storage.out_ptr = prevOutPtr;
        }
    }
};

} //Generator
} //kontr
#endif // GENERATOR_MASTERTEST_H
