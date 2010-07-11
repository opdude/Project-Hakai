//
//	Slider.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _SLIDER_H
#define _SLIDER_H

#include <hgeguictrls.h>
#include <hgesprite.h>
#include <hgecolor.h>

class GameScreen;

class Slider : public hgeGUIObject
{
public:
	Slider(GameScreen* screen, int id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty, float sw, float sh, bool vertical=false);
	~Slider();

	void			SetMode(float _fMin, float _fMax, int _mode) { m_fMin=_fMin; m_fMax=_fMax; m_iMode=_mode; }
	void			SetValue(float _fVal);
	float			GetValue() const { return m_fVal; }

	virtual void	Render();
	virtual bool	MouseMove(float x, float y);
	virtual bool	MouseLButton(bool bDown);

private:
	GameScreen*		m_pScreen;
	bool			m_bPressed;
	bool			m_bVertical;
	int				m_iMode;
	float			m_fMin, m_fMax, m_fVal;
	float			m_SliderW, m_SliderH;
	hgeSprite*		m_pSlider;
	hgeSprite*		m_pLine;
	hgeSprite*		m_pPanel;
	hgeColor		m_cPanel, m_cLine, m_cHighlightLine, m_cButton;
};

#endif