#ifndef KONTR_H
#define KONTR_H

/// File to be used by tests
/// Contains all kontr files, main and macros against using C control flow methods

#include "kontr-internal.h"

#define MAIN \
int main(void) { \
    using namespace std; \
    using namespace kontr; \
    cout << "Kontr Tests Generator & Verificator" << endl << endl; \
\
    Generator::Configuration &cg = Generator::Configuration::instance(); \
\
    cout << "Getting names of tests... "; \
    cg.storage.names = Names::getAll(::Session, cg); \
    cout << "done!" << endl; \
\
    cg.storage.nextFileName = cg.storage.names.session.c_str(); \
\
    cout << "Generating... "; \
    cg.setSession(::Session); \
    cg.session->pre_test(); \
    cg.session->post_test(); \
    cout << "done!" << endl; \
\
    cout << "Output scripts directory: " << cg.session->__getScriptsDir() << endl; \
}

//Has to be the last one
#include "warnings.h"

#endif // KONTR_H
