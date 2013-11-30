#include "kontr.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstring>

using namespace kontr;
using namespace std;

CONFIGURATION(Testing,
              ::kontr::Generator::Session,
              ::kontr::Generator::MasterTest,
              <::kontr::Report::Reporting::ERR_EXCEPTION,
              ::kontr::Report::Reporting::ERR_EXCEPTION,
              ::kontr::Report::Reporting::ERR_EXCEPTION>);

MASTER_TEST(noname) {
    stage_file("dummy");
}

MASTER_TEST(doublename) {
    name("fddf");
    name("fddf");
}

MASTER_TEST(normal) {
    name("master_testing");

    register_unit("unit_matrix_test2.pl");
    register_unit("unit_matrix_test3.pl");
    register_unit("unit_parser_test2.pl");

    stage_compiled_student_file("matrix.cpp");
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
    Testing cg;
    cg.setSession(tmp);
    stringstream buffer;
    unique_ptr<streambuf> old(cerr.rdbuf(buffer.rdbuf()));

    SECTION("Test without name") {
        auto tmp = cg.MasterTestInstance(noname);
        REQUIRE(tmp->getClassName() == string("noname"));
        CHECK_THROWS_AS(tmp->execute(), Report::ReportError);
        stringstream err;
        Report::print_report_text(err, Report::ERROR,
                                  string("No file opened for output; was name the first to call?"));

        CHECK(err.str() == buffer.str().substr(0, err.str().size()));
    }
    buffer.str("");

    SECTION("Test with two names") {
        auto tmp = cg.MasterTestInstance(doublename);
        REQUIRE(tmp->getClassName() == string("doublename"));

        CHECK_THROWS_AS(tmp->execute(), Report::ReportError);

        stringstream err;
        Report::print_report_text(err, Report::ERROR,
                                  string("Name set twice, ignoring"));

        CHECK(err.str() == buffer.str().substr(0, err.str().size()));
    }
    buffer.str("");

    SECTION("Correct test") {
        auto tmp = cg.MasterTestInstance(normal);
        auto filename = "./master_testing.pl";
        REQUIRE(tmp->getClassName() == string("normal"));

        CHECK_NOTHROW(tmp->execute());

        ifstream generated(filename);
        REQUIRE(generated.good());

        const char* result =
R"delimiter($master_test->name('master_testing');
$master_test->register_unit('unit_matrix_test2.pl');
$master_test->register_unit('unit_matrix_test3.pl');
$master_test->register_unit('unit_parser_test2.pl');
$master_test->stage_compiled_student_file('matrix.cpp');
$master_test->stage_compiled_student_file('pagerank.cpp');
$master_test->stage_compiled_student_file('parser.cpp');
$master_test->stage_student_file('matrix.h');
$master_test->stage_student_file('pagerank.h');
$master_test->stage_student_file('parser.h');
$master_test->stage_compiled_file('test_misc.cpp');
$master_test->stage_compiled_file('test_pagerank_case.cpp');
$master_test->stage_compiled_file('test_parser_case.cpp');
$master_test->stage_compiled_file('test_matrix_case.cpp');
$master_test->stage_file('test_misc.hpp');
$master_test->stage_file('test_matrix_case.hpp');
$master_test->stage_file('test_parser_case.hpp');
$master_test->stage_file('test_pagerank_case.hpp');
$master_test->stage_file('data_full_links_index.html');
$master_test->stage_file('data_full_links_page1.html');
$master_test->stage_file('data_full_links_page2.html');
$master_test->stage_file('data_full_links_page3.html');
$master_test->stage_file('data_mul_dead_links_index.html');
$master_test->stage_file('data_mul_dead_links_page1.html');
$master_test->stage_file('data_mul_dead_links_page2.html');
$master_test->stage_file('data_mul_links_index.html');
$master_test->stage_file('data_mul_links_page1.html');
$master_test->stage_file('data_mul_links_page2.html');
$master_test->stage_file('data_no_links_index.html');
$master_test->stage_file('data_one_self_loop_index.html');
$master_test->stage_file('data_simple_structure_index.html');
$master_test->stage_file('data_simple_structure_page1.html');
$master_test->stage_file('data_simple_structure_page2.html');
)delimiter";
        unsigned int index = 0;
        while(generated.good()) {
            int c = generated.get();
            if (c == EOF) break;
            CHECK(c == result[index++]);
        }

        CHECK(index == strlen(result));

        generated.close();
        CHECK(remove(filename) == 0);
    }
    cerr.rdbuf(old.release());
}
