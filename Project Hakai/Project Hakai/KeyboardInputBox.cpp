//
//	KeyboardInputBox.cpp
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#include "KeyboardInputBox.h"
#include "GameScreen.h"
#include "GameTime.h"
#include "InputState.h"
#include "ScreenManager.h"

KeyboardInputBox::KeyboardInputBox(int _id, GameScreen* screen, int maxAllowedChars, float x, float y, float w, 
								   bool typingEnabled, vector<char*> rejectedKeys)
{
	id = _id;
	m_pScreen = screen;
	m_sText = "";
	m_iCursorPos = 0;
	m_iMaxAllowedChars = maxAllowedChars;
	m_bTypingEnabled = typingEnabled;
	rect.Set(x,y,x+w,y+m_pScreen->GetScreenManager()->GetFont()->GetScaledHeight());
	m_fMaxCharsWidth = w;
	m_fKeyStrokeSpeed = 0.250f;
	m_cText = ARGB(255,0,0,0);
	m_cInputBox = ARGB(255,255,255,255);
	m_vDisabledKeys = rejectedKeys;
	bStatic = false;
	
	HTEXTURE tex = GameManager::Instance()->GetResourceManager()->GetTexture("Blank");
	m_pTextBox = new hgeSprite(tex,0,0,w,rect.y2-rect.y1);
}

KeyboardInputBox::~KeyboardInputBox()
{
	delete m_pTextBox;
}

void KeyboardInputBox::Update(float gt, float dt)
{
	if ((int)(gt*1000) % 20 == 0)
		m_bShowCursor = !m_bShowCursor;
}

void KeyboardInputBox::Focus(bool bFocused)
{
	m_bTypingEnabled = bFocused;
}

bool KeyboardInputBox::MouseLButton(bool bDown)
{
	if (bDown)
		gui->SetFocus(id);

	return false;
}

bool KeyboardInputBox::IsDone()
{
	return true;
}

void KeyboardInputBox::Render()
{
	//Update Colour
	m_cText.a = m_pScreen->GetTransitionAlpha();
	m_cInputBox.a = m_pScreen->GetTransitionAlpha();

	//Render Box
	m_pTextBox->SetColor(m_cInputBox.GetHWColor());
	m_pTextBox->Render(rect.x1, rect.y1);

	//Draw Text
	string tmpText = m_sText;

	//Calculate Cursor Pos
	float cursorX = m_pScreen->GetScreenManager()->GetFont()->GetStringWidth(tmpText.substr(0,m_iCursorPos).c_str());
	if (cursorX > m_fMaxCharsWidth)
	{
		int i = 0;
		while (cursorX > m_fMaxCharsWidth)
		{
			i++;
			cursorX = m_pScreen->GetScreenManager()->GetFont()->GetStringWidth(tmpText.substr(i).c_str());
		}
		tmpText = m_sText.substr(i);
	}

	m_pScreen->GetScreenManager()->GetFont()->SetColor(m_cText.GetHWColor());
	m_pScreen->GetScreenManager()->GetFont()->Render(rect.x1,rect.y1,HGETEXT_LEFT, tmpText.c_str());

	//Draw Cursor
	if (m_bShowCursor && m_bTypingEnabled)
	{
		string cursorTmp = tmpText.substr(0,m_iCursorPos);
		float x = m_pScreen->GetScreenManager()->GetFont()->GetStringWidth(cursorTmp.c_str());
		m_pScreen->GetScreenManager()->GetFont()->Render(rect.x1 + x,rect.y1,HGETEXT_LEFT, "|");
	}
}

void KeyboardInputBox::HandleInput(InputState *inputState, GameTime *gameTime)
{
	if (m_bTypingEnabled)
	{
		if (m_fLastKeyboardPress == 0)
			m_fLastKeyboardPress = gameTime->GetTotalTime();

		vector<int> keys = inputState->GetKeyboardState()->GetPressedKeys();
		float lastPress = gameTime->GetTotalTime() - m_fLastKeyboardPress;

		for each(int key in keys)
		{
			if (inputState->GetLastKeyboardState()->IsKeyUp(key) || lastPress > m_fKeyStrokeSpeed)
			{
				HandleKeyPress(inputState, key);
				m_fLastKeyboardPress = gameTime->GetTotalTime();
			}
		}
	}
}

void KeyboardInputBox::HandleKeyPress(InputState *inputState, int key)
{
	int normKey = key;
	key = KeyboardState::KeyToScanCode(key);
	char* charCheck = KeyboardState::AsciiConvert(normKey);

	//Stop if key is disabled
	for each(char* c in m_vDisabledKeys)
	{
		if (!strcmp(charCheck,c))
			return;
	}

	if (key == KEY_ESC)
	{
		EscPressed(m_pScreen, this);
	}
	else if (key == KEY_RETURN)
	{
		EnterPressed(m_pScreen, this);
	}
	else if (key == KEY_BACKSPACE)
	{
		if (m_iCursorPos > 0)
		{
			m_sText = m_sText.erase(m_iCursorPos - 1, 1);
			m_iCursorPos--;
		}
	}
	else if (key == KEY_DEL)
	{
		//Delete a char
		if ((int)m_sText.length() > m_iCursorPos)
			m_sText = m_sText.erase(m_iCursorPos, 1);
	}
	else if (key == KEY_LEFT && inputState->GetKeyboardState()->IsKeyDown(KEY_LSHIFT))
	{
		m_iCursorPos = 0;
	}
	else if (key == KEY_LEFT)
	{
		if (--m_iCursorPos < 0)
			m_iCursorPos = 0;
	}
	else if (key == KEY_RIGHT && inputState->GetKeyboardState()->IsKeyDown(KEY_LSHIFT))
	{
		m_iCursorPos = m_sText.length();
	}
	else if (key == KEY_RIGHT)
	{
		if (++m_iCursorPos > (int)m_sText.length())
			m_iCursorPos = m_sText.length();
	}
	else if (key == KEY_HOME)
	{
		m_iCursorPos = 0;
	}
	else if (key == KEY_END)
	{
		m_iCursorPos = m_sText.size();
	}
	else
	{
		if (m_sText.size() >= m_iMaxAllowedChars)
			return;

		for (int i = 0; i < ALLOWED_INPUT_SIZE; i++)
		{
			if (!strcmp(charCheck,m_aAInput[i]))
			{
				if (m_iCursorPos == 0)
					m_sText += m_aAInput[i];
				else if (m_iCursorPos > 0 && m_iCursorPos <= (int)m_sText.size())
					m_sText = m_sText.insert(m_iCursorPos, m_aAInput[i]);

				m_iCursorPos++;
				break;
			}
		}
	}

	free(charCheck);
}

char* KeyboardInputBox::m_aAInput[] = 
{
	"a","b","c","d","e","f","g","h","i","j",
	"k","l","m","n","o","p","q","r","s","t",
	"u","v","w","x","y","z","A","B","C","D",
	"E","F","G","H","I","J","K","L","M","N",
	"O","P","Q","R","S","T","U","V","W","X",
	"Y","Z","1","2","3","4","5","6","7","8",
	"9","0","-","=","!","/","£","$","%","^",
	"&","*","(",")","-","{","}","[","]",":",
	";","@","'","~","#","<",",",">",".","?",
	"/","\\","_","\""," "
};