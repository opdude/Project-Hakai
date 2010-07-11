//
//	CommandParser.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _COMMANDPARSER_H
#define _COMMANDPARSER_H

#include <string>
#include <vector>

class Command;

using namespace std;

class CommandParser
{
public:
	CommandParser(string cmd);
	bool Parse(string& name, vector<string>& args);
private:
	bool HandleChar(char chr);
	void MoveTempString();

	string m_sCommand;
	string m_sTemp;
	string m_sCommandName;
	bool m_bIsQuoted;		//Quoted (")
	bool m_bIsEscaped;		//Escaped (\)
	bool m_bNameDone;		//End of the command name
	bool m_bShouldAddLast;	//??????
	vector<string> m_lArgs;
};

#endif