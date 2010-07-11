#ifndef _OPTIONSMENUSCREEN_H
#define _OPTIONSMENUSCREEN_H

#include <vector>
#include "MenuScreen.h"

class GameTime;
class GameScreen;

using namespace std;

class ListBox;
class Slider;

class OptionsMenuScreen : public MenuScreen
{
public:
	OptionsMenuScreen();
	virtual ~OptionsMenuScreen();

	virtual void LoadContent();
private:
	static void ApplyButtonPushed(GameScreen* screen, hgeGUIObject* object);
	static void BackMenuEntrySelected(GameScreen* screen);
	void GetResolutions();

	vector<DEVMODE> m_vResolutions;
	ListBox* resoListBox;
	ListBox* displayList;
	Slider* soundSlider;
};

#endif