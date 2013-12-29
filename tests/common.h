#ifndef COMMON_H
#define COMMON_H

#include "kontr.h"

using namespace kontr;
using namespace std;

CONFIGURATION(Testing,
              ::kontr::Generator::Session,
              ::kontr::Generator::MasterTest,
              ::kontr::Generator::Variable,
              ::kontr::Configuration::ConfigurationException);

#endif // COMMON_H
