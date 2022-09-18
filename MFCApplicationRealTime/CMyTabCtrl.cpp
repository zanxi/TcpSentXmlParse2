// CMyTabCtrl.cpp: файл реализации
//

#include "pch.h"
#include "MFCApplicationRealTime.h"
#include "CMyTabCtrl.h"
#include "CTabOne.h"
#include "AppServiceDlg.h"
#include "CTabTwo.h"
#include "ServerSocketDlg.h"


// CMyTabCtrl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{
    m_tabPages[0] = new CTabOne2();
    m_tabPages[1] = new CTabTwo();
    m_tabPages[2] = new CServerSocketDlg();
    m_nNumberOfPages = 3;

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




void CMyTabCtrl::Init()
{
    
    m_tabCurrent = 0;

    //return;

    m_tabPages[0]->Create(IDD_TAB_ONE, this);
    m_tabPages[1]->Create(IDD_TAB_TWO, this);
    m_tabPages[2]->Create(IDD_TAB_THREE_SERVER, this);

    m_tabPages[0]->ShowWindow(SW_SHOW);
    m_tabPages[1]->ShowWindow(SW_HIDE);
    m_tabPages[2]->ShowWindow(SW_HIDE);

    SetRectangle();

}


int CMyTabCtrl::SetRectangle()
{
    CRect tabRect, itemRect;
    int nX, nY, nXc, nYc;

    GetClientRect(&tabRect);
    GetItemRect(0, &itemRect);

    nX = itemRect.left;
    nY = itemRect.bottom + 1;
    nXc = tabRect.right - itemRect.left - 1;
    nYc = tabRect.bottom-nY-1;

    m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
    for (int nCount = 1; nCount < m_nNumberOfPages; nCount++)
    {
         m_tabPages[nCount] -> SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
    }

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
    }

}

void CMyTabCtrl::OnPaint()
{
    return;
    CTabCtrl::OnPaint();
    //CClientDC dc(this);
    if (m_tabPages == NULL)return;
    m_tabPages[0]->Invalidate();
    m_tabPages[0]->UpdateWindow();
}
