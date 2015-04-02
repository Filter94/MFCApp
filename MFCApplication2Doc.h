#pragma once
#include "History.h"
#include "MyDialog.h"
#include "MemoryFile.h"


class CMFCApplication2Doc : public CDocument
{
protected: // ������� ������ �� ������������
	CMFCApplication2Doc();
	DECLARE_DYNCREATE(CMFCApplication2Doc)

// ��������
public:

protected:
	int gameArr[DOC_Y][DOC_X];
	int turns;
	CPoint selected;
	History history;
	MyDialog m_wndMyDlg;
	CString storyFormat;
// ��������
public:
	bool select(int i, int j);	//	True if there is no errors
	void getSelected(int& i, int& j);
	void TryToMoveTo(int& i, int& j);
	bool MoveTo(int& i, int& j);	//	True if there is no errors
	bool ForceMoveTo(int& i, int& j);
	int getElement(const int i, const int j);
	bool isWon();
	int getTurns();
// ���������������
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnUpdateEditUndo(CCmdUI *pCmdUI);
	virtual void OnUpdateEditRedo(CCmdUI *pCmdUI);
	void addStringInListBox(CString str);
	virtual void OnEditUndo();
	virtual void OnEditRedo();
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ����������
public:
	virtual ~CMFCApplication2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ��������������� �������, �������� ���������� ������ ��� ����������� ������
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};
