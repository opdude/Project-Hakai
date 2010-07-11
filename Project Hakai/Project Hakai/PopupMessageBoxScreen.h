//
//	PopupMessageBoxScreen.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _POPUPMESSAGEBOXSCREEN_H
#define _POPUPMESSAGEBOXSCREEN_H

#include <boost/signal.hpp>
#include <hgegui.h>
#include "GameScreen.h"

class PopupMessageBoxScreen : public CursorScreen
{
public:
	PopupMessageBoxScreen(char* title, char* message, bool okayButtonOnly);
	~PopupMessageBoxScreen();
	virtual void LoadContent();
	virtual void UnloadContent();
	virtual void HandleInput(InputState* inputState, GameTime* gameTime);
	virtual void Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
	virtual void Draw(HGE* hge, GameTime* gameTime);

	boost::signal<void (GameScreen*)> Accepted;
	boost::signal<void (GameScreen*)> Rejected;

private:
	static void ButtonAccepted(GameScreen* screen, hgeGUIObject* object);
	static void ButtonRejected(GameScreen* screen, hgeGUIObject* object);

	hgeGUI* m_pGUI;
	char* m_sTitle;
	char* m_sText;
	bool m_bOkayButtonOnly;
};

#endif