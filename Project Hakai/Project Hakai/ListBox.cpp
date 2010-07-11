#include "ListBox.h"
#include "MenuScreen.h"
#include "GameManager.h"

ListBox::ListBox(int _id, MenuScreen* screen, hgeFont* font, float x, float y, float w, int visibleRows)
{
	id = _id;
	bStatic = false;
	bVisible = true;
	bEnabled = true;
	m_pScreen = screen;
	m_pFont = font;
	m_fX = x;
	m_fY = y;
	m_iVisibleRows = visibleRows;
	m_cLine = hgeColor(ARGB(255,20,61,77));
	m_cHighlightLine = hgeColor(ARGB(255,73,116,133));
	m_cText = hgeColor(ARGB(255,255,255,255));
	m_cHighlightText = hgeColor(ARGB(255,255,0,0));
	m_cListBox = hgeColor(ARGB(100,24,71,90));
	m_cSlider = hgeColor(ARGB(255,255,255,255));

	HTEXTURE tex;

	//Create Slider
	tex = LoadTex("Content/Materials/Slider.png");
	m_fSliderWidth = ScrXToVirt(GetTextureSize(tex).x);
	m_fSliderHeight = ScrYToVirt(GetTextureSize(tex).y);
	m_pSlider=new hgeSprite(tex, 0, 0, GetTextureSize(tex).x, GetTextureSize(tex).y);
	bPressed = false;

	//Create Rect
	w += m_fSliderWidth;
	float h = font->GetScaledHeight() * visibleRows;
	rect.Set(x,y,x+w,y+h);

	//Setup ListBox
	m_pItems = 0;
	m_iItems = 0;
	m_iSelectedItem = 0;
	m_iTopItem = 0;
	m_fMouseX=0;
	m_fMouseY=0;

	tex = GameManager::Instance()->GetResourceManager()->GetTexture("Blank");
	m_pListBox = new hgeSprite(tex, 8,8,w-m_fSliderWidth,h);
	m_pListBox->SetColor(m_cListBox.GetHWColor());
	m_pListSide = new hgeSprite(tex, 0,0,1,1);
}

ListBox::~ListBox()
{
	Clear();
	delete m_pSlider;
	delete m_pListBox;
	delete m_pListSide;
}


int ListBox::AddItem(char *item)
{
	hgeGUIListboxItem *pItem=m_pItems, *pPrev=0, *pNew;

	pNew = new hgeGUIListboxItem;
	memcpy(pNew->text, item, min(sizeof(pNew->text), strlen(item)+1));
	pNew->text[sizeof(pNew->text)-1]='\0';
	pNew->next=0;

	while(pItem) { pPrev=pItem;	pItem=pItem->next; }

	if(pPrev) pPrev->next=pNew;
	else m_pItems=pNew;
	m_iItems++;

	return m_iItems-1;
}

void ListBox::DeleteItem(int n)
{
	int i;
	hgeGUIListboxItem *pItem=m_pItems, *pPrev=0;

	if(n<0 || n>=GetNumItems()) return;

	for(i=0;i<n;i++) { pPrev=pItem;	pItem=pItem->next; }

	if(pPrev) pPrev->next=pItem->next;
	else m_pItems=pItem->next;

	delete pItem;
	m_iItems--;
}

char *ListBox::GetItemText(int n)
{
	int i;
	hgeGUIListboxItem *pItem=m_pItems;

	if(n<0 || n>=GetNumItems()) return 0;

	for(i=0;i<n;i++) pItem=pItem->next;

	return pItem->text;
}

void ListBox::Clear()
{
	hgeGUIListboxItem *pItem=m_pItems, *pNext;

	while(pItem)
	{
		pNext=pItem->next;
		delete pItem;
		pItem=pNext;
	}

	m_pItems=0;
	m_iItems=0;
}

