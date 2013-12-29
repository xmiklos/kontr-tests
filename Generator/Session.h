#ifndef GENERATOR_SESSION_H
#define GENERATOR_SESSION_H

#include "../kontr.h"

namespace kontr {
namespace Generator {

template<typename T>
class Session : public ::kontr::Session::Data<T> {
    using ::kontr::Session::Data<T>::scripts_dir;
    using ::kontr::Session::Data<T>::files_dir;
    using ::kontr::Session::Data<T>::nanecisto;
    using ::kontr::Session::Data<T>::naostro;
    using ::kontr::Session::Data<T>::post;
public:
    using ::kontr::Session::Data<T>::Data;

    virtual void pre_test() {
        using namespace std;
        cout << "Scripts dir: " << scripts_dir << endl;
        cout << "Files dir: " << files_dir << endl;

        cout << "Pre test: " << endl;


        cout << "Nanecisto: " << endl;
        for (auto i : nanecisto) {
            auto obj = T::MasterTestInstance(i);
            //unique_ptr<typename T::MasterDelegator> obj(i(this->instance));
            cout << " (" << obj->getClassName() << ") " << endl;
            obj->execute();
        }

        cout << "Naostro: " << endl;
        for (auto i : naostro) {
            auto obj = T::MasterTestInstance(i);
            //unique_ptr<typename T::MasterDelegator> obj(i(this->instance));
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

} //Generator
} //kontr
#endif // GENERATOR_SESSION_H
