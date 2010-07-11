//
//	ChatBox.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _CHATBOX_H
#define _CHATBOX_H

#include <hgeguictrls.h>
#include <hgesprite.h>
#include <hgecolor.h>
#include <vector>
#include <string>

class GameScreen;
class Vector2;

using namespace std;

class ChatBox : public hgeGUIObject
{
public:
	ChatBox(int id, GameScreen* screen, float x, float y, float w, float h);
	~ChatBox();

	virtual void Update(float gt, float dt);
	virtual void Render();
	
	void AddLine(char* str);
private:
	GameScreen* m_pScreen;
	hgeSprite* m_pPanel;
	hgeSprite* m_pLine;
	vector<string> m_vText;
	hgeColor m_cText, m_cPanel, m_cLine, m_cHighlightLine;
	int m_iCurrentLine;
	int m_iChatBoxLines;
};

#endif