#ifndef SESSION_H
#define SESSION_H

#include "kontr.h"
#include <vector>
#include <functional>
#include <iostream>
#include <memory>

namespace kontr {

template<typename T>
class ISession {
public:
    using TMasterTests = std::vector< typename T::MasterDelegator* (*)() > ;
    typedef std::function<void(const ISession&)> TPost;
protected:
    const char* script_dir;
    const char* files_dir;
    TMasterTests nanecisto;
    TMasterTests naostro;
    TPost post;

    ISession<T>() = delete;
public:
    /**
     * Configure Session
     * @brief Session
     * @param script_dir Script dir location
     * @param files_dir Files dir location
     * @param nanecisto Master tests to be executed "nanecisto"
     * @param naostro Master tests to be executed "naostro" (apart from nanecisto tests)
     * @param post Optional post method
     */
    ISession(const char* script_dir, const char* files_dir,
             TMasterTests nanecisto, TMasterTests naostro,
             TPost post = nullptr) :
        script_dir(script_dir), files_dir(files_dir),
        nanecisto(nanecisto), naostro(naostro),
        post(post)
    {}

    /**
     * Execute pre_test - add master tests
     * @brief pre_test
     */
    virtual void pre_test() = 0;

    /**
     * Execute post_test - count points, add summary
     * @brief post_test
     */
    virtual void post_test() = 0;
};

template<typename T>
class SessionDelegator : public ISession<T> {
    typename T::Session delegate;
    using typename ISession<T>::TMasterTests;
    using typename ISession<T>::TPost;
public:
    SessionDelegator(const char* script_dir, const char* files_dir,
                     TMasterTests nanecisto, TMasterTests naostro,
                     TPost post = nullptr) :
        ISession<T>(script_dir, files_dir, nanecisto, naostro, post),
        delegate(script_dir, files_dir, nanecisto, naostro, post)

    {}

    virtual void pre_test() {
        delegate.pre_test();
    }

    virtual void post_test() {
        delegate.post_test();
    }
};

}

#endif // SESSION_H
