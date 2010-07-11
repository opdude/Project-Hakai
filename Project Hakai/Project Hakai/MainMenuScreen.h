//
//	MainMenuScreen.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _MAINMENUSCREEN_H
#define _MAINMENUSCREEN_H

#include "MenuScreen.h"

class GameTime;
class GameScreen;

class MainMenuScreen : public MenuScreen
{
public:
	MainMenuScreen();
	~MainMenuScreen();

	virtual void LoadContent();
	virtual void UnloadContent();
	virtual void Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
	virtual void OnCancel();
private:

	static void ProfileNameEntered(GameScreen* screen, const char* str);
	static void SinglePlayerMenuEntrySelected(GameScreen* screen);
	static void MultiPlayerMenuEntrySelected(GameScreen* screen);
	static void OptionsMenuEntrySelected(GameScreen* screen);
	static void ExitMenuEntrySelected(GameScreen* screen);
};

#endif