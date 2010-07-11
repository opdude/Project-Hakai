//
//	ListBox.h
//	Project Hakai
//
// Created by Daniel Hobley
//
// Purpose: 
//
//
#ifndef _LISTBOX_H
#define _LISTBOX_H

#include <hgecolor.h>
#include <hgeguictrls.h>
#include <hgefont.h>
#include "HelperFunctions.h"

class MenuScreen;

class ListBox : public hgeGUIObject
{
public:
	ListBox(int _id, MenuScreen* screen, hgeFont* font, float x, float y, float w, int visibleRows);
	~ListBox();

	int				AddItem(char *item);
	void			DeleteItem(int n);
	int				GetSelectedItem() { return m_iSelectedItem; }
	void			SetSelectedItem(int n) 
					{ 
						if(n>=0 && n<GetNumItems()) 
							m_iSelectedItem=n; 

						if (m_iSelectedItem > m_iVisibleRows)
							m_iTopItem = m_iSelectedItem - (m_iVisibleRows - 1);

					}
	int				GetTopItem() { return m_iTopItem; }
	void			SetTopItem(int n) { if(n>=0 && n<=GetNumItems()-GetNumRows()) m_iTopItem=n; }

	char			*GetItemText(int n);
	int				GetNumItems() { return m_iItems; }
	int				GetNumRows() { return m_iVisibleRows; }
	void			Clear();

	virtual void	Render();
	virtual bool	MouseMove(float x, float y);
	virtual bool	MouseLButton(bool bDown);
	virtual bool	MouseWheel(int nNotches);
	virtual bool	KeyClick(int key, int chr);

private:
	MenuScreen* m_pScreen;
	float m_fX, m_fY;

	//List
	hgeSprite* m_pListSide;
	hgeSprite* m_pListBox;
	hgeFont* m_pFont;
	int	m_iVisibleRows, m_iItems, m_iSelectedItem, m_iTopItem;
	hgeColor m_cText, m_cHighlightText, m_cLine, m_cHighlightLine, m_cListBox, m_cSlider;

	//Slider
	hgeSprite* m_pSlider;
	hgeGUIListboxItem* m_pItems;
	float m_fMouseX, m_fMouseY;
	float m_fSliderWidth, m_fSliderHeight;
	bool bPressed;
};

#endif