//
//	MultiPlayerMenuScreen.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _MULTIPLAYERMENUSCREEN_H
#define _MULTIPLAYERMENUSCREEN_H

#include <vector>
#include "MenuScreen.h"

class GameTime;
class GameScreen;

using namespace std;

class ListBox;
class Slider;

class MultiPlayerMenuScreen : public MenuScreen
{
public:
	MultiPlayerMenuScreen();
	virtual ~MultiPlayerMenuScreen();

	virtual void LoadContent();
private:
	static void CreateServerMenuEntryPushed(GameScreen* screen);
	static void SearchLocalServerMenuEntryPushed(GameScreen* screen);
	static void JoinServerIPMenuEntryPushed(GameScreen* screen);
	static void BackMenuEntrySelected(GameScreen* screen);
	static void IPEntered(GameScreen* screen, const char* text);
	void GetResolutions();

	vector<DEVMODE> m_vResolutions;
};

#endif