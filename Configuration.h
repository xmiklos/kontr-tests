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
};



}
#endif // CONFIGURATION_H
