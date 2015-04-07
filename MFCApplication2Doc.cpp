
// MFCApplication2Doc.cpp : реализация класса CMFCApplication2Doc
//
#include "stdafx.h"
#include "MainFrm.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"
#include "md5.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCApplication2Doc

IMPLEMENT_DYNCREATE(CMFCApplication2Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFCApplication2Doc, CDocument)
	ON_COMMAND(ID_EDIT_UNDO, &CMFCApplication2Doc::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMFCApplication2Doc::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CMFCApplication2Doc::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CMFCApplication2Doc::OnUpdateEditRedo)
END_MESSAGE_MAP()


// создание/уничтожение CMFCApplication2Doc

CMFCApplication2Doc::CMFCApplication2Doc()
{
	storyFormat.LoadString(MOVED_MSG);
}

CMFCApplication2Doc::~CMFCApplication2Doc()
{
}

BOOL CMFCApplication2Doc::OnNewDocument()
{
	m_wndMyDlg.clearList();
	history.flushHistory();
	gameArr[0][0] = FIRST_TYPE;
	gameArr[0][1] = SECOND_TYPE;
	gameArr[0][2] = PART_OF_OBJECT;
	gameArr[0][3] = FIRST_TYPE;
	gameArr[1][0] = PART_OF_OBJECT;
	gameArr[1][1] = PART_OF_OBJECT;
	gameArr[1][2] = PART_OF_OBJECT;
	gameArr[1][3] = PART_OF_OBJECT;
	gameArr[2][0] = FIRST_TYPE;
	gameArr[2][1] = THIRD_TYPE;
	gameArr[2][2] = PART_OF_OBJECT;
	gameArr[2][3] = FIRST_TYPE;
	gameArr[3][0] = PART_OF_OBJECT;
	gameArr[3][1] = FOURTH_TYPE;
	gameArr[3][2] = FOURTH_TYPE;
	gameArr[3][3] = PART_OF_OBJECT;
	gameArr[4][0] = FOURTH_TYPE;
	gameArr[4][1] = EMPTY;
	gameArr[4][2] = EMPTY;
	gameArr[4][3] = FOURTH_TYPE;
	selected.y = NONE_SELECTED;
	selected.x = NONE_SELECTED;
	turns = 0;
	return TRUE;
}
// сериализация CMFCApplication2Doc

//void CMFCApplication2Doc::Serialize(CArchive& ar)
//{
//	if (ar.IsStoring())
//	{
//		ar << turns;
//		for (int i = 0; i < DOC_Y; i++)
//		{
//			for (int j = 0; j < DOC_X; j++)
//			{
//				ar << gameArr[i][j];
//			}
//		}
//		ar << history;
//	}
//	else
//	{
//		ar >> turns;
//		for (int i = 0; i < DOC_Y; i++)
//		{
//			for (int j = 0; j < DOC_X; j++)
//			{
//				ar >> gameArr[i][j];
//			}
//		}
//		ar >> history;
//		stack<History::HistoryRecord> undoCopy;
//		undoCopy = history.getUndoStackReversedCopy();
//		while (undoCopy.size()){
//			History::HistoryRecord record = undoCopy.top();
//			CString stringBuf;
//			stringBuf.Format(storyFormat, record.before.x, record.before.y, record.after.x, record.after.y);
//			m_wndMyDlg.addString(stringBuf);
//			undoCopy.pop();
//		}
//	}
//}


