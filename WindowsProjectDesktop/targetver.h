#pragma once

// // При включении SDKDDKVer.h будет задана самая новая из доступных платформ Windows.
// Если вы планируете сборку приложения для предыдущей версии платформы Windows, включите WinSDKVer.h и
// задайте желаемую платформу в макросе _WIN32_WINNT, прежде чем включать SDKDDKVer.h.
#include <SDKDDKVer.h>

#include <windows.h>
#include <windowsx.h>
#include <tlhelp32.h>
#include <d2d1.h>

#include <tchar.h>
#include <netsh.h>
#include <commctrl.h>

#include <iostream>

using namespace std;




#pragma comment(lib, "d2d1")
