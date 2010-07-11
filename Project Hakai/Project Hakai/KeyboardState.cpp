//
//	KeyboardState.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "KeyboardState.h"

KeyboardState::KeyboardState()
{
	for (int i=0; i < 256; i++)
	{
		m_bKeyStates[i] = GameManager::Instance()->GetHGE()->Input_GetKeyState(i);
	}
}

vector<int> KeyboardState::GetPressedKeys()
{
	vector<int> output = vector<int>();

	for (int i=0; i < 256; i++)
	{
		if (m_bKeyStates[i])
			output.push_back(i);
	}

	return output;
}

int KeyboardState::KeyToScanCode(int inKey)
{
	// Get the current keyboard  state (i.e. shift/control keys, etc.)
	BYTE keyState[256];
	GetKeyboardState(keyState);

	// Get the keyboard layout
	HKL hkl = GetKeyboardLayout(0);

	// Convert the virtual key to a scan code
	UINT scanCode = MapVirtualKeyEx(inKey,0,hkl);
	return scanCode;
}

char* KeyboardState::AsciiConvert(int inChar)
{
	// Get the current keyboard  state (i.e. shift/control keys, etc.)
	BYTE keyState[256];
	GetKeyboardState(keyState);

	// Get the keyboard layout
	HKL hkl = GetKeyboardLayout(0);

	// Convert the virtual key to a scan code
	UINT scanCode = MapVirtualKeyEx(inChar,0,hkl);

	// Convert the virtual key/scan code to a character
	unsigned short chars[2]; // may be multiple if "dead key" in buffer
	int result = ToAsciiEx(inChar,scanCode,keyState,chars,0,hkl); 


	//TODO: Find a better way to do this!
	char* tmp = (char*)malloc(sizeof(char)*2);
	if (result == 1)
	{
		sprintf(tmp, "%c", chars[0]);
	}
	else
		sprintf(tmp, "%c", inChar);

	return tmp;
}

int KeyboardState::GetChar()
{
	return GameManager::Instance()->GetHGE()->Input_GetChar();
}