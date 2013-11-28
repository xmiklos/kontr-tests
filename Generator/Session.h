#ifndef GENERATOR_SESSION_H
#define GENERATOR_SESSION_H

#include "../kontr.h"

namespace kontr {
namespace Generator {

template<typename T>
class Session : public ::kontr::ISession<T> {
    using ::kontr::ISession<T>::script_dir;
    using ::kontr::ISession<T>::files_dir;
    using ::kontr::ISession<T>::nanecisto;
    using ::kontr::ISession<T>::naostro;
    using ::kontr::ISession<T>::post;
public:

    using ::kontr::ISession<T>::ISession;

    virtual void pre_test() {
        using namespace std;
        cout << "Script dir: " << script_dir << endl;
        cout << "Files dir: " << files_dir << endl;

        cout << "Pre test: " << endl;


        cout << "Nanecisto: " << endl;
        for (auto i : nanecisto) {
            unique_ptr<typename T::MasterDelegator> obj(i());
            cout << " (" << obj->getClassName() << ") " << endl;
            obj->execute();
        }

        cout << "Naostro: " << endl;
        for (auto i : naostro) {
            unique_ptr<typename T::MasterDelegator> obj(i());
            cout << " (" << obj->getClassName() << ") " << endl;
            obj->execute();
        }
    }

    virtual void post_test() {
    }
};

#if 0
template<typename T>
void SessionGenerator<T>::configure(const char *script_dir, const char *files_dir, TMasterTests nanecisto, TMasterTests naostro, TPost post)
{
    this->script_dir = script_dir;
    this->files_dir = files_dir;
    this->nanecisto = nanecisto;
    this->naostro = naostro;
    this->post = post;
}
#endif

}
}

#endif // GENERATOR_SESSION_H
