
// MFCApplicationRealTimeDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "MFCApplicationRealTime.h"
#include "MFCApplicationRealTimeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CMFCApplicationRealTimeDlg



CMFCApplicationRealTimeDlg::CMFCApplicationRealTimeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATIONREALTIME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationRealTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB3, m_tabMyTabCtrl);
}

BEGIN_MESSAGE_MAP(CMFCApplicationRealTimeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// Обработчики сообщений CMFCApplicationRealTimeDlg

BOOL CMFCApplicationRealTimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	m_tabMyTabCtrl.InsertItem(0, _T("System Process"));
	m_tabMyTabCtrl.InsertItem(1, _T("RS-232 Real time graphic"));
	m_tabMyTabCtrl.InsertItem(2, _T("Server Tcp/Udp"));
	m_tabMyTabCtrl.InsertItem(3, _T("Client Tcp/Udp"));
	m_tabMyTabCtrl.InsertItem(4, _T("HyperCube Dynamic 3d"));
	m_tabMyTabCtrl.InsertItem(5, _T("OpenGL dynamic"));

	


	//*************************************

	//CWnd* pCtrl = GetDlgItem(IDC_CONTROL1);
	//// keep it centered
	//CRect rectCtrl;
	//pCtrl->GetWindowRect(rectCtrl);
	//MoveWindow(rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height());

	CRect rectCtrl;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rectCtrl, 0);
	//dlg.GetWindowRect(rectCtrl);
	MoveWindow(rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height());
	//SetWindowPos(this,
	//	1, 1,
	//	(rectCtrl.right), (rectCtrl.bottom),		
	//	//(rectCtrl.Width()) / 2, (rectCtrl.Height()) / 2,
	//	SWP_NOSIZE | SWP_NOZORDER);
	//*************************************

	m_tabMyTabCtrl.Init(rectCtrl);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CMFCApplicationRealTimeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMFCApplicationRealTimeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMFCApplicationRealTimeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

