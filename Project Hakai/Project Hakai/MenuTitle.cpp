#include "MenuTitle.h"
#include "GameScreen.h"
#include "ScreenManager.h"

MenuTitle::MenuTitle(MenuScreen* screen, char* text, float scale)
{
	m_pGameScreen = screen;
	m_sText = text;
	m_fScale = scale;
	m_cNormalColour = ARGB(255,255,255,255);
	m_cHighlightColour = ARGB(255,0,255,255);
	m_cColour = m_cNormalColour;

	bStatic = false;
	bVisible = true;
	bEnabled = true;

	float w = m_pGameScreen->GetScreenManager()->GetMenuFont()->GetStringWidth(m_sText);
	float h = m_pGameScreen->GetScreenManager()->GetMenuFont()->GetScaledHeight();
	rect.Set(GetScreenWidth(),GetScreenHeight(),GetScreenWidth()+w,GetScreenHeight()+h);
}

void MenuTitle::LoadContent()
{

}

void MenuTitle::UnloadContent()
{

}

void MenuTitle::Enter()
{

}

void MenuTitle::Leave()
{

}

bool MenuTitle::IsDone()
{
	if (m_pGameScreen->GetState() == Active || m_pGameScreen->GetState() == Hidden)
		return true;
	else
		return false;
}

void MenuTitle::Focus(bool bFocused)
{
}

void MenuTitle::MouseOver(bool bOver)
{
}

bool MenuTitle::MouseLButton(bool bDown)
{
	return false;
}

bool MenuTitle::KeyClick(int key, int chr)
{
	return false;
}

void MenuTitle::Update(float gt, float dt)
{
	float transitionOffset = pow(m_pGameScreen->GetTransitionPosition(), 2);

	Vector2 pos = m_vDesiredPos;
	if (m_pGameScreen->GetState() == TransitionOn)
		pos -= Vector2(0, transitionOffset * 3000);
	else
		pos += Vector2(0, transitionOffset * -3000);
	SetPosition(pos);

	m_cColour.a = m_pGameScreen->GetTransitionAlpha();
}

void MenuTitle::Render()
{
	float shadowAlpha = (m_pGameScreen->GetTransitionAlpha()* 255 > 100) ? 100 : m_pGameScreen->GetTransitionAlpha();
	m_pGameScreen->GetScreenManager()->GetMenuFont()->SetColor(ARGB(shadowAlpha,0,0,0));
	m_pGameScreen->GetScreenManager()->GetMenuFont()->Render(rect.x1+30,rect.y1+50, HGETEXT_LEFT, m_sText);
	m_pGameScreen->GetScreenManager()->GetMenuFont()->SetColor(m_cColour.GetHWColor());
	m_pGameScreen->GetScreenManager()->GetMenuFont()->Render(rect.x1,rect.y1, HGETEXT_LEFT, m_sText);
}

