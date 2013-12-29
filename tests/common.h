#ifndef COMMON_H
#define COMMON_H

#include "kontr.h"

using namespace kontr;
using namespace std;

CONFIGURATION(Testing,
              ::kontr::Generator::Session,
              ::kontr::Generator::MasterTest,
              ::kontr::Generator::Variable,
              char,
              ::kontr::Report::Exception);

#endif // COMMON_H
