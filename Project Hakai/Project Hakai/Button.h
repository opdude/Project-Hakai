//
//	Button.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _BUTTON_H
#define _BUTTON_H

#include <boost/signal.hpp>
#include <hgeguictrls.h>
#include <hgecolor.h>
#include <hgesprite.h>

class GameScreen;

class Button : public hgeGUIObject
{
public:
	Button(GameScreen* screen, int _id, char* text, float x, float y, float w=0, float h=0);
	virtual			~Button();

	void			SetMode(bool _bTrigger) { m_bTrigger=_bTrigger; }
	void			SetState(bool _bPressed) { m_bPressed=_bPressed; }
	bool			GetState() const { return m_bPressed; }

	virtual void	Render();
	virtual bool	MouseLButton(bool bDown);

	boost::signal<void (GameScreen*, hgeGUIObject*)> Clicked;

	//Accessors
	void SetCenteredText(bool val)
	{
		m_bCenteredText = val;
	}
	void SetText(char* val)
	{
		m_sText = val;
	}
	char* GetText()
	{
		return m_sText;
	}

private:
	GameScreen*		m_pScreen;
	char*			m_sText;
	bool			m_bTrigger;
	bool			m_bPressed;
	bool			m_bOldState;
	bool			m_bCenteredText;
	hgeSprite*		m_pLine;
	hgeSprite*		m_pButton;
	hgeColor		m_cButton, m_cLine, m_cHighlightLine, m_cText, m_cPushedText;
};

#endif