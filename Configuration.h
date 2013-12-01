#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "kontr.h"
#include <type_traits>

namespace kontr {

namespace Configuration {

/// Define configuration
#define CONFIGURATION(NAME, SESSION, MASTER, VARIABLE, ...) \
struct NAME { \
        typedef ::kontr::MasterTestDelegator<NAME> MasterDelegator; \
        typedef ::kontr::SessionDelegator<NAME> SessionDelegator; \
        typedef ::kontr::VariableDelegator<NAME> VariableDelegator; \
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
\
        void setSession(SessionDelegator::Function f) { \
            session = std::unique_ptr<SessionDelegator>(f(*this)); \
        } \
\
        MasterDelegatorInstance MasterTestInstance(MasterDelegator::Function f) { \
            return MasterDelegatorInstance(f(*this)); \
        } \
\
        ::kontr::Report::ReportConfiguration __VA_ARGS__ ReportConfiguration; \
\
        ::kontr::Report::Reporter<NAME> report = (*this); \
        \
    }

CONFIGURATION(Generation,
              ::kontr::Generator::Session,
              ::kontr::Generator::MasterTest,
              ::kontr::Generator::Variable,
              <::kontr::Report::Reporting::ERR,
              ::kontr::Report::Reporting::ERR,
              ::kontr::Report::Reporting::ERR_ABORT>
             );

}

}
#endif // CONFIGURATION_H
