//#include "stdafx.h"
#include "CListCtrl_Category_Groups.h"

#include <shlwapi.h>
#include "Resource.h"
//#include "ListCtrl_Category_GroupsDef.h"

BEGIN_MESSAGE_MAP(CListCtrl_Category_Groups, CListCtrl)
	ON_WM_CONTEXTMENU()	// OnContextMenu
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnHeaderClick)	// Column Click
#if _WIN32_WINNT >= 0x0600
	ON_NOTIFY_REFLECT_EX(LVN_LINKCLICK, OnGroupTaskClick)	// Column Click
#endif
END_MESSAGE_MAP()

LRESULT CListCtrl_Category_Groups::InsertGroupHeader(int nIndex, int nGroupId, const CString& strHeader, DWORD dwState /* = LVGS_NORMAL */, DWORD dwAlign /*= LVGA_HEADER_LEFT*/)
{
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.iGroupId = nGroupId;
	lg.state = dwState;
	lg.mask = LVGF_GROUPID | LVGF_HEADER | LVGF_STATE | LVGF_ALIGN;
	lg.uAlign = dwAlign;

	// Header-title must be unicode (Convert if necessary)
#ifdef UNICODE
	lg.pszHeader = (LPWSTR)(LPCTSTR)strHeader;
	lg.cchHeader = strHeader.GetLength();
#else
	CComBSTR header = strHeader;
	lg.pszHeader = header;
	lg.cchHeader = header.Length();
#endif

	return InsertGroup(nIndex, (PLVGROUP)&lg );
}

BOOL CListCtrl_Category_Groups::SetRowGroupId(int nRow, int nGroupId)
{
	//OBS! Rows not assigned to a group will not show in group-view
	LVITEM lvItem = {0};
	lvItem.mask = LVIF_GROUPID;
	lvItem.iItem = nRow;
	lvItem.iSubItem = 0;
	lvItem.iGroupId = nGroupId;
	return SetItem( &lvItem );
}

int CListCtrl_Category_Groups::GetRowGroupId(int nRow)
{
	LVITEM lvi = {0};
    lvi.mask = LVIF_GROUPID;
    lvi.iItem = nRow;
	VERIFY( GetItem(&lvi) );
    return lvi.iGroupId;
}

CString CListCtrl_Category_Groups::GetGroupHeader(int nGroupId)
{
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.iGroupId = nGroupId;
	lg.mask = LVGF_HEADER | LVGF_GROUPID;
	VERIFY( GetGroupInfo(nGroupId, (PLVGROUP)&lg) != -1 );

#ifdef UNICODE
	return lg.pszHeader;
#else
	CComBSTR header( lg.pszHeader );
	return (LPCTSTR)COLE2T(header);
#endif
}

BOOL CListCtrl_Category_Groups::IsGroupStateEnabled()
{
	if (!IsGroupViewEnabled())
		return FALSE;

	OSVERSIONINFO osver = {0};
	osver.dwOSVersionInfoSize = sizeof(osver);
	GetVersionEx(&osver);
	WORD fullver = MAKEWORD(osver.dwMinorVersion, osver.dwMajorVersion);
	if (fullver < 0x0600)
		return FALSE;

	return TRUE;
}

// Vista SDK - ListView_GetGroupState / LVM_GETGROUPSTATE
BOOL CListCtrl_Category_Groups::HasGroupState(int nGroupId, DWORD dwState)
{
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_STATE;
	lg.stateMask = dwState;
	if ( GetGroupInfo(nGroupId, (PLVGROUP)&lg) == -1)
		return FALSE;

	return lg.state==dwState;
}

