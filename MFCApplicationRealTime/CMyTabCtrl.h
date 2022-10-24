#pragma once


// CMyTabCtrl

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:

	static const int numTabs = 6;
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

	CDialog* m_tabPages[numTabs];
	int m_tabCurrent;
	int m_nNumberOfPages;

protected:
	DECLARE_MESSAGE_MAP()
public:
	void Init(CRect MainRect);
	int SetRectangle(CRect MainRect);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};


