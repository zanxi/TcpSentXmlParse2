#pragma once
#include "afxdialogex.h"


// Диалоговое окно Dynamic3d

class Dynamic3d : public CDialog
{
	DECLARE_DYNAMIC(Dynamic3d)

public:
	Dynamic3d(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~Dynamic3d();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_FIVE__2d };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};


// 3DGraficaDlg.h : header file
//

#pragma once
#include "afxtempl.h"
struct Punct3D
{
	double x, y, z;
};

struct Unire
{
	int i1, i2;
};

// CMy3DGraficaDlg dialog
class CMy3DGraficaDlg : public CDialogEx
{
	// Construction
public:
	CMy3DGraficaDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TAB_FIVE__2d };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	CArray<Punct3D> vp3d;
	CArray<Unire> vu;
	CArray<CPoint> vp2d;
	Punct3D Punct3(double x, double y, double z);
	Unire Unirii(int i1, int i2);
	CPoint Pp(Punct3D p, double dx, double dy);
	double zp;
	void Translatie(double x, double y, double z, double dx, double dy, double dz, double& xp, double& yp, double& zp);
	void Scalare(double x, double y, double z, double xg, double yg, double zg, double sx, double sy, double sz, double& xs, double& ys, double& zs);
	void RotatieOX(double u);
	void RotatieOY(double u);
	void RotatieOZ(double u);

	BOOL PreTranslateMessage(MSG* p);
	void OnTimer(UINT_PTR nIDEvent);
	void OnKeyDown(UINT nChar, UINT, UINT);
	void OnKeyDown_(UINT nChar);
	UINT nCharKeyDownn;
	bool tomoveH;
	bool tomoveV;

};

