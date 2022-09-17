#pragma once
#include "afxdialogex.h"
#include "ColoredListCtrl.h"


// Диалоговое окно AppServiceDlg

   class CTabOne2 : public CDialog
	{
		// Construction
	public:
		CTabOne2(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
		//{{AFX_DATA(CTabOne2)		
		enum {
			IDD = IDD_TAB_ONE,
			IDC_LIST2 = IDD_LISTCTRL_CATEGORY_GROUPS_DIALOG
		};
		CColoredListCtrl m_List;
		CListCtrl	m_List2;
		void SetRectangle(int nX, int nY, int nXc, int nYc);
		BOOL CListCtrlFill();

		//}}AFX_DATA

		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CTabOne2)
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		afx_msg void OnContextMenu(CWnd*, CPoint point);
		HICON m_hIcon;

		// Generated message map functions
		//{{AFX_MSG(CTabOne2)
		virtual BOOL OnInitDialog();
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		afx_msg void OnTimer(UINT_PTR);
		afx_msg void OnDELSERVICE();
		afx_msg void OnStart();
		afx_msg void OnStop();
		afx_msg void OnRewrite();
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	};

	//{{AFX_INSERT_LOCATION}}
	// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
