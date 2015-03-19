
// MFCApplication2Doc.cpp : ���������� ������ CMFCApplication2Doc
//
#include "stdafx.h"
#include "MainFrm.h"
// SHARED_HANDLERS ����� ���������� � ������������ �������� ��������� ���������� ������� ATL, �������
// � ������; ��������� ��������� ������������ ��� ��������� � ������ �������.
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"

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


// ��������/����������� CMFCApplication2Doc

CMFCApplication2Doc::CMFCApplication2Doc()
{
	// TODO: �������� ��� ��� ������������ ������ ������������	 { { 1, 2, 2, 3 }, { 1, 2, 2, 3 }, { 4, 5, 5, 6 }, { 4, 7, 8, 6 }, { 9, 0, 0, 10 } }
	
	//CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();
}

CMFCApplication2Doc::~CMFCApplication2Doc()
{
}

BOOL CMFCApplication2Doc::OnNewDocument()
{
	gameArr[0][0] = 1;
	gameArr[0][1] = 2;
	gameArr[0][2] = 0;
	gameArr[0][3] = 3;
	gameArr[1][0] = 0;
	gameArr[1][1] = 0;
	gameArr[1][2] = 0;
	gameArr[1][3] = 0;
	gameArr[2][0] = 4;
	gameArr[2][1] = 5;
	gameArr[2][2] = 0;
	gameArr[2][3] = 6;
	gameArr[3][0] = 0;
	gameArr[3][1] = 7;
	gameArr[3][2] = 8;
	gameArr[3][3] = 0;
	gameArr[4][0] = 9;
	gameArr[4][1] = -1;
	gameArr[4][2] = -1;
	gameArr[4][3] = 10;
	selected.y = -1;
	selected.x = -1;
	turns = 0;
	return TRUE;
}




// ������������ CMFCApplication2Doc

void CMFCApplication2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << turns;
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 3; j++){
				ar << gameArr[i][j];
			}
		}
		ar << history;
	}
	else
	{
		ar >> turns;
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 3; j++){
				ar >> gameArr[i][j];
			}
		}
		ar >> history;
	}
}


BOOL CMFCApplication2Doc::OnOpenDocument(LPCTSTR lpszPathName){
	CFile storage;
	if (!storage.Open(lpszPathName, CFile::modeRead))
		return FALSE;
	CArchive arc(&storage, CArchive::load);
	Serialize(arc);
	arc.Close();
	storage.Close();
	return TRUE;
}


int CMFCApplication2Doc::getElement(const int i, const int j){
	return gameArr[i][j];
}

