// ResizableSheetEx.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000 by Paolo Messina
// (ppescher@yahoo.com)
//
// Free for non-commercial and commercial use,
// provided that the original author's name
// and copyright is quoted somewhere in the
// final executable and in the program's help
// or documentation.
// You may change the code to your needs,
// provided that credits to the original
// author are given in the modified files.
// Also a copy of your enhancements would
// be nice, but it's not required. Please,
// consider to share your work on CodeProject.
//  
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResizableSheetEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableSheetEx

IMPLEMENT_DYNAMIC(CResizableSheetEx, CPropertySheetEx)

inline void CResizableSheetEx::Construct()
{
	m_bInitDone = FALSE;

	m_bEnableSaveRestore = FALSE;
	m_bSavePage = FALSE;
}


CResizableSheetEx::CResizableSheetEx()
{
	Construct();
}

CResizableSheetEx::CResizableSheetEx(UINT nIDCaption, CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CPropertySheetEx(nIDCaption, pParentWnd, iSelectPage,
				  hbmWatermark, hpalWatermark, hbmHeader)
{
	Construct();
}

CResizableSheetEx::CResizableSheetEx(LPCTSTR pszCaption, CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CPropertySheetEx(pszCaption, pParentWnd, iSelectPage,
					  hbmWatermark, hpalWatermark, hbmHeader)
{
	Construct();
}


CResizableSheetEx::~CResizableSheetEx()
{
}

BEGIN_MESSAGE_MAP(CResizableSheetEx, CPropertySheetEx)
	//{{AFX_MSG_MAP(CResizableSheetEx)
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
// CResizableSheetEx message handlers

int CResizableSheetEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheetEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// change window style to be resizable
	ModifyStyle(DS_MODALFRAME, WS_THICKFRAME, SWP_FRAMECHANGED);

	if (!InitGrip())
		return -1;

	return 0;
}

BOOL CResizableSheetEx::OnInitDialog() 
{
	BOOL bResult = CPropertySheetEx::OnInitDialog();
	
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

	ArrangeLayout();

	return bResult;
}

void CResizableSheetEx::OnDestroy() 
{
	if (m_bEnableSaveRestore)
	{
		SaveWindowRect(m_sSection, m_bRectOnly);
		SavePage();
	}

	RemoveAllAnchors();

	CPropertySheetEx::OnDestroy();
}

// maps an index to a button ID and vice-versa
static UINT _propButtons[] =
{
	IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP,
	ID_WIZBACK, ID_WIZNEXT, ID_WIZFINISH
};

// horizontal line in wizard mode
#define ID_WIZLINE		ID_WIZFINISH+1
#define ID_WIZLINEHDR	ID_WIZFINISH+2

void CResizableSheetEx::PresetLayout()
{
	if (IsWizard() || IsWizard97())	// wizard mode
	{
		// hide tab control
		GetTabControl()->ShowWindow(SW_HIDE);

		AddAnchor(ID_WIZLINE, BOTTOM_LEFT, BOTTOM_RIGHT);

		if (IsWizard97())	// add header line for wizard97 dialogs
			AddAnchor(ID_WIZLINEHDR, TOP_LEFT, TOP_RIGHT);
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

BOOL CResizableSheetEx::ArrangeLayoutCallback(LayoutInfo &layout)
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
	else if (IsWizard97())	// wizard 97
	{
		// use pre-calculated margins
		layout.sizeMarginTL = m_sizePageTL;
		layout.sizeMarginBR = m_sizePageBR;

		if (!(GetActivePage()->m_psp.dwFlags & PSP_HIDEHEADER))
		{
			// add header vertical offset
			CRect rectLine;
			GetDlgItem(ID_WIZLINEHDR)->GetWindowRect(&rectLine);
			ScreenToClient(&rectLine);

			layout.sizeMarginTL.cy = rectLine.bottom;
		}
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

void CResizableSheetEx::OnSize(UINT nType, int cx, int cy) 
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
void CResizableSheetEx::OnPageChanged()
{
	// call default handler to allow page change
	Default();

	// update new wizard page
	ArrangeLayout();
}

BOOL CResizableSheetEx::OnEraseBkgnd(CDC* pDC) 
{
	ClipChildren(pDC);

	return CPropertySheetEx::OnEraseBkgnd(pDC);
}

void CResizableSheetEx::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (!m_bInitDone)
		return;

	MinMaxInfo(lpMMI);
}

// protected members

int CResizableSheetEx::GetMinWidth()
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
void CResizableSheetEx::EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly, BOOL bWithPage)
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

void CResizableSheetEx::SavePage()
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

void CResizableSheetEx::LoadPage()
{
	// restore active page, zero (the first) if not found
	int page = AfxGetApp()->GetProfileInt(m_sSection, ACTIVEPAGE, 0);
	
	if (m_bSavePage)
	{
		SetActivePage(page);
		ArrangeLayout();	// needs refresh
	}
}
