//
//	BackgroundScreen.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _BACKGROUNDSCREEN_H
#define _BACKGROUNDSCREEN_H

#include <vector>
#include "GameScreen.h"
#include "Vector2.h"

using namespace std;

struct Cloud
{
public:
	Vector2 currentCloudPos;
	Vector2 nextCloudPos;
	float currentCloudScale;
	float nextCloudScale;
	float cloudSpeed;
	int currentAlpha;
	int nextAlpha;
	float currentRot;
	float nextRot;
};


class BackgroundScreen : public GameScreen
{
public:
	BackgroundScreen();

	virtual void LoadContent();
	virtual void UnloadContent();

	virtual void Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
	virtual void Draw(HGE* hge, GameTime* gameTime);
	virtual void HandleInput(InputState* inputState, GameTime* gameTime)
	{

	}

private:
	hgeSprite* m_pBackground;
	hgeSprite* m_pCloud;

	//Clouds
	vector<Cloud*> m_vClouds;
	static const int MIN_ALPHA = 2;
	static const int MAX_ALPHA = 15;
	float minCloudScale;
	float maxCloudScale;
	float minSpeed;
	float maxSpeed;
};

#endif