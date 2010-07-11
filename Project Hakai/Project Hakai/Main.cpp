/*!
 * <File comment goes here!!>
 * 
 * Copyright (c) 200x by <your name/ organization here>
 */
//
// Main.cpp
// Project Hakai
//
// Created by Daniel Hobley.
//

#include <hge.h>
#include "GameManager.h"
#include "BackgroundScreen.h"
#include "MainMenuScreen.h"
#include "ScreenManager.h"

struct MemCheck {
	~MemCheck() {
		_CrtDumpMemoryLeaks();
	}
};

/**
* @brief Main entry point
* @return int WINAPI
*/
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#if _DEBUG
	//Setup Memory Leak
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 
	_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
	MemCheck* mem = new MemCheck;
#endif

	GameManager::Instance()->Initialise();
	GameManager::Instance()->GetScreenManager()->AddScreen(new BackgroundScreen());
	GameManager::Instance()->GetScreenManager()->AddScreen(new MainMenuScreen());
	GameManager::Instance()->StartGame();
	GameManager::Instance()->EndGame();

#if _DEBUG
	delete mem;
#endif
}