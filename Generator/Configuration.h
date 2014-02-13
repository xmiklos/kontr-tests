#ifndef GENERATOR_CONFIGURATION_H
#define GENERATOR_CONFIGURATION_H

#include "../kontr-internal.h"

namespace kontr {
namespace Generator {

struct Storage {
    std::ostream* out_ptr = nullptr;
    const char* nextFileName = nullptr;
    Names::All names;
    bool inArrayGenerating = true;
    std::string indent;

    void addIndent() { indent.push_back('\t'); }
    void removeIndent() { indent.pop_back(); }
};

CONFIGURATION(Configuration,
              ::kontr::Generator::Session,
              ::kontr::Generator::MasterTest,
              ::kontr::Generator::UnitTest,
              ::kontr::Generator::Exec,
              ::kontr::Generator::Variable,
              ::kontr::Generator::Language,
              ::kontr::Generator::Storage,
              ::kontr::Report::Default
             );

} //Generator
} //kontr
#endif // GENERATOR_CONFIGURATION_H
