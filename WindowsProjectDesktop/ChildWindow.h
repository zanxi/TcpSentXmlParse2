#include "targetver.h"
#include "Resource.h"
#pragma once

#define MAX_LOADSTRING 100

template <class DERIVED_TYPE>
class ChildWindow
{

    WCHAR szWindowClass1[MAX_LOADSTRING];

public:
    static LRESULT CALLBACK ChildProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE* pThis = NULL;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    //BaseWindow();
    ChildWindow() : m_hwnd(NULL) { }

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        
        WNDCLASSEXW wcex1;
        wcex1.cbSize = sizeof(WNDCLASSEX);
        wcex1.style = CS_HREDRAW | CS_VREDRAW;
        wcex1.lpfnWndProc = (WNDPROC)ChildProc;
        wcex1.cbClsExtra = 0;
        wcex1.cbWndExtra = 0;
        //wcex1.hInstance = hInstance;
        wcex1.hInstance = GetModuleHandle(NULL);
        //wcex1.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINPR_LAB02));
        //wcex1.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECTDESKTOP));
        wcex1.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_WINDOWSPROJECTDESKTOP));
        wcex1.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex1.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
        wcex1.lpszMenuName = nullptr;
        wcex1.lpszClassName = szWindowClass1;
        wcex1.hIconSm = LoadIcon(wcex1.hInstance, MAKEINTRESOURCE(IDI_SMALL));
        return RegisterClassExW(&wcex1);


        m_hwnd = CreateWindowEx(
            dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
        );

        return (m_hwnd ? TRUE : FALSE);
    }


    HWND Window() const { return m_hwnd; }

protected:

    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
    HWND m_child_hwnd;
};



