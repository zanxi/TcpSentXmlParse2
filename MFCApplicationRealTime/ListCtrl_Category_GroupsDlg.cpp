// ListCtrl_Category_GroupsDlg.cpp : implementation file
//

//#include "stdafx.h"
//#include "ListCtrl_Category_GroupsApp.h"
#include "ListCtrl_Category_GroupsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CListCtrl_Category_GroupsDlg dialog



CListCtrl_Category_GroupsDlg::CListCtrl_Category_GroupsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListCtrl_Category_GroupsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CListCtrl_Category_GroupsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CListCtrl_Category_GroupsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CListCtrl_Category_GroupsDlg message handlers

BOOL CListCtrl_Category_GroupsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// Create Columns
	for(int col = 0; col < m_DataModel.GetColCount() ; ++col)
	{
		CString title( m_DataModel.GetColTitle(col).c_str() );
		m_ListCtrl.InsertColumn(col, title, LVCFMT_LEFT, 100);
	}

	// Insert data into list-control by copying from datamodel
	int nItem = 0;
	for(size_t rowId = 0; rowId < m_DataModel.GetRowIds() ; ++rowId)
	{
		CString value( m_DataModel.GetCellText(rowId, 0).c_str() );
		nItem = m_ListCtrl.InsertItem(++nItem, value);
		m_ListCtrl.SetItemData(nItem, rowId);
		for(int col = 0; col < m_DataModel.GetColCount() ; ++col)
		{
			CString value( m_DataModel.GetCellText(rowId, col).c_str() );
			m_ListCtrl.SetItemText(nItem, col, value);
		}
	}
	m_ListCtrl.GroupByColumn(2);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CListCtrl_Category_GroupsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CListCtrl_Category_GroupsDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CListCtrl_Category_GroupsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
