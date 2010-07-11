//
//	OptionsMenuScreen.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "OptionsMenuScreen.h"
#include "Button.h"
#include "CVar.h"
#include "Console.h"
#include "HelperFunctions.h"
#include "ListBox.h"
#include "Label.h"
#include "MenuEntry.h"
#include "ScreenManager.h"
#include "Slider.h"
#include "Panel.h"
#include "PopupMessageBoxScreen.h"
#include "GameManager.h"

#define SCREEN_RES_LABEL	2
#define SCREEN_RES_LIST		3
#define SCREEN_MODE_LABEL	4
#define SCREEN_MODE_LIST	5
#define VOLUME_LABEL		6
#define VOLUME_SLIDER		7
#define APPLY_BUTTON		8
#define CANCEL_BUTTON		9

#define PAD_LEFT			500
#define PAD_RIGHT			500
#define PAD_TOP				500
#define PAD_BOTTOM			500
#define PAD_INSIDE			300
#define PAD_TEXT			200


OptionsMenuScreen::OptionsMenuScreen() : MenuScreen("Options")
{

}

OptionsMenuScreen::~OptionsMenuScreen()
{
	m_vResolutions.clear();

	MenuScreen::~MenuScreen();
}

void OptionsMenuScreen::LoadContent()
{
	//Setup Positions
	const float COMPONENT_WIDTH = m_pScreenManager->GetFont()->GetStringWidth("AAAAAAAAAA");
	const float TEXT_HEIGHT = m_pScreenManager->GetFont()->GetScaledHeight();
	const float PANEL_W = PAD_LEFT + PAD_RIGHT + (PAD_INSIDE * 2) + (COMPONENT_WIDTH * 2);
	const float PANEL_H = PAD_TOP + (TEXT_HEIGHT * 10) + (PAD_INSIDE * 2) + PAD_BOTTOM;
	const float PANEL_X = GetScreenWidth()/2 - PANEL_W/2;
	const float PANEL_Y = GetScreenHeight()/2 - PANEL_H/2;
	const float SCREEN_RES_LABEL_X =	PANEL_X + PAD_LEFT;
	const float SCREEN_RES_LABEL_Y =	PANEL_Y + PAD_TOP;
	const float SCREEN_RES_LIST_X	=	PANEL_X + PAD_LEFT;
	const float SCREEN_RES_LIST_Y	=	SCREEN_RES_LABEL_Y + PAD_TEXT;
	const float SCREEN_RES_LIST_W	=	COMPONENT_WIDTH;
	const float SCREEN_MODE_LABEL_X =	SCREEN_RES_LIST_X + SCREEN_RES_LIST_W + PAD_INSIDE * 2;
	const float SCREEN_MODE_LABEL_Y =	PANEL_Y + PAD_TOP;
	const float SCREEN_MODE_LIST_X =	SCREEN_MODE_LABEL_X;
	const float SCREEN_MODE_LIST_Y =	SCREEN_MODE_LABEL_Y + PAD_TEXT;
	const float SCREEN_MODE_LIST_W =	COMPONENT_WIDTH;
	const float VOLUME_LABEL_X =		PANEL_X + PAD_LEFT;
	const float VOLUME_LABEL_Y =		SCREEN_RES_LIST_Y + (TEXT_HEIGHT * 5 + PAD_INSIDE);
	const float VOLUME_SLIDER_X =		VOLUME_LABEL_X;
	const float VOLUME_SLIDER_Y =		VOLUME_LABEL_Y + PAD_INSIDE;
	const float VOLUME_SLIDER_W =		COMPONENT_WIDTH;
	const float VOLUME_SLIDER_H =		TEXT_HEIGHT;
	const float APPLY_BUTTON_X =		PANEL_X + PAD_LEFT;
	const float APPLY_BUTTON_Y =		VOLUME_SLIDER_Y + VOLUME_SLIDER_H + PAD_INSIDE;
	const float APPLY_BUTTON_W =		COMPONENT_WIDTH;
	const float APPLY_BUTTON_H =		TEXT_HEIGHT;

	//Panel
	Panel* panel = new Panel(this, PANEL_X, PANEL_Y,PANEL_W,PANEL_H);

	//Resolution
	Label* resoText = new Label(this, SCREEN_RES_LABEL,SCREEN_RES_LABEL_X,SCREEN_RES_LABEL_Y,0,0,m_pScreenManager->GetFont());
	resoText->SetText("Resolution");
	resoText->SetMode(HGETEXT_LEFT);

	resoListBox = new ListBox(SCREEN_RES_LIST,this, m_pScreenManager->GetFont(), SCREEN_RES_LIST_X,SCREEN_RES_LIST_Y,SCREEN_RES_LIST_W,5);
	GetResolutions();

	int i =0;
	for each(DEVMODE d in m_vResolutions)
	{
		char str[20];
		sprintf(str, "%i X %i", d.dmPelsWidth, d.dmPelsHeight);
		resoListBox->AddItem(str);

		if (d.dmPelsWidth == Console::Instance()->GetCVar("ScreenWidth")->ToFloat() &&
			d.dmPelsHeight == Console::Instance()->GetCVar("ScreenHeight")->ToFloat())
		{
			resoListBox->SetSelectedItem(i);
		}
		i++;
	}

	//Display Mode
	Label* displayText = new Label(this, SCREEN_MODE_LABEL,SCREEN_MODE_LABEL_X,SCREEN_MODE_LABEL_Y,0,0,m_pScreenManager->GetFont());
	displayText->SetText("Display Mode");
	displayText->SetMode(HGETEXT_LEFT);

	displayList = new ListBox(SCREEN_MODE_LIST,this,m_pScreenManager->GetFont(),SCREEN_MODE_LIST_X,SCREEN_MODE_LIST_Y,SCREEN_MODE_LIST_W,5);
	displayList->AddItem("Fullscreen");
	displayList->AddItem("Windowed");

	if (!Console::Instance()->GetCVar("FullScreen")->ToBool())
		displayList->SetSelectedItem(1);

	//Sound
	Label* soundText = new Label(this, VOLUME_LABEL,VOLUME_LABEL_X,VOLUME_LABEL_Y,0,0,m_pScreenManager->GetFont());
	soundText->SetText("Volume");
	soundText->SetMode(HGETEXT_LEFT);

	HTEXTURE slider = GameManager::Instance()->GetResourceManager()->GetTexture("Slider");
	soundSlider = new Slider(this, VOLUME_SLIDER, VOLUME_SLIDER_X, VOLUME_SLIDER_Y, VOLUME_SLIDER_W, VOLUME_SLIDER_H, slider,0,0,
		ScrXToVirt(GetTextureSize(slider).x), ScrYToVirt(GetTextureSize(slider).y));
	soundSlider->SetMode(0,100,HGESLIDER_SLIDER);
	soundSlider->SetValue(Console::Instance()->GetCVar("AudioLevel")->ToFloat());

	//Apply Button
	Button* applyButton = new Button(this, APPLY_BUTTON, "Apply", APPLY_BUTTON_X, APPLY_BUTTON_Y);
	applyButton->Clicked.connect(&OptionsMenuScreen::ApplyButtonPushed);

	MenuEntry* backMenuEntry = new MenuEntry(this, "Back");
	backMenuEntry->Clicked.connect(&OptionsMenuScreen::BackMenuEntrySelected);
	backMenuEntry->SetDesiredPosition(Vector2(1000, GetScreenHeight() - 1000));

	//Add Controls
	m_pGUI->AddCtrl(panel);
	m_pGUI->AddCtrl(resoText);
	m_pGUI->AddCtrl(resoListBox);
	m_pGUI->AddCtrl(displayText);
	m_pGUI->AddCtrl(displayList);
	m_pGUI->AddCtrl(soundText);
	m_pGUI->AddCtrl(soundSlider);
	m_pGUI->AddCtrl(applyButton);
	m_pGUI->AddCtrl(backMenuEntry);
	
	MenuScreen::LoadContent();
	m_pGUI->SetNavMode(HGEGUI_NONAVKEYS);
}

