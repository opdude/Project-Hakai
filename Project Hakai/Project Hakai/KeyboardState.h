//
//	KeyboardState.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _KEYBOARDSTATE_H
#define _KEYBOARDSTATE_H

#include <vector>
#include <hge.h>
#include "GameManager.h"

class GameManager;

using namespace std;

class KeyboardState
{
public:
	KeyboardState();

	vector<int> GetPressedKeys();
	
	static int KeyToScanCode(int inKey);
	static char* AsciiConvert(int inChar);
	bool IsKeyDown(int key)
	{
		return m_bKeyStates[key];
	}
	bool IsKeyUp(int key)
	{
		return !m_bKeyStates[key];
	}
	int GetChar();
private:
	bool m_bKeyStates[256];

};

#define KEY_ESC 1
#define KEY_BACKSPACE 14
#define KEY_TAB 15
#define KEY_RETURN 28
#define KEY_CTRL 29
#define KEY_CONSOLE 41
#define KEY_LSHIFT 42
#define KEY_RSHIFT 54
#define KEY_ALT 56
#define KEY_HOME 71
#define KEY_UP 72
#define KEY_PGUP 73
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_END 79
#define KEY_DOWN 80
#define KEY_PGDOWN 81
#define KEY_INSERT 82
#define KEY_DEL 83
#define KEY_LWIN 91
#define KEY_RWIN 92
#define KEY_LIST 93

#endif