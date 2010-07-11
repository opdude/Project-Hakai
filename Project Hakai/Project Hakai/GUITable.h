#ifndef _TABLE_H
#define _TABLE_H

#include <boost/signal.hpp>
#include <vector>
#include <hgeguictrls.h>
#include <hgesprite.h>
#include <hgecolor.h>

class GameScreen;
class GameTime;
class GUITable;
class Vector2;

using namespace std;

class Column
{
public:
	Column(char* header, float width)
	{
		m_chHeader = header;
		m_fWidth = width;
		m_vRows = vector<void*>();
		m_bDeletePointers = true; //Assume most rows are strings and need to be deleted
	}

	void AddRow(void* obj)
	{
		m_vRows.push_back(obj);
	}

	//Accessors
	char* GetHeader()
	{
		return m_chHeader;
	}
	vector<void*>* GetRows()
	{
		return &m_vRows;
	}
	float GetWidth()
	{
		return m_fWidth;
	}
	void SetDeletePointers(bool val)
	{
		m_bDeletePointers = val;
	}
	bool ShouldDeletePointers()
	{
		return m_bDeletePointers;
	}

	boost::signal<void (GameScreen*, Column*, float, float, float, GameTime*)> Draw;
private:
	char* m_chHeader;
	vector<void*> m_vRows;
	float m_fWidth;
	bool m_bDeletePointers;		//This is useful if we have objects in the row that the table shouldn't be deleting 
};

class GUITable : public hgeGUIObject
{
public:
	GUITable(int id, GameScreen* screen, vector<Column*> columns, float x, float y, float height);
	~GUITable();

	void AddRow(vector<void*> rowText, void* rowObject);
	vector<void*> GetRow(int row);
	void SetString(int column, int row, char* str);
	void* GetRowObject(int row);
	Vector2 GetSize();
	void SetPosition(Vector2 pos);
	void ClearTable();
	void MoveDownRow();
	void MoveUpRow();
	void MoveToRow(int row);

	virtual bool MouseMove(float x, float y);
	virtual void MouseOver(bool bOver);
	virtual bool MouseWheel(int nNotches);
	virtual bool MouseLButton(bool bDown);
	virtual void Update(float gt, float dt);
	virtual void Render();

	boost::signal<void (GameScreen*, int, void*)> EntryClicked;

	//Accessors
	int GetSelectedRow()
	{
		return m_iSelectedRow;
	}
	float GetMaximumRows()
	{
		return m_fHeight;
	}
	void SetMaximumRows(int val)
	{
		m_iMaximumRows = val;
	}
	unsigned int GetColumnLength()
	{
		return m_vColumns.size();
	}
	int GetRowLength()
	{
		return m_iRows;
	}
private:
	GameScreen *m_pScreen;
	hgeSprite *m_pTable;
	vector<Column*> m_vColumns;
	vector<void*> m_vRowObjects;
	hgeColor m_cBorder, m_cTable, m_cText, m_cHighlightText;
	int m_iRows;
	float m_fHeight;
	int m_iSelectedRow;
	int m_iTopVisibleRow;
	int m_iMaximumRows;
	bool m_bMouseOver;
};

#endif