#ifndef GENERATOR_UNITTEST_H
#define GENERATOR_UNITTEST_H

#include "../kontr.h"

namespace kontr {
namespace Generator {

template <typename T>
class UnitTest : public ::kontr::UnitTest::Interface<T>  {
    const char* variable = "$unit_test";
    std::ofstream out;
    std::ostream* prevOutPtr = nullptr;

    using typename ::kontr::UnitTest::Interface<T>::Variable;
    using typename ::kontr::UnitTest::Interface<T>::Execution;

    //Delegator of Generator::Exec
    Execution executionResult = ::kontr::Generator::Exec<T>("$unit_test->execution");

public:
    UnitTest() : out(T::instance().session->__getScriptsDir() + "/" +
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

    virtual Execution* execution() override {
        return &executionResult;
    }

    virtual ~UnitTest() {
        if (T::instance().storage.out_ptr == &out) {
            T::instance().storage.out_ptr = prevOutPtr;
        }
    }
};

} //Generator
} //kontr
#endif // UNITTEST_H
