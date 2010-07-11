//
//	Panel.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _PANEL_H
#define _PANEL_H

#include <hgeguictrls.h>
#include <hgecolor.h>

class GameScreen;

class Panel : public hgeGUIObject
{
public:
	Panel(GameScreen* screen, float x, float y, float w, float h);
	~Panel();

	virtual void	Render();

private:
	GameScreen* m_pScreen;
	hgeSprite* m_pLine;
	hgeSprite* m_pPanel;
	hgeColor m_cPanel, m_cLine, m_cHighlightLine;
};

#endif