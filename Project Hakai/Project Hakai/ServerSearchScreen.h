//
//	ServerSearchScreen.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _SERVERSEARCHSCREEN_H
#define _SERVERSEARCHSCREEN_H

#include <RakPeerInterface.h>
#include <RakNetTime.h>
#include "MenuScreen.h"

class GameScreen;

using namespace RakNet;

class ServerSearchScreen : public MenuScreen
{
public:
	ServerSearchScreen();
	~ServerSearchScreen();

	virtual void LoadContent();
	virtual void UnloadContent();
	
	virtual void Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
	virtual void Draw(HGE* hge, GameTime* gameTime);

	void SearchForGames();

private:
	static void GameClicked(GameScreen* screen, int rowCount, void* object);
	static void BackMenuEntrySelected(GameScreen* screen);

	RakNet::RakPeerInterface* m_pNetSession;
	bool m_bConnectedToGame;
};

#endif