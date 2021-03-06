//
//	Console.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <hge.h>
#include <hgecolor.h>
#include <hgerect.h>
#include <hgesprite.h>
#include <hgefont.h>
#include "Vector2.h"

class CVar;
class Command;
class GameTime;
class KeyboardState;
class MouseState;

using namespace std;

enum UsageLevel
{
	NONE,               //These commands should be used by the engine only
	DEBUGDEV,
	DEV,
	NORMAL
};

enum ConsoleLevel
{
	TXTFILE,
	DEBUG,
	NORM,
	WARN,
	FAIL
};

enum SaveType;

class Console
{
public:
	static Console* Instance();

	Console();
	~Console();

	void Initialise();
	void Update(GameTime* gameTime);
	void Draw(GameTime* gameTime);

	void AddDefaultCommands();
	void AddDefaultCVars();
	bool RegisterCommand(Command* cmd);
	bool RegisterCVar(CVar* cvar);
	void Print(string str);
	void Print(string str, ConsoleLevel level);
	void HandleCommand(string cmd);
	Command* GetCommand(string cmdName);
	CVar* GetCVar(string cvarName);
	bool SetCVar(string cvarName, vector<string> args);
	bool SetCVar(string cvarName, string strArgs);
	bool HasCommand(string cmd);
	bool HasCVar(string cvar);
	vector<string> GetCommands();
	vector<string> GetCVars();
	vector<CVar*> GetSaveCVars(SaveType saveType);
	void Clear();


	//Accessors
	hgeColor GetColour(ConsoleLevel level);
	void SetTextColour(hgeColor colour, ConsoleLevel level);
	void SetUsageLevel(UsageLevel val)
	{
		m_eUsageLevel = val;
	}
	UsageLevel GetUsageLevel()
	{
		return m_eUsageLevel;
	}

private:
	void AutoComplete();
	void AddToHistory(string str);
	void HandleKeyPress(int key);
	void ResizeMessages();
	void UpdateKeyboard(GameTime* gameTime);
	bool Dispatch(string cmd, vector<string> args);
	bool CalculateLimits(float& lineHeight, float& fontHeight);
	string GetTime();
	vector<string> SplitString(string str);

	static Console* m_pInstance;

	//Console Data
	vector<string> m_lConsoleText;
	vector<string> m_lHistoryText;
	vector<hgeColor> m_lLineColour;
	map<string, Command*> m_dCommands;
	map<string, CVar*> m_dCVars;
	string m_sCommandText;
	int m_iHistoryIndex;
	int m_iCurrentLine;             //Current line to start printing from
	int m_iCursorPos;
	double m_dTimeOpened;           //When was the console opened (Stops the very quick closing)
	bool m_bJustOpened;

	//Console Objects
	Vector2 m_vConsoleSize;
	hgeRect m_rConsoleDestination[3];
	hgeSprite* m_rConsoleTextureSource[3];
	hgeRect m_rTextboxDestination[3];
	hgeSprite* m_rTextboxTextureSource[3];
	HTEXTURE m_tBlankTexture;
	HTEXTURE m_tTextbox;
	HTEXTURE m_tConsole;
	hgeSprite* consoleBackground;
	hgeFont* m_fFont;
	KeyboardState* m_pKeyboardState;
	KeyboardState* m_pLastKeyboardState;
	MouseState* m_pMouseState;
	MouseState* m_pLastMouseState;

	//Console Variables
	UsageLevel m_eUsageLevel;
	hgeColor m_aTextColours[4];
	string m_sConfig;
	string m_sConfigFolder;
	static const int HISTORY_SIZE = 5;     //Size of the history to keep
	static const int INPUT_MAX = 250;      //Limit the input to 250 Chars 
	float m_fKeyStrokeSpeed;
	int m_iConsoleLines;
	int m_iMaxLines;
	float m_fFontHeight;
	double m_lLastKeyPress;
	bool m_bEnabled;
	bool m_bInitialised;
	bool m_bShowCursor;
	bool m_bShowBackground;
	bool m_bOutputToVS;

	vector<string> m_lFileBuffer;     //This is used only when the m_fFile isn't opened.
	fstream m_LogWriter;
	string m_sLog;

	static const int ALLOWED_INPUT_SIZE = 95;
	static char* m_aAInput[];

};

#endif