// Vista SDK - ListView_SetGroupState / LVM_SETGROUPINFO
BOOL CListCtrl_Category_Groups::SetGroupState(int nGroupId, DWORD dwState)
{
	if (!IsGroupStateEnabled())
		return FALSE;

	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_STATE;
	lg.state = dwState;
	lg.stateMask = dwState;

#ifdef LVGS_COLLAPSIBLE
	// Maintain LVGS_COLLAPSIBLE state
	if (HasGroupState(nGroupId, LVGS_COLLAPSIBLE))
		lg.state |= LVGS_COLLAPSIBLE;
#endif

	if (SetGroupInfo(nGroupId, (PLVGROUP)&lg)==-1)
		return FALSE;

	return TRUE;
}

int CListCtrl_Category_Groups::GroupHitTest(const CPoint& point)
{
	if (!IsGroupViewEnabled())
		return -1;

	if (HitTest(point)!=-1)
		return -1;

	if (IsGroupStateEnabled())
	{
		// Running on Vista or newer, but compiled without _WIN32_WINNT >= 0x0600
#ifndef LVM_GETGROUPINFOBYINDEX
#define LVM_GETGROUPINFOBYINDEX   (LVM_FIRST + 153)
#endif
#ifndef LVM_GETGROUPCOUNT
#define LVM_GETGROUPCOUNT         (LVM_FIRST + 152)
#endif
#ifndef LVM_GETGROUPRECT
#define LVM_GETGROUPRECT          (LVM_FIRST + 98)
#endif
#ifndef LVGGR_HEADER
#define LVGGR_HEADER		      (1)
#endif

		LRESULT groupCount = SNDMSG((m_hWnd), LVM_GETGROUPCOUNT, (WPARAM)0, (LPARAM)0);
		if (groupCount <= 0)
			return -1;
		for(int i = 0 ; i < groupCount; ++i)
		{
			LVGROUP lg = {0};
			lg.cbSize = sizeof(lg);
			lg.mask = LVGF_GROUPID;

			VERIFY( SNDMSG((m_hWnd), LVM_GETGROUPINFOBYINDEX, (WPARAM)(i), (LPARAM)(&lg)) );

			CRect rect(0,LVGGR_HEADER,0,0);
			VERIFY( SNDMSG((m_hWnd), LVM_GETGROUPRECT, (WPARAM)(lg.iGroupId), (LPARAM)(RECT*)(&rect)) );

			if (rect.PtInRect(point))
				return lg.iGroupId;
		}
		// Don't try other ways to find the group
		return -1;
	}

	// We require that each group contains atleast one item
	if (GetItemCount()==0)
		return -1;

	// This logic doesn't support collapsible groups
	int nFirstRow = -1;
	CRect gridRect;
	GetWindowRect(&gridRect);
	for(CPoint pt = point ; pt.y < gridRect.bottom ; pt.y += 2)
	{
		nFirstRow = HitTest(pt);
		if (nFirstRow!=-1)
			break;
	}

	if (nFirstRow==-1)
		return -1;

	int nGroupId = GetRowGroupId(nFirstRow);

	// Extra validation that the above row belongs to a different group
	int nAboveRow = GetNextItem(nFirstRow,LVNI_ABOVE);
	if (nAboveRow!=-1 && nGroupId==GetRowGroupId(nAboveRow))
		return -1;

	return nGroupId;
}

void CListCtrl_Category_Groups::CheckEntireGroup(int nGroupId, bool bChecked)
{
	if (!(GetExtendedStyle() & LVS_EX_CHECKBOXES))
		return;

	for (int nRow=0; nRow<GetItemCount(); ++nRow)
	{
		if (GetRowGroupId(nRow) == nGroupId)
		{
			SetCheck(nRow, bChecked ? TRUE : FALSE);
		}
	}
}

void CListCtrl_Category_Groups::DeleteEntireGroup(int nGroupId)
{
	for (int nRow=0; nRow<GetItemCount(); ++nRow)
	{
		if (GetRowGroupId(nRow) == nGroupId)
		{
			DeleteItem(nRow);
			nRow--;
		}
	}
	RemoveGroup(nGroupId);
}

