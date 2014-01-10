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
    name("unit_one.pl");
}

UNIT_TEST(unit_2) {
    name("unit_two.pl");
}

MASTER_TEST(master_name) {
    name("name.pl");
}

MASTER_TEST(master_naostro) {
    name("naostro.pl");
    register_unit(unit_1);
    register_unit(unit_2);
}

MASTER_TEST(m_nopl) {
	name("nopl");
}

UNIT_TEST(u_nopl) {
	name("nopl");
}

SESSION("", "", {}, {}, false, false)

SESSION_NAME(complex, "", "", {master_name}, {master_naostro}, false, false)


TEST_CASE("Session") {
    CHECK(std::string("session.pl") == Names::get(::Session, Testing::instance()));
}

TEST_CASE("Master test") {
    CHECK(std::string("name.pl") == Names::get(master_name, Testing::instance()));

    SECTION("no name") {
        stringstream buffer;
        streambuf* old = cerr.rdbuf(buffer.rdbuf());
        CHECK_THROWS_AS(Names::get(master_empty, Testing::instance()), Report::ReportError);

        stringstream err;
        Report::print_report_text(err, Report::ERROR, string("No name found"));

        CHECK(err.str() == buffer.str().substr(0, err.str().size()));

        cerr.rdbuf(old);
    }
    
    SECTION("no extension") {
        stringstream buffer;
        streambuf* old = cerr.rdbuf(buffer.rdbuf());
        CHECK_THROWS_AS(Names::get(m_nopl, Testing::instance()), Report::ReportWarning);

        stringstream err;
        Report::print_report_text(err, Report::WARNING, 
			string("Master test name does not have .pl extension: nopl"));

        CHECK(err.str() == buffer.str().substr(0, err.str().size()));

        cerr.rdbuf(old);
    }
}

TEST_CASE("Unit test") {
    CHECK(std::string("unit_one.pl") == Names::get(unit_1, Testing::instance()));

    SECTION("no name") {
        stringstream buffer;
        streambuf* old = cerr.rdbuf(buffer.rdbuf());
        CHECK_THROWS_AS(Names::get(master_empty, Testing::instance()), Report::ReportError);

        stringstream err;
        Report::print_report_text(err, Report::ERROR, string("No name found"));

        CHECK(err.str() == buffer.str().substr(0, err.str().size()));

        cerr.rdbuf(old);
    }
    
    SECTION("no extension") {
        stringstream buffer;
        streambuf* old = cerr.rdbuf(buffer.rdbuf());
        CHECK_THROWS_AS(Names::get(u_nopl, Testing::instance()), Report::ReportWarning);

        stringstream err;
        Report::print_report_text(err, Report::WARNING, 
			string("Unit test name does not have .pl extension: nopl"));

        CHECK(err.str() == buffer.str().substr(0, err.str().size()));

        cerr.rdbuf(old);
    }
}

TEST_CASE("Complex") {
    ::kontr::Names::All res = Names::getAll(complex, Testing::instance());
    CHECK(res.session == "session.pl");
    CHECK(res.masterTests.size() == 2);

    CHECK(res.masterTests[0] == (make_tuple<string, string>("master_name", "name.pl")) );
    CHECK(res.masterTests[1] == (make_tuple<string, string>("master_naostro", "naostro.pl")) );

    CHECK(res.unitTests[0].size() == 0);
    CHECK(res.unitTests[1].size() == 2);

    CHECK(res.unitTests[1][0] == (make_tuple<string, string>("unit_1", "unit_one.pl")) );
    CHECK(res.unitTests[1][1] == (make_tuple<string, string>("unit_2", "unit_two.pl")) );
}
