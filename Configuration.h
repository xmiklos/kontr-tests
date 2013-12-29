#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "kontr.h"
#include <type_traits>

namespace kontr {

namespace Configuration {

typedef ::kontr::Report::ReportConfiguration<
    ::kontr::Report::Reporting::ERR,
    ::kontr::Report::Reporting::ERR,
    ::kontr::Report::Reporting::ERR_ABORT> ConfigurationDefault;

typedef ::kontr::Report::ReportConfiguration<
    ::kontr::Report::Reporting::ERR_EXCEPTION,
    ::kontr::Report::Reporting::ERR_EXCEPTION,
    ::kontr::Report::Reporting::ERR_EXCEPTION> ConfigurationException;

/// Define configuration
#define CONFIGURATION(NAME, SESSION, MASTER, VARIABLE, CONFIGURATION) \
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
        std::ostream* out_ptr = nullptr; /*Needed for variable constructor*/ \
\
        void setSession(SessionDelegator::Function f) { \
            session = std::unique_ptr<SessionDelegator>(f(*this)); \
        } \
\
        MasterDelegatorInstance MasterTestInstance(MasterDelegator::Function f) { \
            return MasterDelegatorInstance(f(*this)); \
        } \
\
        CONFIGURATION ReportConfiguration; \
\
        ::kontr::Report::Reporter<NAME> report = (*this); \
        \
    }

CONFIGURATION(Generation,
              ::kontr::Generator::Session,
              ::kontr::Generator::MasterTest,
              ::kontr::Generator::Variable,
              ConfigurationDefault
             );

}

}
#endif // CONFIGURATION_H
