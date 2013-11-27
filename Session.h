#ifndef SESSION_H
#define SESSION_H

#include "kontr.h"
#include <vector>
#include <functional>
#include <iostream>
#include <memory>

namespace kontr {

template <typename T>
class ISession {
public:
    typedef const std::vector< ::kontr::MasterTestDelegator<T>* (*)() > TMasterTests;
    typedef const std::function<void(const ISession&)> TPost;
private:
    const char* script_dir;
    const char* files_dir;
    TMasterTests nanecisto;
    TMasterTests naostro;
    TPost post;
public:

    ISession(const char* script_dir, const char* files_dir,
             TMasterTests nanecisto, TMasterTests naostro,
             TPost post = nullptr);

};

template<typename T>
ISession<T>::ISession(const char *script_dir, const char *files_dir, TMasterTests nanecisto, TMasterTests naostro, TPost post) :
    script_dir(script_dir), files_dir(files_dir),
    nanecisto(nanecisto), naostro(naostro),
    post(post)
{
    using namespace std;
    cout << "Script dir: " << script_dir << endl;
    cout << "Files dir: " << files_dir << endl;

    cout << "Pre test: " << endl;


    cout << "Nanecisto: " << endl;
    for (auto i : nanecisto) {
        unique_ptr<::kontr::MasterTestDelegator<T>> obj(i());
        cout << " (" << obj->getClassName() << ") " << endl;
        obj->execute();
    }

    cout << "Naostro: " << endl;
    for (auto i : naostro) {
        unique_ptr<::kontr::MasterTestDelegator<T>> obj(i());
        cout << " (" << obj->getClassName() << ") " << endl;
        obj->execute();
    }
}

}

#endif // SESSION_H