BOOL CMFCApplication2Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	MemoryFile memFile(lpszPathName);
	int length;
	memFile >> length;
	length;
	string file_content((char*)memFile.szMap, length);
	MD5 md5(file_content);
	string digest = md5.hexdigest();
	string old_digest("");
	int int_buf;
	char char_buf;
	char* buf_char = new char[MD5_SIZE];
	int position = length / sizeof(int) + 2;
	memFile.setPosition(position);
	for (int i = 0; i < MD5_SIZE; i++)
	{
		memFile >> int_buf;
		char_buf = (char)int_buf;
		buf_char[i] = char_buf;
		old_digest += string(buf_char, 1);
	}
	old_digest = string(buf_char, MD5_SIZE);
	if (digest != old_digest)
	{
		CDialog aboutDlg(CHECKSUM_ERROR_DIALOG);
		aboutDlg.DoModal();
	}
	else
	{
		history.flushHistory();
		memFile.setPosition(1);
		m_wndMyDlg.clearList();
		memFile >> turns;
		for (int i = 0; i < DOC_Y; i++)
		{
			for (int j = 0; j < DOC_X; j++)
			{
				memFile >> gameArr[i][j];
			}
		}
		memFile >> history;
		stack<History::HistoryRecord> undoCopy;
		undoCopy = history.getUndoStackReversedCopy();
		while (undoCopy.size()){
			History::HistoryRecord record = undoCopy.top();
			CString stringBuf;
			stringBuf.Format(storyFormat, record.before.x, record.before.y, record.after.x, record.after.y);
			m_wndMyDlg.addString(stringBuf);
			undoCopy.pop();
		}
	}
	return TRUE;
}

BOOL CMFCApplication2Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	SetModifiedFlag(FALSE);
	MemoryFile memFile(lpszPathName);
	int total_size = 1 * sizeof(int); // length of length
	total_size += history.getSize();
	total_size += 1 * sizeof(int); // turns
	total_size += DOC_Y * DOC_X * sizeof(int); // turns
	memFile << total_size;
	memFile << turns;
	for (int i = 0; i < DOC_Y; i++)
	{
		for (int j = 0; j < DOC_X; j++)
		{
			memFile << gameArr[i][j];
		}
	}
	memFile << history;
	string file_content((char*)memFile.szMap, total_size);
	MD5 md5(file_content);
	string digest = md5.hexdigest();
	int length = digest.length();
	char * cstr = new char[digest.length() + 1];
	std::strcpy(cstr, digest.c_str());
	for (int i = 0; i < length; i++)
	{
		memFile << digest[i];
	}
	return TRUE;
}


int CMFCApplication2Doc::getElement(const int i, const int j)
{
	return gameArr[i][j];
}

bool CMFCApplication2Doc::select(int i, int j)
{
	if (gameArr[i][j] == EMPTY || isWon())
		return false;
	if ((gameArr[i][j - 1] != THIRD_TYPE) && (gameArr[i][j - 1] != SECOND_TYPE))
	{
		if (!gameArr[i][j])
		{
			i--;
			if (!gameArr[i][j])
				j--;
		}
	}
	else
		j--;
	selected.x = i, selected.y = j;
	return true;
}

void CMFCApplication2Doc::getSelected(int& i, int& j)
{
	i = selected.x;
	j = selected.y;
}

void CMFCApplication2Doc::TryToMoveTo(int& i, int& j)
{
	bool moved;
	moved = MoveTo(i, j);
	if (moved)
	{
		turns++;
		History::Cell before(selected.x, selected.y);
		History::Cell after(j, i);
		History::HistoryRecord record(before, after);
		CString stringBuf;
		stringBuf.Format(storyFormat, record.before.x, record.before.y, record.after.x, record.after.y);
		m_wndMyDlg.addString(stringBuf);
		history.makeUndoRecord(record);
		history.flushRedo();
		SetModifiedFlag(TRUE);
	}
	else{
		i = -1;
		j = -1;
	}
	selected.y = NONE_SELECTED;
	selected.x = NONE_SELECTED;
}

