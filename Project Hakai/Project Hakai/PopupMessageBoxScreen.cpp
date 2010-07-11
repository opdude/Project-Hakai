//
//	PopupMessageBoxScreen.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "PopupMessageBoxScreen.h"
#include "Button.h"
#include "HelperFunctions.h"
#include "GameTime.h"
#include "Label.h"
#include "Panel.h"
#include "ScreenManager.h"

#define TITLE_ID 1
#define MESSAGE_ID 2
#define ACCEPT_ID 3
#define CANCEL_ID 4
#define MIN_WIDTH 4000

PopupMessageBoxScreen::PopupMessageBoxScreen(char *title, char *message, bool okayButtonOnly)
{
	m_sTitle = title;
	m_sText = message;
	m_bOkayButtonOnly = okayButtonOnly;
	m_pGUI = new hgeGUI(GetScreenWidth(),GetScreenHeight());
	m_bIsPopup = true;

	m_fTransitionOnTime = 200;
	m_fTransitionOffTime = 200;
}

PopupMessageBoxScreen::~PopupMessageBoxScreen()
{
	delete m_pGUI;
}

void PopupMessageBoxScreen::LoadContent()
{
	const int vPad = 300;
	const int hPad = 400;
	const int iPad = 300;
	const float w = (hPad*2 + m_pScreenManager->GetFont()->GetStringWidth(m_sText) > MIN_WIDTH) ? 
		hPad*2 + m_pScreenManager->GetFont()->GetStringWidth(m_sText) : MIN_WIDTH;
	const float h = vPad*2 + m_pScreenManager->GetFont()->GetScaledHeight()*3 + iPad*2;

	const float x = GetScreenWidth()/2 - w/2;
	const float y = GetScreenHeight()/2 - h/2;

	//Create Panel
	Panel* panel = new Panel(this, x,y,w,h);

	//Create Title
	Label* titleLabel = new Label(this, TITLE_ID, x+hPad, y+vPad, 0,0, m_pScreenManager->GetFont());
	titleLabel->SetMode(HGETEXT_LEFT);
	titleLabel->SetText(m_sTitle);

	//Create Message
	Label* messageLabel = new Label(this, MESSAGE_ID, x+hPad, titleLabel->rect.y2+iPad, 0,
		m_pScreenManager->GetFont()->GetScaledHeight(), m_pScreenManager->GetFont());
	messageLabel->SetMode(HGETEXT_LEFT);
	messageLabel->SetText(m_sText);

	m_pGUI->AddCtrl(panel);
	m_pGUI->AddCtrl(titleLabel);
	m_pGUI->AddCtrl(messageLabel);

	//Create Buttons
	Button* acceptButton;
	if (m_bOkayButtonOnly)
	{
		acceptButton = new Button(this, ACCEPT_ID, " OK ", 0, messageLabel->rect.y2+iPad);
		acceptButton->rect.Move((w/2 -(acceptButton->rect.x2 - acceptButton->rect.x1)/2)+x, 0);
		acceptButton->Clicked.connect(&PopupMessageBoxScreen::ButtonRejected);
		acceptButton->SetCenteredText(true);	
	}
	else
	{
		Button* cancelButton = new Button(this, CANCEL_ID, "Cancel", x+w-hPad, messageLabel->rect.y2+iPad);
		cancelButton->Clicked.connect(&PopupMessageBoxScreen::ButtonAccepted);
		cancelButton->rect.Move(-(cancelButton->rect.x2 - cancelButton->rect.x1), 0);
		m_pGUI->AddCtrl(cancelButton);

		acceptButton = new Button(this, ACCEPT_ID, "OK", x+hPad, messageLabel->rect.y2+iPad, cancelButton->rect.x2 - cancelButton->rect.x1);
		acceptButton->Clicked.connect(&PopupMessageBoxScreen::ButtonRejected);
		acceptButton->SetCenteredText(true);	
	}

	m_pGUI->AddCtrl(acceptButton);
	m_pGUI->SetFocus(0);
	m_pGUI->Enter();

	CursorScreen::LoadContent();
}

void PopupMessageBoxScreen::UnloadContent()
{

}

void PopupMessageBoxScreen::HandleInput(InputState *inputState, GameTime *gameTime)
{

}

void PopupMessageBoxScreen::Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	m_pGUI->Update(gameTime->GetTotalTime(), gameTime->GetDeltaTime(), !otherScreenHasFocus);

	CursorScreen::Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);
}

void PopupMessageBoxScreen::Draw(HGE *hge, GameTime *gameTime)
{
	m_pScreenManager->FadeBufferToBlack(GetTransitionAlpha() * 2 / 3);
	m_pGUI->Render();

	CursorScreen::Draw(hge,gameTime);
}

void PopupMessageBoxScreen::ButtonAccepted(GameScreen *screen, hgeGUIObject* object)
{
	((PopupMessageBoxScreen*)screen)->Accepted(screen);
	screen->ExitScreen();
}

void PopupMessageBoxScreen::ButtonRejected(GameScreen *screen, hgeGUIObject* object)
{
	((PopupMessageBoxScreen*)screen)->Rejected(screen);
	screen->ExitScreen();
}