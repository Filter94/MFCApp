
// MainFrm.cpp : реализаци€ класса CMainFrame
//

#include "stdafx.h"
#include "MFCApplication2.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER, &CMainFrame::OnStatusBarUpdate)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // индикатор строки состо€ни€
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// создание/уничтожение CMainFrame

CMainFrame::CMainFrame()
{
	// TODO: добавьте код инициализации члена
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	HANDLE Test_Present = CreateMutex(NULL, false, (LPCWSTR)"My_Program_Already_Present");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(0, (LPCWSTR)_T("ѕрограмма уже запущена"), NULL);
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
		return -1;
	};
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Ќе удалось создать панель инструментов\n");
		return -1;      // не удалось создать
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Ќе удалось создать строку состо€ни€\n");
		return -1;      // не удалось создать
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ”далите эти три строки, если не собираетесь закрепл€ть панель инструментов
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	return 0;
}


LRESULT CMainFrame::OnStatusBarUpdate(WPARAM wParam, LPARAM lparam)
{
	// Load string and update status bar
	CString* str = (CString *)(LPCTSTR)lparam;
	m_wndStatusBar.SetPaneText(0, *str);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: изменить класс Window или стили посредством изменени€
	//  CREATESTRUCT cs

	return TRUE;
}

// диагностика CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// обработчики сообщений CMainFrame

