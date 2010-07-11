#ifndef _KEYBOARDINPUTBOX_H
#define _KEYBOARDINPUTBOX_H

#include <boost/signal.hpp>
#include <string>
#include <vector>
#include <hgeguictrls.h>
#include <hgecolor.h>
#include <hgesprite.h>

class GameScreen;
class GameTime;
class InputState;

using namespace std;

class KeyboardInputBox : public hgeGUIObject
{
public:
	KeyboardInputBox(int id, GameScreen* screen, int maxAllowedChars, float x, float y, float w, bool typingEnabled, vector<char*> rejectedKeys);
	~KeyboardInputBox();

	virtual void Update(float gt, float dt);
	virtual void Focus(bool bFocused);
	virtual bool MouseLButton(bool bDown);
	virtual void Render();
	virtual bool IsDone();

	void HandleInput(InputState* inputState, GameTime* gameTime);

	char* GetText()
	{
		return (char*)m_sText.c_str();
	}

	void SetText(char* str)
	{
		m_sText = str;
		m_iCursorPos = m_sText.size();
	}

	boost::signal<void (GameScreen*, hgeGUIObject*)> EscPressed;
	boost::signal<void (GameScreen*, hgeGUIObject*)> EnterPressed;
	string m_sText;

private:
	void HandleKeyPress(InputState* inputState, int key);

	GameScreen* m_pScreen;
	hgeSprite* m_pTextBox;
	vector<char*> m_vDisabledKeys;
	unsigned int m_iMaxAllowedChars;
	float m_fMaxCharsWidth;
	int m_iCursorPos;
	bool m_bShowCursor;
	bool m_bTypingEnabled;
	float m_fLastKeyboardPress;
	float m_fKeyStrokeSpeed;
	hgeColor m_cText, m_cInputBox;

	static const int ALLOWED_INPUT_SIZE = 95;
	static char* m_aAInput[];
	
};

#endif