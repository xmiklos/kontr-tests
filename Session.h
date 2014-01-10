#ifndef SESSION_H
#define SESSION_H

#include "kontr-internal.h"

namespace kontr {
namespace Session {

/// Pure session interface - no data stored, arguments for constructor are discarded
/// Inteded to be direct parent for Delegator
template<typename T>
class Interface {
public:
    using TMasterTests = std::vector< typename ::kontr::MasterTest::Delegator<T>::Function > ;
    using Variable = typename ::kontr::Variable::Delegator<T>;
    typedef std::function<void()> TPost;

    /**
     * Configure Session
     * @brief Session
     * @param scripts_dir Scripts dir location
     * @param files_dir Files dir location
     * @param nanecisto Master tests to be executed "nanecisto"
     * @param naostro Master tests to be executed "naostro" (apart from nanecisto tests)
     * @param post Post method
     */
    Interface(const char* scripts_dir, const char* files_dir,
             TMasterTests nanecisto, TMasterTests naostro,
             TPost post)
    {
        ::kontr::unused(scripts_dir, files_dir, nanecisto, naostro, post);
    }

    /**
      Configure Session
     * @brief Session
     * @param scripts_dir Scripts dir location
     * @param files_dir Files dir location
     * @param nanecisto Master tests to be executed "nanecisto"
     * @param naostro Master tests to be executed "naostro" (apart from nanecisto tests)
     * @param valgrind Is Valgrind used?
     * @param bonus Is bonus used?
     */
    Interface(const char *scripts_dir, const char *files_dir,
              TMasterTests nanecisto, TMasterTests naostro,
              bool valgrind, bool bonus) {
        ::kontr::unused(scripts_dir, files_dir, nanecisto, naostro, valgrind, bonus);
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

    /**
     * Return scripts dir
     * @brief __getScriptsDir
     * @return
     */
    virtual const std::string& __getScriptsDir() const = 0;

    /**
     * Return files dir
     * @brief __getFilesDir
     * @return
     */
    virtual const std::string& __getFilesDir() const = 0;

    /**
      Return run type - string, either student or teacher
     * @brief run_type
     * @return
     */
    virtual Variable run_type() = 0;

    /**
      Check whether tag (string) exists (bool)
     * @brief has_tag
     * @param tag
     * @return
     */
    virtual Variable has_tag(Variable tag) = 0;

    /**
      Add summary to email
     * @brief add_summary
     * @param message
     */
    virtual void add_summary(Variable message) = 0;

    /**
      Get summary of points
     * @brief get_points
     * @param name
     * @return
     */
    virtual Variable get_points(Variable name) = 0;
};

/// Stores data for session
/// Intended to be used by classes passed to Delegator
template<typename T>
class Data : public Interface<T> {
public:
    using typename Interface<T>::TMasterTests;
    using typename Interface<T>::TPost;

    const std::string scripts_dir;
    const std::string files_dir;
    const TMasterTests nanecisto;
    const TMasterTests naostro;
    ///If this is not nullptr, it is a valid callback and valgrind and bonus are false
    const TPost post;
    const bool valgrind;
    const bool bonus;

    Data(const char* scripts_dir, const char* files_dir,
             TMasterTests nanecisto, TMasterTests naostro,
             TPost post) :
        Interface<T>(scripts_dir, files_dir, nanecisto, naostro, post),
        scripts_dir(scripts_dir), files_dir(files_dir),
        nanecisto(nanecisto), naostro(naostro),
        post(post), valgrind(false), bonus(false)
    {}

    Data(const char *scripts_dir, const char *files_dir,
            TMasterTests nanecisto, TMasterTests naostro,
            bool valgrind, bool bonus) :
        Interface<T>(scripts_dir, files_dir, nanecisto, naostro, valgrind, bonus),
        scripts_dir(scripts_dir), files_dir(files_dir),
        nanecisto(nanecisto), naostro(naostro),
        post(nullptr), valgrind(valgrind), bonus(bonus)
    {}

    virtual ~Data() {}

    virtual const std::string& __getScriptsDir() const override { return scripts_dir; }
    virtual const std::string& __getFilesDir() const override { return files_dir; }
};

/// Empty data class - implements all the methods as empty functions
/// Intended to be used for auxiliary classes
template<typename T>
class Empty : public Data<T> {
public:
    using Data<T>::Data;

    virtual void pre_test() override {}
    virtual void post_test() override {}
    using typename Interface<T>::Variable;
    virtual Variable run_type() override { return ""; }
    virtual Variable has_tag(Variable tag) override { kontr::unused(tag); return false; }
    virtual void add_summary(Variable message) override { kontr::unused(message); }
    virtual Variable get_points(Variable points) override { kontr::unused(points); return false; }
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
    using Function = void (*)(std::unique_ptr<Delegator>&);

protected:
    Delegator(const char* className, const char* scripts_dir, const char* files_dir,
                     TMasterTests nanecisto, TMasterTests naostro,
                     TPost post) :
        Interface<T>(scripts_dir, files_dir, nanecisto, naostro, post),
        delegate(scripts_dir, files_dir, nanecisto, naostro, post),
        className(className)
    {}

    Delegator(const char* className, const char* scripts_dir, const char* files_dir,
                     TMasterTests nanecisto, TMasterTests naostro,
                     bool valgrind, bool bonus) :
        Interface<T>(scripts_dir, files_dir, nanecisto, naostro, valgrind, bonus),
        delegate(scripts_dir, files_dir, nanecisto, naostro, valgrind, bonus),
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

    virtual const std::string& __getScriptsDir() const {
        return delegate.__getScriptsDir();
    }

    virtual const std::string& __getFilesDir() const {
        return delegate.__getFilesDir();
    }

    using typename Interface<T>::Variable;
    virtual Variable run_type() override {
        return delegate.run_type();
    }

    virtual Variable has_tag(Variable tag) override {
        return delegate.has_tag(tag);
    }

    virtual void add_summary(Variable message) override {
        delegate.add_summary(message);
    }

    virtual Variable get_points(Variable points) override {
        return delegate.get_points(points);
    }
};

} //Session
} //kontr
#endif // SESSION_H
