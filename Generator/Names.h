#ifndef NAMES_H
#define NAMES_H
/// Get filename for given Session/MasterTest

#include "../kontr-internal.h"

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
    ::kontr::Report::Reporting warnings;
};

bool has_extension(const std::string& name) {
	return name.length() > 3 && name.substr(name.length() - 3) == ".pl";
}

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
		if (!has_extension(name)) {
            T::instance().report._create(Report::WARNING,
                std::string("Master test name does not have .pl extension: ") + name,
                                         T::instance().storage.warnings, false);
		}
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
		if (!has_extension(name)) {
            T::instance().report._create(Report::WARNING,
                std::string("Unit test name does not have .pl extension: ") + name,
                                        T::instance().storage.warnings, false);
		}
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
template <typename T>
const char* get(Configuration::SessionDelegator::Function f, const T& Conf) {
    kontr::unused(f, Conf);
    return "session.pl"; //Constant
}

/// Get filename for MasterTest
template <typename T>
const char* get(Configuration::MasterDelegator::Function f, const T& Conf) {
    Configuration::instance().report.suppressMacroWarnings(true);
    Configuration::instance().storage.warnings = Conf.ReportConfiguration.report[Report::WARNING];

    Configuration::instance().storage.all = false;
    try {
        auto test = Configuration::MasterTestInstance(f);
        test->execute();
    }
    catch (Stop) {
        return Configuration::instance().storage.name;
    }
    Configuration::instance().report._create(Report::ERROR, "No name found",
                                            Conf.ReportConfiguration.report[Report::ERROR], false);
    return nullptr;
}

/// Get filename for UnitTest
template <typename T>
const char* get(Configuration::UnitDelegator::Function f, const T& Conf) {
    Configuration::instance().report.suppressMacroWarnings(true);
    Configuration::instance().storage.warnings = Conf.ReportConfiguration.report[Report::WARNING];

    Configuration::instance().storage.all = false;
    try {
        auto test = Configuration::UnitTestInstance(f);
        test->execute();
    }
    catch (Stop) {
        return Configuration::instance().storage.name;
    }

    Configuration::instance().report._create(Report::ERROR, "No name found",
                                            Conf.ReportConfiguration.report[Report::ERROR], false);
    return nullptr;
}

/// Get all names
template <typename T>
All getAll(Configuration::SessionDelegator::Function f, const T& Conf) {
    Configuration &cg = Configuration::instance();
    cg.report.suppressMacroWarnings(true);

    cg.storage.data.session = get(f, Conf);
    cg.storage.all = true;
    cg.storage.data.currentMasterIndex = 0;
    cg.setSession(f);
    cg.session->pre_test();
    return cg.storage.data;
}

} //Names
} //kontr
#endif // NAMES_H
