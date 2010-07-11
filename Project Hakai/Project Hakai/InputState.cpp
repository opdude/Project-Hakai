#include "InputState.h"

InputState::InputState()
{
	m_pCurrentKeyboardState = new KeyboardState();
	m_pLastKeyboardState = new KeyboardState();
	m_pCurrentMouseState = new MouseState();
	m_pLastMouseState = new MouseState();
}

InputState::~InputState()
{
	if (m_pCurrentKeyboardState) delete m_pCurrentKeyboardState;
	if (m_pLastKeyboardState) delete m_pLastKeyboardState;
	if (m_pCurrentMouseState) delete m_pCurrentMouseState;
	if (m_pLastMouseState) delete m_pLastMouseState;
}

void InputState::Update()
{
	//Clear old keyboard & mouse state
	delete m_pLastKeyboardState;
	delete m_pLastMouseState;

	m_pLastKeyboardState = m_pCurrentKeyboardState;
	m_pCurrentKeyboardState = new KeyboardState();
	m_pLastMouseState = m_pCurrentMouseState;
	m_pCurrentMouseState = new MouseState();
}