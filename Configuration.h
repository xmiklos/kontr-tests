#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "kontr.h"
#include <type_traits>

namespace kontr {
namespace Configuration {

/// Define configuration
#define CONFIGURATION(NAME, SESSION, MASTER, UNIT, VARIABLE, LANGUAGE, STORAGE, REPORTING) \
struct NAME { \
        typedef ::kontr::MasterTest::Delegator<NAME> MasterDelegator; \
        typedef ::kontr::UnitTest::Delegator<NAME> UnitDelegator; \
        typedef ::kontr::Session::Delegator<NAME> SessionDelegator; \
        typedef ::kontr::Variable::Delegator<NAME> VariableDelegator; \
        typedef ::kontr::Language::Delegator<NAME> LanguageDelegator; \
\
        typedef std::unique_ptr<MasterDelegator> MasterDelegatorInstance; \
        typedef std::unique_ptr<UnitDelegator> UnitDelegatorInstance; \
        typedef std::unique_ptr<SessionDelegator> SessionDelegatorInstance; \
        typedef std::unique_ptr<VariableDelegator> VariableDelegatorInstance; \
\
        typedef SESSION<NAME> Session; \
        typedef MASTER<NAME> Master; \
        typedef UNIT<NAME> Unit; \
        typedef VARIABLE<NAME> Variable; \
        typedef LANGUAGE<NAME> Language; \
\
        LanguageDelegator language; \
        STORAGE storage; \
        SessionDelegatorInstance session = nullptr; \
\
        void setSession(SessionDelegator::Function f) { \
            this->session = f(); \
        }\
        static MasterDelegatorInstance MasterTestInstance(MasterDelegator::Function f) { \
            return f(); \
        } \
        static UnitDelegatorInstance UnitTestInstance(UnitDelegator::Function f) { \
            return f(); \
        } \
\
        REPORTING ReportConfiguration; \
\
        ::kontr::Report::Reporter<NAME> report; \
        static NAME& instance() { \
            static NAME instance; \
            return instance; \
        }\
        \
    }

} //Configuration
} //kontr
#endif // CONFIGURATION_H
