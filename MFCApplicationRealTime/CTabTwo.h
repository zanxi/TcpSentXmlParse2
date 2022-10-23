#pragma once
#include "afxdialogex.h"


// Диалоговое окно CTabTwo

#include "TColorText.h"

#define IDR_MENU1                       101
#define IDC_MYBUTTON 100 // идентификатор кнопки
#define IDC_MYEDIT 102   // идентификатор поля редактирования
#define ID_TIMER_FOR_MOTION 104   // идентификатор таймера
#define ID_STATICTEXT 105   // идентификатор Static
#define IDS_STRING_X                    102
#define IDS_STRING_Y                    103

#define IDR_MENU_EXIT                   40004
#define ID_START_START                  40005
#define ID_STOP_STOP                    40006
#define ID_MOTION_START                 40007
#define ID_MOTION_STOP                  40008
#define ID_STARTTIMER_STARTTIMER        40009
#define ID_TIMER_FOR_MOT                40010
#define ID_EXIT_EXIT                    40011
#define ID_MOTION_EXIT                  40012




static UINT indicators[] = {
	IDS_STRING_X,
	IDS_STRING_Y
};


class CTabTwo : public CDialog
{
	DECLARE_DYNAMIC(CTabTwo)

public:
	CTabTwo(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CTabTwo();

	int OnCreate(LPCREATESTRUCT lpCreateStruct);	
private:
	//CStatic* MyStatic;
	HBRUSH m_brush;
	HBRUSH m_BrushBlack;
	
	TColorText* MyStatic;
	CButton* MyButton;
	CEdit* MyEdit;
	CStatusBar m_wndStatusBar;
	CMenu m_wndMenu;
	//DECLARE_MESSAGE_MAP()
	//CStatus

private:
	int VX = 100, sgn_x = 1;
	int VY = 100, sgn_y = 1;
	CRect newPlace;
	CRect oldPlace;

	//	DECLARE_MESSAGE_MAP();
protected:
	//afx_msg void OnContextMenu(CWnd*, CPoint point);
	HICON m_hIcon;

	// Generated message map functions	
	virtual BOOL OnInitDialog();
	
	void SetPosContr();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void MenuExit();
	afx_msg void MenuStart();
	afx_msg void MenuStop();
	afx_msg void OnTimer(UINT_PTR);
	int m_nTimer;
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT, UINT, UINT);
	afx_msg void OnMouseMove(UINT, CPoint cp);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void PaintGraphCoordSystem();
	void PaintGraph();
	double x, y, xstep, ystep;
	double xmin, xmax, ymin, ymax;
	int xc = 50;
	int yc = 50;
	int H;
	int W;
	void func_plot_init();


// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_TWO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
};

