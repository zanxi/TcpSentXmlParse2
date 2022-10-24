
// OpenGL_MFCDialogDlg.cpp : implementation file
//
#include "pch.h"
#include "stdafx.h"
//#include "OpenGL_MFCDialog.h"
#include "OpenGL_MFCDialogDlg.h"
#include "afxdialogex.h"
#include "resource.h"		// основные символы

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGL_MFCDialogDlg dialog

COpenGL_MFCDialogDlg::COpenGL_MFCDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpenGL_MFCDialogDlg::IDD, pParent)
{
	bOpenGLWindowsExists = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenGL_MFCDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPENGL_LEFT, m_PictCtrl_Left);
	DDX_Control(pDX, IDC_OPENGL_RIGHT, m_PictCtrl_Right);
}

BEGIN_MESSAGE_MAP(COpenGL_MFCDialogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// COpenGL_MFCDialogDlg message handlers

BOOL COpenGL_MFCDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here

	StartOpenGL();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COpenGL_MFCDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COpenGL_MFCDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COpenGL_MFCDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COpenGL_MFCDialogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED)
		return;

	switch (nType)
	{
		case SIZE_RESTORED:
		case SIZE_MAXIMIZED:
		{
			if(bOpenGLWindowsExists)
			{
				//Reshape here is just maps to OnSize();
				m_OGL_Window_Left.Reshape(nType,m_OGL_Window_Left.GetViewPortWidth(), m_OGL_Window_Left.GetViewPortHeight()); //viewport WxH input here
				
				//sizeable
				CRect rect = m_OGL_Window_Right.GetWinRect();
				m_OGL_Window_Right.SetViewPortSize(cx-rect.left-50, cy-rect.top-50);
				m_OGL_Window_Right.Reshape(nType,m_OGL_Window_Right.GetViewPortWidth(), m_OGL_Window_Right.GetViewPortHeight());
			}
			break;
		}

		default:
			break;
	}
}


void COpenGL_MFCDialogDlg::StartOpenGL()
{
	// TODO: Add your control notification handler code here
	CRect rectLeft, rectRight;
	int iPosOffSetX, iPosOffSetY, iWidth, iHeight;
	iWidth = 250;
	iHeight = 250;

	////------------------------
	iPosOffSetX = 0;
	iPosOffSetY = 25;
	m_PictCtrl_Left.MoveWindow(iPosOffSetX,iPosOffSetY,iWidth,iHeight,TRUE); //position window & size first and then
	m_PictCtrl_Left.GetWindowRect(rectLeft);
	// Convert screen coordinates to client coordinates
	ScreenToClient(rectLeft);

	////------------------------
	iPosOffSetX = iPosOffSetX + iWidth + 50;
	iPosOffSetY = iPosOffSetY;
	iWidth = 200;
	iHeight = 200;
	m_PictCtrl_Right.MoveWindow(iPosOffSetX,iPosOffSetY,iWidth,iHeight, TRUE);
	m_PictCtrl_Right.GetWindowRect(rectRight);
	ScreenToClient(rectRight);

	if(bOpenGLWindowsExists)
	{
		m_OGL_Window_Left.DestroyScene();
		m_OGL_Window_Right.DestroyScene();
		bOpenGLWindowsExists = false;
	}

	// Create OpenGL Control window
	if(!bOpenGLWindowsExists)
	{
		m_OGL_Window_Left.CreateGLContext(rectLeft, this);
		m_OGL_Window_Left.PrepareScene(); //will show without this but as black & white.
		m_OGL_Window_Left.SetData(1);
		// Setup the OpenGL Window's timer to render
		m_OGL_Window_Left.m_unpTimer = m_OGL_Window_Left.SetTimer(1, 1, 0);

		m_OGL_Window_Right.CreateGLContext(rectRight, this);
		m_OGL_Window_Right.PrepareScene();
		m_OGL_Window_Right.SetData(3);
		m_OGL_Window_Right.m_unpTimer = m_OGL_Window_Right.SetTimer(1, 1, 0);
		bOpenGLWindowsExists = true;
	}
}
