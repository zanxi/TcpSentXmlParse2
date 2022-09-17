// CTabOne.cpp: файл реализации
//

#include "pch.h"
#include "MFCApplicationRealTime.h"
#include "afxdialogex.h"
#include "CTabOne.h"


// Диалоговое окно CTabOne

IMPLEMENT_DYNAMIC(CTabOne, CDialog)

CTabOne::CTabOne(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TAB_ONE, pParent)
{

}

CTabOne::~CTabOne()
{
}

void CTabOne::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDD_LISTCTRL_CATEGORY_GROUPS_DIALOG, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CTabOne, CDialog)
END_MESSAGE_MAP()


// Обработчики сообщений CTabOne
