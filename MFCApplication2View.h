
// MFCApplication2View.h : интерфейс класса CMFCApplication2View
//

#pragma once


class CMFCApplication2View : public CView
{
private:
	COLORREF backgroundColor = RGB(255,255,255);
	COLORREF rectanglesColor = RGB(235, 235, 235);
	CFont * scoreFont;
	CString selectedMsg;
	CString movedMsg;
	CString cannotMsg;
	const CString pszSubKey = L"Software\\GingerDonkey";
	const CString pszBackgroundColor = L"BackgroundColor";
	const CString pszItemsColor = L"ItemsColor";
	const CString pszScoreFontName = L"ScoreFontName";
	const CString pszScoreFontWidth = L"ScoreFontWidth";

protected: // создать только из сериализации
	CMFCApplication2View();
	DECLARE_DYNCREATE(CMFCApplication2View)

// Атрибуты
public:
	CMFCApplication2Doc* GetDocument() const;
	int	cxFullScreen;
	int	cyFullScreen;
	CDC	memDC;
	CBitmap	hBmpOffscreen;
// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Реализация
public:
	afx_msg virtual int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual ~CMFCApplication2View();
	afx_msg void OnEditBackgroundcolor();
	afx_msg void OnEditRectanglesColor();
	afx_msg void OnEditScoreFont();
	afx_msg void OnUpdatePage(CCmdUI *pCmdUI);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // отладочная версия в MFCApplication2View.cpp
inline CMFCApplication2Doc* CMFCApplication2View::GetDocument() const
   { return reinterpret_cast<CMFCApplication2Doc*>(m_pDocument); }
#endif

