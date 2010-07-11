#ifndef _LABEL_H
#define _LABEL_H

#include <hgeguictrls.h>
#include <hgefont.h>

class GameScreen;

class Label : public hgeGUIObject
{
public:
	Label(GameScreen* screen, int id, float x, float y, float w, float h, hgeFont *fnt);
	~Label() {};

	void			SetMode(int _align);
	void			SetText(const char *_text);
	void			printf(const char *format, ...);

	virtual void	Render();

private:
	GameScreen*		m_pScreen;
	hgeFont*		font;
	float			tx, ty;
	int				align;
	char			text[256];
};

#endif