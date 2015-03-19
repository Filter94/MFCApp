
// MFCApplication2Doc.h : интерфейс класса CMFCApplication2Doc
//

#include "History.h"
#pragma once

class CMFCApplication2Doc : public CDocument
{
protected: // создать только из сериализации
	CMFCApplication2Doc();
	DECLARE_DYNCREATE(CMFCApplication2Doc)

// Атрибуты
public:

protected:
	int gameArr[DOC_Y][DOC_X];
	int turns;
	CPoint selected;
	History history;
	void ApplyHistory(int [DOC_Y][DOC_X]);
// Операции
public:
	bool select(int i, int j);	//	True if there is no errors
	void getSelected(int& i, int& j);
	void TryToMoveTo(int i, int j);
	bool MoveTo(int& i, int& j);	//	True if there is no errors
	bool ForceMoveTo(int& i, int& j);
	int getElement(const int i, const int j);
	bool isWon();
	int getTurns();
// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnUpdateEditUndo(CCmdUI *pCmdUI);
	virtual void OnUpdateEditRedo(CCmdUI *pCmdUI);
	virtual void OnEditUndo();
	virtual void OnEditRedo();
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CMFCApplication2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
