#include "MenuScreen.h"
#include "HelperFunctions.h"
#include "GameManager.h"
#include "GameTime.h"
#include "InputState.h"
#include "MenuEntry.h"
#include "MenuTitle.h"
#include "ScreenManager.h"

#define MENUENTRY_X 600

MenuScreen::MenuScreen(string title)
{
	m_iMenuEntryCount = 0;
	m_sTitle = title;
	m_pGUI = new hgeGUI(GetScreenWidth(),GetScreenHeight());
}

void MenuScreen::LoadContent()
{
	m_pGUI->SetNavMode(HGEGUI_CYCLED | HGEGUI_UPDOWN);
	m_pGUI->SetFocus(0);
	m_pGUI->Enter();

	MenuTitle* title = new MenuTitle(this, (char*)m_sTitle.c_str(), 0);
	title->SetDesiredPosition(Vector2((GetScreenWidth() / 2) - (m_pScreenManager->GetMenuFont()->GetStringWidth(m_sTitle.c_str()) / 2),500));
	m_pGUI->AddCtrl(title);

	CursorScreen::LoadContent();
}

void MenuScreen::UnloadContent()
{
	delete m_pGUI;
}

void MenuScreen::AddMenuEntry(MenuEntry* menuEntry)
{
	float y = 1500 + (100 + (m_pScreenManager->GetMenuFont()->GetScaledHeight())) *  m_iMenuEntryCount;
	menuEntry->SetDesiredPosition(Vector2((float)MENUENTRY_X, y));
	menuEntry->id = ++m_iMenuEntryCount;

	m_pGUI->AddCtrl(menuEntry);
}

void MenuScreen::Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	int focus = m_pGUI->Update(gameTime->GetTotalTime(), gameTime->GetDeltaTime(), !otherScreenHasFocus);
	if (focus > 0)
		m_pGUI->SetFocus(focus);

	CursorScreen::Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);
}

void MenuScreen::HandleInput(InputState* inputState, GameTime* gameTime)
{
	//Cancel Window
	if (inputState->GetMouseState()->GetMouseRightClick())
		OnCancel();
}

void MenuScreen::Draw(HGE *hge, GameTime *gameTime)
{
	m_pGUI->Render();
	CursorScreen::Draw(hge, gameTime);
}

void MenuScreen::ExitScreen()
{
	m_pGUI->Leave();

	CursorScreen::ExitScreen();
}

void MenuScreen::OnCancel()
{
	GameScreen::ExitScreen();
}