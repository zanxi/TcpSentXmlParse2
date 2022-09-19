#pragma once
#include "afxdialogex.h"
#include "CMyListCtrl.h"
#include "ColoredListCtrl.h"


// Диалоговое окно CTabOne

class CTabOne : public CDialog
{
	DECLARE_DYNAMIC(CTabOne)

public:
	CTabOne(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CTabOne();

	CColoredListCtrl m_ListCtrl;
	CMyListCtrl m_ListCtrl2;
	

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { 
		IDD = IDD_TAB_ONE, 
		IDC_LIST2 = IDD_LISTCTRL_CATEGORY_GROUPS_DIALOG
	};
	
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};


