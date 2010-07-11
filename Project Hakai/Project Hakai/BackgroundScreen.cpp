//
//	BackgroundScreen.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "BackgroundScreen.h"
#include "GameManager.h"
#include "HelperFunctions.h"
#include "GameTime.h"

#include "Console.h"

BackgroundScreen::BackgroundScreen()
{
	minSpeed = 0.01f;
	maxSpeed = 10;
	minCloudScale = 0.3f;
	maxCloudScale = 10.0f;

	SetTransitionOnTime(0.5);
	SetTransitionOffTime(0.5);
}

void BackgroundScreen::LoadContent()
{
	m_pBackground = GameManager::Instance()->GetResourceManager()->GetSprite("Background");
	m_pCloud = GameManager::Instance()->GetResourceManager()->GetSprite("Cloud");

	for (int i=0; i < 5; i++)
	{
		m_vClouds.push_back(new Cloud());
		m_vClouds[i]->currentCloudPos = Vector2(RandomFloat(0,GetScreenWidth()),RandomFloat(0,GetScreenHeight()));
		m_vClouds[i]->nextCloudPos = Vector2(RandomFloat(0,GetScreenWidth()),RandomFloat(0,GetScreenHeight()));
		float scale = RandomFloat(minCloudScale, maxCloudScale);
		m_vClouds[i]->currentCloudScale = scale;
		m_vClouds[i]->nextCloudScale = scale;
		m_vClouds[i]->cloudSpeed = RandomFloat(minSpeed,maxSpeed);
		m_vClouds[i]->currentAlpha = RandomInt(MIN_ALPHA, MAX_ALPHA);
		m_vClouds[i]->nextAlpha = RandomInt(MIN_ALPHA, MAX_ALPHA);
		m_vClouds[i]->currentRot = RandomFloat(0, 360);
		m_vClouds[i]->nextRot = RandomFloat(0, 360);
	}
}

void BackgroundScreen::UnloadContent()
{
	for each(Cloud* c in m_vClouds)
		delete c;
	m_vClouds.clear();
}

void BackgroundScreen::Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	//Update Clouds
	for each (Cloud* cld in m_vClouds)
	{
		if ((cld->nextCloudPos - cld->currentCloudPos).length() < 10.0f)
		{
			cld->nextCloudPos = RandomVector2(0,GetScreenWidth(), 0, GetScreenHeight());
			cld->cloudSpeed = RandomFloat(minSpeed, maxSpeed);
			cld->nextRot = RandomFloat(0, 360);
		}
		if ((cld->currentCloudScale - cld->nextCloudScale) < 0.5f)
		{
			cld->nextCloudScale = RandomFloat(minCloudScale, maxSpeed);
			cld->nextAlpha = RandomInt(MIN_ALPHA, MAX_ALPHA);
		}

		//Update Cloud Pos
		Vector2 move = cld->nextCloudPos - cld->currentCloudPos;
		move.normalize();
		move *= cld->cloudSpeed * gameTime->GetDeltaTime();
		cld->currentCloudPos += move;

		//Update Cloud Size
		float size = cld->nextCloudScale - cld->currentCloudScale;
		size *= 0.006f  * gameTime->GetDeltaTime();
		cld->currentCloudScale += size;

		//Update Cloud Alpha
		int tmpAlpha = cld->nextAlpha - cld->currentAlpha;
		cld->currentAlpha += (int)((tmpAlpha * 0.001f)  * gameTime->GetDeltaTime());

		//Update Cloud Rotation
		float tmpRot = cld->nextRot - cld->currentRot;
		cld->currentRot += (tmpRot * 0.1f) * gameTime->GetDeltaTime();
	}

	GameScreen::Update(gameTime, otherScreenHasFocus, false);
}

void BackgroundScreen::Draw(HGE* hge, GameTime* gameTime)
{
	m_pBackground->RenderStretch(0,0,GetScreenWidth(),GetScreenHeight());

	for each(Cloud* c in m_vClouds)
	{
		m_pCloud->SetColor(ARGB(c->currentAlpha, 255,255,255));
		m_pCloud->RenderEx(c->currentCloudPos.x,c->currentCloudPos.y,DegreeToRadian(c->currentRot),c->currentCloudScale, c->currentCloudScale);
	}
}