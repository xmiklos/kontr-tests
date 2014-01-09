#include "kontr.h"

UNIT_TEST(unit_diff) {
    name("unit_diff.pl");

    log("[TEST]\tHello");

    compile();

    //In case the compilation has failed, end the test
    IF (compilation()->result() != "clean",
        {
            add_tag("compilation");
            log_tag("nanecisto", "[FAIL]\tProgram se nepodarilo zkompilovat");
            RETURN();
        });

    //Otherwise continue and run the application
    run("/dev/null");

    IF (!execution()->success(),
        {
            log_run_fail("[FAIL]");
            add_tag("execution");
            add_tag("nanecisto");
            RETURN();
        });

    IF (execution()->exit_value() != 0,
        {
            log_tag("nanecisto", "[FAIL]\tTestovaci program skoncil s nenulovou navratovou hodnotou.");
            add_tag("exit_value");
            RETURN();
        });


    //Compare the output to the expected output file
    diff_stdout("casespace", "output");
    VAR(fail, difference()->exit_value());
    diff_stdout("casespace", "output2");
    IF (difference()->exit_value() != 0 && fail != 0,
        {
            log_tag("nanecisto","Neshodny vystup");
            add_tag("different_output");
            RETURN();
        });

    //Output in case of success
    log_tag("ok", "[OK]\tVystup je spravny, odevzdani funguje.");
    add_points("points", 1);
}

MASTER_TEST(master_nanecisto) {
    name("master_nanecisto.pl");

    stage_compiled_student_file("hello.cpp");

    register_unit(unit_diff);
}

/// USAGE: scripts_dir, files_dir, master test nanecisto, master tests naostro, post_test
/// OR instead of post test two boolean flags: valgrind, bonus
SESSION("bin/", "files/", {master_nanecisto}, {}, {
            IF(has_tag("nanecisto"),
            { add_summary("* test nanecisto neprosel\n"); },
            { add_summary("* test nanecisto prosel\n"); });
        })

MAIN
