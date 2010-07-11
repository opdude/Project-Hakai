#include "GameplayScreen.h"
#include "GameManager.h"

GameplayScreen::GameplayScreen() : GameScreen()
{

}

GameplayScreen::~GameplayScreen()
{

}

void GameplayScreen::LoadContent()
{
	HTEXTURE tex = GameManager::Instance()->GetHGE()->Texture_Load("Content/Materials/Particle.dds");

	sprite = new hgeSprite(tex, 0,0, (float)GameManager::Instance()->GetHGE()->Texture_GetWidth(tex),
									(float)GameManager::Instance()->GetHGE()->Texture_GetHeight(tex));
}

void GameplayScreen::UnloadContent()
{

}

void GameplayScreen::Update(GameTime* gameTime)
{

}

void GameplayScreen::HandleInput(InputState* inputState, GameTime* gameTime)
{

}

void GameplayScreen::Draw(HGE* hge, GameTime* gameTime)
{
	sprite->Render(20,20);
}