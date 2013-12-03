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
class MasterTest : public ::kontr::IMasterTest<T>  {
    const char* variable = "$master_test";
    std::ostream* out_ptr = nullptr;
    using Variable = typename ::kontr::IMasterTest<T>::Variable;
    using ::kontr::IMasterTest<T>::instance;

    bool check_out() {
        if (out_ptr == nullptr) {
            instance.report.create(Report::ERROR,
                                   "No file opened for output; was name the first to call?");
            return false;
        }
        return true;
    }

public:
    using ::kontr::IMasterTest<T>::IMasterTest;

    virtual void name(Variable name) {
        if (out_ptr == nullptr) {
            assert(instance.session != nullptr);
            out_ptr = new std::ofstream(this->instance.session->script_dir
                                        + "/" + name.data.String + ".pl");
        }
        else {
            instance.report.create(Report::ERROR,
                                         "Name set twice, ignoring");
            return;
        }
        instance.out_ptr = out_ptr;
        std::ostream& out = *out_ptr;
        out << variable << "->name(" << name << ");" << std::endl;
    }

    virtual void register_unit(Variable unit) {
        if (!check_out()) return;
        std::ostream& out = *out_ptr;
        out << variable << "->register_unit(" << unit << ");" << std::endl;
    }

    virtual void stage_file(Variable filename) {
        if (!check_out()) return;
        std::ostream& out = *out_ptr;
        out << variable << "->stage_file(" << filename << ");" << std::endl;
    }

    virtual void stage_compiled_file(Variable filename) {
        if (!check_out()) return;
        std::ostream& out = *out_ptr;
        out << variable << "->stage_compiled_file(" << filename << ");" << std::endl;
    }

    virtual void stage_student_file(Variable filename) {
        if (!check_out()) return;
        std::ostream& out = *out_ptr;
        out << variable << "->stage_student_file(" << filename << ");" << std::endl;
    }

    virtual void stage_compiled_student_file(Variable filename) {
        if (!check_out()) return;
        std::ostream& out = *out_ptr;
        out << variable << "->stage_compiled_student_file(" << filename << ");" << std::endl;
    }

    virtual ~MasterTest() {
        if (out_ptr != nullptr) {
            delete out_ptr;
        }
    }
};

}
}

#endif // GENERATOR_MASTERTEST_H
