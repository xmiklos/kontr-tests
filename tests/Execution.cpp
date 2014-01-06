#include "common.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstring>

using namespace kontr;
using namespace std;

UNIT_TEST(execution) {
    name("unit_execution");

    VAR(tmp, "");
    tmp = execution()->cmd();
    tmp = execution()->stdin_path();
    tmp = execution()->stdout_path();
    tmp = execution()->stderr_path();
    tmp = execution()->work_path();
    tmp = execution()->success();
    tmp = execution()->exit_value();
    tmp = execution()->exit_type();

    execution()->log_stdout();
    execution()->log_stderr();
}

UNIT_TEST(compilation) {
    name("unit_compilation");

    VAR(tmp, "");
    tmp = compilation()->cmd();
    tmp = compilation()->stdin_path();
    tmp = compilation()->stdout_path();
    tmp = compilation()->stderr_path();
    tmp = compilation()->work_path();
    tmp = compilation()->success();
    tmp = compilation()->exit_value();
    tmp = compilation()->exit_type();
    tmp = compilation()->result();

    compilation()->log_stdout();
    compilation()->log_stderr();
}

SESSION_NAME(tmp, ".", ".", {}, {}, false, false)

TEST_CASE("execution") {
    Testing& cg = Testing::instance();
    cg.storage.nextFileName = "session"; //Must be done before inicialization
    cg.setSession(tmp);
    ifstream sess("./session.pl");
    REQUIRE(sess.good());

    stringstream buffer;
    streambuf* old = cerr.rdbuf(buffer.rdbuf());

    auto filename = "./unit_execution.pl";
    cg.storage.nextFileName = "unit_execution"; //Must be done before inicialization

    auto tmp = cg.UnitTestInstance(execution);

    CHECK_NOTHROW(tmp->execute());

    ifstream generated(filename);
    REQUIRE(generated.good());

    const char* result =
R"delimiter($unit_test->name('unit_execution');
$tmp = '';
$tmp = $unit_test->execution->cmd;
$tmp = $unit_test->execution->stdin_path;
$tmp = $unit_test->execution->stdout_path;
$tmp = $unit_test->execution->stderr_path;
$tmp = $unit_test->execution->work_path;
$tmp = $unit_test->execution->success;
$tmp = $unit_test->execution->exit_value;
$tmp = $unit_test->execution->exit_type;
$unit_test->execution->log_stdout();
$unit_test->execution->log_stderr();
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


    sess.close();
    cg.session = nullptr;
    CHECK(remove("./session.pl") == 0);

    cerr.rdbuf(old);
}

TEST_CASE("compilation") {
    Testing& cg = Testing::instance();
    cg.storage.nextFileName = "session"; //Must be done before inicialization
    cg.setSession(tmp);
    ifstream sess("./session.pl");
    REQUIRE(sess.good());

    stringstream buffer;
    streambuf* old = cerr.rdbuf(buffer.rdbuf());

    auto filename = "./unit_compilation.pl";
    cg.storage.nextFileName = "unit_compilation"; //Must be done before inicialization

    auto tmp = cg.UnitTestInstance(compilation);
    CHECK_NOTHROW(tmp->execute());

    ifstream generated(filename);
    REQUIRE(generated.good());

    const char* result =
R"delimiter($unit_test->name('unit_compilation');
$tmp = '';
$tmp = $unit_test->compilation->cmd;
$tmp = $unit_test->compilation->stdin_path;
$tmp = $unit_test->compilation->stdout_path;
$tmp = $unit_test->compilation->stderr_path;
$tmp = $unit_test->compilation->work_path;
$tmp = $unit_test->compilation->success;
$tmp = $unit_test->compilation->exit_value;
$tmp = $unit_test->compilation->exit_type;
$tmp = $unit_test->compilation->result;
$unit_test->compilation->log_stdout();
$unit_test->compilation->log_stderr();
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


    sess.close();
    cg.session = nullptr;
    CHECK(remove("./session.pl") == 0);

    cerr.rdbuf(old);
}
