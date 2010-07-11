#include "Button.h"
#include <hgefont.h>
#include "GameScreen.h"
#include "HelperFunctions.h"
#include "ScreenManager.h"
#include "GameManager.h"

#define V_PAD 10
#define H_PAD 10

Button::Button(GameScreen* screen, int _id, char* text, float x, float y, float w, float h)
{
	m_pScreen = screen;
	id=_id;
	m_sText = text;
	bStatic=false;
	bVisible=true;
	bEnabled=true;

	m_bPressed=false;
	m_bTrigger=false;
	m_cButton = hgeColor(ARGB(100,24,71,90));
	m_cLine = hgeColor(ARGB(255,20,61,77));
	m_cHighlightLine = hgeColor(ARGB(255,73,116,133));
	m_cText = hgeColor(ARGB(255,255,255,255));
	m_cPushedText = hgeColor(ARGB(255,100,100,100));

	if (w==0)
		w = ScrXToVirt(H_PAD) + screen->GetScreenManager()->GetButtonFont()->GetStringWidth(text);

	if (h==0)
		h = ScrYToVirt(V_PAD) + screen->GetScreenManager()->GetButtonFont()->GetScaledHeight();

	rect.Set(x, y, x+w, y+h);

	HTEXTURE tex = GameManager::Instance()->GetResourceManager()->GetTexture("Blank");
	m_pButton = new hgeSprite(tex, 8,8,w,h);
	m_pButton->SetColor(m_cButton.GetHWColor());
	m_pLine = new hgeSprite(tex, 0,0,1,1);
}

Button::~Button()
{
	if(m_pButton) delete m_pButton;
	if(m_pLine) delete m_pLine;
}

void Button::Render()
{
	//Update Colour
	m_cButton.a = m_pScreen->GetTransitionAlpha();
	m_cLine.a = m_pScreen->GetTransitionAlpha();
	m_cHighlightLine.a = m_pScreen->GetTransitionAlpha();
	m_cText.a = m_pScreen->GetTransitionAlpha();
	m_cPushedText.a = m_pScreen->GetTransitionAlpha();

	//Render Boxes
	m_pButton->SetColor(m_cButton.GetHWColor());
	m_pButton->Render(rect.x1,rect.y1);

	//Render Lines
	if (m_bPressed)
		m_pLine->SetColor(m_cLine.GetHWColor());
	else
		m_pLine->SetColor(m_cHighlightLine.GetHWColor());
	m_pLine->RenderStretch(rect.x1,rect.y1,rect.x2,rect.y1+ScrYToVirt(1));
	m_pLine->RenderStretch(rect.x1,rect.y1,rect.x1+ScrXToVirt(1),rect.y2);

	if (m_bPressed)
		m_pLine->SetColor(m_cHighlightLine.GetHWColor());
	else
		m_pLine->SetColor(m_cLine.GetHWColor());

	m_pLine->RenderStretch(rect.x2-ScrXToVirt(1),rect.y1,rect.x2,rect.y2);
	m_pLine->RenderStretch(rect.x1,rect.y2,rect.x2,rect.y2+ScrYToVirt(1));

	if (m_bPressed)
		m_pScreen->GetScreenManager()->GetButtonFont()->SetColor(m_cPushedText.GetHWColor());
	else
		m_pScreen->GetScreenManager()->GetButtonFont()->SetColor(m_cText.GetHWColor());

	if (m_bCenteredText)
		m_pScreen->GetScreenManager()->GetButtonFont()->Render(rect.x1 + ((rect.x2 - rect.x1)/2), rect.y1 + ScrYToVirt(V_PAD/2), HGETEXT_CENTER, m_sText);
	else
		m_pScreen->GetScreenManager()->GetButtonFont()->Render(rect.x1 + ScrXToVirt(H_PAD/2), rect.y1 + ScrYToVirt(V_PAD/2), HGETEXT_LEFT, m_sText);
}

bool Button::MouseLButton(bool bDown)
{
	if(bDown)
	{
		m_bOldState=m_bPressed; m_bPressed=true;
		return false;
	}
	else
	{
		if (m_bPressed)
			Clicked(m_pScreen, this);

		if(m_bTrigger) m_bPressed=!m_bOldState;
		else m_bPressed=false;
		return true; 
	}
}