#include "Panel.h"
#include "HelperFunctions.h"
#include "MenuScreen.h"
#include "GameManager.h"

Panel::Panel(GameScreen* screen, float x, float y, float w, float h)
{
	m_pScreen = screen;
	m_cPanel = hgeColor(ARGB(100,24,71,90));
	m_cLine = hgeColor(ARGB(255,20,61,77));
	m_cHighlightLine = hgeColor(ARGB(255,73,116,133));

	HTEXTURE tex = GameManager::Instance()->GetResourceManager()->GetTexture("Blank");
	m_pPanel = new hgeSprite(tex, 8,8,w,h);
	m_pPanel->SetColor(m_cPanel.GetHWColor());
	m_pLine = new hgeSprite(tex, 0,0,1,1);

	rect.Set(x,y,x+w,y+h);
}

Panel::~Panel()
{
	if (m_pPanel) delete m_pPanel;
	if (m_pLine) delete m_pLine;
}

void Panel::Render()
{
	//Update Colour
	m_cPanel.a = m_pScreen->GetTransitionAlpha();
	m_cLine.a = m_pScreen->GetTransitionAlpha();
	m_cHighlightLine.a = m_pScreen->GetTransitionAlpha();

	//Render Boxes
	m_pPanel->SetColor(m_cPanel.GetHWColor());
	m_pPanel->Render(rect.x1,rect.y1);

	//Render Lines
	m_pLine->SetColor(m_cLine.GetHWColor());
	m_pLine->RenderStretch(rect.x1,rect.y1,rect.x2,rect.y1+ScrYToVirt(1));
	m_pLine->RenderStretch(rect.x1,rect.y1,rect.x1+ScrXToVirt(1),rect.y2);
	m_pLine->SetColor(m_cHighlightLine.GetHWColor());
	m_pLine->RenderStretch(rect.x2-ScrXToVirt(1),rect.y1,rect.x2,rect.y2);
	m_pLine->RenderStretch(rect.x1,rect.y2,rect.x2,rect.y2+ScrYToVirt(1));
}