//
//	GameplayScreen.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _GAMEPLAYSCREEN_H
#define _GAMEPLAYSCREEN_H

#include <hge.h>
#include "GameScreen.h"

class GameTime;
class InputState;

class GameplayScreen : public GameScreen
{
public:
	GameplayScreen();
	~GameplayScreen();

	virtual void LoadContent();
	virtual void UnloadContent();
	virtual void Update(GameTime* gameTime);
	virtual void HandleInput(InputState* inputState, GameTime* gameTime);
	virtual void Draw(HGE* hge, GameTime* gameTime);

private:
	hgeSprite* sprite;
};

#endif