
// MFCApplication2View.cpp : реализация класса CMFCApplication2View
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"
#include "MFCApplication2View.h"
#include "registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// CMFCApplication2View

IMPLEMENT_DYNCREATE(CMFCApplication2View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication2View, CView)
	// Стандартные команды печати
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_BACKGROUND_COLOR, CMFCApplication2View::OnEditBackgroundcolor)
	ON_COMMAND(ID_RECTANGLES_COLOR, CMFCApplication2View::OnEditRectanglesColor)
	ON_COMMAND(ID_SCORE_FONT, CMFCApplication2View::OnEditScoreFont)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// создание/уничтожение CMFCApplication2View

CMFCApplication2View::CMFCApplication2View()
{
	scoreFont = new CFont;
	scoreFont->CreatePointFont(FW_REGULAR, _T("Baskerville Old Face"));
	selectedMsg.LoadString(SELECTED_MSG);
	movedMsg.LoadString(MOVED_MSG);
	cannotMsg.LoadString(CANNOT_MOVE);
	Registry registry;
	bool isOK = registry.OpenKey(pszSubKey);

	if (isOK)
	{
		int regBackgroundColor = registry.GetValue(pszBackgroundColor);
		if (regBackgroundColor)
		{
			backgroundColor = (COLORREF)regBackgroundColor;
		}
		int regItemsColor = registry.GetValue(pszItemsColor);
		if (regItemsColor)
		{
			rectanglesColor = (COLORREF)regItemsColor;
		}
		CString fontName = registry.GetStrValue(pszScoreFontName, FONT_NAME_BUFFER);
		int scoreFontWidth = registry.GetValue(pszScoreFontWidth);
		if (scoreFontWidth == 0)
		{
			scoreFontWidth = FW_REGULAR;
		}
		if (fontName)
		{
			CFont* newFont = new CFont;
			newFont->CreatePointFont(scoreFontWidth, fontName);
			scoreFont = newFont;
			
		}
	}
}

BOOL CMFCApplication2View::PreCreateWindow(CREATESTRUCT& cs)
{
	cxFullScreen = GetSystemMetrics(SM_CXFULLSCREEN);
	cyFullScreen = GetSystemMetrics(SM_CYFULLSCREEN);
	return CView::PreCreateWindow(cs);
}

int CMFCApplication2View::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CDC* pDC = GetWindowDC();
	memDC.CreateCompatibleDC(pDC);
	hBmpOffscreen.CreateCompatibleBitmap(pDC, cxFullScreen, cyFullScreen);
	memDC.SelectObject(&hBmpOffscreen);
	PatBlt(memDC, 0, 0, cxFullScreen, cyFullScreen, WHITENESS);


	COLORREF	crPen;
	HPEN	    hPen, hPenOld;
	SetBkMode(memDC, TRANSPARENT);

	crPen = RGB(71, 60, 139);
	hPen = CreatePen(PS_SOLID, 20, crPen);
	hPenOld = (HPEN)SelectObject(memDC, hPen);

	ReleaseDC(pDC);
	return 0;
}

// рисование CMFCApplication2View

