#include "Session.h"
#include <iostream>

namespace kontr {

using namespace std;

ISession::ISession(const char *script_dir, const char *files_dir, TMasterTests nanecisto, TMasterTests naostro, TPost post) :
    script_dir(script_dir), files_dir(files_dir),
    nanecisto(nanecisto), naostro(naostro),
    post(post)
{
    cout << "Script dir: " << script_dir << endl;
    cout << "Files dir: " << files_dir << endl;

    cout << "Pre test: " << endl;


    cout << "Nanecisto: " << endl;
    for (auto i : nanecisto) {
        ::kontr::Generator::MasterTest gen;
        ::kontr::MasterTestDelegator obj = i(gen);
        cout << " (" << obj.getClassName() << ") " << endl;
    }

    cout << "Naostro: " << endl;
    for (auto i : naostro) {
        ::kontr::Generator::MasterTest gen;
        ::kontr::MasterTestDelegator obj = i(gen);
        cout << " (" << obj.getClassName() << ") " << endl;
    }
}
}
