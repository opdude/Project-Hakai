#include "GUITable.h"
#include "GameScreen.h"
#include "GameTime.h"
#include "GameManager.h"
#include "HelperFunctions.h"
#include "ScreenManager.h"
#include "Vector2.h"

#define COL_W 3
#define PAD 10 

GUITable::GUITable(int _id, GameScreen* screen, vector<Column*> columns, float x, float y, float height)
{
	id = _id;
	m_pScreen = screen;
	m_vColumns = columns;
	m_fHeight = height;

	m_iRows = 0;
	m_iTopVisibleRow = 0;
	m_iSelectedRow = -1;
	m_iMaximumRows = (int)(height / m_pScreen->GetScreenManager()->GetTableFont()->GetScaledHeight());
	m_iMaximumRows--; //Remove Header Count

	HTEXTURE tex = GameManager::Instance()->GetResourceManager()->GetTexture("Blank");
	m_pTable = new hgeSprite(tex, 0,0,1,1);

	Vector2 size = GetSize();
	rect.Set(x, y, x + size.x, y + height);

	m_cBorder = hgeColor(ARGB(255,100,100,100));
	m_cTable = hgeColor(ARGB(100,24,71,90));
	m_cText = hgeColor(ARGB(255,255,255,255));
	m_cHighlightText = hgeColor(ARGB(255,0,255,255));
}
GUITable::~GUITable()
{
	for each(Column* c in m_vColumns)
	{
		if (c->ShouldDeletePointers())

		for each(void* r in *c->GetRows())
			delete r;

		delete c;
	}

	m_vColumns.clear();

	delete m_pTable;
}

void GUITable::AddRow(vector<void*> rowText, void* rowObject)
{
	//Needs to be the correct size
	if (rowText.size() != m_vColumns.size())
		return;

	m_vRowObjects.push_back(rowObject);

	for (unsigned int i=0; i < rowText.size(); i++)
	{
		m_vColumns[i]->AddRow(rowText[i]);
	}

	m_iRows++;
}
vector<void*> GUITable::GetRow(int row)
{
	vector<void*> output = vector<void*>();

	for (unsigned int i=0; i < m_vColumns.size(); i++)
	{
		output.push_back((*m_vColumns[i]->GetRows())[row]);
	}

	return output;
}
void GUITable::SetString(int column, int row, char* str)
{
	(*m_vColumns[column]->GetRows())[row] = str;
}
void* GUITable::GetRowObject(int row)
{
	if (row < (int)m_vRowObjects.size() && row >= 0)
		return m_vRowObjects[row];
	else
		return NULL;
}
Vector2 GUITable::GetSize()
{
	float width=0;
	for each(Column* c in m_vColumns)
	{
		width+=c->GetWidth() + ScrXToVirt(10);
	}
	return Vector2(width,m_fHeight);
}

void GUITable::SetPosition(Vector2 pos)
{
	rect.x1 += pos.x;
	rect.y1 += pos.y;
	rect.x2 += pos.x;
	rect.y2 += pos.y;
}

void GUITable::ClearTable()
{
	for each(Column* c in m_vColumns)
	{
		for each (void* obj in (*c->GetRows()))
		{
			if (c->ShouldDeletePointers() && obj)
			{
				delete obj;
				obj = 0;
			}
		}

		c->GetRows()->clear();
	}
}
void GUITable::MoveDownRow()
{
	m_iSelectedRow++;

	if (m_iSelectedRow >= m_iRows)
	{
		m_iSelectedRow = m_iRows - 1;
		return;
	}

	if (m_iSelectedRow >= m_iMaximumRows)
		m_iTopVisibleRow++;
}
void GUITable::MoveUpRow()
{
	m_iSelectedRow--;

	if (m_iSelectedRow < 0)
	{
		m_iSelectedRow = 0;
		return;
	}

	if (m_iSelectedRow < m_iTopVisibleRow)
		m_iTopVisibleRow--;
}
void GUITable::MoveToRow(int row)
{
	if (row >= 0 && row < m_iRows)
		m_iSelectedRow = m_iTopVisibleRow + row;
	else
		m_iSelectedRow = -1;
}

void GUITable::Update(float gt, float dt)
{

}

