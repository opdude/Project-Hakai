#ifndef _INPUTSTATE_H
#define _INPUTSTATE_H

#include "KeyboardState.h"
#include "MouseState.h"

class InputState
{
public:
	InputState();
	~InputState();

	void Update();
	KeyboardState* GetKeyboardState() { return m_pCurrentKeyboardState; }
	KeyboardState* GetLastKeyboardState() { return m_pLastKeyboardState; }
	MouseState* GetMouseState() { return m_pCurrentMouseState; }
	MouseState* GetLastMouseState() { return m_pLastMouseState; }
private:
	KeyboardState* m_pCurrentKeyboardState;
	KeyboardState* m_pLastKeyboardState;
	MouseState* m_pCurrentMouseState;
	MouseState* m_pLastMouseState;
};

#endif