// OpenGL_MFCDialogDlg.h : header file
//

#pragma once


#include "OpenGLRenderer.h"
#include "resource.h"


// COpenGL_MFCDialogDlg dialog
class COpenGL_MFCDialogDlg : public CDialogEx
{
// Construction
public:
	COpenGL_MFCDialogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TAB_6_opengl };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support



// Implementation
protected:
	afx_msg void StartOpenGL();

	HICON m_hIcon;

	OpenGLRenderer m_OGL_Window_Left;
	OpenGLRenderer m_OGL_Window_Right;
	CStatic m_PictCtrl_Left;
	CStatic m_PictCtrl_Right;
	bool bOpenGLWindowsExists;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

};