namespace {
	LRESULT EnableWindowTheme(HWND hwnd, LPCWSTR classList, LPCWSTR subApp, LPCWSTR idlist)
	{
		LRESULT lResult = S_FALSE;
	
		HRESULT (__stdcall *pSetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
		HANDLE (__stdcall *pOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
		HRESULT (__stdcall *pCloseThemeData)(HANDLE hTheme);

		HMODULE hinstDll = ::LoadLibrary(_T("UxTheme.dll"));
		if (hinstDll)
		{
			(FARPROC&)pOpenThemeData = ::GetProcAddress(hinstDll, "OpenThemeData");
			(FARPROC&)pCloseThemeData = ::GetProcAddress(hinstDll, "CloseThemeData");
			(FARPROC&)pSetWindowTheme = ::GetProcAddress(hinstDll, "SetWindowTheme");
			if (pSetWindowTheme && pOpenThemeData && pCloseThemeData)
			{
				HANDLE theme = pOpenThemeData(hwnd,classList);
				if (theme!=NULL)
				{
					VERIFY(pCloseThemeData(theme)==S_OK);
					lResult = pSetWindowTheme(hwnd, subApp, idlist);
				}
			}
			::FreeLibrary(hinstDll);
		}
		return lResult;
	}

	bool IsCommonControlsEnabled()
	{
		bool commoncontrols = false;
	
		// Test if application has access to common controls
		HMODULE hinstDll = ::LoadLibrary(_T("comctl32.dll"));
		if (hinstDll)
		{
			DLLGETVERSIONPROC pDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hinstDll, "DllGetVersion");
			if (pDllGetVersion != NULL)
			{
				DLLVERSIONINFO dvi = {0};
				dvi.cbSize = sizeof(dvi);
				HRESULT hRes = pDllGetVersion ((DLLVERSIONINFO *) &dvi);
				if (SUCCEEDED(hRes))
					commoncontrols = dvi.dwMajorVersion >= 6;
			}
			::FreeLibrary(hinstDll);
		}
		return commoncontrols;
	}

	bool IsThemeEnabled()
	{
		bool XPStyle = false;
		bool (__stdcall *pIsAppThemed)();
		bool (__stdcall *pIsThemeActive)();

		// Test if operating system has themes enabled
		HMODULE hinstDll = ::LoadLibrary(_T("UxTheme.dll"));
		if (hinstDll)
		{
			(FARPROC&)pIsAppThemed = ::GetProcAddress(hinstDll, "IsAppThemed");
			(FARPROC&)pIsThemeActive = ::GetProcAddress(hinstDll,"IsThemeActive");
			if (pIsAppThemed != NULL && pIsThemeActive != NULL)
			{
				if (pIsAppThemed() && pIsThemeActive())
				{
					// Test if application has themes enabled by loading the proper DLL
					XPStyle = IsCommonControlsEnabled();
				}
			}
			::FreeLibrary(hinstDll);
		}
		return XPStyle;
	}
}


BOOL CListCtrl_Category_Groups::GroupByColumn(int nCol)
{
	if (!IsCommonControlsEnabled())
		return FALSE;

	CWaitCursor waitCursor;

	SetSortArrow(-1, false);

	SetRedraw(FALSE);

	RemoveAllGroups();

	EnableGroupView( GetItemCount() > 0 );

	if (IsGroupViewEnabled())
	{
		CSimpleMap<CString,CSimpleArray<int> > groups;

		// Loop through all rows and find possible groups
		for(int nRow=0; nRow<GetItemCount(); ++nRow)
		{
			CString cellText = GetItemText(nRow, nCol);

			int nGroupId = groups.FindKey(cellText);
			if (nGroupId==-1)
			{
				CSimpleArray<int> rows;
				groups.Add(cellText, rows);
				nGroupId = groups.FindKey(cellText);
			}
			groups.GetValueAt(nGroupId).Add(nRow);
		}

		// Look through all groups and assign rows to group
		for(int nGroupId = 0; nGroupId < groups.GetSize(); ++nGroupId)
		{
			const CSimpleArray<int>& groupRows = groups.GetValueAt(nGroupId);
			DWORD dwState = LVGS_NORMAL;

#ifdef LVGS_COLLAPSIBLE
			if (IsGroupStateEnabled())
				dwState = LVGS_COLLAPSIBLE;
#endif

			VERIFY( InsertGroupHeader(nGroupId, nGroupId, groups.GetKeyAt(nGroupId), dwState) != -1);
			SetGroupTask(nGroupId, _T("Task: Check Group"));
			CString subtitle;
			subtitle.Format(_T("Subtitle: %i rows"), groupRows.GetSize());
			SetGroupSubtitle(nGroupId, subtitle );
			SetGroupFooter(nGroupId, _T("Group Footer"));
			
			for(int groupRow = 0; groupRow < groupRows.GetSize(); ++groupRow)
			{
				VERIFY( SetRowGroupId(groupRows[groupRow], nGroupId) );
			}
		}
		SetRedraw(TRUE);
		Invalidate(FALSE);
		return TRUE;
	}

	SetRedraw(TRUE);
	Invalidate(FALSE);
	return FALSE;
}

void CListCtrl_Category_Groups::CollapseAllGroups()
{
	if (!IsGroupStateEnabled())
		return;

	// Loop through all rows and find possible groups
	for(int nRow=0; nRow<GetItemCount(); ++nRow)
	{
		int nGroupId = GetRowGroupId(nRow);
		if (nGroupId!=-1)
		{
			if (!HasGroupState(nGroupId,LVGS_COLLAPSED))
			{
				SetGroupState(nGroupId,LVGS_COLLAPSED);
			}
		}
	}
}

void CListCtrl_Category_Groups::ExpandAllGroups()
{
	if (!IsGroupStateEnabled())
		return;

	// Loop through all rows and find possible groups
	for(int nRow=0; nRow<GetItemCount(); ++nRow)
	{
		int nGroupId = GetRowGroupId(nRow);
		if (nGroupId!=-1)
		{
			if (HasGroupState(nGroupId,LVGS_COLLAPSED))
			{
				SetGroupState(nGroupId,LVGS_NORMAL);
			}
		}
	}
}

void CListCtrl_Category_Groups::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (pWnd==GetHeaderCtrl())
	{
		CPoint pt = point;
		ScreenToClient(&pt);

		HDHITTESTINFO hdhti = {0};
		hdhti.pt = pt;
		hdhti.pt.x += GetScrollPos(SB_HORZ);
		::SendMessage(GetHeaderCtrl()->GetSafeHwnd(), HDM_HITTEST, 0, (LPARAM) &hdhti);
		if (hdhti.iItem!=-1)
		{
			// Retrieve column-title
			LVCOLUMN lvc = {0};
			lvc.mask = LVCF_TEXT;
			TCHAR sColText[256];
			lvc.pszText = sColText;
			lvc.cchTextMax = sizeof(sColText)-1;
			VERIFY( GetColumn(hdhti.iItem, &lvc) );

			CMenu menu;
			UINT uFlags = MF_BYPOSITION | MF_STRING;
			VERIFY( menu.CreatePopupMenu() );
			menu.InsertMenu(0, uFlags, 1, CString(_T("Group by: ")) + lvc.pszText);
			if (IsGroupViewEnabled())
				menu.InsertMenu(1, uFlags, 2, _T("Disable grouping"));
			int nResult = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, point.x, point.y, this, 0);
			switch(nResult)
			{
				case 1:	GroupByColumn(hdhti.iItem); break;
				case 2: RemoveAllGroups(); EnableGroupView(FALSE); break;
			}
		}
		return;
	}

	if ( IsGroupViewEnabled() )
	{
		if (point.x!=-1 && point.y!=-1)
		{
			CMenu menu;
			UINT uFlags = MF_BYPOSITION | MF_STRING;
			VERIFY( menu.CreatePopupMenu() );
		
			CPoint pt = point;
			ScreenToClient(&pt);
			int nGroupId = GroupHitTest(pt);
			if (nGroupId >= 0)
			{
				const CString& groupHeader = GetGroupHeader(nGroupId);

#ifndef LVGS_COLLAPSIBLE
				if (IsGroupStateEnabled())
				{
					if (HasGroupState(nGroupId,LVGS_COLLAPSED))
					{
						CString menuText = CString(_T("Expand group: ")) + groupHeader;
						menu.InsertMenu(0, uFlags, 1, menuText);
					}
					else
					{
						CString menuText = CString(_T("Collapse group: ")) + groupHeader;
						menu.InsertMenu(0, uFlags, 2, menuText);
					}
				}
#endif
				CString menuText = CString(_T("Check group: ")) + groupHeader;
				menu.InsertMenu(1, uFlags, 3, menuText);
				menuText = CString(_T("Uncheck group: ")) + groupHeader;
				menu.InsertMenu(2, uFlags, 4, menuText);
				menuText = CString(_T("Delete group: ")) + groupHeader;
				menu.InsertMenu(3, uFlags, 5, menuText);
			
				menu.InsertMenu(4, uFlags | MF_SEPARATOR, 6, _T(""));
			}

			int nRow = HitTest(pt);
			if (nRow==-1)
			{
				if (IsGroupStateEnabled())
				{
					menu.InsertMenu(5, uFlags, 7, _T("Expand all groups"));
					menu.InsertMenu(6, uFlags, 8, _T("Collapse all groups"));
				}
				menu.InsertMenu(7, uFlags, 9, _T("Disable grouping"));
			}

			int nResult = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, point.x, point.y, this, 0);
			switch(nResult)
			{
				case 1: SetGroupState(nGroupId,LVGS_NORMAL); break;
				case 2: SetGroupState(nGroupId,LVGS_COLLAPSED); break;
				case 3: CheckEntireGroup(nGroupId, true); break;
				case 4: CheckEntireGroup(nGroupId, false); break;
				case 5: DeleteEntireGroup(nGroupId); break;
				case 7: ExpandAllGroups(); break;
				case 8: CollapseAllGroups(); break;
				case 9: RemoveAllGroups(); EnableGroupView(FALSE); break;
			}
		}
	}
}

