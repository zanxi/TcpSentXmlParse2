// Dynamic3d.cpp: файл реализации
//

#include "pch.h"
#include "MFCApplicationRealTime.h"
#include "afxdialogex.h"
#include "Dynamic3d.h"


// Диалоговое окно Dynamic3d

IMPLEMENT_DYNAMIC(Dynamic3d, CDialog)

Dynamic3d::Dynamic3d(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TAB_FIVE__2d, pParent)
{

}

Dynamic3d::~Dynamic3d()
{
}

void Dynamic3d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dynamic3d, CDialog)
END_MESSAGE_MAP()


// Обработчики сообщений Dynamic3d

using namespace std;


// CMy3DGraficaDlg dialog




CMy3DGraficaDlg::CMy3DGraficaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy3DGraficaDlg::IDD, pParent)
	, zp(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy3DGraficaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy3DGraficaDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMy3DGraficaDlg message handlers

BOOL CMy3DGraficaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	zp = -1000;
	vp3d.RemoveAll();
	vp2d.RemoveAll();
	vu.RemoveAll();

	vp3d.Add(Punct3(-250, -250, 400));//A
	vp3d.Add(Punct3(250, -250, 400));//B
	vp3d.Add(Punct3(250, -250, 900));//C
	vp3d.Add(Punct3(-250, -250, 900));//D
	vp3d.Add(Punct3(-250, 250, 400));//A'
	vp3d.Add(Punct3(250, 250, 400));//B'
	vp3d.Add(Punct3(250, 250, 900));//C'
	vp3d.Add(Punct3(-250, 250, 900));//D'

	vu.Add(Unirii(0, 1));//AB
	vu.Add(Unirii(1, 2));//BC
	vu.Add(Unirii(2, 3));//CD
	vu.Add(Unirii(3, 0));//DA
	vu.Add(Unirii(0, 4));//AA'
	vu.Add(Unirii(1, 5));//BB'
	vu.Add(Unirii(2, 6));//CC'
	vu.Add(Unirii(3, 7));//DD'
	vu.Add(Unirii(4, 5));//A'B'
	vu.Add(Unirii(5, 6));//B'C'
	vu.Add(Unirii(6, 7));//C'D'
	vu.Add(Unirii(7, 4));//D'A'

	tomoveH = true;
	tomoveV = true;

	SetTimer(1, 50, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy3DGraficaDlg::OnPaint()
{

	CPaintDC dc(this); // device context for painting

	CPen* pen, * oldPen;
	CBrush* br, * oldBr;
	COLORREF black = RGB(0, 0, 0);
	pen = new CPen(PS_SOLID, 2, black);
	oldPen = (CPen*)dc.SelectObject(pen);
	br = new CBrush(RGB(155, 0, 0));
	oldBr = (CBrush*)dc.SelectObject(br);

	CRect r; GetClientRect(&r);
	double dx = r.Width() / 2; double dy = r.Height() / 2;

	vp2d.RemoveAll();
	for (int i = 0; i < vp3d.GetCount(); i++)
		vp2d.Add(Pp(vp3d[i], dx, dy));

	for (int i = 0; i < vu.GetCount(); i++)
	{
		dc.MoveTo(vp2d[vu[i].i1].x, vp2d[vu[i].i1].y);
		dc.LineTo(vp2d[vu[i].i2].x, vp2d[vu[i].i2].y);
	}
}



// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy3DGraficaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


Punct3D CMy3DGraficaDlg::Punct3(double x, double y, double z)
{
	Punct3D p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;
}


Unire CMy3DGraficaDlg::Unirii(int i1, int i2)
{
	Unire u;
	u.i1 = i1;
	u.i2 = i2;
	return u;
}


CPoint CMy3DGraficaDlg::Pp(Punct3D p, double dx, double dy)
{
	CPoint pp;
	double cz = zp / (zp - p.z);
	pp.x = (int)dx + p.x * cz;
	pp.y = (int)dy + p.y * cz;
	return pp;
}

void CMy3DGraficaDlg::Translatie(double x, double y, double z, double dx, double dy, double dz, double& xp, double& yp, double& zp)
{
	xp = x + dx;
	yp = y + dy;
	zp = z + dz;
}

void CMy3DGraficaDlg::Scalare(double x, double y, double z, double xg, double yg, double zg, double sx, double sy, double sz, double& xs, double& ys, double& zs)
{
	xs = sx * (x - xg) + xg;
	ys = sy * (y - yg) + yg;
	zs = sz * (z - zg) + zg;
}

void Rotatie(double& x, double& y, double xc, double yc, double cosu, double sinu)
{
	double x0 = x;
	x = (x - xc) * cosu - (y - yc) * sinu + xc;
	y = (x0 - xc) * sinu + (y - yc) * cosu + yc;
}

void CMy3DGraficaDlg::RotatieOX(double u)
{
	if (!vp3d.GetSize())
		return;

	double cosu = cos(u), sinu = sin(u);

	double yg = 0, zg = 0;

	for (int i = 0; i < vp3d.GetSize(); i++)
	{
		yg += vp3d[i].y;
		zg += vp3d[i].z;
	}

	yg /= vp3d.GetSize();
	zg /= vp3d.GetSize();

	for (int i = 0; i < vp3d.GetSize(); i++)
		Rotatie(vp3d[i].y, vp3d[i].z, yg, zg, cosu, sinu);
}

void CMy3DGraficaDlg::RotatieOY(double u)
{
	if (!vp3d.GetSize())
		return;

	double cosu = cos(u), sinu = sin(u);

	double zg = 0, xg = 0;

	for (int i = 0; i < vp3d.GetSize(); i++)
	{
		zg += vp3d[i].z;
		xg += vp3d[i].x;
	}

	zg /= vp3d.GetSize();
	xg /= vp3d.GetSize();

	for (int i = 0; i < vp3d.GetSize(); i++)
		Rotatie(vp3d[i].z, vp3d[i].x, zg, xg, cosu, sinu);
}
void CMy3DGraficaDlg::RotatieOZ(double u)
{
	if (!vp3d.GetSize())
		return;

	double cosu = cos(u), sinu = sin(u);

	double xg = 0, yg = 0;

	for (int i = 0; i < vp3d.GetSize(); i++)
	{
		xg += vp3d[i].x;
		yg += vp3d[i].y;
	}

	xg /= vp3d.GetSize();
	yg /= vp3d.GetSize();

	for (int i = 0; i < vp3d.GetSize(); i++)
		Rotatie(vp3d[i].x, vp3d[i].y, xg, yg, cosu, sinu);
}

BOOL CMy3DGraficaDlg::PreTranslateMessage(MSG* p)
{
	if (p->message == WM_KEYDOWN)
	{
		if (p->wParam == VK_LEFT)
		{
			for (int i = 0; i < vp3d.GetCount(); i++)
			{
				Translatie(vp3d[i].x, vp3d[i].y, vp3d[i].z, -10, 0, 0, vp3d[i].x, vp3d[i].y, vp3d[i].z);
			}

			Invalidate();
		}
		if (p->wParam == VK_UP)
		{
			for (int i = 0; i < vp3d.GetCount(); i++)
			{
				Translatie(vp3d[i].x, vp3d[i].y, vp3d[i].z, 0, -10, 0, vp3d[i].x, vp3d[i].y, vp3d[i].z);
			}

			Invalidate();
		}
		if (p->wParam == VK_RIGHT)
		{
			for (int i = 0; i < vp3d.GetCount(); i++)
			{
				Translatie(vp3d[i].x, vp3d[i].y, vp3d[i].z, 10, 0, 0, vp3d[i].x, vp3d[i].y, vp3d[i].z);
			}

			Invalidate();
		}
		if (p->wParam == VK_DOWN)
		{
			for (int i = 0; i < vp3d.GetCount(); i++)
			{
				Translatie(vp3d[i].x, vp3d[i].y, vp3d[i].z, 0, 10, 0, vp3d[i].x, vp3d[i].y, vp3d[i].z);
			}

			Invalidate();
		}
		if (p->wParam == VK_ADD)
		{
			double xg = 0, yg = 0, zg = 0;
			for (int i = 0; i < vp3d.GetCount(); i++)
			{
				xg += vp3d[i].x;
				yg += vp3d[i].y;
				zg += vp3d[i].z;
			}
			xg = xg / vp3d.GetCount();
			yg = yg / vp3d.GetCount();
			zg = zg / vp3d.GetCount();

			for (int i = 0; i < vp3d.GetCount(); i++)
			{
				Scalare(vp3d[i].x, vp3d[i].y, vp3d[i].z, xg, yg, zg, 1.1, 1.1, 1.1, vp3d[i].x, vp3d[i].y, vp3d[i].z);
				Translatie(vp3d[i].x, vp3d[i].y, vp3d[i].z, 0, 10, 0, vp3d[i].x, vp3d[i].y, vp3d[i].z);
			}

			Invalidate();
		}
		if (p->wParam == VK_SUBTRACT)
		{
			double xg = 0, yg = 0, zg = 0;
			for (int i = 0; i < vp3d.GetCount(); i++)
			{
				xg += vp3d[i].x;
				yg += vp3d[i].y;
				zg += vp3d[i].z;
			}
			xg = xg / vp3d.GetCount();
			yg = yg / vp3d.GetCount();
			zg = zg / vp3d.GetCount();

			for (int i = 0; i < vp3d.GetCount(); i++)
			{
				Scalare(vp3d[i].x, vp3d[i].y, vp3d[i].z, xg, yg, zg, 1 / 1.1, 1 / 1.1, 1 / 1.1, vp3d[i].x, vp3d[i].y, vp3d[i].z);
				Translatie(vp3d[i].x, vp3d[i].y, vp3d[i].z, 0, 10, 0, vp3d[i].x, vp3d[i].y, vp3d[i].z);
			}

			Invalidate();
		}
		if (p->wParam == 0x51)
		{
			RotatieOX(-5);
			Invalidate();
		}
		if (p->wParam == 0x41)
		{
			RotatieOX(5);
			Invalidate();
		}
		if (p->wParam == 0x57)
		{
			RotatieOY(-5);
			Invalidate();
		}
		if (p->wParam == 0x53)
		{
			RotatieOY(5);
			Invalidate();
		}
		if (p->wParam == 0x45)
		{
			RotatieOZ(-5);
			Invalidate();
		}
		if (p->wParam == 0x44)
		{
			RotatieOZ(5);
			Invalidate();
		}



	}
	return CDialog::PreTranslateMessage(p);
}

void CMy3DGraficaDlg::OnTimer(UINT_PTR nIDEvent)
{
	//OnKeyDown_(nCharKeyDownn);
	CRect r; GetClientRect(&r);

	if (tomoveH) {
		OnKeyDown_(39);
	}
	else
	{
		OnKeyDown_(37);
	}

	if (tomoveV) {
		OnKeyDown_(38);
	}
	else
	{
		OnKeyDown_(40);
	}

	if (vp3d[0].x < -r.Width()) tomoveH = false;
	else if (vp3d[0].x > r.Width()) tomoveH = true;

	if (vp3d[0].y < -r.Height()) tomoveV = false;
	else if (vp3d[0].y > r.Height()) tomoveV = true;

	//Invalidate();
}

HBRUSH CMy3DGraficaDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//ID_STATICTEXT

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// Return handle to our CBrush object
	hbr = CreateSolidBrush(RGB(255, 255, 255));

	return hbr;

}


void CMy3DGraficaDlg::OnKeyDown(UINT nChar, UINT, UINT)
{
	nCharKeyDownn = nChar;
}

void CMy3DGraficaDlg::OnKeyDown_(UINT nChar)
{

	//return;
	switch (nChar)
	{
	case 40:
	{
		for (int i = 0; i < vp3d.GetCount(); i++)
		{
			Translatie(vp3d[i].x, vp3d[i].y, vp3d[i].z, 0, 10, 0, vp3d[i].x, vp3d[i].y, vp3d[i].z);
		}

		Invalidate();
	}

	RotatieOY(-10);
	Invalidate();
	//if (sgn_y > 0)VY++; else VY--;		
	break;
	case 38:
	{
		for (int i = 0; i < vp3d.GetCount(); i++)
		{
			Translatie(vp3d[i].x, vp3d[i].y, vp3d[i].z, 0, -10, 0, vp3d[i].x, vp3d[i].y, vp3d[i].z);
		}

		Invalidate();
	}
	RotatieOY(10);
	Invalidate();
	//if (sgn_y > 0)VY--; else VY++;
	break;
	case 37:
	{
		for (int i = 0; i < vp3d.GetCount(); i++)
		{
			Translatie(vp3d[i].x, vp3d[i].y, vp3d[i].z, 10, 0, 0, vp3d[i].x, vp3d[i].y, vp3d[i].z);
		}

		Invalidate();
	}
	RotatieOX(10);
	Invalidate();
	//if (sgn_x > 0)VX--; else VX++;
	break;
	case 39:
	{
		for (int i = 0; i < vp3d.GetCount(); i++)
		{
			Translatie(vp3d[i].x, vp3d[i].y, vp3d[i].z, -10, 0, 0, vp3d[i].x, vp3d[i].y, vp3d[i].z);
		}

		Invalidate();
	}
	RotatieOX(-10);
	Invalidate();
	//if (sgn_x > 0)VX++; else VX--;
	break;
	}
}


