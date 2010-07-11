//
//	DefaultCommands.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include <vector>

#include "DefaultCommands.h"
#include "Console.h"
#include "GameManager.h"

Console* Command::m_pConsole;

bool Command_Init::Invoke(vector<string> args)
{
	return true;
}

Command_CLS::Command_CLS() : Command("cls","Clears the screen",NORMAL)
{
}

bool Command_CLS::Invoke(vector<string> args)
{
	m_pConsole->Clear();
	return true;
}

Command_Quit::Command_Quit() : Command("quit","Exit the game",NORMAL)
{
}

bool Command_Quit::Invoke(vector<string> args)
{
	GameManager::m_bRunning = false;
	return true;
}

Command_CVar::Command_CVar() : Command("cvar","Allows you to view and modify CVars",NORMAL)
{
}

bool Command_CVar::Invoke(vector<string> args)
{
	if (args.size() < 1)
		return false;

	string cvarName = args[0];
	vector<string> tmpArgs = vector<string>(args);
	tmpArgs.erase(tmpArgs.begin());

	if (m_pConsole->HasCVar(cvarName))
	{
		m_pConsole->SetCVar(cvarName, tmpArgs);
	}
	else
		return false;

	return true;
}

Command_Help::Command_Help() : Command("help","Displays help about commands",NORMAL)
{
}

bool Command_Help::Invoke(vector<string> args)
{
	if (args.size() <= 0)
		return false;

	m_pConsole->Print(m_pConsole->GetCommand(args[0])->GetUsage(), NORM);
	return true;
}

Command_ListCommands::Command_ListCommands() : Command("listCommands","Lists all available commands",NORMAL)
{
}

bool Command_ListCommands::Invoke(vector<string> args)
{
	vector<string> cmds = m_pConsole->GetCommands();
	string output = "Available Commands: ";
	for each (string s in cmds)
	{
		if (m_pConsole->GetCommand(s)->GetUsageLevel() < m_pConsole->GetUsageLevel())
			continue;

		output += s + ", ";
	}
	output.erase(output.begin() + (output.size() - 2), output.begin() + 2);
	m_pConsole->Print(output);
	return true;
}

Command_LoadConfig::Command_LoadConfig() : Command("loadConfig","Loads the CVar values in the config file to the current CVars.",DEV)
{
}

bool Command_LoadConfig::Invoke(vector<string> args)
{
	//TODO:
	return true;
}

Command_SaveConfig::Command_SaveConfig() : Command("saveConfig","Saves the current save CVar values to the config file",DEV)
{
}

bool Command_SaveConfig::Invoke(vector<string> args)
{
	//TODO:
	return true;
}