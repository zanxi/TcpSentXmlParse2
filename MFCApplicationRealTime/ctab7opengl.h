#pragma once
#include "afxdialogex.h"


// Диалоговое окно ctab7opengl

class ctab7opengl : public CDialogEx
{
	DECLARE_DYNAMIC(ctab7opengl)

public:
	ctab7opengl(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~ctab7opengl();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_6_opengl };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};
