//
//	MenuEntry.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "MenuEntry.h"
#include "GameScreen.h"
#include "ScreenManager.h"

MenuEntry::MenuEntry(MenuScreen* screen, char* text)
{
	m_pGameScreen = screen;
	m_sText = text;
	m_cNormalColour = ARGB(255,255,255,255);
	m_cHighlightColour = ARGB(255,0,255,255);
	m_cColour = m_cNormalColour;
	m_fSelectionFade = 0;
	m_fPulsate = 0;
	m_bIsHighlighted = false;

	bStatic = false;
	bVisible = true;
	bEnabled = true;

	float w = m_pGameScreen->GetScreenManager()->GetMenuFont()->GetStringWidth(m_sText);
	float h = m_pGameScreen->GetScreenManager()->GetMenuFont()->GetScaledHeight();
	rect.Set(-GetScreenWidth(),-GetScreenHeight(),-GetScreenWidth()+w,-GetScreenHeight()+h);
}

void MenuEntry::LoadContent()
{

}

void MenuEntry::UnloadContent()
{

}

void MenuEntry::Enter()
{

}

void MenuEntry::Leave()
{

}

bool MenuEntry::IsDone()
{
	if (m_pGameScreen->GetState() == Active || m_pGameScreen->GetState() == Hidden)
		return true;
	else
		return false;
}

void MenuEntry::Focus(bool bFocused)
{

}

void MenuEntry::MouseOver(bool bOver)
{
	if (bOver)
		m_cColour = m_cHighlightColour;
	else
		m_cColour = m_cNormalColour;

	m_bIsHighlighted = bOver;
}

bool MenuEntry::MouseLButton(bool bDown)
{
	if (!bDown && m_pGameScreen->GetState() == Active)
	{
		Clicked(m_pGameScreen);
	}

	return false;
}

bool MenuEntry::KeyClick(int key, int chr)
{
	return false;
}

void MenuEntry::Update(float gt, float dt)
{
	float transitionOffset = pow(m_pGameScreen->GetTransitionPosition(), 2);

	Vector2 pos = m_vDesiredPos;
	if (m_pGameScreen->GetState() == TransitionOn)
		pos -= Vector2(transitionOffset * 5000, 0);
	else
		pos += Vector2(transitionOffset * GetScreenWidth(), 0);
	SetPosition(pos);

	m_cColour.a = m_pGameScreen->GetTransitionAlpha();

	float fadeSpeed = dt * 4;

	//Gradually fade between modes
	if (m_bIsHighlighted)
		m_fSelectionFade = (m_fSelectionFade + fadeSpeed < 1) ?  m_fSelectionFade + fadeSpeed : 1;
	else
		m_fSelectionFade = (m_fSelectionFade - fadeSpeed > 0) ? m_fSelectionFade - fadeSpeed : 0;

	m_fPulsate = sin(gt * 6) + 1;
}

void MenuEntry::Render()
{
	//Calculate pulse
	float oldScale = m_pGameScreen->GetScreenManager()->GetMenuFont()->GetScale();;
	float scale = oldScale + m_fPulsate * 1 * m_fSelectionFade;
	m_pGameScreen->GetScreenManager()->GetMenuFont()->SetScale(scale);

	float shadowAlpha = (m_pGameScreen->GetTransitionAlpha() * 255 > 100) ? 100 : m_pGameScreen->GetTransitionAlpha();
	m_pGameScreen->GetScreenManager()->GetMenuFont()->SetColor(ARGB(shadowAlpha,0,0,0));
	m_pGameScreen->GetScreenManager()->GetMenuFont()->Render(rect.x1+30,rect.y1+50, HGETEXT_LEFT, m_sText);
	m_pGameScreen->GetScreenManager()->GetMenuFont()->SetColor(m_cColour.GetHWColor());
	m_pGameScreen->GetScreenManager()->GetMenuFont()->Render(rect.x1,rect.y1, HGETEXT_LEFT, m_sText);

	m_pGameScreen->GetScreenManager()->GetMenuFont()->SetScale(oldScale);
}

