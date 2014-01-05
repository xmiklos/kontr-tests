#ifndef GENERATOR_UNITTEST_H
#define GENERATOR_UNITTEST_H

#include "../kontr.h"

namespace kontr {
namespace Generator {

template <typename T>
class UnitTest : public ::kontr::UnitTest::Interface<T>  {
    std::string variable = "$unit_test";
    std::ofstream out;
    std::ostream* prevOutPtr = nullptr;

    using typename ::kontr::UnitTest::Interface<T>::Variable;
    using typename ::kontr::UnitTest::Interface<T>::Execution;
    using typename ::kontr::UnitTest::Interface<T>::Compilation;

    //Delegator of Generator::Exec::Execution
    Execution executionResult = typename ::kontr::Generator::Exec<T>::Execution("$unit_test->execution");
    Execution analysisResult = typename ::kontr::Generator::Exec<T>::Execution("$unit_test->analysis");
    Execution differenceResult = typename ::kontr::Generator::Exec<T>::Execution("$unit_test->difference");

    //Delegator of Generator::Exec::Compilation
    Compilation compilationResult = typename ::kontr::Generator::Exec<T>::Compilation("$unit_test->compilation");

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


    virtual void addTag(Variable tag) override {
        out << variable << "->addTag(" << tag << ");" << std::endl;
    }
    virtual void addPoints(Variable name, Variable points) override {
        out << variable << "->addPoints(" << name << " => " << points << ");" << std::endl;
    }

#define GETTER(NAME, TYPE) virtual Variable NAME() override {\
        return T::VariableDelegator::__create(::kontr::Generator::Variable<T>(::kontr::Variable::DataType::TYPE, variable + "->" #NAME));\
    }
    GETTER(work_path, String)
    GETTER(file_path, String)

    virtual Compilation* compilation() override { return &compilationResult; }
    GETTER(extra_compiler_flags, String)
    GETTER(compilation_log_errors, Bool)

    virtual Execution* execution() override { return &executionResult; }
    virtual Execution* analysis() override { return &analysisResult; }
    virtual Execution* difference() override { return &differenceResult; }

    virtual void compile() override {
        out << variable << "->compile();" << std::endl;
    }

    virtual void run(Variable input, std::vector<Variable> args = {}) override {
        out << variable << "->run(" << input;
        for (Variable v : args) { out << ", " << v; }
        out << ");" << std::endl;
    }
    virtual void run_grind(Variable input, std::vector<Variable> args = {}) override {
        out << variable << "->run_grind(" << input;
        for (Variable v : args) { out << ", " << v; }
        out << ");" << std::endl;
    }

    virtual void diff_stdout(Variable mode, Variable file) override {
        out << variable << "->diff_stdout(" << mode << ", " << file << ");" << std::endl;
    }
    virtual void diff_stderr(Variable mode, Variable file) override {
        out << variable << "->diff_stderr(" << mode << ", " << file << ");" << std::endl;
    }
    virtual void diff_generic(Variable mode, Variable file1, Variable file2) override {
        out << variable << "->diff_generic(" << mode << ", " << file1 << ", " << file2 << ");" << std::endl;
    }

    virtual void analyze_stdout(Variable desc, Variable cmd) override {
        out << variable << "->analyze_stdout(" << desc << ", " << cmd << ");" << std::endl;
    }
    virtual void analyze_stderr(Variable desc, Variable cmd) override {
        out << variable << "->analyze_stderr(" << desc << ", " << cmd << ");" << std::endl;
    }
    virtual void analyze(Variable desc, Variable input, Variable cmd) override {
        out << variable << "->analyze(" << desc << ", " << input << ", " << cmd << ");" << std::endl;
    }

    virtual void log(Variable text, Variable type = "both") override {
        out << variable << "->log(" << text << ", " << type << ");" << std::endl;
    }
    virtual void log_file(Variable filename, Variable type = "both") override {
        out << variable << "->log_file(" << filename << ", " << type << ");" << std::endl;
    }

    virtual void log_run_fail(Variable message) override {
        out << variable << "->log_run_fail(" << message << ");" << std::endl;
    }
    virtual void log_tag(Variable tag, Variable text, Variable type = "both") override {
        out << variable << "->log_tag(" << tag << ", " << text << ", " << type << ");" << std::endl;
    }
    virtual void log_valgrind(Variable tag, Variable text, Variable type = "both") override {
        out << variable << "->log_valgrind(" << tag << ", " << text << ", " << type << ");" << std::endl;
    }
    virtual void subtest(Variable name) override {
        out << variable << "->subtest(" << name << ");" << std::endl;
    }
#undef GETTER

    virtual ~UnitTest() {
        if (T::instance().storage.out_ptr == &out) {
            T::instance().storage.out_ptr = prevOutPtr;
        }
    }
};

} //Generator
} //kontr
#endif // UNITTEST_H
