#ifndef _CVARINITIALISE_H
#define _CVARINITIALISE_H

#include "NetworkCVars.h"
#include "CVar.h"

static void InitialiseCVars()
{
	Console::Instance()->RegisterCVar(new CVar_PlayerName());
	Console::Instance()->RegisterCVar(new CVar_Port());
}


#endif