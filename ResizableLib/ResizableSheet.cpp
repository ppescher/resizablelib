// ResizableSheet.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000-2001 by Paolo Messina
// (http://www.geocities.com/ppescher - ppescher@yahoo.com)
//
// The contents of this file are subject to the Artistic License (the "License").
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.opensource.org/licenses/artistic-license.html
//
// If you find this code useful, credits would be nice!
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResizableSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableSheet

IMPLEMENT_DYNAMIC(CResizableSheet, CPropertySheet)

inline void CResizableSheet::Construct()
{
	m_bInitDone = FALSE;

	m_bEnableSaveRestore = FALSE;
	m_bSavePage = FALSE;
}


CResizableSheet::CResizableSheet()
{
	Construct();
}

CResizableSheet::CResizableSheet(UINT nIDCaption, CWnd *pParentWnd, UINT iSelectPage)
	 : CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct();
}

CResizableSheet::CResizableSheet(LPCTSTR pszCaption, CWnd *pParentWnd, UINT iSelectPage)
	 : CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct();
}

CResizableSheet::~CResizableSheet()
{
}

BEGIN_MESSAGE_MAP(CResizableSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CResizableSheet)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_WIZBACK, OnPageChanged)
	ON_BN_CLICKED(ID_WIZNEXT, OnPageChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableSheet message handlers

int CResizableSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// keep client area
	CRect rect;
	GetClientRect(&rect);
	// set resizable style
	ModifyStyle(DS_MODALFRAME, WS_POPUP | WS_THICKFRAME);
	// adjust size to reflect new style
	::AdjustWindowRectEx(&rect, GetStyle(),
		::IsMenu(GetMenu()->GetSafeHmenu()), GetExStyle());
	SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_FRAMECHANGED|
		SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREPOSITION);

	if (!InitGrip())
		return -1;

	return 0;
}

BOOL CResizableSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	// set the initial size as the min track size
	CRect rc;
	GetWindowRect(&rc);
	SetMinTrackSize(rc.Size());

	// init

	UpdateGripPos();

	PresetLayout();

	// prevent flickering
	GetTabControl()->ModifyStyle(0, WS_CLIPSIBLINGS);

	m_bInitDone = TRUE;

	return bResult;
}

void CResizableSheet::OnDestroy() 
{
	if (m_bEnableSaveRestore)
	{
		SaveWindowRect(m_sSection, m_bRectOnly);
		SavePage();
	}

	RemoveAllAnchors();

	CPropertySheet::OnDestroy();
}

// maps an index to a button ID and vice-versa
static UINT _propButtons[] =
{
	IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP,
	ID_WIZBACK, ID_WIZNEXT, ID_WIZFINISH
};

// horizontal line in wizard mode
#define ID_WIZLINE	ID_WIZFINISH+1

void CResizableSheet::PresetLayout()
{
	if (IsWizard())	// wizard mode
	{
		// hide tab control
		GetTabControl()->ShowWindow(SW_HIDE);

		AddAnchor(ID_WIZLINE, BOTTOM_LEFT, BOTTOM_RIGHT);
	}
	else	// tab mode
	{
		AddAnchor(AFX_IDC_TAB_CONTROL, TOP_LEFT, BOTTOM_RIGHT);
	}

	// add a callback for active page (which can change at run-time)
	AddAnchorCallback(1);

	// use *total* parent size to have correct margins
	CRect rectPage, rectSheet;
	GetTotalClientRect(&rectSheet);

	GetActivePage()->GetWindowRect(&rectPage);
	ScreenToClient(&rectPage);

	// pre-calculate margins
	m_sizePageTL = rectPage.TopLeft() - rectSheet.TopLeft();
	m_sizePageBR = rectPage.BottomRight() - rectSheet.BottomRight();

	// add all possible buttons, if they exist
	for (int i = 0; i < 7; i++)
	{
		if (NULL != GetDlgItem(_propButtons[i]))
			AddAnchor(_propButtons[i], BOTTOM_RIGHT);
	}
}

