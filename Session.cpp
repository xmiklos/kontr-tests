#include "Session.h"
#include <iostream>

namespace kontr {

using namespace std;

ISession::ISession(SessionConfiguration conf) :
    conf(conf)
{
    cout << "Script dir: " << conf.script_dir << endl;
    cout << "Files dir: " << conf.files_dir << endl;

    cout << "Pre test: " << endl;


    cout << "Nanecisto: " << endl;
    for (auto i : conf.nanecisto) {
        ::kontr::Generator::MasterTest gen;
        ::kontr::MasterTestDelegator obj = i(gen);
        cout << " (" << obj.getClassName() << ") " << endl;
    }

    cout << "Naostro: " << endl;
    for (auto i : conf.naostro) {
        ::kontr::Generator::MasterTest gen;
        ::kontr::MasterTestDelegator obj = i(gen);
        cout << " (" << obj.getClassName() << ") " << endl;
    }
}
}
