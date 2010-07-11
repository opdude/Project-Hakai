#include "CommandParser.h"

CommandParser::CommandParser(string str)
{
	m_sCommand = str;
}

bool CommandParser::Parse(string& name, vector<string>& args)
{
	char spaceChar = ' ';

	//Clear the input
	m_lArgs = vector<string>();
	name = "";

	//Check to see if its just the command name
	if (m_sCommand.find_first_of(spaceChar) == -1)
	{
		name = m_sCommand;
		args = m_lArgs;
		return true;
	}

	//Rest Variables
	m_bIsEscaped = false;
	m_bIsQuoted = false;
	m_bNameDone = false;
	m_bShouldAddLast = true;

	for (unsigned int i = 0; i < m_sCommand.size(); i++)
	{
		if (!HandleChar(m_sCommand[i]))
		{
			args = m_lArgs;
			return false;
		}
	}

	if (m_bShouldAddLast)
		MoveTempString();

	args = m_lArgs;
	name = m_sCommandName;
	return true;
}

bool CommandParser::HandleChar(char chr)
{
	char spaceChar = ' ';
	char escapeChar = '\\';
	char quoteChar = '\"';

	//If the char is a space
	if (chr == spaceChar)
	{
		//If we don't have a quote then just add it as a new arg
		if (!m_bIsQuoted)
		{
			MoveTempString();
		}
		else
		{
			m_sTemp += (spaceChar);
			m_bShouldAddLast = true;
		}
	}

	//If we've found a quote char
	else if (chr == quoteChar)
	{
		//If we have just escaped then ignore this and add as a normal char
		if (m_bIsEscaped)
		{
			m_sTemp += quoteChar;
			m_bIsEscaped = false;
			m_bShouldAddLast = true;
		}

		//If this is the end of a quote then the whole quoted section is one arg
		else if (m_bIsQuoted)
		{
			MoveTempString();
			m_bIsQuoted = false;
		}

		//Start a new quotation
		else
			m_bIsQuoted = true;
	}

	//If we've found an escape char
	else if (chr == escapeChar)
	{
		//If we have already escaped then add it as normal text
		if (m_bIsEscaped)
		{
			m_sTemp += escapeChar;
			m_bIsEscaped = false;
			m_bShouldAddLast = true;
		}
		//If this is the first escape then escape the next char
		else
			m_bIsEscaped = true;
	}

	//Any other character
	else
	{
		//If we have escaped then the next char is not going to be used
		if (m_bIsEscaped)
			return false;

		m_sTemp += chr;
		m_bShouldAddLast = true;
	}

	return true;
}

void CommandParser::MoveTempString()
{
	//If we already have the name
	if (m_bNameDone)
	{
		if (m_sTemp.size() != 0)
			m_lArgs.push_back(m_sTemp);
	}

	//If we don't have a command name then this is the first arg
	//and will become the command name.
	else
	{
		m_sCommandName = m_sTemp;
		m_bNameDone = true;
	}
	m_bShouldAddLast = false;
	m_sTemp = "";
}