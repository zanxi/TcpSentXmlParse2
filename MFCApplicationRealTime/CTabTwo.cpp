// CTabTwo.cpp: файл реализации
//

#include "pch.h"
#include "MFCApplicationRealTime.h"
#include "afxdialogex.h"
#include <afxwin.h>
#include "CTabTwo.h"



// Диалоговое окно CTabTwo

IMPLEMENT_DYNAMIC(CTabTwo, CDialog)

BEGIN_MESSAGE_MAP(CTabTwo, CDialog)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_MOTION_EXIT, MenuExit)
	ON_COMMAND(ID_MOTION_START, MenuStart)
	ON_COMMAND(ID_MOTION_STOP, MenuStop)
	//ON_WM_TIMER(ID_TIMER_FOR_MOTION, OnTimer)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


BOOL CTabTwo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	BOOL bRet = CDialog::OnInitDialog();
	m_nTimer = SetTimer(1, 1, NULL);
	return bRet;

	//return TRUE;  // return TRUE  unless you set the focus to a control
}


void CTabTwo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CTabTwo::OnMouseMove(UINT, CPoint cp)
{
	//char chX[10];
	//char chY[10];
	//atoi(cp.x, chX, 10);
	//itoa(cp.y, chY, 10);
	//CString csStatusX(chX);
	//CString csStatusY(chY);
	//m_wndStatusBar.SetPaneText(0, std::to_string(cp.x));
	//m_wndStatusBar.SetPaneText(1, std::to_string(cp.y));
	//m_wndStatusBar.SetPaneText(1, csStatusY);
}

CTabTwo::CTabTwo(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TAB_TWO, pParent)
{
	m_brush = CreateSolidBrush(RGB(0, 255, 0));

	//Create(NULL, "Users window application", WS_OVERLAPPEDWINDOW, rectDefault, NULL, NULL);
	MyStatic = new TColorText();	
	if (MyStatic != NULL)MyStatic->Create(L"Классы инструментов", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(10, 10, 100, 50), this);

	//MyStatic->SetTextColor(RGB(255, 0, 255));
	//MyStatic->setTransparent(true);

	MyButton = new CButton();
	if (MyButton != NULL)MyButton->Create(L"Редактировать", WS_CHILD | WS_VISIBLE | SS_CENTER, CRect(120, 10, 220, 50), this, IDC_MYBUTTON);
	MyEdit = new CEdit();
	if (MyEdit != NULL)MyEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(240, 10, 340, 50), this, IDC_MYEDIT);

	oldPlace.left = 150;
	oldPlace.top = 150;
	oldPlace.right = 170;
	oldPlace.bottom = 170;
	newPlace = oldPlace;
	VX = VY = sgn_x = sgn_y = 1;
	VX = VY = 4; 
	sgn_x = sgn_y = 4;

	//SetTimer(ID_TIMER_FOR_MOTION, 10, NULL);
	//SetTimer(IDD_TAB_TWO, 10, NULL);

}

HBRUSH CTabTwo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//ID_STATICTEXT

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// Return handle to our CBrush object
	hbr = m_brush;

	return hbr;

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		//pDC->SetTextColor(RGB(255, 0, 0));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

void CTabTwo::MenuStart()
{
	//SetTimer(ID_TIMER_FOR_MOTION, 1, NULL);
	return;
}


void CTabTwo::MenuStop()
{
	KillTimer(ID_TIMER_FOR_MOTION);
	return;
}

void CTabTwo::OnTimer(UINT_PTR nIDEvent)
{
	//return;
	CRect rectClient;
	GetClientRect(&rectClient);
	//rectClient.bottom -= 10;
	//rectClient.left += 10;
	if (newPlace.right > rectClient.right || newPlace.left < rectClient.left) sgn_x = -sgn_x;
	if (newPlace.bottom > rectClient.bottom || newPlace.top < rectClient.top) sgn_y = -sgn_y;

	newPlace.left += sgn_x * VX;
	newPlace.right += sgn_x * VX;
	newPlace.top += sgn_y * VY;
	newPlace.bottom += sgn_y * VY;

	/*newPlace.left += sgn_x * VX;
	newPlace.right += sgn_x * VX;
	newPlace.top += sgn_y * VY;
	newPlace.bottom += sgn_y * VY;*/

	//int xc = rectClient.left + int((x_current - x_start) / (x_end - x_start) * (rectClient.right - rectClient.left)) + 1;
	//int yc = int((y_next - ymin) / (ymax - ymin) * rectClient.bottom) + 1;

	//InvalidateRect(oldPlace);
	InvalidateRect(newPlace);
	oldPlace = newPlace;
}


CTabTwo::~CTabTwo()
{
	if (MyStatic != NULL)delete MyStatic;
	if (MyButton != NULL)delete MyButton;
	if (MyEdit != NULL)delete MyEdit;
}

int CTabTwo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1) return -1;
	m_wndStatusBar.Create(this);
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
	m_wndStatusBar.SetPaneInfo(0, 0, 0, 50);
	m_wndStatusBar.SetPaneInfo(1, 0, 0, 50);
	m_wndMenu.LoadMenuW(IDR_MENU1);
	SetMenu(&m_wndMenu);
	return 0;
}

void CTabTwo::OnPaint()
{
	//return;
	CPaintDC dc(this);
	CBrush bBrush(RGB(0, 0, 255));
	dc.SelectObject(&bBrush);
	dc.Ellipse(newPlace);
}

void CTabTwo::OnKeyDown(UINT nChar, UINT, UINT)
{
	//return;
	switch (nChar)
	{
	case 40:
		if (sgn_y > 0)VY++; else VY--;
		break;
	case 38:
		if (sgn_y > 0)VY--; else VY++;
		break;
	case 37:
		if (sgn_x > 0)VX--; else VX++;
		break;
	case 39:
		if (sgn_x > 0)VX++; else VX--;
		break;
	}
}

void CTabTwo::MenuExit()
{
	DestroyWindow();
}


// Обработчики сообщений CTabTwo
