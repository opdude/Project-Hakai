#include "Slider.h"
#include "GameScreen.h"
#include "HelperFunctions.h"
#include "GameManager.h"

Slider::Slider(GameScreen* screen, int _id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty, float sw, float sh, bool vertical)
{
	m_pScreen = screen;
	id=_id;
	bStatic=false;
	bVisible=true;
	bEnabled=true;
	m_bPressed=false;
	m_bVertical=vertical;
	rect.Set(x, y, x+w, y+h);
	m_cPanel = hgeColor(ARGB(100,24,71,90));
	m_cLine = hgeColor(ARGB(255,20,61,77));
	m_cHighlightLine = hgeColor(ARGB(255,73,116,133));
	m_cButton = hgeColor(ARGB(255,255,255,255));

	m_iMode=HGESLIDER_BAR;
	m_fMin=0; m_fMax=100; m_fVal=50;
	m_SliderW=sw; m_SliderH=sh;

	m_pSlider=new hgeSprite(tex, tx, ty, VirtXToScr(sw), VirtYToScr(sh));

	tex = GameManager::Instance()->GetResourceManager()->GetTexture("Blank");
	m_pPanel = new hgeSprite(tex, 8,8,w,h);
	m_pPanel->SetColor(m_cPanel.GetHWColor());
	m_pLine = new hgeSprite(tex, 0,0,1,1);
}

Slider::~Slider()
{
	if(m_pSlider) delete m_pSlider;
	if (m_pLine) delete m_pLine;
	if (m_pPanel) delete m_pPanel;
}

void Slider::SetValue(float _fVal)
{
	if(_fVal<m_fMin) m_fVal=m_fMin;
	else if(_fVal>m_fMax) m_fVal=m_fMax;
	else m_fVal=_fVal;
}

void Slider::Render()
{
	//Update Colour
	m_cPanel.a = m_pScreen->GetTransitionAlpha();
	m_cLine.a = m_pScreen->GetTransitionAlpha();
	m_cHighlightLine.a = m_pScreen->GetTransitionAlpha();
	m_cButton.a = m_pScreen->GetTransitionAlpha();

	//Render Boxes
	m_pPanel->SetColor(m_cPanel.GetHWColor());
	m_pPanel->Render(rect.x1,rect.y1);

	//Render Lines
	m_pLine->SetColor(m_cLine.GetHWColor());
	m_pLine->RenderStretch(rect.x1,rect.y1,rect.x2,rect.y1+ScrYToVirt(1));
	m_pLine->RenderStretch(rect.x1,rect.y1,rect.x1+ScrXToVirt(1),rect.y2);
	m_pLine->SetColor(m_cHighlightLine.GetHWColor());
	m_pLine->RenderStretch(rect.x2-ScrXToVirt(1),rect.y1,rect.x2,rect.y2);
	m_pLine->RenderStretch(rect.x1,rect.y2,rect.x2,rect.y2+ScrYToVirt(1));

	float xx, yy;
	float x1,y1,x2,y2;

	xx=rect.x1+((rect.x2 - m_SliderH)-rect.x1)*(m_fVal-m_fMin)/(m_fMax-m_fMin);
	yy=rect.y1+(rect.y2-rect.y1)*(m_fVal-m_fMin)/(m_fMax-m_fMin);

	if(m_bVertical)
		switch(m_iMode)
	{
		case HGESLIDER_BAR: x1=rect.x1; y1=rect.y1; x2=rect.x2; y2=yy; break;
		case HGESLIDER_BARRELATIVE: x1=rect.x1; y1=(rect.y1+rect.y2)/2; x2=rect.x2; y2=yy; break;
		case HGESLIDER_SLIDER: x1=(rect.x1+rect.x2-m_SliderW)/2; y1=yy-m_SliderH/2; x2=(rect.x1+rect.x2+m_SliderW)/2; y2=yy+m_SliderH/2; break;
	}
	else
		switch(m_iMode)
	{
		case HGESLIDER_BAR: x1=rect.x1; y1=rect.y1; x2=xx; y2=rect.y2; break;
		case HGESLIDER_BARRELATIVE: x1=(rect.x1+rect.x2)/2; y1=rect.y1; x2=xx; y2=rect.y2; break;
		case HGESLIDER_SLIDER: x1=xx-m_SliderW/2; y1=(rect.y1+rect.y2-m_SliderH)/2; x2=xx+m_SliderW/2; y2=(rect.y1+rect.y2+m_SliderH)/2; break;
	}

	m_pSlider->SetColor(m_cButton.GetHWColor());
	if (!m_bVertical)
	{
		float xScale = m_SliderH/m_pSlider->GetHeight();
		float yScale = (rect.y2 - rect.y1)/m_pSlider->GetWidth();
		x1=xx+m_SliderH;
		y1=rect.y1;
		m_pSlider->RenderEx(x1,y1,DegreeToRadian(90), yScale,xScale);
	}
	else
		m_pSlider->RenderStretch(x1, y1, x2, y2);
}

bool Slider::MouseLButton(bool bDown)
{
	m_bPressed=bDown;
	return false;
}

bool Slider::MouseMove(float x, float y)
{
	if(m_bPressed)
	{
		if(m_bVertical)
		{
			if(y>rect.y2-rect.y1) y=rect.y2-rect.y1;
			if(y<0) y=0;
			m_fVal=m_fMin+(m_fMax-m_fMin)*y/(rect.y2-rect.y1);
		}
		else
		{
			if(x>rect.x2-rect.x1) x=rect.x2-rect.x1;
			if(x<0) x=0;
			m_fVal=m_fMin+(m_fMax-m_fMin)*x/(rect.x2-rect.x1);
		}
		return true;
	}

	return false;
}