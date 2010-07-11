#ifndef _POPUPINPUTSCREEN_H
#define  _POPUPINPUTSCREEN_H

#include <vector>
#include <boost/signal.hpp>
#include <hgegui.h>
#include "GameScreen.h"

class KeyboardInputBox;

class PopupInputScreen : public CursorScreen
{
public:
	PopupInputScreen(char* text);
	~PopupInputScreen();

	virtual void LoadContent();
	virtual void UnloadContent();

	virtual void Update(GameTime* gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen);
	virtual void HandleInput(InputState* inputState, GameTime* gameTime);
	virtual void Draw(HGE* hge, GameTime* gameTime);
	void AddRejectedKey(char* key);

	boost::signal<void (GameScreen*, const char*)> InputAccepted;
	boost::signal<void (GameScreen*)> InputRejected;
private:
	static void EnterPressed(GameScreen* screen, hgeGUIObject* object);
	static void EscPressed(GameScreen* screen, hgeGUIObject* object);

	hgeGUI* m_pGUI;
	char*	m_sText;
	vector<char*> m_vRejectedKeys;
};

#endif