#include "common.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

using namespace kontr;
using namespace std;

MASTER_TEST(master_empty) {
}

MASTER_TEST(master_name) {
    name("name");
}

MASTER_TEST(master_naostro) {
    name("naostro");
}

SESSION("", "", {}, {})

SESSION_NAME(complex, "", "", {master_name}, {master_naostro})

TEST_CASE("Session") {
    CHECK(std::string("session") == Names::get(::Session));
}

TEST_CASE("Master test") {
    CHECK(std::string("name") == Names::get(master_name));

    SECTION("no name") {
        stringstream buffer;
        streambuf* old = cerr.rdbuf(buffer.rdbuf());
        CHECK_THROWS_AS(Names::get(master_empty), Report::ReportError);

        stringstream err;
        Report::print_report_text(err, Report::ERROR, string("No name found"));

        CHECK(err.str() == buffer.str().substr(0, err.str().size()));

        cerr.rdbuf(old);
    }
}

TEST_CASE("Complex") {
    ::kontr::Names::All res = Names::getAll(complex);
    CHECK(res.session == "session");
    CHECK(res.masterTests.size() == 2);

    CHECK(res.masterTests["master_name"] == "name");
    CHECK(res.masterTests["master_naostro"] == "naostro");
}