namespace {
	struct PARAMSORT
	{
		PARAMSORT(HWND hWnd, int nCol, bool bAscending)
			:m_hWnd(hWnd)
			,m_ColumnIndex(nCol)
			,m_Ascending(bAscending)
		{}

		HWND m_hWnd;
		int  m_ColumnIndex;
		bool m_Ascending;
		CSimpleMap<int,CString> m_GroupNames;

		const CString& LookupGroupName(int nGroupId)
		{
			int groupIdx = m_GroupNames.FindKey(nGroupId);
			if (groupIdx==-1)
			{
				static const CString emptyStr;
				return emptyStr;
			}			
			return m_GroupNames.GetValueAt(groupIdx);
		}
	};

	// Comparison extracts values from the List-Control
	int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		PARAMSORT& ps = *(PARAMSORT*)lParamSort;

		TCHAR left[256] = _T(""), right[256] = _T("");
		ListView_GetItemText(ps.m_hWnd, lParam1, ps.m_ColumnIndex, left, sizeof(left));
		ListView_GetItemText(ps.m_hWnd, lParam2, ps.m_ColumnIndex, right, sizeof(right));

		if (ps.m_Ascending)
			return _tcscmp( left, right );
		else
			return _tcscmp( right, left );			
	}

	int CALLBACK SortFuncGroup(int nLeftId, int nRightId, void* lParamSort)
	{
		PARAMSORT& ps = *(PARAMSORT*)lParamSort;

		const CString& left = ps.LookupGroupName(nLeftId);
		const CString& right = ps.LookupGroupName(nRightId);

		if (ps.m_Ascending)
			return _tcscmp( left, right );
		else
			return _tcscmp( right, left );	
	}
}

