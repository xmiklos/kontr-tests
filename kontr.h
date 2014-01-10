#ifndef KONTR_H
#define KONTR_H

/// File to be used by tests
/// Contains all kontr files, main and macros against using C control flow methods

#include "kontr-internal.h"

namespace kontr {
    /// Main for the MAIN macro.
    void main(
            Names::Configuration::SessionDelegator::Function NameSession,
             Generator::Configuration::SessionDelegator::Function GeneratorSession
            ) {
        //Both parameters must have explicit type, so the type inference works

        using namespace std;
        using namespace kontr;
        cout << "Kontr Tests Generator & Verificator" << endl << endl;
        Generator::Configuration &cg = Generator::Configuration::instance();

        cout << "Getting names of tests... ";
        cg.storage.names = Names::getAll(NameSession, cg);
        cout << "done!" << endl;

        cg.storage.nextFileName = cg.storage.names.session.c_str();

        cout << "Generating... ";
        cg.setSession(GeneratorSession);
        cg.session->pre_test();
        cg.session->post_test();
        cout << "done!" << endl;

        cout << "Output scripts directory: " << cg.session->__getScriptsDir() << endl;
        exit(0); //Since ::main does not return anything
    }
}

#define MAIN \
int main(void) { \
    ::kontr::main(::Session, ::Session); \
}

//Has to be the last one
#include "warnings.h"

#endif // KONTR_H
