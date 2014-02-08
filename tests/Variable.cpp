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
    CHECK( ss.str() == "my $a = 10;\n");
    ss.str("");

    CHECK( a.__getDelegate().variableType == Variable::VariableType::Variable);
    CHECK( a.__getDelegate().dataType == Variable::DataType::Int);
    CHECK( a.__getDelegate().data.Int == 10);

    a = 25.2;
    CHECK( ss.str() == "$a = 25.2;\n");
    ss.str("");

    Variable::Delegator<Testing> other("other", "string");
    CHECK( ss.str() == "my $other = \'string\';\n");
    ss.str("");

    a = other;
    CHECK( ss.str() == "$a = $other;\n");
    ss.str("");

    ss << a;
    CHECK( ss.str() == "$a" );

    CHECK( a.__getDelegate().dataType == Type::String );
    CHECK( a.__getDelegate().data.String == "string" );
    CHECK( a.__getDelegate().variableName == "a" );

    ss.str("");
    Variable::Delegator<Testing> b("b", a);
    CHECK( ss.str() == "my $b = $a;\n");

    ss.str("");
    Variable::Delegator<Testing> c("c", a == b);
    CHECK( ss.str() == "my $c = $a eq $b;\n");

    c = b == a;
    ss.str("");
    Variable::Delegator<Testing> d("d", c);
    CHECK( ss.str() == "my $d = $c;\n");
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

TEST_CASE("Comparison and logical operators"){
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

    b = 44;
    ss.str("");
    c = a == b;
    CHECK( ss.str() == "$c = $a eq $b;\n" );

    ss.str("");
    c = a != b;
    CHECK( ss.str() == "$c = $a ne $b;\n" );

    b = "4";
    a = 4;
    ss.str("");
    c = a == b;
    CHECK( ss.str() == "$c = $a eq $b;\n" );

    ss.str("");
    c = a != b;
    CHECK( ss.str() == "$c = $a ne $b;\n" );

    ss.str("");
    c = !a;
    CHECK( ss.str() == "$c = !$a;\n");

    ss.str("");
    c = a && b;
    CHECK( ss.str() == "$c = $a && $b;\n");

    ss.str("");
    c = a || b;
    CHECK( ss.str() == "$c = $a || $b;\n");

    SECTION("Check with non-variable") {
        ss.str("");
        c = false == a;
        CHECK( ss.str() == "$c = 0 == $a;\n");

        ss.str("");
        c = 1 == a;
        CHECK( ss.str() == "$c = 1 == $a;\n");

        ss.str("");
        c = 1.5 == a;
        CHECK( ss.str() == "$c = 1.5 == $a;\n");

        ss.str("");
        c = "nope" == a;
        CHECK( ss.str() == "$c = 'nope' eq $a;\n");
    }

}

TEST_CASE("Plus"){
    Testing& cg = Testing::instance();;
    stringstream ss;
    cg.storage.out_ptr = &ss;
    using Type = Variable::DataType;

    Variable::Delegator<Testing> a("a", "fa");
    Variable::Delegator<Testing> b("b", "bu");
    Variable::Delegator<Testing> c("c", 10);

    ss.str("");
    c = a + b;
    CHECK( ss.str() == "$c = $a . $b;\n");
    CHECK( c.__getDelegate().dataType == Type::String);

    b = 0.5;
    ss.str("");
    c = a + b;
    CHECK( ss.str() == "$c = $a . $b;\n");
    CHECK( c.__getDelegate().dataType == Type::String);

    a = 4;
    ss.str("");
    c = a + b;
    CHECK( ss.str() == "$c = $a + $b;\n");
    CHECK( c.__getDelegate().dataType == Type::Float);

    b = 3;
    ss.str("");
    c = a + b;
    CHECK( ss.str() == "$c = $a + $b;\n");
    CHECK( c.__getDelegate().dataType == Type::Int);
}

TEST_CASE("Array"){
    Testing& cg = Testing::instance();;
    stringstream ss;
    cg.storage.out_ptr = &ss;

    Variable::Delegator<Testing> a("a", {10, 10, 10});
    CHECK( ss.str() == "my @a = (10, 10, 10);\n");

    ss.str("");
    Variable::Delegator<Testing> b("b", {0.3, 0.4, -5.2});
    CHECK( ss.str() == "my @b = (0.3, 0.4, -5.2);\n");

    ss.str("");
    Variable::Delegator<Testing> c("c", {false, true, false});
    CHECK( ss.str() == "my @c = (0, 1, 0);\n");

    ss.str("");
    Variable::Delegator<Testing> d("d", {"a", "b", "c"});
    CHECK( ss.str() == "my @d = ('a', 'b', 'c');\n");

    ss.str("");
    d = {};
    CHECK( ss.str() == "@d = ();\n");

    ss.str("");
    d = {1, 1, 1, 2, 3};
    CHECK( ss.str() == "@d = (1, 1, 1, 2, 3);\n");

    ss.str("");
    d = c;
    CHECK( ss.str() == "@d = @c;\n");


    SECTION("Array in array") {
        stringstream buffer;
        streambuf* old = cerr.rdbuf(buffer.rdbuf());
        CHECK_THROWS_AS(a = { {b} }, Report::ReportError);

        stringstream err;
        Report::print_report_text(err, Report::ERROR, string("Array of arrays is not valid"));

        CHECK(err.str() == buffer.str().substr(0, err.str().size()));

        cerr.rdbuf(old);
    }

    SECTION("Multiple types") {
        stringstream buffer;
        streambuf* old = cerr.rdbuf(buffer.rdbuf());
        CHECK_THROWS_AS((a = { 1, 2, 3, "bla" }), Report::ReportError);

        stringstream err;
        Report::print_report_text(err, Report::ERROR, string("Element number 4 has different type then the first element"));

        CHECK(err.str() == buffer.str().substr(0, err.str().size()));

        cerr.rdbuf(old);
    }

    SECTION("Subscript") {
        ss.str("");
        c = a[1];
        CHECK( ss.str() == "$c = $a[1];\n");

        a = 10;

        ss.str("");
        c = b[a];
        CHECK( ss.str() == "$c = $b[$a];\n");

        ss.str("");
        c = d[0] + 25;
        CHECK( ss.str() == "$c = ($d[0]) + 25;\n");
    }

    ss.str("");
    a = d.size();
    CHECK( ss.str() == "$a = scalar @d;\n");
}

TEST_CASE("Strings") {
    Testing& cg = Testing::instance();;
    stringstream ss;
    cg.storage.out_ptr = &ss;

    Variable::Delegator<Testing> a("a", "fsfstr");
    Variable::Delegator<Testing> b("b", 0);

    ss.str("");
    b = a[4];
    CHECK( ss.str() == "$b = substr $a, 4, 1;\n");

    ss.str("");
    b = a.substr(2, 5);
    CHECK( ss.str() == "$b = substr $a, 2, 5;\n");

    ss.str("");
    b = a.size();
    CHECK( ss.str() == "$b = length $a;\n");
}
