#ifndef _COMMAND_H
#define _COMMAND_H

#include <string>
#include <vector>

class Console;
enum UsageLevel;

using namespace std;

class Command
{
public:
	virtual bool Invoke(vector<string> args) = 0;

	//Accessors
	UsageLevel GetUsageLevel()
	{
		return m_eUsageLevel;
	}
	string GetName()
	{
		return m_sName;
	}
	string GetUsage()
	{
		return m_sUsage;
	}
protected:
	Command(Console* console)
	{
		m_pConsole = console;
	}
	Command(string cmdName, string usage, UsageLevel usageLevel)
	{
		m_sName = cmdName;
		m_sUsage = usage;
		m_eUsageLevel = usageLevel;
	}

	static Console* m_pConsole;
	string m_sName;
	string m_sUsage;
	UsageLevel m_eUsageLevel;
	
};

class Command_Init : public Command
{
public:
	Command_Init(Console* console) : Command(console)
	{
	}
	virtual bool Invoke(vector<string> args);
};

#endif