void OptionsMenuScreen::ApplyButtonPushed(GameScreen* screen, hgeGUIObject* button)
{
	//Store Old Values
	bool oldFullScreen = Console::Instance()->GetCVar("FullScreen")->ToBool();
	float screenWidth = Console::Instance()->GetCVar("ScreenWidth")->ToFloat();
	float screenHeight = Console::Instance()->GetCVar("ScreenHeight")->ToFloat();

	OptionsMenuScreen* opScreen = (OptionsMenuScreen*)screen;
	Console::Instance()->SetCVar("FullScreen",(opScreen->displayList->GetSelectedItem() == 0) ? "true" : "false");

	DEVMODE dm = opScreen->m_vResolutions[opScreen->resoListBox->GetSelectedItem()];

	Console::Instance()->SetCVar("ScreenWidth",toString(dm.dmPelsWidth).c_str());
	Console::Instance()->SetCVar("ScreenHeight",toString(dm.dmPelsHeight).c_str());
	Console::Instance()->SetCVar("AudioLevel",toString(opScreen->soundSlider->GetValue()).c_str());

	//Check for screen setting changes
	if (oldFullScreen != Console::Instance()->GetCVar("FullScreen")->ToBool() ||
		screenWidth != Console::Instance()->GetCVar("ScreenWidth")->ToFloat() ||
		screenHeight != Console::Instance()->GetCVar("ScreenHeight")->ToFloat())
	{
		//Show Popup
		screen->GetScreenManager()->AddScreen(new PopupMessageBoxScreen("Screen Settings Changed","In order to use your new screen settings please restart the game",true));
	}
}

void OptionsMenuScreen::BackMenuEntrySelected(GameScreen* screen)
{
	screen->ExitScreen();
}

void OptionsMenuScreen::GetResolutions()
{
	m_vResolutions = std::vector<DEVMODE>();

	DEVMODE dm;
	DEVMODE lastdm;

	// initialize the DEVMODE structure
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);

	int i=0;
	while (0 != EnumDisplaySettings(NULL, i++, &dm))
	{
		if (m_vResolutions.size() == 0 || dm.dmPelsWidth != lastdm.dmPelsWidth)
		{
			m_vResolutions.push_back(dm);
			lastdm = dm;
		}
	}

}