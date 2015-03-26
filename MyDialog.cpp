// MyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "MyDialog.h"
#include "afxdialogex.h"


// MyDialog dialog

IMPLEMENT_DYNAMIC(MyDialog, CDialogEx)

MyDialog::MyDialog(CWnd* pParent /*=NULL*/)
: CDialog(MyDialog::IDD, pParent)
{
	BOOL enable = true;
	Create(HISTORY_DIALOG);
}

void MyDialog::addString(CString str)
{
	m_wndListBox.InsertString(0, str);
}

void MyDialog::deleteString()
{
	m_wndListBox.DeleteString(0);
}

void MyDialog::setEnable()
{
	enable = !enable;
}

bool MyDialog::getEnable()
{
	return enable;
}

void MyDialog::clearList()
{
	m_wndListBox.ResetContent();
}

MyDialog::~MyDialog()
{
}

void MyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, HISTORY_LISTBOX, m_wndListBox);
}


BEGIN_MESSAGE_MAP(MyDialog, CDialog)
	ON_WM_KEYDOWN()
	ON_LBN_SELCHANGE(HISTORY_LISTBOX, &MyDialog::OnLbnSelchangeListbox)
END_MESSAGE_MAP()

void MyDialog::OnCancel()
{
	CDialog::OnCancel();
	enable = !enable;
}


void MyDialog::OnLbnSelchangeListbox()
{
	// TODO: Add your control notification handler code here
}
