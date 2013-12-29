#ifndef KONTR_H
#define KONTR_H

namespace kontr {
    /// Method for not printing unused parameters in interfaces' constructors
    template<class... T> void unused(T&&...) {}
}

#include "Reporter.h"

#include "MasterTest.h"
#include "Generator/MasterTest.h"
#include "Session.h"
#include "Generator/Session.h"
#include "Variable.h"
#include "Generator/Variable.h"

#include "Configuration.h"

#include "Generator/Names.h"
#include "macros-types.h"

#endif // KONTR_H
