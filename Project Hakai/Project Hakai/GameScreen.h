#ifndef _GAMESCREEN_H
#define _GAMESCREEN_H

#include <hge.h>
#include <hgesprite.h>
#include <string>

class ScreenManager;
class InputState;
class GameTime;

using namespace std;

enum ScreenState
{
	TransitionOn,
	Active,
	TransitionOff,
	Hidden
};

class GameScreen
{
public:
	GameScreen();
	virtual ~GameScreen();

	virtual void LoadContent() = 0;
	virtual void UnloadContent() = 0;

	virtual void Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
	virtual void HandleInput(InputState* inputState, GameTime* gameTime) = 0;
	virtual void Draw(HGE* hge, GameTime* gameTime) = 0;
	virtual void ExitScreen();

	bool UpdateTransition(GameTime* gameTime, float time, int direction);


	//Accessors
	void SetScreenManager(ScreenManager* screenManager)
	{
		m_pScreenManager = screenManager;
	}
	ScreenManager* GetScreenManager()
	{
		return m_pScreenManager;
	}
	ScreenState GetState()
	{
		return m_eState;
	}
	void SetExiting(bool val)
	{
		m_bIsExiting = val;
	}
	float GetTransitionOnTime()
	{
		return m_fTransitionOnTime;
	}
	void SetTransitionOnTime(float val)
	{
		m_fTransitionOnTime = val;
	}
	float GetTransitionOffTime()
	{
		return m_fTransitionOffTime;
	}
	void SetTransitionOffTime(float val)
	{
		m_fTransitionOffTime = val;
	}
	float GetTransitionPosition()
	{
		return m_fTransitionPosition;
	}
	float GetTransitionAlpha()
	{
		return 1 - m_fTransitionPosition;
	}
	bool IsActive()
	{
		return m_bOtherScreenHasFocus && (m_eState == TransitionOn || m_eState == Active);
	}
	bool IsExiting()
	{
		return m_bIsExiting;
	}
	bool IsPopup()
	{
		return m_bIsPopup;
	}

	static const int GUI_SCALEX = 10000;
	static const int GUI_SCALEY = 10000;

protected:
	string GetText(string id);
	
	ScreenManager* m_pScreenManager;
	ScreenState m_eState;
	float m_fTransitionOnTime;
	float m_fTransitionOffTime;
	float m_fTransitionPosition;
	bool m_bIsPopup;
	bool m_bIsExiting;
	bool m_bOtherScreenHasFocus;
};

class CursorScreen : public GameScreen
{
public:
	CursorScreen();
	~CursorScreen();
	virtual void LoadContent();
	virtual void Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
	virtual void Draw(HGE* hge, GameTime* gameTime);

	static hgeSprite* m_sCursor;
};

#endif