//
//	ChatBox.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "ChatBox.h"
#include "GameScreen.h"
#include "ScreenManager.h"
#include "GameManager.h"

#define MAX_TEXT_LINES 50

ChatBox::ChatBox(int _id, GameScreen* screen, float x, float y, float w, float h)
{
	id = _id;
	m_pScreen = screen;
	rect.Set(x,y,x+w,y+h);
	m_vText = vector<string>();
	m_iCurrentLine = 0;
	m_iChatBoxLines = h/m_pScreen->GetScreenManager()->GetFont()->GetScaledHeight();
	m_cPanel = hgeColor(ARGB(100,24,71,90));
	m_cLine = hgeColor(ARGB(255,20,61,77));
	m_cHighlightLine = hgeColor(ARGB(255,73,116,133));
	m_cText = hgeColor(ARGB(255,255,255,255));

	HTEXTURE tex = GameManager::Instance()->GetResourceManager()->GetTexture("Blank");
	m_pPanel = new hgeSprite(tex, 8,8,w,h);
	m_pPanel->SetColor(m_cPanel.GetHWColor());
	m_pLine = new hgeSprite(tex, 0,0,1,1);

	rect.Set(x,y,x+w,y+h);
}

ChatBox::~ChatBox()
{
	if (m_pPanel) delete m_pPanel;
	if (m_pLine) delete m_pLine;
}

void ChatBox::Update(float gt, float dt)
{

}

void ChatBox::Render()
{
	//Update Colour
	m_cPanel.a = m_pScreen->GetTransitionAlpha();
	m_cLine.a = m_pScreen->GetTransitionAlpha();
	m_cHighlightLine.a = m_pScreen->GetTransitionAlpha();
	m_cText.a = m_pScreen->GetTransitionAlpha();

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

	Vector2 tmpPos = Vector2(rect.x1 + 1,rect.y1);

	//Get first & last line (Starts drawing with the right line)
	int fLine = 0;
	int	lLine = m_vText.size();
	if ((int)m_vText.size() > m_iChatBoxLines)
	{
		fLine = (m_iCurrentLine - m_iChatBoxLines) + m_vText.size();
		lLine = fLine + m_iChatBoxLines;
	}
	
	//Draw the console Text
	for (int i = fLine; i < lLine; i++)
	{
		m_pScreen->GetScreenManager()->GetFont()->SetColor(m_cText.GetHWColor());
		m_pScreen->GetScreenManager()->GetFont()->Render(tmpPos.x, tmpPos.y, HGETEXT_LEFT, m_vText[i].c_str());
		tmpPos += Vector2(0,m_pScreen->GetScreenManager()->GetFont()->GetScaledHeight());
	}
}

void ChatBox::AddLine(char *str)
{
	m_vText.push_back(string(str));
}