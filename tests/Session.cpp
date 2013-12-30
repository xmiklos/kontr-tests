#include "common.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

#include <fstream>
#include <array>

using namespace std;

MASTER_TEST(nanecisto) {
    this->name("nanecisto");
}

MASTER_TEST(naostro) {
    this->name("naostro");
}

SESSION_NAME(both, ".", "files", {nanecisto}, {naostro})

TEST_CASE("master_test") {
    array<const char*, 3> filenames = {"./session.pl", "./nanecisto.pl", "./naostro.pl"};
    Testing &cg = Testing::instance();
    cg.storage.names = kontr::Names::getAll(both);
    cg.storage.nextFileName = cg.storage.names.session.c_str();
    cg.setSession(both);

    CHECK(cg.session->__getFilesDir() == std::string("files"));
    CHECK(cg.session->__getScriptsDir() == std::string("."));

    ifstream session, nanecisto, naostro;
    session.open(filenames[0]);
    nanecisto.open(filenames[1]);
    naostro.open(filenames[2]);

    REQUIRE(session.good());
    CHECK(!nanecisto.good());
    CHECK(!nanecisto.good());

    //Pre-test
    cg.session->pre_test();
    cg.session = nullptr;

    nanecisto.open(filenames[1]);
    naostro.open(filenames[2]);
    CHECK(nanecisto.good());
    CHECK(naostro.good());

    //Check contents
    const char* result =
    "sub pre_test {\n"
    "\t$session->register_master('nanecisto.pl');\n"
    "\tif($session->run_type eq 'teacher') {\n"
    "\t\t$session->register_master('naostro.pl');\n"
    "\t}\n"
    "}\n";
    stringstream buf;
    buf << result;

    while(session.good() && buf.good()) {
        string gen, res;
        getline(session, gen);
        getline(buf, res);
        CHECK(gen == res);
    }
    CHECK(session.eof());
    CHECK(buf.eof());

    // Cleanup
    session.close();
    nanecisto.close();
    naostro.close();
    REQUIRE(remove(filenames[0]) == 0);
    REQUIRE(remove(filenames[1]) == 0);
    REQUIRE(remove(filenames[2]) == 0);
}
