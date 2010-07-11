//
//	MainMenuScreen.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "MainMenuScreen.h"
#include "Console.h"
#include "CVar.h"
#include "GameManager.h"
#include "MenuEntry.h"
#include "MultiPlayerMenuScreen.h"
#include "ScreenManager.h"
#include "OptionsMenuScreen.h"
#include "PopupInputScreen.h"

MainMenuScreen::MainMenuScreen() : MenuScreen("Main Menu")
{
	
}

MainMenuScreen::~MainMenuScreen()
{

}

void MainMenuScreen::LoadContent()
{
	MenuEntry* singlePlayerMenuEntry = new MenuEntry(this, "Single Player");
	MenuEntry* multiPlayerMenuEntry = new MenuEntry(this, "Multi-Player");
	MenuEntry* optionsMenuEntry = new MenuEntry(this, "Options");
	MenuEntry* exitMenuEntry = new MenuEntry(this, "Exit");

	singlePlayerMenuEntry->Clicked.connect(&MainMenuScreen::SinglePlayerMenuEntrySelected);
	multiPlayerMenuEntry->Clicked.connect(&MainMenuScreen::MultiPlayerMenuEntrySelected);
	optionsMenuEntry->Clicked.connect(&MainMenuScreen::OptionsMenuEntrySelected);
	exitMenuEntry->Clicked.connect(&MainMenuScreen::ExitMenuEntrySelected);

	AddMenuEntry(singlePlayerMenuEntry);
	AddMenuEntry(multiPlayerMenuEntry);
	AddMenuEntry(optionsMenuEntry);
	AddMenuEntry(exitMenuEntry);

	//Check Player Name
	if (!strcmp(Console::Instance()->GetCVar("PlayerName")->ToString().c_str(),""))
	{
		PopupInputScreen* popup = new PopupInputScreen("Enter your name");
		popup->InputAccepted.connect(&MainMenuScreen::ProfileNameEntered);
		m_pScreenManager->AddScreen(popup);
	}

	MenuScreen::LoadContent();
}

void MainMenuScreen::UnloadContent()
{
	MenuScreen::UnloadContent();
}

void MainMenuScreen::Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	MenuScreen::Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);
}

void MainMenuScreen::ProfileNameEntered(GameScreen* screen, const char* str)
{
	Console::Instance()->SetCVar("PlayerName", str);
}

void MainMenuScreen::SinglePlayerMenuEntrySelected(GameScreen* screen)
{
	screen->GetScreenManager()->AddScreen(new MainMenuScreen());
	screen->ExitScreen();
}

void MainMenuScreen::MultiPlayerMenuEntrySelected(GameScreen* screen)
{
	screen->GetScreenManager()->AddScreen(new MultiPlayerMenuScreen());
}

void MainMenuScreen::OptionsMenuEntrySelected(GameScreen* screen)
{
	screen->GetScreenManager()->AddScreen(new OptionsMenuScreen());
}

void MainMenuScreen::ExitMenuEntrySelected(GameScreen* screen)
{
	//TODO: Add popup message box
	GameManager::Instance()->m_bRunning = false;
}


void MainMenuScreen::OnCancel()
{
	//Do Nothing
}