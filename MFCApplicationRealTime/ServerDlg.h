#pragma once
#include "afxdialogex.h"


// Диалоговое окно ServerDlg

class ServerDlg : public CDialog
{
	DECLARE_DYNAMIC(ServerDlg)

public:
	ServerDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~ServerDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_THREE_SERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};
