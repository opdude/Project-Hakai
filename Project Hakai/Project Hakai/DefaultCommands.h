#ifndef _DEFAULTCOMMANDS_H
#define _DEFAULTCOMMANDS_H

#include <vector>
#include "Command.h"

using namespace std;

class Command_CLS : public Command
{
public:
	Command_CLS();
	virtual bool Invoke(vector<string> args);
};

class Command_Quit : public Command
{
public:
	Command_Quit();
	virtual bool Invoke(vector<string> args);
};

class Command_CVar : public Command
{
public:
	Command_CVar();
	virtual bool Invoke(vector<string> args);
};

class Command_Help : public Command
{
public:
	Command_Help();
	virtual bool Invoke(vector<string> args);
};

class Command_ListCommands : public Command
{
public:
	Command_ListCommands();
	virtual bool Invoke(vector<string> args);
};

class Command_LoadConfig : public Command
{
public:
	Command_LoadConfig();
	virtual bool Invoke(vector<string> args);
};

class Command_SaveConfig : public Command
{
public:
	Command_SaveConfig();
	virtual bool Invoke(vector<string> args);
};

#endif