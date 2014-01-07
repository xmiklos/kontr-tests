#ifndef KONTR_H
#define KONTR_H

namespace kontr {
    /// Method for not printing unused parameters in interfaces' constructors
    template<class... T> void unused(T&&...) {}
}

#include "Reporter.h"
#include "Configuration.h"
#include "macros-types.h"


#include "Session.h"
#include "MasterTest.h"
#include "Exec.h"
#include "UnitTest.h"
#include "Variable.h"
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
