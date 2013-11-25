#include <iostream>
#include "kontr.h"

class Cl_master_testing : public kontr::MasterTestDelegator {
public:
    Cl_master_testing(IMasterTest &generator);
};
Cl_master_testing master_testing(kontr::IMasterTest& generator) {
    return Cl_master_testing(generator);
}
Cl_master_testing::Cl_master_testing(kontr::IMasterTest &generator) :
    kontr::MasterTestDelegator(generator, "master_testing") {
    name("master_testing");

    register_unit("unit_matrix_test2.pl");
    register_unit("unit_matrix_test3.pl");
    register_unit("unit_parser_test2.pl");

    stage_compiled_student_file("matrix.cpp");
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

class Cl_Session : public ::kontr::ISession {
public:
    Cl_Session();
};
Cl_Session Session() {
    return Cl_Session();
}
Cl_Session::Cl_Session() :
    kontr::ISession(
                    "script_dir",
                    "files_dir",
                    {master_testing},
                    {}
                )
{}

int main()
{
    Session();
    return 0;

}

