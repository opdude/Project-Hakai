#include "Label.h"
#include <hgecolor.h>
#include "GameScreen.h"

Label::Label(GameScreen* screen, int _id, float x, float y, float w, float h, hgeFont *fnt)
{
	m_pScreen = screen;
	id=_id;
	bStatic=true;
	bVisible=true;
	bEnabled=true;
	rect.Set(x, y, x+w, y+h);

	font=fnt;
	tx=x;
	ty=y+(h-fnt->GetHeight())/2.0f;

	text[0]=0;
}

void Label::SetMode(int _align)
{
	align=_align;
	if(align==HGETEXT_RIGHT) tx=rect.x2;
	else if(align==HGETEXT_CENTER) tx=(rect.x1+rect.x2)/2.0f;
	else tx=rect.x1;
}

void Label::SetText(const char *_text)
{
	strcpy(text, _text);
}

void Label::printf(const char *format, ...)
{
	vsprintf(text, format, (char *)&format+sizeof(format));
}

void Label::Render()
{
	//Update Colour
	hgeColor c = hgeColor(color);
	c.a = m_pScreen->GetTransitionAlpha();
	color = c.GetHWColor();

	font->SetColor(color);
	font->Render(tx,ty,align,text);
}