bool CListCtrl_Category_Groups::SortColumn(int nCol, bool bAscending)
{
	CWaitCursor waitCursor;

	PARAMSORT paramsort(m_hWnd, nCol, bAscending);

	if (IsGroupViewEnabled())
	{
		SetRedraw(FALSE);

		GroupByColumn(nCol);

		// Cannot use GetGroupInfo during sort
		for(int nRow=0 ; nRow < GetItemCount() ; ++nRow)
		{
			int nGroupId = GetRowGroupId(nRow);
			if (nGroupId!=-1 && paramsort.m_GroupNames.FindKey(nGroupId)==-1)
				paramsort.m_GroupNames.Add(nGroupId, GetGroupHeader(nGroupId));
		}

		SetRedraw(TRUE);
		Invalidate(FALSE);

		// Avoid bug in CListCtrl::SortGroups() which differs from ListView_SortGroups
		if (!ListView_SortGroups(m_hWnd, SortFuncGroup, &paramsort))
			return false;
	}
	else
	{
		ListView_SortItemsEx(m_hWnd, SortFunc, &paramsort);
	}

	return true;
}

BOOL CListCtrl_Category_Groups::SetGroupFooter(int nGroupID, const CString& footer, DWORD dwAlign /*= LVGA_FOOTER_CENTER*/)
{
	if (!IsGroupStateEnabled())
		return FALSE;

#if _WIN32_WINNT >= 0x0600
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_FOOTER | LVGF_ALIGN;
	lg.uAlign = dwAlign;
#ifdef UNICODE
	lg.pszFooter = (LPWSTR)(LPCTSTR)footer;
	lg.cchFooter = footer.GetLength();
#else
	CComBSTR bstrFooter = footer;
	lg.pszFooter = bstrFooter;
	lg.cchFooter = bstrFooter.Length();
#endif

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg)==-1)
		return FALSE;

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CListCtrl_Category_Groups::SetGroupTask(int nGroupID, const CString& task)
{
	if (!IsGroupStateEnabled())
		return FALSE;

#if _WIN32_WINNT >= 0x0600
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_TASK;
#ifdef UNICODE
	lg.pszTask = (LPWSTR)(LPCTSTR)task;
	lg.cchTask = task.GetLength();
#else
	CComBSTR bstrTask = task;
	lg.pszTask = bstrTask;
	lg.cchTask = bstrTask.Length();
#endif

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg)==-1)
		return FALSE;

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CListCtrl_Category_Groups::SetGroupSubtitle(int nGroupID, const CString& subtitle)
{
	if (!IsGroupStateEnabled())
		return FALSE;

#if _WIN32_WINNT >= 0x0600
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_SUBTITLE;
#ifdef UNICODE
	lg.pszSubtitle = (LPWSTR)(LPCTSTR)subtitle;
	lg.cchSubtitle = subtitle.GetLength();
#else
	CComBSTR bstrSubtitle = subtitle;
	lg.pszSubtitle = bstrSubtitle;
	lg.cchSubtitle = bstrSubtitle.Length();
#endif

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg)==-1)
		return FALSE;

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CListCtrl_Category_Groups::SetGroupTitleImage(int nGroupID, int nImage, const CString& topDesc, const CString& bottomDesc)
{
	if (!IsGroupStateEnabled())
		return FALSE;

#if _WIN32_WINNT >= 0x0600
	LVGROUP lg = {0};
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_TITLEIMAGE;
	lg.iTitleImage = nImage;	// Index of the title image in the control imagelist.

#ifdef UNICODE
	if (!topDesc.IsEmpty())
	{
		// Top description is drawn opposite the title image when there is
		// a title image, no extended image, and uAlign==LVGA_HEADER_CENTER.
		lg.mask |= LVGF_DESCRIPTIONTOP;
		lg.pszDescriptionTop = (LPWSTR)(LPCTSTR)topDesc;
		lg.cchDescriptionTop = topDesc.GetLength();
	}
	if (!bottomDesc.IsEmpty())
	{
		// Bottom description is drawn under the top description text when there is
		// a title image, no extended image, and uAlign==LVGA_HEADER_CENTER.
		lg.mask |= LVGF_DESCRIPTIONBOTTOM;
		lg.pszDescriptionBottom = (LPWSTR)(LPCTSTR)bottomDesc;
		lg.cchDescriptionBottom = bottomDesc.GetLength();
	}
#else
	CComBSTR bstrTopDesc = topDesc;
	CComBSTR bstrBottomDesc = bottomDesc;
	if (!topDesc.IsEmpty())
	{
		lg.mask |= LVGF_DESCRIPTIONTOP;
		lg.pszDescriptionTop = bstrTopDesc;
		lg.cchDescriptionTop = bstrTopDesc.Length();
	}
	if (!bottomDesc.IsEmpty())
	{
		lg.mask |= LVGF_DESCRIPTIONBOTTOM;
		lg.pszDescriptionBottom = bstrBottomDesc;
		lg.cchDescriptionBottom = bstrBottomDesc.Length();
	}
#endif

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg)==-1)
		return FALSE;

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CListCtrl_Category_Groups::OnGroupTaskClick(NMHDR* pNMHDR, LRESULT* pResult)
{
#if _WIN32_WINNT >= 0x0600
	NMLVLINK* pLinkInfo = (NMLVLINK*)pNMHDR;
	int nGroupId = pLinkInfo->iSubItem;
	CheckEntireGroup(nGroupId, true);
#endif
	return FALSE;
}

