#include "pch.h"
#include "CMyButton.h"

void CMyButton::OnLButtonDown(UINT, CPoint)
{
	MoveWindow(10, 110, 90, 30);
}

void CMyButton::OnRButtonDown(UINT, CPoint)
{
	MoveWindow(10, 40, 90, 30);
}

BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()
