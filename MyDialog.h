#pragma once
#include "afxwin.h"


// MyDialog dialog

class MyDialog : public CDialog
{
	DECLARE_DYNAMIC(MyDialog)

private:
	CListBox m_wndListBox;
	BOOL enable;
public:
	void addString(CString);
	void deleteString();
	void clearList();
	void setEnable();
	bool getEnable();

public:
	MyDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MyDialog();
	virtual void OnCancel();


	// Dialog Data
	enum { IDD = HISTORY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
};
