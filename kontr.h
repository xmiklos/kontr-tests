#ifndef KONTR_H
#define KONTR_H

namespace kontr {
    /// Method for not printing unused parameters in interfaces' constructors
    template<class... T> void unused(T&&...) {}
}

#include "Reporter.h"
#include "Configuration.h"

#include "Session.h"
#include "MasterTest.h"
#include "Variable.h"

#include "Generator/Names.h"
#include "Generator/MasterTest.h"
#include "Generator/Session.h"
#include "Generator/Variable.h"
#include "Generator/Configuration.h"

#include "macros-types.h"

#endif // KONTR_H
