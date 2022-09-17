// AppServiceDlg.cpp: файл реализации
//

#include "pch.h"
#include "MFCApplicationRealTime.h"
#include "afxdialogex.h"
#include "AppServiceDlg.h"


// AppServiceDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "AppService.h"
#include "AppServiceDlg.h"
#include "winsvc.h"
#include "resource.h"
//#include "ProgDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//резервируем память для структуры сервиса
#define SIZE_BUF 4096

//Описываем структуру для сервиса

ENUM_SERVICE_STATUS			Status[SIZE_BUF];
DWORD   Size = sizeof(Status);
DWORD   Needed = 0;
DWORD   Return = 0;
DWORD   Handle = 0;
SC_HANDLE hSCM;

SERVICE_STATUS ssStatus;
DWORD dwOldCheckPoint;

//Структура для позиции курсора
POSITION pos;
//Имя сервиса
CString SZSERVICENAME;




/////////////////////////////////////////////////////////////////////////////
// CTabOne2 dialog

CTabOne2::CTabOne2(CWnd* pParent /*=NULL*/)
	: CDialog(CTabOne2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabOne2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTabOne2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabOne2)
	DDX_Control(pDX, IDC_LIST2, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabOne2, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CTabOne2)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_COMMAND(DELSERVICE, OnDELSERVICE)
//	ON_COMMAND(IDSTART, OnStart)
//	ON_COMMAND(IDSTOP, OnStop)
//	ON_COMMAND(IDREWRITE, OnRewrite)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabOne2 message handlers

BOOL CTabOne2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTabOne2::OnPaint()
{
	//if (IsIconic())
	
	if (TRUE)
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		//m_List.UpdateData(TRUE);
		m_List.UpdateWindow();
		m_List.Invalidate();
		dc.DrawIcon(x, y, m_hIcon);
		CDialog::OnPaint();
		
		//InvalidateRect(m_List);
	}
	else
	{
		//CDialog::OnPaint();
	}

	//CPaintDC dc(this);
	//CBrush bBrush(RGB(0, 0, 255));
	//dc.SelectObject(&bBrush);
	//dc.Ellipse(newPlace);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTabOne2::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

void CTabOne2::SetRectangle(int nX, int nY, int nXc, int nYc)
{
	m_List.SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);	
}

void CTabOne2::OnTimer(UINT_PTR nIDEvent)
{
	//CListCtrlFill();
}

BOOL CTabOne2::CListCtrlFill()
{
	LV_ITEM item;
	item.iItem = 0;
	item.iImage = 0;

	item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;

	item.iSubItem = 0;

	m_List.DeleteAllItems();

	CString csText;

	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM != NULL)
	{
		if (EnumServicesStatus(hSCM, SERVICE_WIN32, SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUS)&Status, Size, &Needed, &Return, &Handle))
		{
			//m_List.SetItemCount(Return);
			for (unsigned int x = 0; x < Return; x++)
			{
				csText = Status[x].lpServiceName;

				m_List.InsertItem(x, csText);

				m_List.SetItemText(x, 2, Status[x].lpDisplayName);
				if (Status[x].ServiceStatus.dwCurrentState == SERVICE_RUNNING)
					csText = "Работает";
				else
				{
					csText = "";
				}
				m_List.SetItemText(x, 1, csText);

			}
		}
		else {
			CloseServiceHandle(hSCM);
			return FALSE;
		}
	}
	else {
		CloseServiceHandle(hSCM);
		return FALSE;
	};



	CloseServiceHandle(hSCM);
}



int CTabOne2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	CRect rect;
	GetClientRect(rect);

	m_List.Create(LVS_ICON | LVS_REPORT | WS_VISIBLE, rect, this, 100);
	m_List.InsertColumn(0, _T("Имя Сервиса"), LVCFMT_LEFT, rect.Width() / 3);
	m_List.InsertColumn(1, _T("Состояние"), LVCFMT_LEFT, rect.Width() / 3);
	m_List.InsertColumn(2, _T("Описание"), LVCFMT_LEFT, rect.Width() / 3);

	CListCtrlFill();
	//SetTimer(IDC_LIST2, 1, NULL);


	return 0;
}

void CTabOne2::OnDELSERVICE()
{
	// TODO: Add your command handler code here
	pos = m_List.GetFirstSelectedItemPosition();

	(int)pos--;
	SZSERVICENAME = m_List.GetItemText((int)pos, 0);



	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{

		AfxMessageBox(L"Does not open SCManager");

	}

	SC_HANDLE hService = OpenService(hSCM, SZSERVICENAME, SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		//cout<<"Does not open SCManager"<<endl;
		AfxMessageBox(L"Does not open SCManager");
		CloseServiceHandle(hSCM);
		//return FALSE;
	}
	SERVICE_STATUS status;
	ControlService(hService, SERVICE_CONTROL_STOP, &status);

	BOOL bDelete = DeleteService(hService);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCM);

	if (bDelete)
	{

		AfxMessageBox(L"The service is removed");



	}
	if (!bDelete)
	{
		AfxMessageBox(L"The service is not removed");
	}
	m_List.SetItemText((int)pos, 0, L" ");
	m_List.SetItemText((int)pos, 1, L" ");
	m_List.SetItemText((int)pos, 2, L" ");
}



