//
//	CVar.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "CVar.h"
#include "Parser.h"

Console* CVar::m_pConsole;

CVar::CVar(Console* console)
{
	m_pConsole = console;
	m_eSaveToFile = SAVE_NONE;
}

CVar::CVar(string name, string defaultData, CVarType cvarType, UsageLevel usageLevel, SaveType save)
{
	m_sName = name;
	m_sDefaultVal = defaultData;
	m_eCVarType = cvarType;
	m_eUsageLevel = usageLevel;
	m_eSaveToFile = save;
}

string CVar::GetUsage()
{
	switch (m_eCVarType)
	{
	case CVAR_BOOL:
		return "<i>";
	case CVAR_COLOR:
		return "<r> <g> <b> <a>";
	case CVAR_FLOAT:
		return "<i>";
	case CVAR_INT:
		return "<i>";
	case CVAR_STRING:
		return "<string>";
	case CVAR_VECTOR2:
		return "<x> <y>";
	case CVAR_RECTANGLE:
		return "<x> <y> <w> <h>";
	default:
		return "<i>";
	}
}

string CVar::ToString()
{
	if (m_sCVarVal.length() != 0)
		return m_sCVarVal;

	return m_sDefaultVal;
}

int CVar::ToInt()
{
	if (m_sCVarVal.size() > 0)
		return Parser::ToInt(m_sCVarVal);

	else
		return Parser::ToInt(m_sDefaultVal);
}

float CVar::ToFloat()
{
	if (m_sCVarVal.size() > 0)
		return Parser::ToFloat(m_sCVarVal);

	else
		return Parser::ToFloat(m_sDefaultVal);
}

hgeColor CVar::ToColour()
{
	if (m_sCVarVal.size() > 0)
		return Parser::ToColour(m_sCVarVal);
	else
		return Parser::ToColour(m_sDefaultVal);
}

Vector2 CVar::ToVector2()
{
	if (m_sCVarVal.size() > 0)
		return Parser::ToVector2(m_sCVarVal);
	else
		return Parser::ToVector2(m_sDefaultVal);
}

bool CVar::ToBool()
{
	if (m_sCVarVal.size() > 0)
		return Parser::ToBool(m_sCVarVal);

	else
		return Parser::ToBool(m_sDefaultVal);
}

char CVar::ToChar()
{ 
	//TODO: Check this works
	if (m_sCVarVal.size() > 0)
		return Parser::ToChar(m_sCVarVal);
	else
		return Parser::ToChar(m_sDefaultVal);

}

bool CVar::SetValue(vector<string> args)
{
	m_sCVarVal = "";

	if (!argsValid(args))
		return false;

	for each (string str in args)
		m_sCVarVal += str;

	remove(m_sCVarVal.begin(), m_sCVarVal.end(), ' ');

	return true;
}

/// <summary>
/// Checks the input args to make sure they are valid
/// </summary>
/// <param name="args"></param>
/// <returns></returns>
bool CVar::argsValid(vector<string> args)
{
	//Temp Variables
	/*switch (m_eCVarType)
	{
	case CVAR_BOOL:
		try
		{
			Parse.TParse(args[0]);

			if (args.Count == 1)
				return true;
			break; 
		}
		catch (System.FormatException)
		{
			break;
		}

	case CVAR_COLOR:
		if (args.Count > 0 && args.Count <= 3)
		{
			for each (String str in args)
			{
				try
				{
					int.Parse(str);
				}
				catch (System.FormatException)
				{
					break;
				}
			}

			return true;
		}
		break;
	case CVAR_FLOAT:
		try
		{
			if (args.Count == 1)
				float.Parse(args[0]);
		}
		catch (System.FormatException)
		{
			break;
		}
		return true;

	case INT:
		try
		{
			if (args.Count == 1)
				int.Parse(args[0]);
		}
		catch (System.FormatException)
		{
			break;
		}
		return true;

	case CVAR_STRING:
		if (args.Count > 0)
			return true;
		break;

	case CVAR_VECTOR2:
		if (args.Count > 0 && args.Count <= 2)
		{
			for each (String str in args)
			{
				try
				{
					if (args.Count == 1)
						float.Parse(str);
				}
				catch (System.FormatException)
				{
					break;
				}
			}
			return true;
		}
		break;

	case CVAR_RECTANGLE:
		if (args.Count > 0 && args.Count <= 4)
		{
			for each (String str in args)
			{
				try
				{
					if (args.Count == 1)
						int.Parse(str);
				}
				catch (System.FormatException)
				{
					break;
				}
			}
			return true;
		}
		break;
	}*/

	return true;

	//return false;
}


