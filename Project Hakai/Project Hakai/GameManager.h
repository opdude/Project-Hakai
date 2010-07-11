#ifndef _GAMEMANAGER_H
#define _GAMEMANAGER_H

#include <hge.h>
#include <hgeresource.h>

class ScreenManager;
class Console;

class GameManager
{
public:
	static GameManager* Instance();
	static bool m_bRunning;
	static bool m_bGotFocus;

	GameManager();
	~GameManager();

	void Initialise();
	void StartGame();
	void EndGame();

	//Accessors
	HGE* GetHGE()
	{
		return m_pHGE;
	}
	hgeResourceManager* GetResourceManager()
	{
		return m_pResourceManager;
	}
	ScreenManager* GetScreenManager()
	{
		return m_pScreenManager;
	}
	bool Initialised()
	{
		return m_bInitialised;
	}

private:
	void InitialiseHGE();
	void InitialiseManagers();

	void SetScreenResolution();

	DEVMODE GetCurrentResolution();

	static bool UpdateHandler();
	static bool RenderHandler();
	static bool LostFocus();
	static bool GainedFocus();

	Console* m_pConsole;
	ScreenManager* m_pScreenManager;
	hgeResourceManager* m_pResourceManager;
	char* m_sLastScriptLoaded;

	static GameManager* m_pInstance;
	static HGE* m_pHGE;
	static bool m_bInitialised;
};

#endif