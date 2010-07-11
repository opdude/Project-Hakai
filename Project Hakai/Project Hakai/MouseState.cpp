#include "MouseState.h"
#include "GameManager.h"

MouseState::MouseState()
{
	m_iMouseWheel = GameManager::Instance()->GetHGE()->Input_GetMouseWheel();
	m_bLeftClick = GameManager::Instance()->GetHGE()->Input_GetKeyState(HGEK_LBUTTON);
	m_bMiddleClick = GameManager::Instance()->GetHGE()->Input_GetKeyState(HGEK_MBUTTON);
	m_bRightClick = GameManager::Instance()->GetHGE()->Input_GetKeyState(HGEK_RBUTTON);
}

MouseState::~MouseState()
{

}