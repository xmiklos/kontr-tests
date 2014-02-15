#include "kontr.h"

//Diff of stdout
UNIT_TEST(unit_diff) {
    name("unit_diff.pl");
    VAR(input_file, "input");
    VAR(expected_file, "expected_output");
    VAR(fail_tag, "nanecisto"); //Or "naostro"
    VAR(detailed_output, "teacher"); //In case of all tests, this should be "both"
    IF (session()->run_type() == "teacher", { //This may be ommited for "naostro"
            detailed_output = "both";
    });
    VAR(detailed_info, "Studentsky main, diff stdout, vstupni soubor " + input_file + ", ocekavany vystup " + expected_file);

    //Stage student main - if this is common for all the unit tests in master tests, this line should be in master test
    stage_compiled_student_file("main.cpp");

    stage_file(input_file);
    stage_file(expected_file);

    log("[TEST] Test pomoci studentskeho mainu a diffu");

    compile();

    // In case the compilation failed, end the test
    IF (compilation()->result() != "clean", {
        log_tag("compilation", "[FAIL] Program se nepodarilo zkompilovat");
        add_tag(fail_tag);
        RETURN();
    });

    // Otherwise continue and run the application
    run(input_file);
    //OR with Valgrind:
    //run_grind(input_file);

    IF(!execution()->success(), {
        log_run_fail("[FAIL]");
        log(detailed_info, detailed_output);
        add_tag(fail_tag);
        add_tag("execution");
        RETURN();
    });
    IF(execution()->exit_value() != 0, {
        log_tag("exit_value", "[FAIL] Testovaci program skoncil s nenulovou navratovou hodnotou.");
        log(detailed_info, detailed_output);
        add_tag(fail_tag);
        RETURN();
    });

    // Compare the ouput to the expected output file
    diff_stdout("normal", expected_file);
    IF(difference()->exit_value() != 0, {
        // Output info and send log to the teacher in case of failure
        log_tag("diff_output", "[FAIL] Vystup testovaciho vstupu se neshoduje s ocekavanym vysledkem.");
        log(detailed_info, detailed_output);
        add_tag(fail_tag);
        difference()->log_stdout("teacher");
        RETURN();
    });

    //In case of Valgrind:
    //IF (valgrind()->grind_errors(), {
    //        log_valgrind("valgrind", "[VALGRIND] Chyba pri kontrole Valgrindem:");
    //    });

    //Output info in case of success
    log_tag("ok", "[OK] Vystup testovaciho vstupu se shoduje s ocekavanym vysledkem.");
    add_points("points", 1);
}

//Diff of created file
UNIT_TEST(unit_diff_files) {
    name("unit_diff_files.pl");
    VAR(input_file, "input");
    VAR(output_file, "output");
    VAR(expected_file, "expected_output");
    VAR(fail_tag, "nanecisto"); //Or "naostro"
    VAR(detailed_output, "teacher"); //In case of all tests, this should be "both"
    IF (session()->run_type() == "teacher", { //This may be ommited for "naostro"
            detailed_output = "both";
    });
    VAR(detailed_info, "Studentsky main, diff vytvoreneho souboru, vstupni soubor " + input_file +
        ", vystupni soubor " + output_file + ", ocekavany vystup " + expected_file);

    //Stage student main - if this is common for all the unit tests in master tests, this line should be in master test
    stage_compiled_student_file("main.cpp");

    stage_file(input_file);
    stage_file(expected_file);

    log("[TEST] Test pomoci diffu externich souboru");

    compile();

    // In case the compilation failed, end the test
    IF (compilation()->result() != "clean", {
        log_tag("compilation", "[FAIL] Program se nepodarilo zkompilovat");
        add_tag(fail_tag);
        RETURN();
    });

    // Otherwise continue and run the application
    run("/dev/null", {input_file, output_file});
    //OR with Valgrind:
    //run_grind("/dev/null", {input_file, output_file});

    IF(!execution()->success(), {
        log_run_fail("[FAIL]");
        log(detailed_info, detailed_output);
        add_tag(fail_tag);
        add_tag("execution");
        RETURN();
    });
    IF(execution()->exit_value() != 0, {
        log_tag("exit_value", "[FAIL] Testovaci program skoncil s nenulovou navratovou hodnotou.");
        log(detailed_info, detailed_output);
        add_tag(fail_tag);
        RETURN();
    });

    // Compare the ouput to the expected output file
    diff_generic("normal", output_file, expected_file);
    IF(difference()->exit_value() != 0, {
        // Output info and send log to the teacher in case of failure
        log_tag("diff_output", "[FAIL] Vystup testovaciho vstupu se neshoduje s ocekavanym vysledkem.");
        log(detailed_info, detailed_output);
        add_tag(fail_tag);
        difference()->log_stdout("teacher");
        RETURN();
    });

    //In case of Valgrind:
    //IF (valgrind()->grind_errors(), {
    //        log_valgrind("valgrind", "[VALGRIND] Chyba pri kontrole Valgrindem:");
    //    });

    //Output info in case of success
    log_tag("ok", "[OK] Vystup testovaciho vstupu se shoduje s ocekavanym vysledkem.");
    add_points("points", 1);
}

