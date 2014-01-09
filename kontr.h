#ifndef KONTR_H
#define KONTR_H

namespace kontr {
    /// Method for not printing unused parameters in interfaces' constructors
    template<class... T> void unused(T&&...) {}
}

// Includes
#include <vector>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <set>
#include <map>
#include <utility>
#include "backtracexx/backtracexx.hpp"
#include <cstdlib>
#include <type_traits>
#include <fstream>
#include <algorithm>
#include <sstream>

// Declarations
namespace kontr {

namespace Session {

template<typename T>
class Delegator;

} //Session

} //kontr

#include "Reporter.h"
#include "Configuration.h"
#include "macros-types.h"

#include "Variable.h"
#include "Exec.h"
#include "UnitTest.h"
#include "MasterTest.h"
#include "Session.h"
#include "Language.h"

#include "Generator/Names.h"
#include "Generator/MasterTest.h"
#include "Generator/Variable.h"
#include "Generator/Exec.h"
#include "Generator/UnitTest.h"
#include "Generator/Session.h"
#include "Generator/Language.h"
#include "Generator/Configuration.h"

#endif // KONTR_H