bool CMFCApplication2Doc::MoveTo(int& i, int& j)
{
	int movedToX = j;
	int movedToY = i;
	int element_type = gameArr[selected.x][selected.y];
	bool change = false;
	if (!isWon())
		switch (element_type)
	{
		case FIRST_TYPE: {
			if ((abs(selected.x - i) <= THIRD_TYPE) && (selected.y - j == 0))
			{
				if (i > selected.x)
				{
					if (gameArr[selected.x + 2][j] == EMPTY)
					{
						gameArr[selected.x][j] = EMPTY;
						gameArr[selected.x + 1][j] = EMPTY;
						gameArr[i][j] = PART_OF_OBJECT;
						gameArr[i - 1][j] = element_type;
						movedToY = i - 1;
						change = true;
					}
				}
				else
				{	// EMPTY < PART_OF_OBJECT
					if (abs(selected.x - i) < THIRD_TYPE && gameArr[i][j] <= 0 && (gameArr[i + 1][j] <= 0 || gameArr[i + 1][j] == element_type))
					{
						gameArr[selected.x][selected.y] = EMPTY;
						gameArr[selected.x + 1][selected.y] = EMPTY;
						gameArr[i][j] = element_type;
						gameArr[i + 1][j] = PART_OF_OBJECT;
						change = true;
					}
				}
			}
			if ((selected.x - i <= 0) && (abs(selected.y - j) == 1))
			{
				if (selected.x - i == -1)
				{
					if (gameArr[i - 1][j] == EMPTY && gameArr[i][j] == EMPTY)
					{
						gameArr[selected.x][selected.y] = EMPTY;
						gameArr[selected.x + 1][selected.y] = EMPTY;
						gameArr[i][j] = PART_OF_OBJECT;
						gameArr[i - 1][j] = element_type;
						movedToY = i - 1;
						change = true;
					}
				}
				else
				{
					if (selected.x - i == 0)
					{
						if (gameArr[i + 1][j] == EMPTY && gameArr[i][j] == EMPTY)
						{
							gameArr[i][selected.y] = EMPTY;
							gameArr[i + 1][selected.y] = EMPTY;
							gameArr[i][j] = element_type;
							gameArr[i + 1][j] = PART_OF_OBJECT;
							change = true;
						}
					}
				}
			}
			break;
		}
		case SECOND_TYPE: 
		{
			if (selected.y - j == 0 || selected.y - j == -1)
			{
				if (selected.x - i == -2 && gameArr[selected.x + 2][selected.y] == EMPTY && gameArr[selected.x + 2][selected.y + 1] == EMPTY)
				{
					gameArr[selected.x][selected.y] = EMPTY;
					gameArr[selected.x][selected.y + 1] = EMPTY;
					gameArr[selected.x + 1][selected.y] = element_type;
					gameArr[selected.x + 2][selected.y] = PART_OF_OBJECT;
					gameArr[selected.x + 2][selected.y + 1] = PART_OF_OBJECT;
					movedToY = selected.x + 1;
					movedToX = selected.y;
					change = true;
				}
				if (selected.x - i == 1 && gameArr[selected.x - 1][selected.y] == EMPTY && gameArr[selected.x - 1][selected.y + 1] == EMPTY)
				{
					gameArr[selected.x][selected.y] = PART_OF_OBJECT;
					gameArr[selected.x + 1][selected.y] = EMPTY;
					gameArr[selected.x + 1][selected.y + 1] = EMPTY;
					gameArr[selected.x - 1][selected.y + 1] = PART_OF_OBJECT;
					gameArr[selected.x - 1][selected.y] = element_type;
					movedToY = selected.x - 1;
					movedToX = selected.y;
					change = true;
				}
			}
			if (selected.x - i == 0 || selected.x - i == -1)
			{
				if (selected.y - j == -2 && gameArr[selected.x + 1][selected.y + 2] == EMPTY && gameArr[selected.x][selected.y + 2] == EMPTY)
				{
					gameArr[selected.x][selected.y] = EMPTY;
					gameArr[selected.x + 1][selected.y] = EMPTY;
					gameArr[selected.x][selected.y + 1] = element_type;
					gameArr[selected.x][selected.y + 2] = PART_OF_OBJECT;
					gameArr[selected.x + 1][selected.y + 2] = PART_OF_OBJECT;
					movedToY = selected.x;
					movedToX = selected.y + 1;
					change = true;
				}
				if (selected.y - j == 1 && gameArr[selected.x + 1][selected.y - 1] == EMPTY && gameArr[selected.x][selected.y - 1] == EMPTY)
				{
					gameArr[selected.x][selected.y] = PART_OF_OBJECT;
					gameArr[selected.x][selected.y + 1] = EMPTY;
					gameArr[selected.x + 1][selected.y + 1] = EMPTY;
					gameArr[selected.x + 1][selected.y - 1] = PART_OF_OBJECT;
					gameArr[selected.x][selected.y - 1] = element_type;
					movedToY = selected.x;
					movedToX = selected.y - 1;
					change = true;
				}
			}
			break;
		}
		case THIRD_TYPE: 
		{
			if ((abs(selected.y - j) <= 3) && (selected.x - i == 0))
			{
				if (j > selected.y)
				{
					if (gameArr[i][selected.y + 2] == EMPTY)
					{
						gameArr[i][selected.y] = EMPTY;
						gameArr[i][selected.y + 1] = EMPTY;
						gameArr[i][j] = PART_OF_OBJECT;
						gameArr[i][j - 1] = element_type;
						movedToX = j - 1;
						change = true;
					}
				}
				else
				{
					if (abs(selected.y - j) < 3 && gameArr[i][j] <= 0 && (gameArr[i][j + 1] <= PART_OF_OBJECT || gameArr[i][j + 1] == element_type))
					{
						gameArr[i][selected.y] = EMPTY;
						gameArr[i][selected.y + 1] = EMPTY;
						gameArr[i][j] = element_type;
						gameArr[i][j + 1] = PART_OF_OBJECT;
						change = true;
					}
				}
			}
			if ((selected.y - j <= 0) && (abs(selected.x - i) == 1))
			{
				if (selected.y - j == -1)
				{
					if (gameArr[i][j - 1] == -1 && gameArr[i][j] == EMPTY)
					{
						gameArr[selected.x][selected.y] = EMPTY;
						gameArr[selected.x][selected.y + 1] = EMPTY;
						gameArr[i][j] = PART_OF_OBJECT;
						gameArr[i][j - 1] = element_type;
						movedToX = j - 1;
						change = true;
					}
				}
				else
				{
					if (selected.y - j == 0)
					{
						if (gameArr[i][j + 1] == EMPTY && gameArr[i][j] == EMPTY)
						{
							gameArr[selected.x][j] = EMPTY;
							gameArr[selected.x][j + 1] = EMPTY;
							gameArr[i][j] = element_type;
							gameArr[i][j + 1] = PART_OF_OBJECT;
							change = true;
						}
					}
				}
			}
			break;
		}
		case FOURTH_TYPE:
		{
			if (((abs(selected.y - j) == 1) || (abs(selected.x - i) == 1)) && (abs(selected.x - i) + abs(selected.y - j) < 2))
			{
				gameArr[selected.x][selected.y] = EMPTY;
				gameArr[i][j] = element_type;
				change = true;
			}
			if (selected.x - i == 0)
				switch (selected.y - j)
			{
				case -2:
				{
					if (gameArr[i][j - 1] == EMPTY)
					{
						gameArr[i][selected.y] = EMPTY;
						gameArr[i][j] = element_type;
						change = true;
					}
					break;
				}
				case 2:
				{
					if (gameArr[i][j + 1] == EMPTY)
					{
						gameArr[i][selected.y] = EMPTY;
						gameArr[i][j] = element_type;
						change = true;
					}
					break;
				}
			}
			if (selected.y - j == 0)
				switch (selected.x - i)
			{
				case -2:
				{
					if (gameArr[i - 1][j] == EMPTY)
					{
						gameArr[selected.x][j] = EMPTY;
						gameArr[i][j] = element_type;
						change = true;
					}
					break;
				}
				case 2:
				{
					if (gameArr[i + 1][j] == EMPTY)
					{
						gameArr[selected.x][selected.y] = EMPTY;
						gameArr[i][j] = element_type;
						change = true;
					}
					break;
				}
			}
			break;
		}
	}
	i = movedToX;
	j = movedToY;
	return change;
}


