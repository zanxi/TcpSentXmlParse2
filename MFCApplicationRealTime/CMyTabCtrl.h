#pragma once


// CMyTabCtrl

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

	CDialog* m_tabPages[5];
	int m_tabCurrent;
	int m_nNumberOfPages;

protected:
	DECLARE_MESSAGE_MAP()
public:
	void Init();
	int SetRectangle();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};


