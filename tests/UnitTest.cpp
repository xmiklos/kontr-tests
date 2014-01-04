#include "common.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstring>

using namespace kontr;
using namespace std;

UNIT_TEST(normal) {
    name("unit_testing");

    VAR(matrix, "matrix.cpp");

    stage_compiled_student_file(matrix);
    stage_compiled_student_file("pagerank.cpp");
    stage_compiled_student_file("parser.cpp");

    stage_student_file("matrix.h");
    stage_student_file("pagerank.h");
    stage_student_file("parser.h");

    stage_compiled_file("test_misc.cpp");

    stage_compiled_file("test_pagerank_case.cpp");
    stage_compiled_file("test_parser_case.cpp");
    stage_compiled_file("test_matrix_case.cpp");

    stage_file("test_misc.hpp");
    stage_file("test_matrix_case.hpp");
    stage_file("test_parser_case.hpp");
    stage_file("test_pagerank_case.hpp");

    stage_file("data_full_links_index.html");
    stage_file("data_full_links_page1.html");
    stage_file("data_full_links_page2.html");
    stage_file("data_full_links_page3.html");
    stage_file("data_mul_dead_links_index.html");
    stage_file("data_mul_dead_links_page1.html");
    stage_file("data_mul_dead_links_page2.html");


    stage_file("data_mul_links_index.html");
    stage_file("data_mul_links_page1.html");
    stage_file("data_mul_links_page2.html");

    stage_file("data_no_links_index.html");
    stage_file("data_one_self_loop_index.html");
    stage_file("data_simple_structure_index.html");
    stage_file("data_simple_structure_page1.html");
    stage_file("data_simple_structure_page2.html");
}

SESSION_NAME(tmp, ".", ".", {}, {})

TEST_CASE("master_test") {
    Testing& cg = Testing::instance();
    cg.storage.nextFileName = "session"; //Must be done before inicialization
    cg.setSession(tmp);
    ifstream sess("./session.pl");
    REQUIRE(sess.good());

    stringstream buffer;
    streambuf* old = cerr.rdbuf(buffer.rdbuf());

    SECTION("Correct test") {
        auto filename = "./unit_testing.pl";
        cg.storage.nextFileName = "unit_testing"; //Must be done before inicialization

        auto tmp = cg.UnitTestInstance(normal);

        REQUIRE(tmp->__getClassName() == string("normal"));

        CHECK_NOTHROW(tmp->execute());

        ifstream generated(filename);
        REQUIRE(generated.good());

        const char* result =
R"delimiter($unit_test->name('unit_testing');
$matrix = 'matrix.cpp';
$unit_test->stage_compiled_student_file($matrix);
$unit_test->stage_compiled_student_file('pagerank.cpp');
$unit_test->stage_compiled_student_file('parser.cpp');
$unit_test->stage_student_file('matrix.h');
$unit_test->stage_student_file('pagerank.h');
$unit_test->stage_student_file('parser.h');
$unit_test->stage_compiled_file('test_misc.cpp');
$unit_test->stage_compiled_file('test_pagerank_case.cpp');
$unit_test->stage_compiled_file('test_parser_case.cpp');
$unit_test->stage_compiled_file('test_matrix_case.cpp');
$unit_test->stage_file('test_misc.hpp');
$unit_test->stage_file('test_matrix_case.hpp');
$unit_test->stage_file('test_parser_case.hpp');
$unit_test->stage_file('test_pagerank_case.hpp');
$unit_test->stage_file('data_full_links_index.html');
$unit_test->stage_file('data_full_links_page1.html');
$unit_test->stage_file('data_full_links_page2.html');
$unit_test->stage_file('data_full_links_page3.html');
$unit_test->stage_file('data_mul_dead_links_index.html');
$unit_test->stage_file('data_mul_dead_links_page1.html');
$unit_test->stage_file('data_mul_dead_links_page2.html');
$unit_test->stage_file('data_mul_links_index.html');
$unit_test->stage_file('data_mul_links_page1.html');
$unit_test->stage_file('data_mul_links_page2.html');
$unit_test->stage_file('data_no_links_index.html');
$unit_test->stage_file('data_one_self_loop_index.html');
$unit_test->stage_file('data_simple_structure_index.html');
$unit_test->stage_file('data_simple_structure_page1.html');
$unit_test->stage_file('data_simple_structure_page2.html');
)delimiter";

        stringstream buf;
        buf << result;

        while(generated.good() && buf.good()) {
            string gen, res;
            getline(generated, gen);
            getline(buf, res);
            CHECK(gen == res);
        }
        CHECK(generated.eof());
        CHECK(buf.eof());

        generated.close();
        tmp = nullptr;
        CHECK(remove(filename) == 0);
    }

    sess.close();
    cg.session = nullptr;
    CHECK(remove("./session.pl") == 0);

    cerr.rdbuf(old);
}

