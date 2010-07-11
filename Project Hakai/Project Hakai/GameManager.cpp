
#include "GameManager.h"
#include "SettingsManager.h"
#include "Console.h"
#include "CVar.h"
#include "CVarInitialise.h"
#include "FontLoader.h"
#include "GameTime.h"
#include "GameScreen.h"
#include "ScreenManager.h"

//Static Variables
GameManager* GameManager::m_pInstance;
HGE* GameManager::m_pHGE;
bool GameManager::m_bInitialised = false;
bool GameManager::m_bRunning = false;
bool GameManager::m_bGotFocus = true;


/**
* @brief Singleton of the game manager
* @return GameManager*
*/
GameManager* GameManager::Instance()
{
	if (!m_pInstance)
		m_pInstance = new GameManager();

	return m_pInstance;
}

GameManager::GameManager()
{

}
/**
* @brief Initialise the game manager
* @return void
*/
void GameManager::Initialise()
{
	InitialiseHGE();
	InitialiseManagers();
	InitialiseCVars();
	SettingsManager::LoadCVars();

	//Setup Window CVars
	char tmpVal[8];
	sprintf(tmpVal, "%i", m_pHGE->System_GetState(HGE_SCREENWIDTH));
	Console::Instance()->SetCVar("ScreenWidth", string(tmpVal));
	sprintf(tmpVal, "%i", m_pHGE->System_GetState(HGE_SCREENHEIGHT));
	Console::Instance()->SetCVar("ScreenHeight", string(tmpVal));

	if (m_pHGE->System_GetState(HGE_WINDOWED))
		Console::Instance()->SetCVar("FullScreen","false");
	else
		Console::Instance()->SetCVar("FullScreen","true");
}

GameManager::~GameManager()
{
	//Clean Up Memory
	delete m_pResourceManager;
	delete m_pConsole;
	delete m_pScreenManager;

	//Clear up nicely
	m_pHGE->System_Shutdown();
	m_pHGE->Release();
}

