#include "common.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

#include <sstream>

using namespace kontr;
using namespace std;

void testType(Variable::DataType t, const Testing::VariableDelegator& input) {
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
    using Type = Variable::DataType;

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
    using Type = Variable::DataType;
    stringstream ss;
    cg.storage.out_ptr = &ss;

    Variable::Delegator<Testing> a("a", 10);
    CHECK( ss.str() == "$a = 10;\n");
    ss.str("");

    CHECK( a.__getDelegate().variableType == Variable::VariableType::Variable);
    CHECK( a.__getDelegate().dataType == Variable::DataType::Int);
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

TEST_CASE("Conversion"){
    Testing& cg = Testing::instance();;
    stringstream ss;
    cg.storage.out_ptr = &ss;

    Variable::Delegator<Testing> a("a", 10);
    Variable::Delegator<Testing> b("b", 10);

    // Int
    ss.str("");
    b = a.toInt();
    CHECK( ss.str() == "$b = $a;\n");

    a = "40";
    ss.str("");
    b = a.toInt();
    CHECK( ss.str() == "$b = 0 + $a;\n");


    // Float
    ss.str("");
    b = a.toFloat();
    CHECK( ss.str() == "$b = 0.0 + $a;\n");

    a = 10.0;
    ss.str("");
    b = a.toFloat();
    CHECK( ss.str() == "$b = $a;\n");

    // Combination float/int
    ss.str("");
    b = a.toInt().toFloat();
    CHECK( ss.str() == "$b = 0.0 + (0 + $a);\n");

    // Bool
    ss.str("");
    b = a.toBool();
    CHECK( ss.str() == "$b = !!$a;\n");

    a = false;
    ss.str("");
    b = a.toBool();
    CHECK( ss.str() == "$b = $a;\n");

    // String
    ss.str("");
    b = a.toString();
    CHECK( ss.str() == "$b = \"\" . $a;\n");

    a = "tmp";
    ss.str("");
    b = a.toString();
    CHECK( ss.str() == "$b = $a;\n");
}

TEST_CASE("Comparison"){
    Testing& cg = Testing::instance();;
    stringstream ss;
    cg.storage.out_ptr = &ss;

    Variable::Delegator<Testing> a("a", 10);
    Variable::Delegator<Testing> b("b", 10);
    Variable::Delegator<Testing> c("c", 10);

    ss.str("");
    c = a == b;
    CHECK( ss.str() == "$c = $a == $b;\n" );

    ss.str("");
    c = a != b;
    CHECK( ss.str() == "$c = $a != $b;\n" );

    // String
    a = "324";
    b = "44";

    ss.str("");
    c = a == b;
    CHECK( ss.str() == "$c = $a eq $b;\n" );

    ss.str("");
    c = a != b;
    CHECK( ss.str() == "$c = $a ne $b;\n" );
}
