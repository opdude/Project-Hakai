//
//	DefaultCVars.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "DefaultCVars.h"
#include "Console.h"

CVar_ScreenWidth::CVar_ScreenWidth() : CVar("ScreenWidth", "1024", CVAR_INT, NONE, SAVE_INI)
{
}

bool CVar_ScreenWidth::Invoke(std::vector<string> args)
{
	if (args.size() < 1 || args.size() > 1)
		return false;

	return SetValue(args);
}

CVar_ScreenHeight::CVar_ScreenHeight() : CVar("ScreenHeight", "768", CVAR_INT, NONE, SAVE_INI)
{
}

bool CVar_ScreenHeight::Invoke(std::vector<string> args)
{
	if (args.size() < 1 || args.size() > 1)
		return false;

	return SetValue(args);
}

CVar_FullScreen::CVar_FullScreen() : CVar("FullScreen", "true", CVAR_BOOL, NONE, SAVE_INI)
{
}

bool CVar_FullScreen::Invoke(std::vector<string> args)
{
	if (args.size() < 1 || args.size() > 1)
		return false;

	return SetValue(args);
}

CVar_Language::CVar_Language() : CVar("Language", "English", CVAR_STRING, NONE, SAVE_INI)
{
}

bool CVar_Language::Invoke(std::vector<string> args)
{
	if (args.size() < 1 || args.size() > 1)
		return false;

	return SetValue(args);
}

CVar_AudioLevel::CVar_AudioLevel() : CVar("AudioLevel", "100", CVAR_FLOAT, NONE, SAVE_INI)
{
}

bool CVar_AudioLevel::Invoke(std::vector<string> args)
{
	if (args.size() < 1 || args.size() > 1)
		return false;

	return SetValue(args);
}