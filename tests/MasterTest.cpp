#include "common.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <array>

using namespace kontr;
using namespace std;

/*
 * This will be tested in Checker
MASTER_TEST(noname) {
    stage_file("dummy");
}

MASTER_TEST(doublename) {
    name("fddf");
    name("fddf");
}
*/

UNIT_TEST(matrix_2) {
    name("unit_matrix_test2.pl");
}

UNIT_TEST(matrix_3) {
    name("unit_matrix_test3.pl");
}

UNIT_TEST(parser) {
    name("unit_parser_test2.pl");
}

MASTER_TEST(normal) {
    name("master_testing.pl");

    VAR(matrix, "matrix.cpp");

    register_unit(matrix_2);
    register_unit(matrix_3);
    register_unit(parser);

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

SESSION_NAME(tmp, ".", ".", {normal}, {}, false, false)

TEST_CASE("master_test") {
    Testing& cg = Testing::instance();
    cg.storage.nextFileName = "session.pl"; //Must be done before inicialization
    cg.storage.names = kontr::Names::getAll(tmp, cg);
    cg.setSession(tmp);
    ifstream sess("./session.pl");
    REQUIRE(sess.good());

    stringstream buffer;
    streambuf* old = cerr.rdbuf(buffer.rdbuf());

    /*
     * This will be tested in Checker
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
    buffer.str("");*/

    SECTION("Correct test") {
        auto filename = "./master_testing.pl";
        cg.storage.nextFileName = "master_testing.pl"; //Must be done before inicialization

        auto tmp = cg.MasterTestInstance(normal);

        REQUIRE(tmp->__getClassName() == string("normal"));

        CHECK_NOTHROW(cg.session->pre_test());

        ifstream generated(filename);
        REQUIRE(generated.good());

        //Check unit tests existance
        array<const char*, 3> unit_filenames = {"unit_matrix_test2.pl", "unit_matrix_test3.pl", "unit_parser_test2.pl"};
        ifstream unit1(unit_filenames[0]), unit2(unit_filenames[1]), unit3(unit_filenames[2]);
        CHECK(unit1.good());
        CHECK(unit2.good());
        CHECK(unit3.good());

        //Remove unit tests
        unit1.close();
        CHECK(remove(unit_filenames[0]) == 0);
        unit2.close();
        CHECK(remove(unit_filenames[1]) == 0);
        unit3.close();
        CHECK(remove(unit_filenames[2]) == 0);

        //Check contents
        const char* result =
R"delimiter($master_test->name('master_testing.pl');
my $matrix = 'matrix.cpp';
$master_test->register_unit('unit_matrix_test2.pl');
$master_test->register_unit('unit_matrix_test3.pl');
$master_test->register_unit('unit_parser_test2.pl');
$master_test->stage_compiled_student_file($matrix);
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

        //Remove generated
        generated.close();
        tmp = nullptr;
        CHECK(remove(filename) == 0);
    }

    //Remove session
    sess.close();
    cg.session = nullptr;
    CHECK(remove("./session.pl") == 0);

    cerr.rdbuf(old);
}
