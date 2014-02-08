#include "common.h"
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

#include <fstream>
#include <array>

using namespace std;

MASTER_TEST(nanecisto) {
    name("nanecisto.pl");
}

MASTER_TEST(naostro) {
    name("naostro.pl");
}

SESSION_NAME(both, ".", "files", {nanecisto}, {naostro}, false, false)

TEST_CASE("master_test") {
    array<const char*, 3> filenames = {"./session.pl", "./nanecisto.pl", "./naostro.pl"};
    Testing &cg = Testing::instance();
    cg.storage.names = kontr::Names::getAll(both, cg);
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

SESSION_NAME(post_default, ".", "files", {}, {}, false, false)

SESSION_NAME(post_both, ".", "files", {}, {}, true, true)

SESSION_NAME(post_func, ".", "files", {}, {}, { add_summary("No points"); })

TEST_CASE("post_func") {
    Testing &cg = Testing::instance();
    cg.storage.names = kontr::Names::getAll(post_func, cg);
    cg.storage.nextFileName = cg.storage.names.session.c_str();
    cg.setSession(post_func);

    ifstream session;
    session.open("./session.pl");
    REQUIRE(session.good());

    cg.session->post_test();
    cg.session = nullptr;

    //Check contents
    const char* result =
R"delimiter(sub post_test {
$session->add_summary('No points');
}
)delimiter";
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
    REQUIRE(remove("./session.pl") == 0);
}

TEST_CASE("post_default") {
    Testing &cg = Testing::instance();
    cg.storage.names = kontr::Names::getAll(post_default, cg);
    cg.storage.nextFileName = cg.storage.names.session.c_str();
    cg.setSession(post_default);

    ifstream session;
    session.open("./session.pl");
    REQUIRE(session.good());

    cg.session->post_test();
    cg.session = nullptr;

    //Check contents
    const char* result =
R"delimiter(sub post_test {
if (($session->run_type) eq 'student') {
if ($session->has_tag('nanecisto')) {
$session->add_summary('* test nanecisto neprosel');
} else {
$session->add_summary('* test nanecisto prosel');
}
} else {
my $points = $session->get_points('points');
if (($session->has_tag('nanecisto')) || ($session->has_tag('naostro'))) {
$session->add_summary("* v testu byla nalezena chyba\n");
$session->add_summary(('* pocet bodu je: ' . $points) . "\n");
} else {
$session->add_summary("* test prosel kompletne spravne\n");
$session->add_summary(('* pocet bodu za funcionalitu je: ' . $points) . "\n");
}
}
}
)delimiter";
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
    REQUIRE(remove("./session.pl") == 0);
}

TEST_CASE("post_both") {
    Testing &cg = Testing::instance();
    cg.storage.names = kontr::Names::getAll(post_both, cg);
    cg.storage.nextFileName = cg.storage.names.session.c_str();
    cg.setSession(post_both);

    ifstream session;
    session.open("./session.pl");
    REQUIRE(session.good());

    cg.session->post_test();
    cg.session = nullptr;

    //Check contents
    const char* result =
R"delimiter(sub post_test {
if (($session->run_type) eq 'student') {
if ($session->has_tag('nanecisto')) {
$session->add_summary('* test nanecisto neprosel');
} else {
$session->add_summary('* test nanecisto prosel');
}
} else {
my $points = $session->get_points('points');
if (($session->has_tag('nanecisto')) || ($session->has_tag('naostro'))) {
$session->add_summary("* v testu byla nalezena chyba\n");
$session->add_summary(('* pocet bodu je: ' . $points) . "\n");
} else {
$session->add_summary("* test prosel kompletne spravne\n");
$session->add_summary(('* pocet bodu za funcionalitu je: ' . $points) . "\n");
my $bonus = $session->get_points('bonus');
$session->add_summary(('* pocet bodu za bonus je: ' . $bonus) . "\n");
}
if ($session->has_tag('valgrind')) {
$session->add_summary("NEPROSLA kontrola Valgrindem, -1 bod\n");
} else {
$session->add_summary("Prosla kontrola Valgrindem.\n");
}
}
}
)delimiter";
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
    REQUIRE(remove("./session.pl") == 0);
}
