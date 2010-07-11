#ifndef _NETWORKCVARS_H
#define _NETWORKCVARS_H

#include <string>
#include <vector>
#include "Cvar.h"

class CVar_PlayerName : public CVar
{
public:
	CVar_PlayerName();
	virtual bool Invoke(vector<string> args);
};

class CVar_Port : public CVar
{
public:
	CVar_Port();
	virtual bool Invoke(vector<string> args);
};

#endif