void ListBox::Render()
{
	//Update Colour
	m_cText.a = m_pScreen->GetTransitionAlpha();
	m_cLine.a = m_pScreen->GetTransitionAlpha();
	m_cHighlightLine.a = m_pScreen->GetTransitionAlpha();
	m_cHighlightText.a = m_pScreen->GetTransitionAlpha();
	m_cListBox.a = m_pScreen->GetTransitionAlpha();
	m_cSlider.a = m_pScreen->GetTransitionAlpha();

	//Render Boxes
	m_pListBox->SetColor(m_cListBox.GetHWColor());
	m_pListBox->Render(rect.x1,rect.y1);

	if (m_iVisibleRows <= m_iItems)
		m_pListBox->RenderStretch(rect.x2-m_fSliderWidth+ScrXToVirt(2),rect.y1,rect.x2,rect.y2);

	//Render Lines
	m_pListSide->SetColor(m_cLine.GetHWColor());
	m_pListSide->RenderStretch(rect.x1,rect.y1,rect.x2-m_fSliderWidth,rect.y1+ScrYToVirt(1));
	m_pListSide->RenderStretch(rect.x1,rect.y1,rect.x1+ScrXToVirt(1),rect.y2);

	if (m_iVisibleRows <= m_iItems)
	{
		m_pListSide->RenderStretch(rect.x2-m_fSliderWidth+ScrXToVirt(1),rect.y1,rect.x2,rect.y1+ScrXToVirt(1));
		m_pListSide->RenderStretch(rect.x2-m_fSliderWidth+ScrXToVirt(2),rect.y1,rect.x2-m_fSliderWidth+ScrXToVirt(1),rect.y2);
	}

	m_pListSide->SetColor(m_cHighlightLine.GetHWColor());
	m_pListSide->RenderStretch(rect.x2-m_fSliderWidth-ScrXToVirt(1),rect.y1,rect.x2-m_fSliderWidth,rect.y2);
	m_pListSide->RenderStretch(rect.x1,rect.y2,rect.x2-m_fSliderWidth,rect.y2+ScrYToVirt(1));

	if (m_iVisibleRows <= m_iItems)
	{
		m_pListSide->RenderStretch(rect.x2-ScrXToVirt(1),rect.y1,rect.x2,rect.y2);
		m_pListSide->RenderStretch(rect.x2-m_fSliderWidth+ScrXToVirt(2),rect.y2,rect.x2,rect.y2+ScrYToVirt(1));
	}


	int i;
	hgeGUIListboxItem *pItem=m_pItems;

	for(i=0;i<m_iTopItem;i++) pItem=pItem->next;
	for(i=0;i<GetNumRows();i++)
	{
		if(i>=m_iItems) return;

		if(m_iTopItem+i == m_iSelectedItem)
		{
			m_pFont->SetColor(m_cHighlightText.GetHWColor());
		}
		else
			m_pFont->SetColor(m_cText.GetHWColor());

		m_pFont->Render(rect.x1+ScrXToVirt(3), rect.y1+i*m_pFont->GetScaledHeight(), HGETEXT_LEFT, pItem->text);
		pItem=pItem->next;
	}


	//Render Slider
	float xx, yy;
	float x1,y1,x2,y2;

	xx=rect.x1+(rect.x2-rect.x1)* m_iTopItem/(m_iItems-m_iVisibleRows);
	yy=rect.y1+((rect.y2 - m_fSliderHeight)-rect.y1)* m_iTopItem/(m_iItems-m_iVisibleRows);

	x1=rect.x2-m_fSliderWidth+ScrXToVirt(2); 
	y1=yy+ScrYToVirt(1);
	x2=rect.x2; 
	y2=yy+m_fSliderHeight;

	m_pSlider->SetColor(m_cSlider.GetHWColor());
	m_pSlider->RenderStretch(x1, y1, x2, y2);
}

bool ListBox::MouseMove(float x, float y)
{
	m_fMouseX=x; m_fMouseY=y; 

	if(bPressed)
	{
		if(y>rect.y2-rect.y1) y=rect.y2-rect.y1;
		if(y<0) y=0;
		m_iTopItem=(int)((m_iItems-m_iVisibleRows)*y/(rect.y2-rect.y1-m_fSliderWidth));
		if (m_iTopItem>(m_iItems-m_iVisibleRows))
			m_iTopItem = m_iItems-m_iVisibleRows;
		return true;
	}

	return false;
}

bool ListBox::MouseLButton(bool bDown)
{
	int nItem;

	if(bDown)
	{
		if (m_fMouseX> (rect.x2 - rect.x1) - m_fSliderWidth*2)
		{
			bPressed = true;
		}
		else
		{
			nItem=m_iTopItem+int(m_fMouseY)/int(m_pFont->GetScaledHeight());
			if(nItem<m_iItems)
			{
				m_iSelectedItem=nItem;
				return true;
			}
		}
	}
	else
		bPressed = false;

	return false;
}


bool ListBox::MouseWheel(int nNotches)
{
	m_iTopItem-=nNotches;
	if(m_iTopItem<0) m_iTopItem=0;
	if(m_iTopItem>GetNumItems()-GetNumRows()) m_iTopItem=GetNumItems()-GetNumRows();

	return true;
}

bool ListBox::KeyClick(int key, int chr)
{
	switch(key)
	{
	case HGEK_DOWN:
		if(m_iSelectedItem < m_iItems-1)
		{
			m_iSelectedItem++;
			if(m_iSelectedItem > m_iTopItem+GetNumRows()-1) m_iTopItem=m_iSelectedItem-GetNumRows()+1;
			return true;
		}
		break;

	case HGEK_UP:
		if(m_iSelectedItem > 0)
		{
			m_iSelectedItem--;
			if(m_iSelectedItem < m_iTopItem) m_iTopItem=m_iSelectedItem;
			return true;
		}
		break;
	}
	return false;
}


