#ifndef COMMON_H
#define COMMON_H

#include "kontr.h"

using namespace kontr;
using namespace std;

CONFIGURATION(Testing,
              ::kontr::Generator::Session,
              ::kontr::Generator::MasterTest,
              ::kontr::Generator::Variable,
              <::kontr::Report::Reporting::ERR_EXCEPTION,
              ::kontr::Report::Reporting::ERR_EXCEPTION,
              ::kontr::Report::Reporting::ERR_EXCEPTION>);

#endif // COMMON_H
