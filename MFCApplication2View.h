
// MFCApplication2View.h : ��������� ������ CMFCApplication2View
//

#pragma once


class CMFCApplication2View : public CView
{
private:
	COLORREF backgroundColor = RGB(255,255,255);
	COLORREF rectanglesColor = RGB(235, 235, 235);
	CFont * scoreFont;

protected: // ������� ������ �� ������������
	CMFCApplication2View();
	DECLARE_DYNCREATE(CMFCApplication2View)

// ��������
public:
	CMFCApplication2Doc* GetDocument() const;

// ��������
public:

// ���������������
public:
	virtual void OnDraw(CDC* pDC);  // �������������� ��� ��������� ����� �������������
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����������
public:
	virtual ~CMFCApplication2View();
	afx_msg void OnEditBackgroundcolor();
	afx_msg void OnEditRectanglesColor();
	afx_msg void OnEditScoreFont();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // ���������� ������ � MFCApplication2View.cpp
inline CMFCApplication2Doc* CMFCApplication2View::GetDocument() const
   { return reinterpret_cast<CMFCApplication2Doc*>(m_pDocument); }
#endif

