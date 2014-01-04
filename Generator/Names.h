#ifndef NAMES_H
#define NAMES_H
/// Get filename for given Session/MasterTest

#include "../kontr.h"
#include <vector>
#include <tuple>
#include <map>

namespace kontr {
namespace Names {

class Stop : std::exception {};

struct All {
    /// Tuple <class name, string from name()>
    typedef std::tuple<std::string, std::string> Name;
    std::string session;
    std::vector<Name> masterTests;
    std::map<unsigned, std::vector<Name>> unitTests;
    unsigned currentMasterIndex;
};

struct NameStorage {
    const char* name;
    bool all;
    const char* className;
    All data;
};

template<typename T>
class Session : public ::kontr::Session::Empty<T> {
public:
    using ::kontr::Session::Empty<T>::Empty;

    virtual void pre_test() {
        if (!T::instance().storage.all) return;
        NameStorage& storage = T::instance().storage;
        storage.data.currentMasterIndex = 0;

        for (auto test : this->nanecisto) {
            typename T::MasterDelegatorInstance instance = T::MasterTestInstance(test);
            storage.className = instance->__getClassName();
            instance->execute();
            ++ storage.data.currentMasterIndex;
        }

        for (auto test : this->naostro) {
            typename T::MasterDelegatorInstance instance = T::MasterTestInstance(test);
            storage.className = instance->__getClassName();
            instance->execute();
            ++ storage.data.currentMasterIndex;
        }
    }
};

template<typename T>
class MasterTest : public ::kontr::MasterTest::Empty<T> {
public:
    using typename ::kontr::MasterTest::Empty<T>::Unit;

    virtual void name(const char* name) {
        if (T::instance().storage.all) {
            NameStorage& storage = T::instance().storage;
            storage.data.masterTests.push_back(std::make_tuple<std::string, std::string>(storage.className, name));
        }
        else {
            T::instance().storage.name = name;
            throw Stop();
        }
    }

    virtual void register_unit(Unit unit) {
        NameStorage& storage = T::instance().storage;
        typename T::UnitDelegatorInstance instance = T::UnitTestInstance(unit);
        storage.className = instance->__getClassName();
        try {
            instance->execute();
        }
        catch (Stop) {}
    }
};

template<typename T>
class UnitTest : public ::kontr::UnitTest::Empty<T> {
public:
    virtual void name(const char* name) {
        if (T::instance().storage.all) {
            NameStorage& storage = T::instance().storage;
            storage.data.unitTests[storage.data.currentMasterIndex].push_back(
                            std::make_tuple<std::string, std::string>(storage.className, name)
                        );
            throw Stop();
        }
        else {
            T::instance().storage.name = name;
            throw Stop();
        }
    }
};

CONFIGURATION(Configuration,
              ::kontr::Names::Session,
              ::kontr::Names::MasterTest,
              ::kontr::Names::UnitTest,
              ::kontr::Exec::Empty,
              ::kontr::Variable::Empty,
              ::kontr::Language::Empty,
              NameStorage,
              ::kontr::Report::Exception
             );

/// Get filename for Session - always session
const char* get(Configuration::SessionDelegator::Function f) {
    kontr::unused(f);
    return "session"; //Constant
}

/// Get filename for MasterTest
const char* get(Configuration::MasterDelegator::Function f) {
    Configuration::instance().storage.all = false;
    try {
        auto test = Configuration::MasterTestInstance(f);
        test->execute();
    }
    catch (Stop) {
        return Configuration::instance().storage.name;
    }
    Configuration::instance().report.create(Report::ERROR, "No name found", false);
    return nullptr;
}

/// Get filename for UnitTest
const char* get(Configuration::UnitDelegator::Function f) {
    Configuration::instance().storage.all = false;
    try {
        auto test = Configuration::UnitTestInstance(f);
        test->execute();
    }
    catch (Stop) {
        return Configuration::instance().storage.name;
    }
    Configuration::instance().report.create(Report::ERROR, "No name found", false);
    return nullptr;
}


All getAll(Configuration::SessionDelegator::Function f) {
    Configuration &cg = Configuration::instance();
    cg.storage.data.session = get(f);
    cg.storage.all = true;
    cg.storage.data.currentMasterIndex = 0;
    cg.setSession(f);
    cg.session->pre_test();
    return cg.storage.data;
}

} //Names
} //kontr
#endif // NAMES_H
