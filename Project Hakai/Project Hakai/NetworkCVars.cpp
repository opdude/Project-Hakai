//
//	NetworkCVars.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "NetworkCVars.h"
#include "Console.h"

CVar_PlayerName::CVar_PlayerName() : CVar("PlayerName", "", CVAR_STRING, NONE, SAVE_SERIALISE)
{
}

bool CVar_PlayerName::Invoke(std::vector<string> args)
{
	if (args.size() < 1 || args.size() > 1)
		return false;

	return SetValue(args);
}

CVar_Port::CVar_Port() : CVar("Port", "1337", CVAR_STRING, NONE, SAVE_SERIALISE)
{
}

bool CVar_Port::Invoke(std::vector<string> args)
{
	if (args.size() < 1 || args.size() > 1)
		return false;

	return SetValue(args);
}