#ifndef _SCREENMANAGER_H
#define _SCREENMANAGER_H

#include <hgefont.h>
#include <vector>
#include "HelperFunctions.h"

class GameScreen;
class GameManager;
class GameTime;
class InputState;

using namespace std;

class ScreenManager
{
public:
	ScreenManager();
	~ScreenManager();

	void AddScreen(GameScreen* screen);
	void RemoveScreen(GameScreen* screen);

	void LoadContent();
	void UnloadContent();

	void Draw(HGE* hge, GameTime* gameTime);
	void Update(GameTime* gameTime);

	void FadeBufferToBlack(float alpha);

	//Accessors
	hgeFont* GetFont()
	{
		return m_fFont;
	}
	hgeFont* GetMenuFont()
	{
		return m_fMenuFont;
	}
	hgeFont* GetMonoFont()
	{
		return m_fMonoFont;
	}
	hgeFont* GetTableFont()
	{
		return m_fTableFont;
	}
	hgeFont* GetButtonFont()
	{
		return m_fButtonFont;
	}

	static const int MENUFONT_SCALE = 300;
	static const int MONOFONT_SCALE = 100;
	static const int TABLEFONT_SCALE = 100;
	static const int BUTTONFONT_SCALE = 100;

private:

	vector<GameScreen*> m_vScreens;
	vector<GameScreen*> m_vScreensToUpdate;
	InputState* m_pInputState;

	//Resources
	hgeFont* m_fFont;
	hgeFont* m_fMenuFont;
	hgeFont* m_fMonoFont;
	hgeFont* m_fTableFont;
	hgeFont* m_fButtonFont;
	HTEXTURE m_tBlankTex; 
};

#endif