void CTabOne2::OnStart()
{
	// TODO: Add your command handler code here
	//CProgressDlg m_Progress;
//	m_Progress.Create(this);
//	m_Progress.SetRange(0, 10);
//
//	//m_Progress.SetPos(0);
//
//	pos = m_List.GetFirstSelectedItemPosition();
//
//	(int)pos--;
//	SZSERVICENAME = m_List.GetItemText((int)pos, 0);
//
//
//
//	m_Progress.SetPos(1);
//	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
//
//	SC_HANDLE schService = OpenService(
//		hSCM,          // SCM database 
//		SZSERVICENAME,    // service name
//		SERVICE_ALL_ACCESS);
//	m_Progress.SetPos(2);
//
//	if (schService == NULL)
//
//		AfxMessageBox(L"OpenService");
//
//	if (!StartService(
//		schService,  // handle to service 
//		0,           // number of arguments 
//		NULL))      // no arguments 
//	{
//
//		AfxMessageBox(L"The service is started");
//	}
//	else
//		;
//	//AfxMessageBox("Service start pending");
//
//
//
//// Check the status until the service is no longer start pending.
//	m_Progress.SetPos(3);
//
//	if (!QueryServiceStatus(
//		schService,   // handle to service 
//		&ssStatus))  // address of status information
//
//		AfxMessageBox(L"QueryServiceStatus");
//
//
//	while (ssStatus.dwCurrentState == SERVICE_START_PENDING)
//	{
//		// Save the current checkpoint. 
//
//		dwOldCheckPoint = ssStatus.dwCheckPoint;
//
//		// Wait for the specified interval.
//		m_Progress.SetPos(4);
//
//		Sleep(ssStatus.dwWaitHint);
//		m_Progress.SetPos(5);
//
//		// Check the status again.
//
//
//		if (!QueryServiceStatus(
//			schService,   // handle to service 
//			&ssStatus))  // address of status information
//			break;
//
//		// Break if the checkpoint has not been incremented. 
//
//		if (dwOldCheckPoint >= ssStatus.dwCheckPoint)
//			break;
//	}
//
//	/*if (ssStatus.dwCurrentState == SERVICE_RUNNING)
//
//		AfxMessageBox("StartService SUCCESS");
//	else
//	{
//
//		AfxMessageBox("The service has not started");
//	} */
//	m_Progress.SetPos(8);
//	CloseServiceHandle(schService);
//	CTabOne2::OnRewrite();
//	m_Progress.SetPos(9);



}

void CTabOne2::OnStop()
{
	//// TODO: Add your command handler code here
	////CProgressDlg m_Progress;
	////m_Progress.Create(this);
	////m_Progress.SetRange(0, 10);

	//pos = m_List.GetFirstSelectedItemPosition();
	//m_Progress.SetPos(1);

	//(int)pos--;
	//SZSERVICENAME = m_List.GetItemText((int)pos, 0);

	//hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	//if (hSCM == NULL)
	//{
	//	AfxMessageBox("Does not open SCManager");

	//}
	//m_Progress.SetPos(2);
	//SC_HANDLE hService = OpenService(hSCM, SZSERVICENAME, SERVICE_STOP);

	//if (hService == NULL)
	//{
	//	CloseServiceHandle(hSCM);
	//	AfxMessageBox("Does not open Service");

	//}
	//SERVICE_STATUS status;

	//m_Progress.SetPos(3);
	//ControlService(hService, SERVICE_CONTROL_STOP, &status);
	//m_Progress.SetPos(4);

	//CloseServiceHandle(hService);
	//m_Progress.SetPos(8);
	//CloseServiceHandle(hSCM);
	//m_Progress.SetPos(9);
	////AfxMessageBox("Service stopped ");

	//CTabOne2::OnRewrite();

}

void CTabOne2::OnRewrite()
{
	// TODO: Add your command handler code here

	pos = m_List.GetFirstSelectedItemPosition();

	(int)pos--;
	CString SZSERVICENAME = m_List.GetItemText((int)pos, 0);

	SERVICE_STATUS ssStatus;


	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	SC_HANDLE schService = OpenService(
		hSCM,          // SCM database 
		SZSERVICENAME,    // service name
		SERVICE_ALL_ACCESS);

	if (!QueryServiceStatus(
		schService,   // handle to service 
		&ssStatus))  // address of status information

		AfxMessageBox(L"Отметьте сервис");



	if (ssStatus.dwCurrentState == SERVICE_RUNNING)
		m_List.SetItemText((int)pos, 1, L"Работает");


	if (ssStatus.dwCurrentState == SERVICE_STOPPED)
		m_List.SetItemText((int)pos, 1, L" ");

	CloseServiceHandle(schService);

}



void CTabOne2::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1) {
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		/*CMenu menu;
		VERIFY(menu.LoadMenu(CG_IDR_POPUP_APP_SERVICE_DLG));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);*/
	}
}


