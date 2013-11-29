#include <iostream>
#include "kontr.h"
#include "backtracexx/backtracexx.hpp"

template<typename T>
class Cl_master_testing : public kontr::MasterTestDelegator<T> {
public:
    Cl_master_testing(T& instance) : kontr::MasterTestDelegator<T>(instance, "master_testing") {}
    virtual void execute () override;
    using kontr::MasterTestDelegator<T>::name;
    using kontr::MasterTestDelegator<T>::register_unit;
    using kontr::MasterTestDelegator<T>::stage_compiled_file;
    using kontr::MasterTestDelegator<T>::stage_compiled_student_file;
    using kontr::MasterTestDelegator<T>::stage_file;
    using kontr::MasterTestDelegator<T>::stage_student_file;
};
template<typename T>
kontr::MasterTestDelegator<T>* master_testing(T& instance) {
    return new Cl_master_testing<T>(instance);
}
template<typename T>
void Cl_master_testing<T>::execute () {
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

template <typename T>
class Cl_Session : public ::kontr::SessionDelegator<T> {
public:
    using ::kontr::SessionDelegator<T>::pre_test;
    using ::kontr::SessionDelegator<T>::post_test;
    Cl_Session(T& instance);
};
template <typename T>
::kontr::SessionDelegator<T>* Session(T& instance) {
    return new Cl_Session<T>(instance);
}
template <typename T>
Cl_Session<T>::Cl_Session(T &instance) :
    ::kontr::SessionDelegator<T>(instance,
                ".",
                "files_dir",
                {master_testing},
                {}
             )
{}

#if 0
#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch.hpp"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}
#endif


int main()
{

    kontr::ConfigurationGeneration cg;
    //cg.report.create(kontr::Report::NOTICE, "test");
    //cg.report.suppress(kontr::Report::NOTICE, "test", [&] {} );
    //cg.report.suppress(kontr::Report::NOTICE, "test", [&] { cg.report.create(kontr::Report::NOTICE, "test"); } );

    cg.setSession(Session);
    cg.session->pre_test();

    //std::cout << backtracexx::scan() << std::endl;
    return 0;

}
