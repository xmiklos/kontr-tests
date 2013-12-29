#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "kontr.h"
#include <type_traits>

namespace kontr {
namespace Configuration {

/// Define configuration
#define CONFIGURATION(NAME, SESSION, MASTER, VARIABLE, REPORTING) \
struct NAME { \
        typedef ::kontr::MasterTest::Delegator<NAME> MasterDelegator; \
        typedef ::kontr::Session::Delegator<NAME> SessionDelegator; \
        typedef ::kontr::Variable::Delegator<NAME> VariableDelegator; \
\
        typedef std::unique_ptr<MasterDelegator> MasterDelegatorInstance; \
        typedef std::unique_ptr<SessionDelegator> SessionDelegatorInstance; \
        typedef std::unique_ptr<VariableDelegator> VariableDelegatorInstance; \
\
        typedef SESSION<NAME> Session; \
        typedef MASTER<NAME> Master; \
        typedef VARIABLE<NAME> Variable; \
\
        std::unique_ptr<SessionDelegator> session = nullptr; \
        std::ostream* out_ptr = nullptr; /*Needed for variable constructor*/ \
\
        void setSession(SessionDelegator::Function f) { \
            session = f(); \
        } \
\
        static MasterDelegatorInstance MasterTestInstance(MasterDelegator::Function f) { \
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

CONFIGURATION(Generation,
              ::kontr::Generator::Session,
              ::kontr::Generator::MasterTest,
              ::kontr::Generator::Variable,
              ::kontr::Report::Default
             );

} //Configuration
} //kontr
#endif // CONFIGURATION_H