void CMFCApplication2View::OnDraw(CDC* pDC)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	PatBlt(memDC, 0, 0, cxFullScreen, cyFullScreen, WHITENESS);
	CRect rect, rOuterRect, rCellRect, rSelectedRect;
	GetClientRect(&rect);
	rOuterRect.top = rect.bottom / 2 - (CELL_SIZE * DOC_Y) / 2;
	rOuterRect.left = rect.right / 2 - (CELL_SIZE * DOC_X) / 2;
	rOuterRect.bottom = rOuterRect.top + CELL_SIZE * DOC_Y;
	rOuterRect.right = rOuterRect.left + CELL_SIZE * DOC_X;
	CPen pOuterPen(PS_SOLID, FRAME_WIDTH, RGB(255, 0, 255)),
		pCellPen(PS_SOLID, CELL_WIDTH, RGB(255, 255, 0)),
		pSelectPen(PS_SOLID, CELL_WIDTH, RGB(0, 255, 255));
	CBrush bCellBrush(rectanglesColor),
		bBackgroundBrush(backgroundColor);
	memDC.SelectObject(pOuterPen);
	memDC.SelectObject(bBackgroundBrush);
	memDC.Rectangle(rOuterRect);
	rCellRect.top = rOuterRect.top;
	int g, k;
	pDoc->getSelected(g, k);
	for (int i = 0; i < DOC_Y; i++)
	{
		rCellRect.left = rOuterRect.left;
		for (int j = 0; j < DOC_X; j++)
		{
			int element_type = pDoc->getElement(i, j);
			switch (element_type)
			{
			case FIRST_TYPE:
				rCellRect.right = rCellRect.left + CELL_SIZE;
				rCellRect.bottom = rCellRect.top + 2 * CELL_SIZE;
				break;
			case SECOND_TYPE:
			{
				rCellRect.right = rCellRect.left + 2 * CELL_SIZE;
				rCellRect.bottom = rCellRect.top + 2 * CELL_SIZE;
				break;
			}
			case THIRD_TYPE:
			{
				rCellRect.right = rCellRect.left + 2 * CELL_SIZE;
				rCellRect.bottom = rCellRect.top + CELL_SIZE;
				break;
			}
			case FOURTH_TYPE:
			{
				rCellRect.right = rCellRect.left + CELL_SIZE;
				rCellRect.bottom = rCellRect.top + CELL_SIZE;
				break;
			}
			}	//	Any type of object is > than EMPTY of PART_OF_OBJECT
			if (element_type > PART_OF_OBJECT)
			{
				memDC.SelectObject(pCellPen);
				memDC.SelectObject(bCellBrush);
				memDC.Rectangle(rCellRect);
			}
			if ((g >= PART_OF_OBJECT && k >= PART_OF_OBJECT) && ((i == g) && (j == k)))
			{
				rSelectedRect.top = rCellRect.top;
				rSelectedRect.bottom = rCellRect.bottom;
				rSelectedRect.right = rCellRect.right;
				rSelectedRect.left = rCellRect.left;
			}
			rCellRect.left += CELL_SIZE;
		}
		rCellRect.top += CELL_SIZE;
	}
	if ((g >= PART_OF_OBJECT && k >= PART_OF_OBJECT))
	{
		memDC.SelectObject(pSelectPen);
		memDC.SelectObject(bCellBrush);
		memDC.Rectangle(rSelectedRect);
	}
	CString  s, f;
	CRect textRect;
	textRect.top = rect.bottom - 80;
	textRect.left = rect.right / 2 - 300;
	textRect.bottom = rect.bottom - 10;
	textRect.right = rect.right / 2 + 300;
	memDC.SelectObject(scoreFont);
	s.LoadString(IDS_SCORE);
	CString cszTemp;
	f.LoadString(TURN_MSG_FORMAT);
	cszTemp.Format(f, s, pDoc->getTurns());
	memDC.DrawText(cszTemp, textRect, DT_CENTER);
	if (pDoc->isWon()){
		textRect.top = 10;
		textRect.left = rect.right / 2 - 300;
		textRect.bottom = textRect.top + 100;
		textRect.right = rect.right / 2 + 300;
		s.LoadString(IDS_WON_MESSAGE);
		memDC.SelectObject(scoreFont);
		memDC.DrawText(s, textRect, DT_CENTER);
	}
	pDC->BitBlt(0, 0, cxFullScreen, cyFullScreen, &memDC, 0, 0, SRCCOPY);
}


// печать CMFCApplication2View

BOOL CMFCApplication2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CMFCApplication2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью

}

void CMFCApplication2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// диагностика CMFCApplication2View

#ifdef _DEBUG
void CMFCApplication2View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication2Doc* CMFCApplication2View::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication2Doc)));
	return (CMFCApplication2Doc*)m_pDocument;
}
#endif //_DEBUG


// обработчики сообщений CMFCApplication2View

bool clickInDocument(const CMFCApplication2View& obj, CPoint pPoint)
{
	CRect rect, rOuterRect, rCellRect;
	obj.GetClientRect(&rect);
	rOuterRect.top = rect.bottom / 2 - (CELL_SIZE * DOC_Y) / 2;
	rOuterRect.left = rect.right / 2 - (CELL_SIZE * DOC_X) / 2;
	rOuterRect.bottom = rOuterRect.top + CELL_SIZE * DOC_Y;
	rOuterRect.right = rOuterRect.left + CELL_SIZE * DOC_X;
	if (pPoint.y < rOuterRect.bottom && pPoint.y > rOuterRect.top && pPoint.x < rOuterRect.right && pPoint.x > rOuterRect.left)
		return true;
	return false;
}

