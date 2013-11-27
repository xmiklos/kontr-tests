#include "Session.h"
#include <iostream>

namespace kontr {

using namespace std;

#if 0
template<typename T>
ISession<T>::ISession(const char *script_dir, const char *files_dir, TMasterTests nanecisto, TMasterTests naostro, TPost post) :
    script_dir(script_dir), files_dir(files_dir),
    nanecisto(nanecisto), naostro(naostro),
    post(post)
{
    cout << "Script dir: " << script_dir << endl;
    cout << "Files dir: " << files_dir << endl;

    cout << "Pre test: " << endl;


    cout << "Nanecisto: " << endl;
    for (auto i : nanecisto) {
        cout << " (" << i.getClassName() << ") " << endl;
    }

    cout << "Naostro: " << endl;
    for (auto i : naostro) {
        cout << " (" << i.getClassName() << ") " << endl;
    }
}
#endif
}
