#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "kontr.h"
#include <type_traits>

namespace kontr {

struct ConfigurationGeneration {
    typedef ::kontr::Generator::Session<ConfigurationGeneration> Session;
    typedef ::kontr::Generator::MasterTest<ConfigurationGeneration> Master;

    typedef ::kontr::MasterTestDelegator<ConfigurationGeneration> MasterDelegator;
    typedef ::kontr::SessionDelegator<ConfigurationGeneration> SessionDelegator;

    ::kontr::Report::ReportConfiguration <
                ::kontr::Report::Reporting::ERR,
                ::kontr::Report::Reporting::ERR,
                ::kontr::Report::Reporting::ERR_ABORT> ReportConfiguration;

    ::kontr::Report::Reporter<ConfigurationGeneration> report = (*this);
};

}
#endif // CONFIGURATION_H
