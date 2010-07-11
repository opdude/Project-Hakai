#ifndef _MOUSESTATE_H
#define _MOUSESTATE_H

class GameManager;

class MouseState
{
public:
	MouseState();
	~MouseState();
	int GetMouseWheelValue()
	{
		return m_iMouseWheel;
	}
	bool GetMouseLeftClick()
	{
		return m_bLeftClick;
	}
	bool GetMouseRightClick()
	{
		return m_bRightClick;
	}
	bool GetMouseMiddleClick()
	{
		return m_bMiddleClick;
	}
protected:
	int m_iMouseWheel;
	bool m_bLeftClick;
	bool m_bMiddleClick;
	bool m_bRightClick;
};

#endif