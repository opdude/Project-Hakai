//
//	ScreenManager.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "ScreenManager.h"
#include "FontLoader.h"
#include "GameScreen.h"
#include "GameManager.h"
#include "InputState.h"

ScreenManager::ScreenManager()
{
	//Setup Vectors
	m_vScreens = vector<GameScreen*>();
	m_vScreensToUpdate = vector<GameScreen*>();
}

ScreenManager::~ScreenManager()
{
	while (m_vScreens.size() > 0)
		RemoveScreen(m_vScreens.at(0));

	delete m_pInputState;
}

void ScreenManager::AddScreen(GameScreen *screen)
{
	screen->SetScreenManager(this);
	screen->SetExiting(false);

	m_vScreens.push_back(screen);

	if (GameManager::Instance()->Initialised())
		screen->LoadContent();
}

void ScreenManager::RemoveScreen(GameScreen *screen)
{
	if (GameManager::Instance()->Initialised())
		screen->UnloadContent();

	int i=0;
	for each (GameScreen* compareScreen in m_vScreens)
	{
		if (screen == compareScreen)
			break;

		i++;
	}

	m_vScreens.erase(m_vScreens.begin() + i);

	delete screen;
}

void ScreenManager::LoadContent()
{
	//Load Menu Content TODO
	//GameManager::Instance()->GetHGE()->Resource_AttachPack("MenuContent.opi", "fn0rfn0r");

	m_pInputState = new InputState();

	m_fFont = GameManager::Instance()->GetResourceManager()->GetFont("Font");
	m_fMenuFont = GameManager::Instance()->GetResourceManager()->GetFont("MenuFont");
	m_fMonoFont = GameManager::Instance()->GetResourceManager()->GetFont("MonoFont");
	m_fButtonFont = GameManager::Instance()->GetResourceManager()->GetFont("ButtonFont");
	m_fTableFont = GameManager::Instance()->GetResourceManager()->GetFont("MonoFont");

	m_tBlankTex = GameManager::Instance()->GetResourceManager()->GetTexture("Blank");

	for each (GameScreen* screen in m_vScreens)
	{
		screen->LoadContent();
	}
}

void ScreenManager::UnloadContent()
{
	for each(GameScreen* screen in m_vScreens)
	{
		screen->UnloadContent();
	}
}

void ScreenManager::Update(GameTime *gameTime)
{
	m_pInputState->Update();

	m_vScreensToUpdate.clear();

	for each (GameScreen* screen in m_vScreens)
		m_vScreensToUpdate.push_back(screen);

	bool otherScreenHasFocus = false;	//TODO: Get Window Active
	bool coveredByOtherScreen = false;

	//Loop all screens to update
	while (m_vScreensToUpdate.size() > 0)
	{
		//Pop the topmost screen off the list
		GameScreen* screen = m_vScreensToUpdate[m_vScreensToUpdate.size() - 1];
		m_vScreensToUpdate.pop_back();

		//Update Screen
		screen->Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);

		if (screen->GetState() == TransitionOn ||
			screen->GetState() == Active)
		{
			//If this is the first active screen let it 
			//take control
			if (!otherScreenHasFocus)
			{
				if (GameManager::m_bGotFocus && screen->GetState() == Active)
					screen->HandleInput(m_pInputState, gameTime);

				otherScreenHasFocus = true;
			}

			//If this is an active non-popup, inform any prior
			//screen that they are covered by it
			if (!screen->IsPopup())
				coveredByOtherScreen = true;
		}
	}
}


void ScreenManager::Draw(HGE *hge, GameTime *gameTime)
{
	string title = boost::str(boost::format("Project Hakai %i") % GameManager::Instance()->GetHGE()->Timer_GetFPS());
	GameManager::Instance()->GetHGE()->System_SetState(HGE_TITLE, title.c_str());

	for each(GameScreen* screen in m_vScreens)
	{
		if (screen->GetState() == Hidden)
			continue;

		screen->Draw(hge, gameTime);
	}
}

void ScreenManager::FadeBufferToBlack(float alpha)
{
	hgeSprite* rect = new hgeSprite(m_tBlankTex,0,0,GetScreenWidth(), GetScreenHeight());
	rect->SetColor(ARGB(alpha*255,0,0,0));
	rect->Render(0,0);

	delete rect;
}