#ifndef _CVAR_H
#define _CVAR_H

#include <vector>
#include <hgecolor.h>
#include "Vector2.h"

enum UsageLevel;
class GameManager;
class Console;
class Parser;

using namespace std;

enum CVarType
{
	CVAR_INT,
	CVAR_FLOAT,
	CVAR_VECTOR2,
	CVAR_COLOR,
	CVAR_RECTANGLE,
	CVAR_BOOL,
	CVAR_STRING
};

enum SaveType
{
	SAVE_NONE,
	SAVE_INI,
	SAVE_SERIALISE,
};

class CVar
{
public:
	virtual bool Invoke(vector<string> args) = 0;

	bool SetValue(vector<string> args);
	bool argsValid(vector<string> args);

	int ToInt();
	float ToFloat();
	bool ToBool();
	char ToChar();
	hgeColor ToColour();
	string ToString();
	Vector2 ToVector2();

	//Accessors
	UsageLevel GetUsageLevel()
	{
		return m_eUsageLevel;
	}
	string GetName()
	{
		return m_sName;
	}
	string GetUsage();
	string GetDefaultValue()
	{
		return m_sDefaultVal;
	}
	SaveType GetSaveToFile()
	{
		return m_eSaveToFile;
	}
protected:
	CVar(Console* console);
	CVar(string name, string defaultData, CVarType type, UsageLevel usageLevel, SaveType save);
private:
	static Console* m_pConsole;
	CVarType m_eCVarType;
	UsageLevel m_eUsageLevel;
	SaveType m_eSaveToFile;
	string m_sName;
	string m_sCVarVal;
	string m_sDefaultVal;
};

class CVar_Init : public CVar
{
public:
	CVar_Init(Console* console)
		: CVar(console)
	{
	}

	virtual bool Invoke(vector<string> args)
	{
		return true;
	}
};

#endif