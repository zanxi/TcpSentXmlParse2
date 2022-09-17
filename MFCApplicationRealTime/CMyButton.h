#pragma once
#include "afxdialogex.h"

class CMyButton : public CButton
{
public:
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnRButtonDown(UINT, CPoint);
private:
	DECLARE_MESSAGE_MAP();
};
