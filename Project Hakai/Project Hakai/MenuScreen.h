//
//	MenuScreen.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _MENUSCREEN_H
#define _MENUSCREEN_H

#include <string>
#include <hgegui.h>
#include <hgeguictrls.h>
#include <hgesprite.h>
#include "GameScreen.h"

class GameTime;
class InputState;
class MenuEntry;

using namespace std;

class MenuScreen : public CursorScreen
{
public:
	MenuScreen(string title);

	virtual void LoadContent();
	virtual void UnloadContent();

	void AddMenuEntry(MenuEntry* menuEntry);

	virtual void Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
	virtual void HandleInput(InputState* inputState, GameTime* gameTime);
	virtual void Draw(HGE* hge, GameTime* gameTime);

	virtual void ExitScreen();
	virtual void OnCancel();

protected:
	hgeGUI* m_pGUI;
	string m_sTitle;
	int m_iMenuEntryCount;
	bool m_bOtherScreenHasFocus;
};

#endif