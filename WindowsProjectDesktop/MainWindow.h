#pragma once

#include "basewin.h"
#include "Resource.h"


//int WINAPI wWinMain2(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow);

template <class T> void SafeRelease(T** ppT);

class DPIScale
{
    static float scaleX;
    static float scaleY;

public:
    static void Initialize(ID2D1Factory* pFactory);

    template <typename T>
    static D2D1_POINT_2F PixelsToDips(T x, T y);
};


class MainWindow : public BaseWindow<MainWindow>
{
    ID2D1Factory* pFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    ID2D1SolidColorBrush* pBrush;
    D2D1_ELLIPSE            ellipse;
    D2D1_POINT_2F           ptMouse;

    void    CalculateLayout();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint();
    void    Resize();
    void    OnLButtonDown(int pixelX, int pixelY, DWORD flags);
    void    OnLButtonUp();
    void    OnMouseMove(int pixelX, int pixelY, DWORD flags);

public:
    void    CreateTabControl();
    //HWND    CreateListViewMain(HWND hWndParent, HINSTANCE hInst, int flags);
    HWND    CreateListView(HWND hWndParent, HINSTANCE hInst, int flags)
    {
        INITCOMMONCONTROLSEX icex;
        icex.dwICC = ICC_LISTVIEW_CLASSES;
        InitCommonControlsEx(&icex);
        HWND hWndListView = NULL;

        RECT rcClient;
        GetClientRect(hWndParent, &rcClient);

        if (flags == 1) {
            hWndListView = CreateWindow(WC_LISTVIEW,
                L"listviewleft",
                WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_LIST | LVS_EDITLABELS | CS_HREDRAW | CS_VREDRAW,
                0, 0,
                (rcClient.right / 3),
                rcClient.bottom - 20,
                hWndParent,
                (HMENU)IDC_WINDOWSPROJECTDESKTOP,
                hInst,
                NULL);
            //	ListView_SetBkColor(hWndListView, BACKGROUND_BLUE);
        }
        else if (flags == 2)
        {
            hWndListView = CreateWindow(
                WC_LISTVIEW,
                L"listviewleft",
                WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_LIST | LVS_EDITLABELS | CS_HREDRAW | CS_VREDRAW,
                0,
                0,
                (rcClient.right / 6),
                rcClient.bottom,
                hWndParent,
                (HMENU)IDC_WINDOWSPROJECTDESKTOP,
                hInst,
                NULL);
            //ListView_SetBkColor(hWndListView, BACKGROUND_BLUE);
        }
        else
        {
            MessageBox(hWndParent, L"Wrong flag in CreateListViewLeft()!", L"CreateListViewLeft()", NULL);
            hWndListView = NULL;
        }

        if (!hWndListView)
        {
            MessageBox(NULL,
                _T("Call to CreateWindow failed!"),
                _T("Win32 Guided Tour"),
                NULL);

            hWndListView = NULL;
        }

        return hWndListView;
    }

    BOOL    InsertListViewItem(HWND hWndListView);

public:

    MainWindow();
    PCWSTR  ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

