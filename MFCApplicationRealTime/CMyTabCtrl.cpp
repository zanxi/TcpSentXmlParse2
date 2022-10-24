// CMyTabCtrl.cpp: файл реализации
//

#include "pch.h"
#include "MFCApplicationRealTime.h"
#include "CMyTabCtrl.h"
#include "CTabOne.h"
#include "AppServiceDlg.h"
#include "CTabTwo.h"
#include "ServerSocketDlg.h"
#include "ClientSocketDlg.h"
#include "Dynamic3d.h"
#include "OpenGL_MFCDialogDlg.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{
    m_tabPages[0] = new CTabOne2();
    m_tabPages[1] = new CTabTwo();
    m_tabPages[2] = new CServerSocketDlg();
    m_tabPages[3] = new CClientSocketDlg();
    m_tabPages[4] = new CMy3DGraficaDlg();
    m_tabPages[5] = new COpenGL_MFCDialogDlg();
    m_nNumberOfPages = 6;

    //Init();

}

CMyTabCtrl::~CMyTabCtrl()
{
    for (int nCount = 1; nCount < m_nNumberOfPages; nCount++)
    {
        delete m_tabPages[nCount];
    }
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
    ON_WM_LBUTTONDOWN()    
END_MESSAGE_MAP()



// Обработчики сообщений CMyTabCtrl




void CMyTabCtrl::Init(CRect MainRect)
{
    
    
    m_tabCurrent = 0;

    //return;

    m_tabPages[0]->Create(IDD_TAB_ONE, this);
    m_tabPages[1]->Create(IDD_TAB_TWO, this);
    m_tabPages[2]->Create(IDD_TAB_THREE_SERVER, this);
    m_tabPages[3]->Create(IDD_TAB_FOTH_CLIENT, this);
    m_tabPages[4]->Create(IDD_TAB_FIVE__2d, this);
    m_tabPages[5]->Create(IDD_TAB_6_opengl, this);

    m_tabPages[0]->ShowWindow(SW_SHOW);
    m_tabPages[1]->ShowWindow(SW_HIDE);
    m_tabPages[2]->ShowWindow(SW_HIDE);
    m_tabPages[3]->ShowWindow(SW_HIDE);
    m_tabPages[4]->ShowWindow(SW_HIDE);
    m_tabPages[5]->ShowWindow(SW_HIDE);

    SetRectangle(MainRect);

}


int CMyTabCtrl::SetRectangle(CRect tabRect)
{
    CRect //tabRect
        itemRect;
    int nX, nY, nXc, nYc;

    //GetClientRect(&tabRect);
    GetItemRect(0, &itemRect);

    nX = itemRect.left;
    nY = itemRect.bottom + 1;
    nXc = tabRect.right;// - itemRect.left - 1;
    nYc = tabRect.bottom;// -nY - 1;

    m_tabPages[0]->SetWindowPos(&wndTop, 
        nX, nY,
        tabRect.Width(), tabRect.Height(),
        //nX, nY, nXc, nYc, 
        SWP_SHOWWINDOW);
    //m_tabPages[1]->ShowWindow(SW_SHOW);
    for (int nCount = 1; nCount < m_nNumberOfPages; nCount++)
    {
         m_tabPages[nCount] -> 
             SetWindowPos(&wndTop, 
                 //nX, nY, nXc, nYc, 
                 nX,nY,
                 tabRect.Width(), tabRect.Height(),
                 SWP_HIDEWINDOW);
         //if(nCount!=1)m_tabPages[nCount]->ShowWindow(SW_HIDE);
         //m_tabPages[nCount]->MoveWindow(1, 1, tabRect.Width(), tabRect.Height());
         //CRect rectCtrl;
          //pCtrl->GetWindowRect(rectCtrl);
         //MoveWindow(rectCtrl.left, rectCtrl.top, rectCtrl.Width(), rectCtrl.Height());
    }

    //m_tabPages[1]->MoveWindow(1, 1, tabRect.Width()*2, tabRect.Height()*2);
    MoveWindow(nX, nY, tabRect.Width(), tabRect.Height());

    //(CTabOne2)m_tabPages[0]


    return 0;
}


void CMyTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: добавьте свой код обработчика сообщений или вызов стандартного

    CTabCtrl::OnLButtonDown(nFlags, point);

    if (m_tabCurrent != GetCurFocus())
    {
        m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
        m_tabCurrent = GetCurFocus();

        m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
        m_tabPages[m_tabCurrent]->SetFocus();
        if (m_tabCurrent == 0)
        {
            ((CTabOne2*)m_tabPages[0])->m_List.Invalidate();
            ((CTabOne2*)m_tabPages[0])->m_List.UpdateWindow();
        }
    }

}

void CMyTabCtrl::OnPaint()
{
    return;
    CTabCtrl::OnPaint();
    //CClientDC dc(this);
    if (m_tabPages == NULL)return;

    CRect rectCtrl;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rectCtrl, 0);
    
    //((CTabOne2*)m_tabPages[0])->m_List.MoveWindow(10, 10, (int)(rectCtrl.Width() / 1.1), rectCtrl.Height());
    ((CTabOne2*)m_tabPages[0])->m_List.Invalidate();
    ((CTabOne2*)m_tabPages[0])->m_List.UpdateWindow();
    m_tabPages[0]->Invalidate();
    m_tabPages[0]->UpdateWindow();
}