void GUITable::Render()
{
	//Update Colours
	m_cBorder.a = (m_pScreen->GetTransitionAlpha() < 0.4f) ? m_pScreen->GetTransitionAlpha() : 0.4f;
	m_cTable.a = (m_pScreen->GetTransitionAlpha() < 0.1f) ? m_pScreen->GetTransitionAlpha() : 0.1f;
	m_cText.a = m_pScreen->GetTransitionAlpha();
	m_cHighlightText.a = m_pScreen->GetTransitionAlpha();

	//Draw Lines
	m_pTable->RenderStretch(rect.x1, rect.y1 - ScrYToVirt(COL_W), rect.x2  + ScrXToVirt(COL_W), rect.y1);
	m_pTable->RenderStretch(rect.x1, rect.y2,rect.x2 + ScrXToVirt(COL_W), rect.y2 + ScrXToVirt(COL_W));

	float columnCurrWidth=0;
	for each (Column* c in m_vColumns)
	{
		//Draw Background
		hgeRect tmpRect = hgeRect(rect.x1 + columnCurrWidth, rect.y1, (rect.x1 + columnCurrWidth) + (c->GetWidth() + ScrXToVirt(10)), rect.y1 + m_fHeight);
		m_pTable->SetColor(m_cTable.GetHWColor());
		m_pTable->RenderStretch(tmpRect.x1+ScrXToVirt(COL_W),tmpRect.y1,tmpRect.x2,tmpRect.y2);
		m_pTable->SetColor(m_cBorder.GetHWColor());
		m_pTable->RenderStretch(tmpRect.x2, tmpRect.y1,tmpRect.x2+ScrXToVirt(COL_W),tmpRect.y2);

		//If First
		if (columnCurrWidth == 0)
		{
			m_pTable->RenderStretch(tmpRect.x1, tmpRect.y1, tmpRect.x1 + ScrXToVirt(COL_W),tmpRect.y2);
		}

		//Draw Header
		string drawnString = c->GetHeader();
		while (m_pScreen->GetScreenManager()->GetTableFont()->GetStringWidth(drawnString.c_str()) > c->GetWidth())
		{
			drawnString = drawnString.substr(0, drawnString.size()-1);
		}
		m_pScreen->GetScreenManager()->GetTableFont()->SetColor(m_cText.GetHWColor());
		m_pScreen->GetScreenManager()->GetTableFont()->Render(rect.x1 + columnCurrWidth + ScrXToVirt(COL_W), rect.y1, HGETEXT_LEFT, drawnString.c_str());


		//If the column has a draw override let it do it's business TODO: FIX TIME!
		if (!c->Draw.empty())
		{
			GameTime* gt = new GameTime(GameManager::Instance()->GetHGE()->Timer_GetDelta(), GameManager::Instance()->GetHGE()->Timer_GetTime());
			c->Draw(m_pScreen, c, columnCurrWidth, rect.x1, rect.y1, gt);
			delete gt;
			columnCurrWidth += c->GetWidth() + ScrXToVirt(10);
			continue;
		}

		int rowCount = 1;
		if (c->GetRows()->size() != 0)
		{
			for (int i=m_iTopVisibleRow; i < (int)c->GetRows()->size() && i < (m_iMaximumRows + m_iTopVisibleRow); i++)
			{
				if ((*c->GetRows())[i] == NULL)
					continue;

				drawnString = ((string*)(*c->GetRows())[i])->c_str();
				while (m_pScreen->GetScreenManager()->GetTableFont()->GetStringWidth(drawnString.c_str()) > c->GetWidth())
				{
					drawnString = drawnString.substr(0, drawnString.size()-1);
				}

				if (m_iSelectedRow - m_iTopVisibleRow == rowCount - 1)
					m_pScreen->GetScreenManager()->GetTableFont()->SetColor(m_cHighlightText.GetHWColor());
				else
					m_pScreen->GetScreenManager()->GetTableFont()->SetColor(m_cText.GetHWColor());

				//Draw String
				m_pScreen->GetScreenManager()->GetTableFont()->Render(rect.x1 + columnCurrWidth + ScrXToVirt(COL_W), rect.y1 + 
					(m_pScreen->GetScreenManager()->GetTableFont()->GetScaledHeight() * rowCount), HGETEXT_LEFT, drawnString.c_str());

				rowCount++;
			}
		}

		columnCurrWidth += c->GetWidth() + ScrXToVirt(PAD);
	}
}

bool GUITable::MouseMove(float x, float y)
{
	if (m_bMouseOver)
	{
		m_iSelectedRow = m_iTopVisibleRow + (int)(y / m_pScreen->GetScreenManager()->GetTableFont()->GetScaledHeight()) - 1;
	}

	return false;
}

void GUITable::MouseOver(bool bOver)
{
	m_bMouseOver = bOver;

}

bool GUITable::MouseWheel(int nNotches)
{
	m_iTopVisibleRow -= nNotches;

	if (m_iTopVisibleRow > (m_iRows - m_iMaximumRows))
		m_iTopVisibleRow = (m_iRows - m_iMaximumRows);
	else if (m_iTopVisibleRow < 0)
		m_iTopVisibleRow = 0;

	return false;
}

bool GUITable::MouseLButton(bool bDown)
{
	if (bDown)
	{
		if (m_iSelectedRow > -1 && m_iSelectedRow < m_iRows)
			EntryClicked(m_pScreen, m_iSelectedRow, m_vRowObjects[m_iSelectedRow]);
	}

	return false;
}