bool CMFCApplication2Doc::select(int i, int j){
	if (gameArr[i][j] == -1 || isWon())
		return false;
	if ((gameArr[i][j - 1] != 5 ) && ( gameArr[i][j - 1] != 2)){
		if (!gameArr[i][j]){
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

void CMFCApplication2Doc::getSelected(int& i, int& j){
	i = selected.x;
	j = selected.y;
}

void CMFCApplication2Doc::TryToMoveTo(int i, int j){
	bool change;
	History::HistoryRecord record(gameArr);
	change = MoveTo(i, j);
	if (change){
		turns++;
		history.makeUndoRecord(record);
		history.flushRedo();
	}
	selected.y = -1;
	selected.x = -1;
}

bool CMFCApplication2Doc::MoveTo(int& i, int& j){
	int movedToX = j;
	int movedToY = i;
	int element_type = gameArr[selected.x][selected.y];
	bool change = false;
	if (!isWon())
	switch (element_type){
		case 1:
		case 3:
		case 4:
		case 6:
			if ((abs(selected.x - i) <= 3) && (selected.y - j == 0)){
				if (i > selected.x){
					if (gameArr[selected.x + 2][j] == -1){
						gameArr[selected.x][selected.y] = -1;
						gameArr[selected.x + 1][selected.y] = -1;
						gameArr[i][j] = 0;
						gameArr[i - 1][j] = element_type;
						movedToY = i - 1;
						change = true;
					}
				}
				else{
					if (abs(selected.x - i) < 3 && gameArr[i][j] <= 0 && (gameArr[i + 1][j] <= 0 || gameArr[i + 1][j] == element_type)){
						gameArr[selected.x][selected.y] = -1;
						gameArr[selected.x + 1][selected.y] = -1;
						gameArr[i][j] = element_type;
						gameArr[i + 1][j] = 0;
						change = true;
					}
				}
			}
			if ((selected.x - i <= 0) && (abs(selected.y - j) == 1)){
				if (selected.x - i == -1){
					if (gameArr[i - 1][j] == -1 && gameArr[i][j] == -1){
						gameArr[selected.x][selected.y] = -1;
						gameArr[selected.x + 1][selected.y] = -1;
						gameArr[i][j] = 0;
						gameArr[i - 1][j] = element_type;
						movedToY = i - 1;
						change = true;
					}
				}
					else{
						if (selected.x - i == 0){
							if (gameArr[i + 1][j] == -1 && gameArr[i][j] == -1){
								gameArr[selected.x][selected.y] = -1;
								gameArr[selected.x + 1][selected.y] = -1;
								gameArr[i][j] = element_type;
								gameArr[i + 1][j] = 0;
								change = true;
							}
						}
					}
			}
			break;
		case 2: {
					if (selected.y - j == 0 || selected.y - j == -1){
						if (selected.x - i == -2 && gameArr[selected.x + 2][selected.y] == -1 && gameArr[selected.x + 2][selected.y + 1] == -1){
							gameArr[selected.x][selected.y] = -1;
							gameArr[selected.x][selected.y + 1] = -1;
							gameArr[selected.x + 1][selected.y] = element_type;
							gameArr[selected.x + 2][selected.y] = 0;
							gameArr[selected.x + 2][selected.y + 1] = 0;
							movedToY = selected.y;
							movedToX = selected.x + 1;
							change = true;
						}
						if (selected.x - i == 1 && gameArr[selected.x - 1][selected.y] == -1 && gameArr[selected.x - 1][selected.y + 1] == -1){
							gameArr[selected.x][selected.y] = 0;
							gameArr[selected.x + 1][selected.y] = -1;
							gameArr[selected.x + 1][selected.y + 1] = -1;
							gameArr[selected.x - 1][selected.y + 1] = 0;
							gameArr[selected.x - 1][selected.y] = element_type;
							movedToY = selected.y;
							movedToX = selected.x - 1;
							change = true;
						}
					}
					if (selected.x - i == 0 || selected.x - i == -1){
						if (selected.y - j == -2 && gameArr[selected.x + 1][selected.y + 2] == -1 && gameArr[selected.x][selected.y + 2] == -1){
							gameArr[selected.x][selected.y] = -1;
							gameArr[selected.x + 1][selected.y] = -1;
							gameArr[selected.x][selected.y + 1] = element_type;
							gameArr[selected.x][selected.y + 2] = 0;
							gameArr[selected.x + 1][selected.y + 2] = 0;
							movedToY = selected.y + 1;
							movedToX = selected.x;
							change = true;
						}
						if (selected.y - j == 1 && gameArr[selected.x + 1][selected.y - 1] == -1 && gameArr[selected.x][selected.y - 1] == -1){
							gameArr[selected.x][selected.y] = 0;
							gameArr[selected.x][selected.y + 1] = -1;
							gameArr[selected.x + 1][selected.y + 1] = -1;
							gameArr[selected.x + 1][selected.y - 1] = 0;
							gameArr[selected.x][selected.y - 1] = element_type;
							movedToY = selected.y;
							movedToX = selected.x - 1;
							change = true;
						}
					}
					break;
		}
		case 5: {
					if ((abs(selected.y - j) <= 3) && (selected.x - i == 0)){
						if (j > selected.y){
							if (gameArr[i][selected.y + 2] == -1){
								gameArr[selected.x][selected.y] = -1;
								gameArr[selected.x][selected.y + 1] = -1;
								gameArr[i][j] = 0;
								gameArr[i][j - 1] = element_type;
								movedToX = j - 1;
								change = true;
							}
						}
						else{
							if (abs(selected.y - j) < 3 && gameArr[i][j] <= 0 && (gameArr[i][j + 1] <= 0 || gameArr[i][j + 1] == element_type)){
								gameArr[selected.x][selected.y] = -1;
								gameArr[selected.x][selected.y + 1] = -1;
								gameArr[i][j] = element_type;
								gameArr[i][j + 1] = 0;
								change = true;
							}
						}
					}
					if ((selected.y - j <= 0) && (abs(selected.x - i) == 1)){
						if (selected.y - j == -1){
							if (gameArr[i][j - 1] == -1 && gameArr[i][j] == -1){
								gameArr[selected.x][selected.y] = -1;
								gameArr[selected.x][selected.y + 1] = -1;
								gameArr[i][j] = 0;
								gameArr[i][j - 1] = element_type;
								movedToX = j - 1;
								change = true;
							}
						}
						else{
							if (selected.y - j == 0){
								if (gameArr[i][j + 1] == -1 && gameArr[i][j] == -1){
									gameArr[selected.x][selected.y] = -1;
									gameArr[selected.x][selected.y + 1] = -1;
									gameArr[i][j] = element_type;
									gameArr[i][j + 1] = 0;
									change = true;
								}
							}
						}
					}
					break;
					break;
		}
		case 7:
		case 8:
		case 9:
		case 10: {
					 if (((abs(selected.y - j) == 1) || (abs(selected.x - i) == 1) ) && (abs(selected.x - i) + abs(selected.y - j) < 2)){
						 gameArr[selected.x][selected.y] = -1;
						 gameArr[i][j] = element_type;
						 change = true;
					 }
					 if (selected.x - i == 0)
						 switch (selected.y - j){
							case -2:{
										if (gameArr[i][j - 1] == -1){
											gameArr[selected.x][selected.y] = -1;
											gameArr[i][j] = element_type;
											change = true;
										}
										break;
							}
							case 2:{
									if (gameArr[i][j + 1] == -1){
										gameArr[selected.x][selected.y] = -1;
										gameArr[i][j] = element_type;
										change = true;
									}
									break;
							}
						}
					 if (selected.y - j == 0)
					 switch (selected.x - i){
					 case -2:{
								 if (gameArr[i - 1][j] == -1){
									 gameArr[selected.x][selected.y] = -1;
									 gameArr[i][j] = element_type;
									 change = true;
								 }
								 break;
					 }
					 case 2:{
								if (gameArr[i + 1][j] == -1){
									gameArr[selected.x][selected.y] = -1;
									gameArr[i][j] = element_type;
									change = true;
								}
								break;
					 }
					 }
		}
	}
	i = movedToX;
	j = movedToY;
	return change;
}


bool CMFCApplication2Doc::ForceMoveTo(int& i, int& j){
	int movedToX = j;
	int movedToY = i;
	int element_type = gameArr[selected.x][selected.y];
	bool change = false;
	if (!isWon())
		switch (element_type){
		case 1:
		case 3:
		case 4:
		case 6:
			if ((abs(selected.x - i) <= 3) && (selected.y - j == 0)){
				if (i > selected.x){
					if (gameArr[selected.x + 2][j] == -1){
						gameArr[selected.x][selected.y] = -1;
						gameArr[selected.x + 1][selected.y] = -1;
						gameArr[i][j] = 0;
						gameArr[i - 1][j] = element_type;
						movedToY = i - 1;
						change = true;
					}
				}
				else{
					if (abs(selected.x - i) < 3 && gameArr[i][j] <= 0 && (gameArr[i + 1][j] <= 0 || gameArr[i + 1][j] == element_type)){
						gameArr[selected.x][selected.y] = -1;
						gameArr[selected.x + 1][selected.y] = -1;
						gameArr[i][j] = element_type;
						gameArr[i + 1][j] = 0;
						change = true;
					}
				}
			}
			if ((selected.x - i <= 0) && (abs(selected.y - j) == 1)){
				if (selected.x - i == -1){
					if (gameArr[i - 1][j] == -1 && gameArr[i][j] == -1){
						gameArr[selected.x][selected.y] = -1;
						gameArr[selected.x + 1][selected.y] = -1;
						gameArr[i][j] = 0;
						gameArr[i - 1][j] = element_type;
						movedToY = i - 1;
						change = true;
					}
				}
				else{
					if (selected.x - i == 0){
						if (gameArr[i + 1][j] == -1 && gameArr[i][j] == -1){
							gameArr[selected.x][selected.y] = -1;
							gameArr[selected.x + 1][selected.y] = -1;
							gameArr[i][j] = element_type;
							gameArr[i + 1][j] = 0;
							change = true;
						}
					}
				}
			}
			break;
		case 2: {
			if (selected.y - j == 0 || selected.y - j == -1){
				if (selected.x - i == -2 && gameArr[selected.x + 2][selected.y] == -1 && gameArr[selected.x + 2][selected.y + 1] == -1){
					gameArr[selected.x][selected.y] = -1;
					gameArr[selected.x][selected.y + 1] = -1;
					gameArr[selected.x + 1][selected.y] = element_type;
					gameArr[selected.x + 2][selected.y] = 0;
					gameArr[selected.x + 2][selected.y + 1] = 0;
					movedToY = selected.y;
					movedToX = selected.x + 1;
					change = true;
				}
				if (selected.x - i == 1 && gameArr[selected.x - 1][selected.y] == -1 && gameArr[selected.x - 1][selected.y + 1] == -1){
					gameArr[selected.x][selected.y] = 0;
					gameArr[selected.x + 1][selected.y] = -1;
					gameArr[selected.x + 1][selected.y + 1] = -1;
					gameArr[selected.x - 1][selected.y + 1] = 0;
					gameArr[selected.x - 1][selected.y] = element_type;
					movedToY = selected.y;
					movedToX = selected.x - 1;
					change = true;
				}
			}
			if (selected.x - i == 0 || selected.x - i == -1){
				if (selected.y - j == -2 && gameArr[selected.x + 1][selected.y + 2] == -1 && gameArr[selected.x][selected.y + 2] == -1){
					gameArr[selected.x][selected.y] = -1;
					gameArr[selected.x + 1][selected.y] = -1;
					gameArr[selected.x][selected.y + 1] = element_type;
					gameArr[selected.x][selected.y + 2] = 0;
					gameArr[selected.x + 1][selected.y + 2] = 0;
					movedToY = selected.y + 1;
					movedToX = selected.x;
					change = true;
				}
				if (selected.y - j == 1 && gameArr[selected.x + 1][selected.y - 1] == -1 && gameArr[selected.x][selected.y - 1] == -1){
					gameArr[selected.x][selected.y] = 0;
					gameArr[selected.x][selected.y + 1] = -1;
					gameArr[selected.x + 1][selected.y + 1] = -1;
					gameArr[selected.x + 1][selected.y - 1] = 0;
					gameArr[selected.x][selected.y - 1] = element_type;
					movedToY = selected.y;
					movedToX = selected.x - 1;
					change = true;
				}
			}
			break;
		}
		case 5: {
			if ((abs(selected.y - j) <= 3) && (selected.x - i == 0)){
				if (j > selected.y){
					if (gameArr[i][selected.y + 2] == -1){
						gameArr[selected.x][selected.y] = -1;
						gameArr[selected.x][selected.y + 1] = -1;
						gameArr[i][j] = 0;
						gameArr[i][j - 1] = element_type;
						movedToX = j - 1;
						change = true;
					}
				}
				else{
					if (abs(selected.y - j) < 3 && gameArr[i][j] <= 0 && (gameArr[i][j + 1] <= 0 || gameArr[i][j + 1] == element_type)){
						gameArr[selected.x][selected.y] = -1;
						gameArr[selected.x][selected.y + 1] = -1;
						gameArr[i][j] = element_type;
						gameArr[i][j + 1] = 0;
						change = true;
					}
				}
			}
			if ((selected.y - j <= 0) && (abs(selected.x - i) == 1)){
				if (selected.y - j == -1){
					if (gameArr[i][j - 1] == -1 && gameArr[i][j] == -1){
						gameArr[selected.x][selected.y] = -1;
						gameArr[selected.x][selected.y + 1] = -1;
						gameArr[i][j] = 0;
						gameArr[i][j - 1] = element_type;
						movedToX = j - 1;
						change = true;
					}
				}
				else{
					if (selected.y - j == 0){
						if (gameArr[i][j + 1] == -1 && gameArr[i][j] == -1){
							gameArr[selected.x][selected.y] = -1;
							gameArr[selected.x][selected.y + 1] = -1;
							gameArr[i][j] = element_type;
							gameArr[i][j + 1] = 0;
							change = true;
						}
					}
				}
			}
			break;
			break;
		}
		case 7:
		case 8:
		case 9:
		case 10: {
			if (((abs(selected.y - j) == 1) || (abs(selected.x - i) == 1)) && (abs(selected.x - i) + abs(selected.y - j) < 2)){
				gameArr[selected.x][selected.y] = -1;
				gameArr[i][j] = element_type;
				change = true;
			}
			if (selected.x - i == 0)
				switch (selected.y - j){
				case -2:{
					if (gameArr[i][j - 1] == -1){
						gameArr[selected.x][selected.y] = -1;
						gameArr[i][j] = element_type;
						change = true;
					}
					break;
				}
				case 2:{
					if (gameArr[i][j + 1] == -1){
						gameArr[selected.x][selected.y] = -1;
						gameArr[i][j] = element_type;
						change = true;
					}
					break;
				}
			}
			if (selected.y - j == 0)
				switch (selected.x - i){
				case -2:{
					if (gameArr[i - 1][j] == -1){
						gameArr[selected.x][selected.y] = -1;
						gameArr[i][j] = element_type;
						change = true;
					}
					break;
				}
				case 2:{
					if (gameArr[i + 1][j] == -1){
						gameArr[selected.x][selected.y] = -1;
						gameArr[i][j] = element_type;
						change = true;
					}
					break;
				}
			}
		}
	}
	i = movedToX;
	j = movedToY;
	return change;
}


bool CMFCApplication2Doc::isWon(){
	return gameArr[WIN_Y][WIN_X] == 2;
}

int CMFCApplication2Doc::getTurns(){
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

void CMFCApplication2Doc::OnEditUndo(){
	turns--;
	History::HistoryRecord record(gameArr);
	history.makeRedoRecord(record);
	History::HistoryRecord step;
	step = history.makeUndo();
	ApplyHistory(step.matrix);
	selected.x = -1;
	selected.y = -1;
	UpdateAllViews(NULL);
}

void CMFCApplication2Doc::OnEditRedo(){
	turns++;
	History::HistoryRecord record(gameArr);
	history.makeUndoRecord(record);
	History::HistoryRecord step;
	step = history.makeRedo();
	ApplyHistory(step.matrix);
	selected.x = -1;
	selected.y = -1;
	UpdateAllViews(NULL);
}

void CMFCApplication2Doc::ApplyHistory(int matrix[DOC_Y][DOC_X]){
	for (int i = 0; i < DOC_Y; i++)
		for (int j = 0; j< DOC_X; j++){
			gameArr[i][j] = matrix[i][j];
		}
}

#ifdef SHARED_HANDLERS

// ��������� ��� �������
void CMFCApplication2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �������� ���� ��� ��� ����������� ������ ���������
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

// ��������� ������������ ������
void CMFCApplication2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������� ���������� ������ �� ������ ���������. 
	// ����� ����������� ������ ����������� ������ � ������� ";"

	// ��������:  strSearchContent = _T("�����;�������������;����;������ ole;");
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

// ����������� CMFCApplication2Doc

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


// ������� CMFCApplication2Doc
