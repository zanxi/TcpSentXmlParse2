#include "MainWindow.h"
#include "Resource.h"

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}


void DPIScale::Initialize(ID2D1Factory* pFactory)
{
    FLOAT dpiX, dpiY;
    //pFactory->GetDesktopDpi(&dpiX, &dpiY);
    dpiX = (FLOAT)GetDpiForWindow(GetDesktopWindow());
    dpiY = dpiX;
    scaleX = dpiX / 96.0f;
    scaleY = dpiY / 96.0f;
}

template<typename T>
inline D2D1_POINT_2F DPIScale::PixelsToDips(T x, T y)
{
    return D2D1::Point2F(static_cast<float>(x) / scaleX, static_cast<float>(y) / scaleY);
}

float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;



//MainWindow::MainWindow(void): pFactory(NULL), pRenderTarget(NULL), pBrush(NULL),
//ellipse(D2D1::Ellipse(D2D1::Point2F(), 0, 0)),
//ptMouse(D2D1::Point2F())
//{
//}

MainWindow::MainWindow() : BaseWindow() {
    pFactory = NULL;
    pRenderTarget = NULL; 
    pBrush = NULL;
    ellipse = D2D1::Ellipse(D2D1::Point2F(), 0, 0);
    ptMouse = D2D1::Point2F();
}


void    MainWindow::CalculateLayout() {}

PCWSTR  MainWindow::ClassName() const { return L"Circle Window Class"; }

HRESULT MainWindow::CreateGraphicsResources()
{
    HRESULT hr = S_OK;
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTarget);

        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
            hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

            if (SUCCEEDED(hr))
            {
                CalculateLayout();
            }
        }
    }
    return hr;
}

void MainWindow::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}

void MainWindow::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);

        pRenderTarget->BeginDraw();

        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
        pRenderTarget->FillEllipse(ellipse, pBrush);

        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