void countIndexes(CMFCApplication2View& obj, CPoint& pPoint)
{
	CRect rect, rOuterRect, rCellRect;
	obj.GetClientRect(&rect);
	rOuterRect.top = rect.bottom / 2 - (CELL_SIZE * DOC_Y) / 2;
	rOuterRect.left = rect.right / 2 - (CELL_SIZE * DOC_X) / 2;
	rOuterRect.bottom = rOuterRect.top + CELL_SIZE * DOC_Y;
	rOuterRect.right = rOuterRect.left + CELL_SIZE * DOC_X;
	rCellRect.top = rOuterRect.top;
	rCellRect.bottom = rCellRect.top + CELL_SIZE;
	rCellRect.right = rOuterRect.right;
	rCellRect.left = rOuterRect.left;
	int i;
	for (i = 0; i < DOC_Y; ++i)
	{
		if (PtInRect(rCellRect, pPoint))
		{
			break;
		}
		rCellRect.top += CELL_SIZE;
		rCellRect.bottom += CELL_SIZE;
	}
	rCellRect.top = rOuterRect.top;
	rCellRect.bottom = rOuterRect.bottom;
	rCellRect.left = rOuterRect.left;
	rCellRect.right = rOuterRect.left + CELL_SIZE;
	for (int j = 0; j < DOC_X; j++)
	{
		if (PtInRect(rCellRect, pPoint))
		{
			pPoint.x = j, pPoint.y = i;
			break;
		}
		rCellRect.right += CELL_SIZE;
		rCellRect.left += CELL_SIZE;
	}
}

void CMFCApplication2View::OnLButtonDown(UINT nFlags, CPoint pPoint)
{
	CPoint indexes(pPoint.x, pPoint.y);
	if (clickInDocument(*this, pPoint))
	{
		int selectedX;
		int selectedY;
		countIndexes(*this, indexes);
		CMFCApplication2Doc* pDoc;
		pDoc = GetDocument();
		pDoc->getSelected(selectedX, selectedY);
		CString* cszTemp = new CString;
		if (!pDoc->select(indexes.y, indexes.x)){
			int i, j;
			pDoc->getSelected(i, j);
			if (i != -1 && j != -1){
				int movedToX = indexes.x;
				int movedToY = indexes.y;
				pDoc->TryToMoveTo(movedToY, movedToX);
				if (movedToX != -1 && movedToY != -1){
					cszTemp->Format(movedMsg, selectedX, selectedY, movedToX, movedToY);
					AfxGetMainWnd()->PostMessage(WM_USER, 0, (LPARAM)(LPCSTR)cszTemp);
				}
				else{
					cszTemp = new CString(cannotMsg);
					AfxGetMainWnd()->PostMessage(WM_USER, 0, (LPARAM)(LPCSTR)cszTemp);
				}
			}
		}
		else{
			int i, j;
			pDoc->getSelected(i, j);
			cszTemp->Format(selectedMsg, i, j);
			AfxGetMainWnd()->SendMessage(WM_USER, 0, (LPARAM)cszTemp);
		}
		Invalidate();
	}
	CView::OnLButtonDown(nFlags, pPoint);
}


void CMFCApplication2View::OnEditBackgroundcolor()
{
	CColorDialog cc;
	if (cc.DoModal() == IDOK)
	{
		backgroundColor = cc.GetColor();
	}
	GetDocument()->UpdateAllViews(NULL);
}


void CMFCApplication2View::OnEditRectanglesColor()
{
	CColorDialog cc;
	if (cc.DoModal() == IDOK)
	{
		rectanglesColor = cc.GetColor();
	}
	GetDocument()->UpdateAllViews(NULL);
}

void CMFCApplication2View::OnEditScoreFont()
{

	LOGFONT lf;
	scoreFont->GetLogFont(&lf);
	CFontDialog cc(&lf);
	if (cc.DoModal() == IDOK)
	{
		scoreFont->DeleteObject();
		scoreFont->CreateFontIndirect(&lf);
	}
	GetDocument()->UpdateAllViews(NULL);
}

BOOL CMFCApplication2View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


CMFCApplication2View::~CMFCApplication2View()
{
	Registry registry;

	bool isOK = registry.CreateKey(pszSubKey);

	if (isOK)
	{
		registry.SetKeyValue(NULL, pszBackgroundColor, (int)backgroundColor);
		registry.SetKeyValue(NULL, pszItemsColor, (int)rectanglesColor);
		LOGFONT lf;
		scoreFont->GetLogFont(&lf);
		registry.SetKeyValue(NULL, pszScoreFontName, (TCHAR*)&lf.lfFaceName);
		registry.SetKeyValue(NULL, pszScoreFontWidth, (int)lf.lfWeight);
	}
}