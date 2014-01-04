#include "common.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

#include <tuple>

using namespace kontr;
using namespace std;

MASTER_TEST(master_empty) {
}

UNIT_TEST(unit_empty) {
}

UNIT_TEST(unit_1) {
    name("unit_one");
}

UNIT_TEST(unit_2) {
    name("unit_two");
}

MASTER_TEST(master_name) {
    name("name");
}

MASTER_TEST(master_naostro) {
    name("naostro");
    register_unit(unit_1);
    register_unit(unit_2);
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

TEST_CASE("Unit test") {
    CHECK(std::string("unit_one") == Names::get(unit_1));

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

    CHECK(res.masterTests[0] == (make_tuple<string, string>("master_name", "name")) );
    CHECK(res.masterTests[1] == (make_tuple<string, string>("master_naostro", "naostro")) );

    CHECK(res.unitTests[0].size() == 0);
    CHECK(res.unitTests[1].size() == 2);

    CHECK(res.unitTests[1][0] == (make_tuple<string, string>("unit_1", "unit_one")) );
    CHECK(res.unitTests[1][1] == (make_tuple<string, string>("unit_2", "unit_two")) );
}
