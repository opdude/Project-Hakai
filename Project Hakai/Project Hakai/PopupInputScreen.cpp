#include "PopupInputScreen.h"
#include "Button.h"
#include "HelperFunctions.h"
#include "GameTime.h"
#include "KeyboardInputBox.h"
#include "Label.h"
#include "Panel.h"
#include "ScreenManager.h"

#define INPUTBOX_ID 1
#define LABEL_ID 2
#define ACCEPT_ID 3
#define CANCEL_ID 4


PopupInputScreen::PopupInputScreen(char* text)
{
	m_sText = text;
	m_bIsPopup = true;
	m_pGUI = new hgeGUI(GetScreenWidth(),GetScreenHeight());
	m_vRejectedKeys = vector<char*>();
}

PopupInputScreen::~PopupInputScreen()
{
	delete m_pGUI;
}

void PopupInputScreen::LoadContent()
{
	const int vPad = 300;
	const int hPad = 400;
	const int iPad = 100;
	const float w = hPad*2 + m_pScreenManager->GetFont()->GetStringWidth("A") * 20;
	const float h = vPad*2 + m_pScreenManager->GetFont()->GetScaledHeight()*4 + iPad*2;
	const float x = GetScreenWidth()/2 - w/2;
	const float y = GetScreenHeight()/2 - h/2;
	Panel* panel = new Panel(this, x,y,w,h);

	Label* label = new Label(this, LABEL_ID, x+hPad, y+vPad, 0,0, m_pScreenManager->GetFont());
	label->SetMode(HGETEXT_LEFT);
	label->SetText(m_sText);

	KeyboardInputBox* inputBox = new KeyboardInputBox(INPUTBOX_ID, this, 35, x+hPad,y+vPad+iPad + m_pScreenManager->GetFont()->GetScaledHeight(),
		w-hPad*2, true, m_vRejectedKeys);
	inputBox->EnterPressed.connect(&PopupInputScreen::EnterPressed);
	inputBox->EscPressed.connect(&PopupInputScreen::EscPressed);

	Button* cancelButton = new Button(this, CANCEL_ID, "Cancel", x+w-hPad, inputBox->rect.y2+iPad);
	cancelButton->Clicked.connect(&PopupInputScreen::EscPressed);
	cancelButton->rect.Move(-(cancelButton->rect.x2 - cancelButton->rect.x1), 0);
	Button* acceptButton = new Button(this, ACCEPT_ID, "OK", x+hPad, inputBox->rect.y2+iPad, cancelButton->rect.x2 - cancelButton->rect.x1);
	acceptButton->Clicked.connect(&PopupInputScreen::EnterPressed);
	acceptButton->SetCenteredText(true);

	m_pGUI->AddCtrl(panel);
	m_pGUI->AddCtrl(label);
	m_pGUI->AddCtrl(inputBox);
	m_pGUI->AddCtrl(acceptButton);
	m_pGUI->AddCtrl(cancelButton);
	m_pGUI->SetFocus(0);
	m_pGUI->Enter();

	CursorScreen::LoadContent();
}

void PopupInputScreen::UnloadContent()
{

}

void PopupInputScreen::Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
{
	m_pGUI->Update(gameTime->GetTotalTime(),gameTime->GetDeltaTime(), !otherScreenHasFocus);

	CursorScreen::Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);
}

void PopupInputScreen::HandleInput(InputState* inputState, GameTime* gameTime)
{
	((KeyboardInputBox*)m_pGUI->GetCtrl(INPUTBOX_ID))->HandleInput(inputState,gameTime);
}

void PopupInputScreen::Draw(HGE *hge, GameTime *gameTime)
{
	m_pScreenManager->FadeBufferToBlack(GetTransitionAlpha() * 2 / 3);
	m_pGUI->Render();

	CursorScreen::Draw(hge,gameTime);
}

void PopupInputScreen::AddRejectedKey(char* key)
{
	m_vRejectedKeys.push_back(key);
}

void PopupInputScreen::EnterPressed(GameScreen* screen, hgeGUIObject* object)
{
	PopupInputScreen* pScreen = ((PopupInputScreen*)screen);
	pScreen->InputAccepted(screen, ((KeyboardInputBox*)pScreen->m_pGUI->GetCtrl(INPUTBOX_ID))->m_sText.c_str());
	screen->ExitScreen();
}

void PopupInputScreen::EscPressed(GameScreen* screen, hgeGUIObject* object)
{
	((PopupInputScreen*)screen)->InputRejected(screen);
	screen->ExitScreen();
}