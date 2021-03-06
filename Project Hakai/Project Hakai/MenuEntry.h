//
//	MenuEntry.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _MENUENTRY_H
#define _MENUENTRY_H

#include <boost/signal.hpp>
#include <hgecolor.h>
#include <hgegui.h>
#include "Vector2.h"

#include "MenuScreen.h"

class MenuScreen;

class MenuEntry : public hgeGUIObject
{
public:
	MenuEntry(MenuScreen* screen, char* text);
	~MenuEntry() {};

	virtual void LoadContent();
	virtual void UnloadContent();
	virtual void Render();
	virtual void Update(float gt, float dt);

	virtual void Enter();
	virtual void Leave();
	virtual bool IsDone();
	virtual void Focus(bool bFocused);
	virtual void MouseOver(bool bOver);

	virtual bool MouseLButton(bool bDown);
	virtual bool KeyClick(int key, int chr);

	boost::signal<void (GameScreen*)> Clicked;

	//Accessors
	void SetPosition(Vector2 val)
	{
		rect.x2 += val.x - rect.x1;
		rect.y2 += val.y - rect.y1;
		rect.x1 = val.x;
		rect.y1 = val.y;
	}
	Vector2 GetPosition()
	{
		return Vector2(rect.x1, rect.y1);
	}
	void SetDesiredPosition(Vector2 val)
	{
		m_vDesiredPos = val;
	}
	void SetHighlighted(bool val)
	{
		MouseOver(val);
	}
private:
	MenuScreen* m_pGameScreen;
	char *m_sText;
	HEFFECT m_sClickSound;
	Vector2 m_vDesiredPos;
	float m_fSelectionFade;
	float m_fPulsate;
	bool m_bIsHighlighted;
	hgeColor m_cColour, m_cNormalColour, m_cHighlightColour;
};

#endif