/**
* @brief Initialise HGE
* @return void
*/
void GameManager::InitialiseHGE()
{
	//Create HGE
	m_pHGE = hgeCreate(HGE_VERSION);

#if _DEBUG
	m_pHGE->System_SetState(HGE_SHOWSPLASH, false);
	m_pHGE->System_SetState(HGE_LOGFILE, "hge.log");
#endif
	m_pHGE->System_SetState(HGE_FRAMEFUNC, &GameManager::UpdateHandler);
	m_pHGE->System_SetState(HGE_RENDERFUNC, &GameManager::RenderHandler);
	m_pHGE->System_SetState(HGE_FOCUSGAINFUNC, &GameManager::GainedFocus);
	m_pHGE->System_SetState(HGE_FOCUSLOSTFUNC, &GameManager::LostFocus);
	m_pHGE->System_SetState(HGE_TITLE, "Project Hakai");
	m_pHGE->System_SetState(HGE_USESOUND, false); //TODO: Enable this
	m_pHGE->System_SetState(HGE_FPS, 60);
	m_pHGE->System_SetState(HGE_DONTSUSPEND, true);
	m_pHGE->Resource_AttachPack("Content.zip");

	SetScreenResolution();

	if (m_pHGE->System_Initiate())
	{
		m_bInitialised = true;
	}
	else
	{
		MessageBox(NULL, m_pHGE->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}
}

/**
* @brief Start the game running
* @return void
*/
void GameManager::StartGame()
{
	//Start HGE
	m_bRunning = true;
	m_pHGE->System_Start();
}

/**
* @brief Initialise all the internal managers
* @return void
*/
void GameManager::InitialiseManagers()
{
	m_sLastScriptLoaded = "Menu.opr";
	m_pResourceManager = new hgeResourceManager(m_sLastScriptLoaded);
	m_pConsole = Console::Instance();
	m_pConsole->SetUsageLevel(DEBUGDEV);
	m_pScreenManager = new ScreenManager();

	m_pScreenManager->LoadContent();
	m_pConsole->Initialise();

	Console::Instance()->Print("--- Managers Initialised --- ", DEBUG);
}

/**
* @brief End the game
* @return void
*/
void GameManager::EndGame()
{
	SettingsManager::SaveCVars();
	Console::Instance()->Print("--- Shutting Engine Down ---");
	delete m_pInstance;
}

/**
* @brief Set the current screen resolution, based on either the Profile found or the current resolution
* @return void
*/
void GameManager::SetScreenResolution()
{
	//Window Settings
	m_pHGE->System_SetState(HGE_INIFILE, "config.ini");

	int screenWidth = m_pHGE->Ini_GetInt("CVars","ScreenWidth", 0);
	int screenHeight = m_pHGE->Ini_GetInt("CVars","ScreenHeight", 0);

	if (screenWidth == 0 || screenHeight == 0)
	{
		DEVMODE dispMode = GetCurrentResolution();
		screenWidth = dispMode.dmPelsWidth;
		screenHeight = dispMode.dmPelsHeight;
	}

	m_pHGE->System_SetState(HGE_SCREENWIDTH, screenWidth);
	m_pHGE->System_SetState(HGE_SCREENHEIGHT, screenHeight);

	char* fullScreen = m_pHGE->Ini_GetString("CVars","FullScreen", "true");

	if (!strcmp(fullScreen,"true") || !strcmp(fullScreen,"1") || !strcmp(fullScreen,"yes"))
		m_pHGE->System_SetState(HGE_WINDOWED, false);
	else
		m_pHGE->System_SetState(HGE_WINDOWED, true);
}

/**
* @brief Get the current resolution for this monitor
* @return DEVMODE
*/
DEVMODE GameManager::GetCurrentResolution()
{
	DEVMODE dm;

	// initialize the DEVMODE structure
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);

	if (0 != EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm))
	{
		return dm;
	}

	return dm;
}

/**
* @brief Update handler which is called every frame by HGE 
* @return bool
*/
bool GameManager::UpdateHandler()
{
	if (!m_bInitialised)
		return false;

	if (!m_bRunning) 
	{	
		return true;
	}

	//Update
	GameTime* gameTime = new GameTime(m_pHGE->Timer_GetDelta(), m_pHGE->Timer_GetTime());
	GameManager::Instance()->GetScreenManager()->Update(gameTime);
	Console::Instance()->Update(gameTime);

	//Clean Up
	delete gameTime;

	return false;
}

/**
* @brief Render handler which is called every frame by HGE
* @return bool
*/
bool GameManager::RenderHandler()
{
	if (!m_bInitialised || !m_bRunning)
		return false;


	//Scaling
	m_pHGE->Gfx_SetTransform(0, 0, 0, 0, 0, 
		m_pHGE->System_GetState(HGE_SCREENWIDTH) * 1.f /GetScreenWidth(), 
		m_pHGE->System_GetState(HGE_SCREENHEIGHT)  * 1.f/GetScreenHeight()); 

	m_pHGE->Gfx_BeginScene();
	m_pHGE->Gfx_Clear(ARGB(255,0,0,0));
	
	//Render
	GameTime* gameTime = new GameTime(m_pHGE->Timer_GetDelta(), m_pHGE->Timer_GetTime());
	GameManager::Instance()->GetScreenManager()->Draw(m_pHGE, gameTime);
	Console::Instance()->Draw(gameTime);

	//Clean Up
	delete gameTime;

	m_pHGE->Gfx_EndScene();


	return false;
}

/**
* @brief The game window has gained focus
* @return bool
*/
bool GameManager::GainedFocus()
{
	m_bGotFocus = true;
	return false;
}

/**
* @brief The game window has lost focus
* @return bool
*/
bool GameManager::LostFocus()
{
	m_bGotFocus = false;
	return false;
}