bool CMFCApplication2Doc::ForceMoveTo(int& i, int& j)
{
	int movedToX = j;
	int movedToY = i;
	int element_type = gameArr[selected.x][selected.y];
	bool change = false;
	if (!isWon())
		switch (element_type)
		{
			case FIRST_TYPE:
			{
				gameArr[selected.x][selected.y] = EMPTY;
				gameArr[selected.x + 1][selected.y] = EMPTY;
				gameArr[i][j] = element_type;
				gameArr[i + 1][j] = 0;
				break;
			}
			case SECOND_TYPE:
			{
				gameArr[selected.x][selected.y] = EMPTY;
				gameArr[selected.x][selected.y + 1] = EMPTY;
				gameArr[selected.x + 1][selected.y] = EMPTY;
				gameArr[selected.x + 1][selected.y + 1] = EMPTY;
				gameArr[i][j] = element_type;
				gameArr[i][j + 1] = PART_OF_OBJECT;
				gameArr[i + 1][j] = PART_OF_OBJECT;
				gameArr[i + 1][j + 1] = PART_OF_OBJECT;
				break;
			}
			case THIRD_TYPE:
			{
				gameArr[selected.x][selected.y] = EMPTY;
				gameArr[selected.x][selected.y + 1] = EMPTY;
				gameArr[i][j] = element_type;
				gameArr[i][j + 1] = PART_OF_OBJECT;
				break;
			}
			case FOURTH_TYPE:
			{
				gameArr[selected.x][selected.y] = EMPTY;
				gameArr[i][j] = element_type;
			}
	}
	return change;
}

