#include "GameScreen.h"
#include "GameTime.h"
#include "GameManager.h"
#include "HelperFunctions.h"
#include "ScreenManager.h"

GameScreen::GameScreen()
{
	m_eState = TransitionOn;
	m_fTransitionOnTime = 1000;
	m_fTransitionOffTime = 500;
	m_fTransitionPosition = 1;
	m_bIsPopup = false;
}

GameScreen::~GameScreen()
{
}

void GameScreen::Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	m_bOtherScreenHasFocus = otherScreenHasFocus;

	if (IsExiting())
	{
		m_eState = TransitionOff;

		if (!UpdateTransition(gameTime, m_fTransitionOffTime, 1))
			m_pScreenManager->RemoveScreen(this);
	}
	else if (coveredByOtherScreen)
	{
		if (UpdateTransition(gameTime, m_fTransitionOffTime, 1))
			m_eState = TransitionOff;
		else
			m_eState = Hidden;
	}
	else
	{
		if (UpdateTransition(gameTime, m_fTransitionOnTime, -1))
			m_eState = TransitionOn;
		else
			m_eState = Active;
	}
}

bool GameScreen::UpdateTransition(GameTime *gameTime, float time, int direction)
{
	float transitionDelta;

	if (time == 0)
		transitionDelta = 1;
	else
		transitionDelta = gameTime->GetDeltaTime() * 1000 / time;

	m_fTransitionPosition += transitionDelta * direction;

	if (((direction < 0) && (m_fTransitionPosition <= 0)) ||
		((direction > 0) && (m_fTransitionPosition >= 1)))
	{
		m_fTransitionPosition = Clamp(m_fTransitionPosition, 0, 1);
		return false;
	}

	return true;
}

void GameScreen::ExitScreen()
{
	if (m_fTransitionOffTime == 0)
	{
		m_pScreenManager->RemoveScreen(this);
	}
	else
	{
		m_bIsExiting = true;
	}
}

hgeSprite* CursorScreen::m_sCursor = NULL;

CursorScreen::CursorScreen()
{
}
CursorScreen::~CursorScreen()
{
	if (CursorScreen::m_sCursor && !GameManager::m_bRunning)
	{
		//delete CursorScreen::m_sCursor;
		//CursorScreen::m_sCursor = NULL;
	}
}
void CursorScreen::LoadContent()
{
	if (CursorScreen::m_sCursor == NULL)
	{
		m_sCursor = GameManager::Instance()->GetResourceManager()->GetSprite("Cursor");
	}
}
void CursorScreen::Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	m_bOtherScreenHasFocus = otherScreenHasFocus;

	GameScreen::Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);
}
void CursorScreen::Draw(HGE* hge, GameTime* gameTime)
{
	float x, y;
	GameManager::Instance()->GetHGE()->Input_GetMousePos(&x,&y);

	if (!m_bOtherScreenHasFocus && CursorScreen::m_sCursor)
		CursorScreen::m_sCursor->RenderStretch(ScrXToVirt(x),ScrYToVirt(y), ScrXToVirt(x)+ScrXToVirt(32), ScrYToVirt(y)+ScrYToVirt(32));
}