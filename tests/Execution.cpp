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
    tmp = execution()->exit_type();

    execution()->log_stdout();
    execution()->log_stderr();
}

SESSION_NAME(tmp, ".", ".", {}, {})

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

    REQUIRE(tmp->__getClassName() == string("execution"));

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