void CMFCApplication2Doc::addStringInListBox(CString str)
{
	m_wndMyDlg.addString(str);
}

bool CMFCApplication2Doc::isWon()
{
	return gameArr[WIN_Y][WIN_X] == WIN_TYPE;
}

int CMFCApplication2Doc::getTurns()
{
	return turns;
}

void CMFCApplication2Doc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	if (!history.canUndo())
		pCmdUI->Enable(FALSE);
	else pCmdUI->Enable(TRUE);
}

void CMFCApplication2Doc::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	if (!history.canRedo())
		pCmdUI->Enable(FALSE);
	else pCmdUI->Enable(TRUE);
}

void CMFCApplication2Doc::OnEditUndo()
{
	turns--;
	History::HistoryRecord step;
	step = history.makeUndo();
	history.makeRedoRecord(step);
	selected.x = step.after.x;
	selected.y = step.after.y;
	int toI, toJ;
	toI = step.before.x;
	toJ = step.before.y;
	ForceMoveTo(toI, toJ);
	selected.x = NONE_SELECTED;
	selected.y = NONE_SELECTED;
	m_wndMyDlg.deleteString();
	UpdateAllViews(NULL);
}

void CMFCApplication2Doc::OnEditRedo()
{
	turns++;
	History::HistoryRecord step;
	step = history.makeRedo();
	history.makeUndoRecord(step);
	selected.x = step.before.x;
	selected.y = step.before.y;
	int toI, toJ;
	toI = step.after.x;
	toJ = step.after.y;
	ForceMoveTo(toI, toJ);
	selected.x = NONE_SELECTED;
	selected.y = NONE_SELECTED;
	CString stringBuf;
	stringBuf.Format(storyFormat, step.before.x, step.before.y, step.after.x, step.after.y);
	m_wndMyDlg.addString(stringBuf);
	UpdateAllViews(NULL);
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CMFCApplication2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CMFCApplication2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задайте содержимое поиска из данных документа. 
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CMFCApplication2Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// диагностика CMFCApplication2Doc

#ifdef _DEBUG
void CMFCApplication2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCApplication2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
