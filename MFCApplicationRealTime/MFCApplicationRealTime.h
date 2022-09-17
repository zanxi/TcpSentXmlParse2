
// MFCApplicationRealTime.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMFCApplicationRealTimeApp:
// Сведения о реализации этого класса: MFCApplicationRealTime.cpp
//

class CMFCApplicationRealTimeApp : public CWinApp
{
public:
	CMFCApplicationRealTimeApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplicationRealTimeApp theApp;
