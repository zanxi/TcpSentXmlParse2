#pragma once


// CMyListCtrl

class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTCTRL_CATEGORY_GROUPS_DIALOG };
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


