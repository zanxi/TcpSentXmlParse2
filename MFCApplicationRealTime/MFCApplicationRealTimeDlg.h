
// MFCApplicationRealTimeDlg.h: файл заголовка
//

#pragma once
#include "CMyTabCtrl.h"

// Диалоговое окно CMFCApplicationRealTimeDlg
class CMFCApplicationRealTimeDlg : public CDialogEx
{
// Создание
public:
	CMFCApplicationRealTimeDlg(CWnd* pParent = nullptr);	// стандартный конструктор

	CMyTabCtrl m_tabMyTabCtrl;

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATIONREALTIME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
