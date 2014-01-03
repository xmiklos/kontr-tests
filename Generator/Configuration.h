#ifndef GENERATOR_CONFIGURATION_H
#define GENERATOR_CONFIGURATION_H

#include "../kontr.h"

namespace kontr {
namespace Generator {

struct Storage {
    std::ostream* out_ptr = nullptr;
    const char* nextFileName = nullptr;
    Names::All names;
};

CONFIGURATION(Configuration,
              ::kontr::Generator::Session,
              ::kontr::Generator::MasterTest,
              ::kontr::Generator::Variable,
              ::kontr::Generator::Language,
              ::kontr::Generator::Storage,
              ::kontr::Report::Default
             );

} //Generator
} //kontr
#endif // GENERATOR_CONFIGURATION_H