void MainWindow::Resize()
{
    if (pRenderTarget != NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pRenderTarget->Resize(size);
        CalculateLayout();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

void MainWindow::OnLButtonDown(int pixelX, int pixelY, DWORD flags)
{
    SetCapture(m_hwnd);
    ellipse.point = ptMouse = DPIScale::PixelsToDips(pixelX, pixelY);
    ellipse.radiusX = ellipse.radiusY = 1.0f;
    InvalidateRect(m_hwnd, NULL, FALSE);
}

void MainWindow::OnMouseMove(int pixelX, int pixelY, DWORD flags)
{
    if (flags & MK_LBUTTON)
    {
        const D2D1_POINT_2F dips = DPIScale::PixelsToDips(pixelX, pixelY);

        const float width = (dips.x - ptMouse.x) / 2;
        const float height = (dips.y - ptMouse.y) / 2;
        const float x1 = ptMouse.x + width;
        const float y1 = ptMouse.y + height;

        ellipse = D2D1::Ellipse(D2D1::Point2F(x1, y1), width, height);

        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

void MainWindow::OnLButtonUp()
{
    ReleaseCapture();
}


//int WINAPI wWinMain2(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
//{
//    MainWindow win;
//
//    if (!win.Create(L"Circle", WS_OVERLAPPEDWINDOW))
//    {
//        return 0;
//    }
//
//    ShowWindow(win.Window(), nCmdShow);
//
//    // Run the message loop.
//
//    MSG msg = { };
//    while (GetMessage(&msg, NULL, 0, 0))
//    {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//
//    return 0;
//}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {

        CreateTabControl();


        //HINSTANCE hInstance = GetModuleHandle(NULL);

        //HWND hWndLeft = CreateListView(m_hwnd, hInstance, 1);
        
        //if (hWndLeft == NULL)
        //{
        //    //MessageBox(hWnd, "hWndLeft failed to create", "hWndLeft", 0);
        //}
        //HWND hWndMain = CreateListViewMain(m_hwnd, hInstance, 1);
        //if (hWndMain == NULL)
        //{
        //    //MessageBox(hWnd, "hWndMain failed to create", "hWndMain", 0);
        //}


        if (FAILED(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            return -1;  // Fail CreateWindowEx.
        }
        DPIScale::Initialize(pFactory);
        return 0;
    }

    case WM_DESTROY:
        DiscardGraphicsResources();
        SafeRelease(&pFactory);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        OnPaint();
        return 0;

    case WM_SIZE:
        Resize();
        return 0;

    case WM_LBUTTONDOWN:
        OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
        return 0;

    case WM_LBUTTONUP:
        OnLButtonUp();
        return 0;

    case WM_MOUSEMOVE:
        OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
        return 0;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void MainWindow::CreateTabControl()
{
    static HWND hButton1, hButton2, hTControl1, hTControl2;
    int wmId;
    TC_ITEM tabitem;
    static HWND d;
    static HWND tab[3];
    TCHAR tabLBL1[256];

    hTControl1 = CreateWindow(WC_TABCONTROL,
        NULL,
        WS_CHILD | WS_VISIBLE,
        0,
        0,
        940,
        428,
        m_hwnd,
        NULL,
        //hInstance, 
        GetModuleHandle(NULL),
        NULL);
    //SendMessage(hTControl1, TCM_INSERTITEM, 1, LPARAM(&tabitem));

    tabitem.mask = TCIF_TEXT;
    tabitem.iImage = 0;
    wsprintf(tabLBL1, L"TCP Client");
    tabitem.pszText = tabLBL1; //std::string("1st tab").c_str());
    SendMessage(hTControl1, TCM_INSERTITEM, 1, LPARAM(&tabitem));
    wsprintf(tabLBL1, L"TCP Server");
    tabitem.pszText = tabLBL1;
    tabitem.iImage = 1;
    SendMessage(hTControl1, TCM_INSERTITEM, 1, LPARAM(&tabitem));

    tab[0] = CreateWindow(
        L"button", 
        L"kn1", 
        WS_CHILD | WS_VISIBLE, 
        30, 
        30, 
        40, 
        68, 
        hTControl1,
        NULL, 
        GetModuleHandle(NULL),
        NULL);
    tab[1] = CreateWindow(
        L"button",
        L"kn1",
        WS_CHILD | WS_VISIBLE,
        30,
        30,
        40,
        68,
        hTControl1,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    ShowWindow(tab[0], SW_SHOW);
    
    //tab[1] = CreateWindow("button", "kn2", WS_CHILD, 30, 30, 40, 68, d, NULL, hInstance, NULL);
    //tab[2] = CreateWindow("button", "kn3", WS_CHILD, 30, 30, 40, 68, d, NULL, hInstance, NULL);

    /*CreateWindow(_T("BUTTON"), _T("Button"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 10, 70, 30, hwnd, (HMENU)101, hInstance, NULL);
    CreateWindow(CUSTOM_WC, _T("Custom"), WS_CHILD | WS_VISIBLE,
        10, 45, 70, 30, hwnd, (HMENU)100, hInstance, NULL);
    break;*/

}

//BOOL MainWindow::InsertListViewItem(HWND hWndListView)
//{
//    LVITEM lvI;
//    HANDLE hProcessSnap;
//    HANDLE hProcess;
//    PROCESSENTRY32 pe32;
//    DWORD dwPriorityClass;
//
//    // Take a snapshot of all processes in the system.
//    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//    if (hProcessSnap == INVALID_HANDLE_VALUE)
//    {
//        //printError(TEXT("CreateToolhelp32Snapshot (of processes)"));
//        return(FALSE);
//    }
//
//    // Set the size of the structure before using it.
//    pe32.dwSize = sizeof(PROCESSENTRY32);
//
//    // Retrieve information about the first process,
//    // and exit if unsuccessful
//    if (!Process32First(hProcessSnap, &pe32))
//    {
//        //printError(TEXT("Process32First")); // show cause of failure
//        CloseHandle(hProcessSnap);          // clean the snapshot object
//        return(FALSE);
//    }
//
//    // Now walk the snapshot of processes, and
//    // display information about each process in turn
//    do
//    {
//        lvI.pszText = pe32.szExeFile;
//        lvI.mask = LVIF_TEXT | LVIF_STATE;
//        lvI.iSubItem = 0;
//        lvI.state = 0;
//        lvI.stateMask = 0;
//        lvI.iItem = ListView_GetItemCount(hWndListView) + 1;
//
//        int Index = ListView_InsertItem(hWndListView, &lvI);
//        if (Index == -1)
//        {
//            MessageBox(NULL, L"Failed to insert listview item", L"InsertListViewItem()", 0);
//        }
//
//    } while (Process32Next(hProcessSnap, &pe32));
//
//    CloseHandle(hProcessSnap);
//    return true;
//}

//HWND MainWindow::CreateListView(HWND hWndParent, HINSTANCE hInst, int flags)

//HWND MainWindow::CreateListViewMain(HWND hWndParent, HINSTANCE hInst, int flags)
//{
//    INITCOMMONCONTROLSEX icex;
//    icex.dwICC = ICC_LISTVIEW_CLASSES;
//    InitCommonControlsEx(&icex);
//    HWND hWndListView = NULL;
//
//    RECT rcClient;
//    GetClientRect(hWndParent, &rcClient);
//
//    if (flags == 1)
//    {
//        hWndListView = CreateWindow(
//            WC_LISTVIEW, 
//            L"listviewmain", 
//            WS_CHILD | WS_BORDER | LVS_LIST | LVS_EDITLABELS | WS_VISIBLE | CS_HREDRAW | CS_VREDRAW,
//            (rcClient.right / 3), 
//            0, 
//            (rcClient.right - (rcClient.right / 3) - 0.2), 
//            (rcClient.bottom - rcClient.top) - 20, 
//            hWndParent, 
//            (HMENU)IDC_WINDOWSPROJECTDESKTOP, 
//            hInst, 
//            NULL);
//        //ListView_SetBkColor(hWndListView, BACKGROUND_BLUE);
//    }
//    else if (flags == 2)
//    {
//        hWndListView = CreateWindow(
//            WC_LISTVIEW, 
//            L"listviewmain", 
//            WS_CHILD | WS_BORDER | LVS_LIST | LVS_EDITLABELS | WS_VISIBLE | CS_HREDRAW | CS_VREDRAW,
//            (rcClient.right / 6), 
//            0, 
//            (rcClient.right - (rcClient.right / 6)), 
//            (rcClient.bottom - rcClient.top), 
//            hWndParent, 
//            (HMENU)IDC_WINDOWSPROJECTDESKTOP, 
//            hInst, 
//            NULL);
//        //ListView_SetBkColor(hWndListView, BACKGROUND_BLUE);
//    }
//    else
//    {
//        MessageBox(hWndParent, L"Wrong flag in CreateListViewMain()!", L"CreateListViewMain()", NULL);
//        hWndListView = NULL;
//    }
//
//    if (!hWndListView)
//    {
//        MessageBox(NULL,
//            _T("Call to CreateWindow failed!"),
//            _T("Win32 Guided Tour"),
//            NULL);
//
//        hWndListView = NULL;
//    }
//
//    return hWndListView;
//}
