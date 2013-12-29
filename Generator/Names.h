#ifndef NAMES_H
#define NAMES_H
/// Get filename for given Session/MasterTest

#include "../kontr.h"

namespace kontr {
namespace Names {

class Stop : std::exception {};

template<typename T>
class MasterTest : public ::kontr::MasterTest::Empty<T> {
public:
    virtual void name(const char* name) {
        T::instance().storage.name = name;
        throw Stop();
    }
};

struct Storage {
    const char* name;
};

CONFIGURATION(Configuration,
              ::kontr::Session::Empty,
              ::kontr::Names::MasterTest,
              ::kontr::Variable::Empty,
              Storage,
              ::kontr::Report::Exception
             );

/// Get filename for Session - always session
const char* get(Configuration::SessionDelegator::Function f) {
    kontr::unused(f);
    return "session"; //Constant
}

/// Get filename for MasterTest
const char* get(Configuration::MasterDelegator::Function f) {
    try {
        auto test = Configuration::MasterTestInstance(f);
        test->execute();
    }
    catch (Stop e) {
        return Configuration::instance().storage.name;
    }
    Configuration::instance().report.create(Report::ERROR, "No name found", false);
    return nullptr;
}

} //Names
} //kontr
#endif // NAMES_H
