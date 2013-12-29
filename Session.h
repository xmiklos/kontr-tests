#ifndef SESSION_H
#define SESSION_H

#include "kontr.h"
#include <vector>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>

namespace kontr {
namespace Session {

/// Pure session interface - no data stored, arguments for constructor are discarded
/// Inteded to be direct parent for Delegator
template<typename T>
class Interface {
public:
    using TMasterTests = std::vector< typename T::MasterDelegator::Function > ;
    typedef std::function<void(const Interface&)> TPost;

    /**
     * Configure Session
     * @brief Session
     * @param script_dir Script dir location
     * @param files_dir Files dir location
     * @param nanecisto Master tests to be executed "nanecisto"
     * @param naostro Master tests to be executed "naostro" (apart from nanecisto tests)
     * @param post Optional post method
     */
    Interface(const char* script_dir, const char* files_dir,
             TMasterTests nanecisto, TMasterTests naostro,
             TPost post = nullptr)
    {
        ::kontr::unused(script_dir, files_dir, nanecisto, naostro, post);
    }

    virtual ~Interface() {}

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

/// Stores data for session
/// Intended to be used by classes passed to Delegator
template<typename T>
class Data : public Interface<T> {
public:
    using typename Interface<T>::TMasterTests;
    using typename Interface<T>::TPost;

    const std::string script_dir;
    const std::string files_dir;
    const TMasterTests nanecisto;
    const TMasterTests naostro;
    const TPost post;

    Data(const char* script_dir, const char* files_dir,
             TMasterTests nanecisto, TMasterTests naostro,
             TPost post = nullptr) :
        Interface<T>(script_dir, files_dir, nanecisto, naostro, post),
        script_dir(script_dir), files_dir(files_dir),
        nanecisto(nanecisto), naostro(naostro),
        post(post)
    {}

    virtual ~Data() {}
};

/// Empty data class - implements all the methods as empty functions
/// Intended to be used for auxiliary classes
template<typename T>
class Empty : public Data<T> {
public:
    using Data<T>::Data;

    virtual void pre_test() override {}
    virtual void post_test() override {}
};

/// Delegator class
/// Used as parent for Session
template<typename T>
class Delegator : public Interface<T> {
    typename T::Session delegate;
    const char* className;

public:
    using typename Interface<T>::TMasterTests;
    using typename Interface<T>::TPost;

    /// Type of a function returning unique pointer to this delegator type
    using Function = std::unique_ptr<Delegator> (*)();

protected:
    Delegator(const char* className, const char* script_dir, const char* files_dir,
                     TMasterTests nanecisto, TMasterTests naostro,
                     TPost post = nullptr) :
        Interface<T>(script_dir, files_dir, nanecisto, naostro, post),
        delegate(script_dir, files_dir, nanecisto, naostro, post),
        className(className)
    {}
public:
    /// Get class name of the session
    const char* getClassName() const {
        return className;
    }

    virtual void pre_test() {
        delegate.pre_test();
    }

    virtual void post_test() {
        delegate.post_test();
    }
};

} //Session
} //kontr
#endif // SESSION_H
