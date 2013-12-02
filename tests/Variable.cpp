#include "common.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

#include <sstream>

using namespace kontr;
using namespace std;

void testType(Testing& cg, Variable<Testing>::Type t, const Testing::VariableDelegator& input) {
    input.__setInstance(cg);
    CHECK(input.type == t);
}

void testGenerate(Testing& cg, const Testing::VariableDelegator& input, string result) {
    Testing::VariableDelegator copy(input);

    input.__setInstance(cg);
    stringstream ss;
    input.__generate(ss);
    CHECK( ss.str() == result );

    // Check with operator<<
    ss.str("");
    copy.__setInstance(cg);
    ss << copy;
    CHECK( ss.str() == result );
}

TEST_CASE("Constants type deduction") {
    Testing cg;
    using Type = Variable<Testing>::Type;

    testType (cg, Type::Int, 10);
    testType (cg, Type::Int, 0);

    testType (cg, Type::Float, 0.0);

    testType (cg, Type::Float, 4.3);
    testType (cg, Type::Float, 4.3f);
    testType (cg, Type::Float, 4.3d);

    testType (cg, Type::Bool, false);
    testType (cg, Type::Bool, true);

    testType (cg, Type::String, "blabla");
    testType (cg, Type::String, "");
}

TEST_CASE("Generator") {
    Testing cg;

    testGenerate (cg, 10, "10");
    testGenerate (cg, 0, "0");
    testGenerate (cg, -2, "-2");

    testGenerate (cg, 0.0, "0");
    testGenerate (cg, 4.3, "4.3");
    testGenerate (cg, -5.6, "-5.6");

    // BEWARE! Perl does not have false/true
    testGenerate (cg, false, "0");
    testGenerate (cg, true, "1");

    testGenerate (cg, "no quotes", "'no quotes'");
    testGenerate (cg, "single ''quotes", "\"single ''quotes\"");
    testGenerate (cg, "double \"\"quotes", "'double \"\"quotes'");

    testGenerate (cg, "both '\" quotes", "'both \\'\" quotes'");
    testGenerate (cg, "both ''\" quotes", "\"both ''\\\" quotes\"");
    testGenerate (cg, "both '\"\" quotes", "'both \\'\"\" quotes'");

    testGenerate (cg, "escape \\", "'escape \\'");
}
