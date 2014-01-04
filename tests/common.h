#ifndef COMMON_H
#define COMMON_H

#include "kontr.h"

using namespace std;

CONFIGURATION(Testing,
              ::kontr::Generator::Session,
              ::kontr::Generator::MasterTest,
              ::kontr::Generator::UnitTest,
              ::kontr::Generator::Exec,
              ::kontr::Generator::Variable,
              ::kontr::Generator::Language,
              ::kontr::Generator::Storage,
              ::kontr::Report::Exception);

#endif // COMMON_H
