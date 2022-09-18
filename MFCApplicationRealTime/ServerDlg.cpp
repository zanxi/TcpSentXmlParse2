// ServerDlg.cpp: файл реализации
//

#include "pch.h"
#include "MFCApplicationRealTime.h"
#include "afxdialogex.h"
#include "ServerDlg.h"


// Диалоговое окно ServerDlg

IMPLEMENT_DYNAMIC(ServerDlg, CDialog)

ServerDlg::ServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TAB_THREE_SERVER, pParent)
{

}

ServerDlg::~ServerDlg()
{
}

void ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ServerDlg, CDialog)
END_MESSAGE_MAP()


// Обработчики сообщений ServerDlg