void CListCtrl_Category_Groups::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDblClk(nFlags, point);

	if (!IsGroupStateEnabled())
		return;
	
	int nGroupId = GroupHitTest(point);
}

BOOL CListCtrl_Category_Groups::OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pLV = reinterpret_cast<NMLISTVIEW*>(pNMHDR);

	int nCol = pLV->iSubItem;
	if (m_SortCol==nCol)
	{
		m_Ascending = !m_Ascending;
	}
	else
	{
		m_SortCol = nCol;
		m_Ascending = true;
	}

	SortColumn(m_SortCol, m_Ascending);
	SetSortArrow(m_SortCol, m_Ascending);
	return FALSE;	// Let parent-dialog get chance
}

void CListCtrl_Category_Groups::SetSortArrow(int colIndex, bool ascending)
{
	if (IsThemeEnabled())
	{
#if (_WIN32_WINNT >= 0x501)
		for(int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
		{
			HDITEM hditem = {0};
			hditem.mask = HDI_FORMAT;
			VERIFY( GetHeaderCtrl()->GetItem( i, &hditem ) );
			hditem.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
			if (i == colIndex)
			{
				hditem.fmt |= ascending ? HDF_SORTDOWN : HDF_SORTUP;
			}
			VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
		}
#endif
	}
	else
	{
		UINT bitmapID = m_Ascending ? IDB_DOWNARROW : IDB_UPARROW; 
		for(int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
		{
			HDITEM hditem = {0};
			hditem.mask = HDI_BITMAP | HDI_FORMAT;
			VERIFY( GetHeaderCtrl()->GetItem( i, &hditem ) );
			if (hditem.fmt & HDF_BITMAP && hditem.fmt & HDF_BITMAP_ON_RIGHT)
			{
				if (hditem.hbm)
				{
					DeleteObject(hditem.hbm);
					hditem.hbm = NULL;
				}
				hditem.fmt &= ~(HDF_BITMAP|HDF_BITMAP_ON_RIGHT);
				VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
			}
			if (i == colIndex)
			{
				hditem.fmt |= HDF_BITMAP|HDF_BITMAP_ON_RIGHT;
				hditem.hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0,0, LR_LOADMAP3DCOLORS); 
				VERIFY( hditem.hbm!=NULL );
				VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
			}
		}
	}
}

void CListCtrl_Category_Groups::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// Focus retangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_CHECKBOXES);

	// Enable Vista-look if possible
	EnableWindowTheme(GetSafeHwnd(), L"ListView", L"Explorer", NULL);
}