BOOL CResizableSheet::ArrangeLayoutCallback(LayoutInfo &layout)
{
	if (layout.nCallbackID != 1)	// we only added 1 callback
		return CResizableLayout::ArrangeLayoutCallback(layout);

	// set layout info for active page
	layout.hWnd = GetActivePage()->GetSafeHwnd();

	// set margins
	if (IsWizard())	// wizard mode
	{
		// use pre-calculated margins
		layout.sizeMarginTL = m_sizePageTL;
		layout.sizeMarginBR = m_sizePageBR;
	}
	else	// tab mode
	{
		CRect rectPage, rectSheet;
		GetTotalClientRect(&rectSheet);

		CTabCtrl* pTab = GetTabControl();
		pTab->GetWindowRect(&rectPage);
		pTab->AdjustRect(FALSE, &rectPage);
		ScreenToClient(&rectPage);

		// use tab control
		layout.sizeMarginTL = rectPage.TopLeft() - rectSheet.TopLeft();
		layout.sizeMarginBR = rectPage.BottomRight() - rectSheet.BottomRight();
	}

	// set anchor types
	layout.sizeTypeTL = TOP_LEFT;
	layout.sizeTypeBR = BOTTOM_RIGHT;

	// use this layout info
	return TRUE;
}

void CResizableSheet::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW)
		return;		// arrangement not needed

	if (m_bInitDone)
	{
		// update size-grip
		UpdateGripPos();

		ArrangeLayout();
	}
}

// only gets called in wizard mode
// (when back or next button pressed)
void CResizableSheet::OnPageChanged()
{
	// call default handler to allow page change
	Default();

	// update new wizard page
	ArrangeLayout();
}

BOOL CResizableSheet::OnEraseBkgnd(CDC* pDC) 
{
	ClipChildren(pDC);

	return CPropertySheet::OnEraseBkgnd(pDC);
}

void CResizableSheet::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (!m_bInitDone)
		return;

	MinMaxInfo(lpMMI);
}

// protected members

int CResizableSheet::GetMinWidth()
{
	CWnd* pWnd = NULL;
	CRect rectWnd, rectSheet;
	GetTotalClientRect(&rectSheet);

	int max = 0, min = rectSheet.Width();
	// search for leftmost and rightmost button margins
	for (int i = 0; i < 7; i++)
	{
		pWnd = GetDlgItem(_propButtons[i]);
		// exclude not present or hidden buttons
		if (pWnd == NULL || !(pWnd->GetStyle() & WS_VISIBLE))
			continue;

		// left position is relative to the right border
		// of the parent window (negative value)
		pWnd->GetWindowRect(&rectWnd);
		ScreenToClient(&rectWnd);
		int left = rectSheet.right - rectWnd.left;
		int right = rectSheet.right - rectWnd.right;

		if (left > max)
			max = left;
		if (right < min)
			min = right;
	}

	// sizing border width
	int border = GetSystemMetrics(SM_CXSIZEFRAME);
	
	// compute total width
	return max + min + 2*border;
}


// NOTE: this must be called after all the other settings
//       to have the window and its controls displayed properly
void CResizableSheet::EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly, BOOL bWithPage)
{
	m_sSection = pszSection;
	m_bSavePage = bWithPage;

	m_bEnableSaveRestore = TRUE;
	m_bRectOnly = bRectOnly;

	// restore immediately
	LoadWindowRect(pszSection, bRectOnly);
	LoadPage();
}

// private memebers

// used to save/restore active page
// either in the registry or a private .INI file
// depending on your application settings

#define ACTIVEPAGE 	_T("ActivePage")

void CResizableSheet::SavePage()
{
	if (!m_bSavePage)
		return;

	// saves active page index, zero (the first) if problems
	// cannot use GetActivePage, because it always fails

	CTabCtrl *pTab = GetTabControl();
	int page = 0;

	if (pTab != NULL) 
		page = pTab->GetCurSel();
	if (page < 0)
		page = 0;

	AfxGetApp()->WriteProfileInt(m_sSection, ACTIVEPAGE, page);
}

void CResizableSheet::LoadPage()
{
	// restore active page, zero (the first) if not found
	int page = AfxGetApp()->GetProfileInt(m_sSection, ACTIVEPAGE, 0);
	
	if (m_bSavePage)
	{
		SetActivePage(page);
		ArrangeLayout();	// needs refresh
	}
}