//Series of inputs
UNIT_TEST(unit_series) {
    name("unit_series.pl");

    VAR(fail_tag, "nanecisto"); //Or "naostro"
    VAR(detailed_output, "teacher"); //In case of all tests, this should be "both"
    IF(session()->run_type() == "teacher", {
           detailed_output = "both";
       }); //This may be ommited for "naostro"
    VAR(detailed_info, ""); //Will be set for every input

    VAR(i, 0);
    FOREACH(i, RANGE(1, 6), {
                stage_file("vstup_korektny_" + i + ".csv");
            });

    stage_student_file("sitemap.h");
    stage_compiled_student_file("main.c");
    stage_compiled_student_file("sitemap.h");

    compile();
    IF( compilation()->result() == "clean", {
            log_tag("compilation", "[FAIL] Program se nepodarilo zkompilovat");
            add_tag(fail_tag);
            RETURN();
        });

    FOREACH(i, RANGE(1, 6), {
            detailed_info = "Studentsky main, vstupni soubor vstup_korektny_" + i + ".csv, generovani SVG";
            subtest("vstup_" + i);
            log("[TEST]\tKorektni vstup " + i);
            run_grind("vstup_korektny_" + i + ".csv", { "output_" + i + ".svg" });
            IF (!execution()->success(), {
                log_run_fail("[FAIL]");
                log(detailed_info, detailed_output);
                add_tag(fail_tag);
                add_tag("execution");
                RETURN(); //If you want to run all tests, replace with CONTINUE();
            });
            IF (execution()->exit_value() != 0, {
                log_tag("exit_value", "[FAIL] Testovaci program skoncil s nenulovou navratovou hodnotou.");
                log(detailed_info, detailed_output);
                add_tag(fail_tag);
                RETURN(); //If you want to run all tests, replace with CONTINUE();
            });
            IF (valgrind()->grind_errors(), {
                log_valgrind("valgrind", "[VALGRIND] Chyba pri kontrole Valgrindem:");
            });
            add_attachment("output_" + i + ".svg", "teacher");
            log("Porovnejte prilohu output_" + i + ".svg se http://fi.muni.cz/~xbrukner/p12_pb071_vystupy/output_" + i + ".svg a udelte az jeden dalsi bod, pokud se dle zadani shoduje.", "teacher");
            log_tag("ok", "[OK] Test uspesne prosel");
            add_points("points", 0.1);
    });

    log("[OK] Test korektnich vstupu prosel.\n");
}

//Detection of bonus files
UNIT_TEST(unit_bonus_files_test) {
    name("unit_bonus_files_test.pl");

    log("[BONUS] Test existence bonusovych souboru.");

    IF(session()->available_file("main_bonus.c") &&
       session()->available_file("diffman_bonus.c") &&
       session()->available_file("diffman_bonus.h"), {
           log_tag("ok", "[OK] Soubory nalezeny");
       }, {
           log_tag("bonus_files", "[FAIL] Jeden nebo vice souboru chybi.");
       });
}

//Generic master test
MASTER_TEST(master_nanecisto) {
    name("master_nanecisto.pl");

    register_unit(unit_diff);
    register_unit(unit_diff_files);
    register_unit(unit_series);
    register_unit(unit_bonus_files_test);

    stage_file("BarCode.h");
    stage_compiled_file("BarCode.cpp");
    stage_student_file("BarCodeEAN13.h");
    stage_compiled_student_file("BarCodeEAN13.cpp");
}

//Generic session
SESSION("bin/", "files/", {master_nanecisto}, {}, true, false)

MAIN
