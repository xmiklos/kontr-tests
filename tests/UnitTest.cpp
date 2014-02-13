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
    name("unit_testing.pl");

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

UNIT_TEST(functions) {
    name("functions.pl");

    VAR(tmp, "");

    add_tag("test");
    add_points("points", 1);
    tmp = work_path();
    tmp = file_path();
    tmp = compilation()->result();
    extra_compiler_flags() = "flag";
    compilation_log_errors() = true;
    tmp = execution()->success();
    tmp = difference()->exit_value() != 0;
    tmp = analysis()->exit_type() == "normal";
    compile();

    run("/dev/null");
    run("file1", {"file2", "-o", "file3"});
    run_grind("em");
    run_grind("file1", {"file2", "-o", "file3"});

    diff_stdout("case", "file");
    diff_stderr("case", "file");
    diff_generic("case", "file", "file2");

    analyze_stdout("analysis", "cmd");
    analyze_stderr("analysis", "cmd");
    analyze("analysis", "input", "cmd");

    log("text both");
    log("text teacher", "teacher");
    log("text student", "student");

    log_file("file");
    log_run_fail("[FAIL]");
    log_tag("nanecisto", "smula");
    log_valgrind("valgrind", "Valgrind:");
    subtest("nekorektni");

    tmp = session()->run_type();
    tmp = session()->has_tag("none");
    session()->add_summary("done");

    IF(valgrind()->grind_errors(),
       {
           log_valgrind("valgrind", "[VALGRIND] Chyba pri kontrole Valgrindem:");
       });
}

SESSION_NAME(tmp, ".", ".", {}, {}, false, false)

TEST_CASE("master_test") {
    Testing& cg = Testing::instance();
    cg.storage.nextFileName = "session.pl"; //Must be done before inicialization
    cg.setSession(tmp);
    ifstream sess("./session.pl");
    REQUIRE(sess.good());

    stringstream buffer;
    streambuf* old = cerr.rdbuf(buffer.rdbuf());

    SECTION("Correct test") {
        auto filename = "./unit_testing.pl";
        cg.storage.nextFileName = "unit_testing.pl"; //Must be done before inicialization

        auto tmp = cg.UnitTestInstance(normal);

        REQUIRE(tmp->__getClassName() == string("normal"));

        CHECK_NOTHROW(tmp->execute());

        ifstream generated(filename);
        REQUIRE(generated.good());

        const char* result =
R"delimiter($unit_test->name('unit_testing.pl');
my $matrix = 'matrix.cpp';
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

TEST_CASE("functions") {
    Testing& cg = Testing::instance();
    cg.storage.nextFileName = "session.pl"; //Must be done before inicialization
    cg.setSession(tmp);
    ifstream sess("./session.pl");
    REQUIRE(sess.good());

    stringstream buffer;
    streambuf* old = cerr.rdbuf(buffer.rdbuf());

    SECTION("Correct test") {
        auto filename = "./functions.pl";
        cg.storage.nextFileName = "functions.pl"; //Must be done before inicialization

        auto tmp = cg.UnitTestInstance(functions);

        REQUIRE(tmp->__getClassName() == string("functions"));

        CHECK_NOTHROW(tmp->execute());

        ifstream generated(filename);
        REQUIRE(generated.good());

        const char* result =
R"delimiter($unit_test->name('functions.pl');
my $tmp = '';
$unit_test->add_tag('test');
$unit_test->add_points('points' => 1);
$tmp = $unit_test->work_path;
$tmp = $unit_test->file_path;
$tmp = $unit_test->compilation->result;
$unit_test->extra_compiler_flags = 'flag';
$unit_test->compilation_log_errors = 1;
$tmp = $unit_test->execution->success;
$tmp = ($unit_test->difference->exit_value) != 0;
$tmp = ($unit_test->analysis->exit_type) eq 'normal';
$unit_test->compile();
$unit_test->run('/dev/null');
$unit_test->run('file1', 'file2', '-o', 'file3');
$unit_test->run_grind('em');
$unit_test->run_grind('file1', 'file2', '-o', 'file3');
$unit_test->diff_stdout('case', 'file');
$unit_test->diff_stderr('case', 'file');
$unit_test->diff_generic('case', 'file', 'file2');
$unit_test->analyze_stdout('analysis', 'cmd');
$unit_test->analyze_stderr('analysis', 'cmd');
$unit_test->analyze('analysis', 'input', 'cmd');
$unit_test->log('text both', 'both');
$unit_test->log('text teacher', 'teacher');
$unit_test->log('text student', 'student');
$unit_test->log_file('file', 'both');
$unit_test->log_run_fail('[FAIL]');
$unit_test->log_tag('nanecisto', 'smula', 'both');
$unit_test->log_valgrind('valgrind', 'Valgrind:', 'both');
$unit_test->subtest('nekorektni');
$tmp = $session->run_type;
$tmp = $session->has_tag('none');
$session->add_summary('done');
if ($unit_test->valgrind->grind_errors) {
	$unit_test->log_valgrind('valgrind', '[VALGRIND] Chyba pri kontrole Valgrindem:', 'both');
}
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


