#include "common.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

#include <sstream>

using namespace kontr;
using namespace std;

void testType(Variable::Data<Testing>::DataType t, const Testing::VariableDelegator& input) {
    CHECK(input.__getDelegate().dataType == t);
}

void testGenerate(const Testing::VariableDelegator& input, string result) {
    Testing::VariableDelegator copy(input);

    stringstream ss;
    input.__generate(ss);
    CHECK( ss.str() == result );

    // Check with operator<<
    ss.str("");
    ss << copy;
    CHECK( ss.str() == result );
}

TEST_CASE("Constants type deduction") {
    using Type = Variable::Data<Testing>::DataType;

    testType (Type::Int, 10);
    testType (Type::Int, 0);

    testType (Type::Float, 0.0);

    testType (Type::Float, 4.3);
    testType (Type::Float, 4.3f);
    testType (Type::Float, 4.3d);

    testType (Type::Bool, false);
    testType (Type::Bool, true);

    testType (Type::String, "blabla");
    testType (Type::String, "");
}

TEST_CASE("Generator") {
    testGenerate (10, "10");
    testGenerate (0, "0");
    testGenerate (-2, "-2");

    testGenerate (0.0, "0");
    testGenerate (4.3, "4.3");
    testGenerate (-5.6, "-5.6");

    // BEWARE! Perl does not have false/true
    testGenerate (false, "0");
    testGenerate (true, "1");

    testGenerate ("no quotes", "'no quotes'");
    testGenerate ("single ''quotes", "\"single ''quotes\"");
    testGenerate ("double \"\"quotes", "'double \"\"quotes'");

    testGenerate ("both '\" quotes", "'both \\'\" quotes'");
    testGenerate ("both ''\" quotes", "\"both ''\\\" quotes\"");
    testGenerate ("both '\"\" quotes", "'both \\'\"\" quotes'");

    testGenerate ("escape \\", "'escape \\'");
}

TEST_CASE("Variable") {
    Testing& cg = Testing::instance();;
    using Type = Variable::Data<Testing>::DataType;
    stringstream ss;
    cg.out_ptr = &ss;

    Variable::Delegator<Testing> a("a", 10);
    CHECK( ss.str() == "$a = 10;\n");
    ss.str("");

    CHECK( a.__getDelegate().variableType == Variable::Data<Testing>::VariableType::Variable);
    CHECK( a.__getDelegate().dataType == Variable::Data<Testing>::DataType::Int);
    CHECK( a.__getDelegate().data.Int == 10);

    a = 25.2;
    CHECK( ss.str() == "$a = 25.2;\n");
    ss.str("");

    Variable::Delegator<Testing> other("other", "string");
    CHECK( ss.str() == "$other = \'string\';\n");
    ss.str("");

    a = other;
    CHECK( ss.str() == "$a = $other;\n");
    ss.str("");

    ss << a;
    CHECK( ss.str() == "$a" );

    CHECK( a.__getDelegate().dataType == Type::String );
    CHECK( a.__getDelegate().data.String == "string" );
    CHECK( a.__getDelegate().variableName == "a" );
}
