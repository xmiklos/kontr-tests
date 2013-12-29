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

SESSION("", "", {}, {})

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
