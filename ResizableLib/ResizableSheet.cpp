// ResizableSheet.cpp : implementation file
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
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
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
	
	// change window style to be resizable
	// propertysheet's children erase background themselves
	ModifyStyle(DS_MODALFRAME, WS_THICKFRAME | WS_CLIPCHILDREN, SWP_FRAMECHANGED);

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
	CPropertySheet::OnDestroy();

	if (m_bEnableSaveRestore)
	{
		SaveWindowRect(m_sSection, m_bRectOnly);
		SavePage();
	}
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
	CWnd* pWnd;	// points to various children
	CRect wndrc, objrc;
	GetClientRect(&wndrc);

	// tab control or wizard line position
	if (m_psh.dwFlags & PSH_WIZARD)	// wizard mode
	{
		// get wizard line's bottom-right corner
		pWnd = GetDlgItem(ID_WIZLINE);

		// hide tab control
		GetTabControl()->ShowWindow(SW_HIDE);
	}
	else	// tabbed mode
	{
		// get tab control's bottom-right corner
		pWnd = GetTabControl();
	}
	// whatever it is, take the right margin
	pWnd->GetWindowRect(&objrc);
	ScreenToClient(&objrc);

	m_szLayoutTabLine.cx = objrc.right - wndrc.right;
	m_szLayoutTabLine.cy = objrc.bottom - wndrc.bottom;

	// get child dialog's bottom-right corner
	pWnd = GetActivePage();

	pWnd->GetWindowRect(&objrc);
	ScreenToClient(&objrc);

	m_szLayoutPage.cx = objrc.right - wndrc.right;
	m_szLayoutPage.cy = objrc.bottom - wndrc.bottom;

	// store buttons position
	for (int i = 0; i < 7; i++)
	{
		pWnd = GetDlgItem(_propButtons[i]);
		
		if (pWnd == NULL)
		{
			// invalid position, button does not exist
			// (just to initialize, any button you may activate
			// in the future is present, but hidden)
			m_szLayoutButton[i].cx = 0;
			m_szLayoutButton[i].cy = 0;
			continue;
		}
		
		pWnd->GetWindowRect(&objrc);
		ScreenToClient(&objrc);

		m_szLayoutButton[i].cx = objrc.left - wndrc.right;
		m_szLayoutButton[i].cy = objrc.top - wndrc.bottom;
	}
}

void CResizableSheet::ArrangeLayout()
{
	// init some vars
	CWnd* pWnd;
	CRect wndrc, objrc;
	GetClientRect(&wndrc);

	// usually no more than
	// 4 buttons +
	// 1 tab control or wizard line +
	// 1 active page
	HDWP hdwp = BeginDeferWindowPos(6);

	// assume wizard mode
	CPoint ptPagePos(0,0);	// holds a page's top-left corner
	DWORD dwPageFlags = SWP_NOMOVE;

	if (m_psh.dwFlags & PSH_WIZARD)	// wizard mode
	{
		// get wizard line's bottom-right corner
		pWnd = GetDlgItem(ID_WIZLINE);

		pWnd->GetWindowRect(&objrc);
		ScreenToClient(&objrc);

		int oldHeight = objrc.Height();
		objrc.right = m_szLayoutTabLine.cx + wndrc.right;
		objrc.bottom = m_szLayoutTabLine.cy + wndrc.bottom;
		objrc.top = objrc.bottom - oldHeight;

		// add the control
		hdwp = DeferWindowPos(hdwp, *pWnd, NULL, objrc.left, objrc.top,
			objrc.Width(), objrc.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
	else	// tabbed mode
	{
		// get tab control's bottom-right corner
		pWnd = GetTabControl();

		pWnd->GetWindowRect(&objrc);
		ScreenToClient(&objrc);

		objrc.right = m_szLayoutTabLine.cx + wndrc.right;
		objrc.bottom = m_szLayoutTabLine.cy + wndrc.bottom;

		// add the control, only resize
		hdwp = DeferWindowPos(hdwp, *pWnd, NULL, 0, 0, objrc.Width(),
			objrc.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE |
			SWP_NOCOPYBITS | SWP_DRAWFRAME);

		// correct page position (for stacked tabs)
		pWnd->GetWindowRect(&objrc);
		((CTabCtrl*)pWnd)->AdjustRect(FALSE, &objrc);
		ScreenToClient(&objrc);
		ptPagePos = objrc.TopLeft();
		dwPageFlags = 0;
	}

	// get child dialog's bottom-right corner
	pWnd = GetActivePage();

	pWnd->GetWindowRect(&objrc);
	ScreenToClient(&objrc);

	objrc.right = m_szLayoutPage.cx + wndrc.right;
	objrc.bottom = m_szLayoutPage.cy + wndrc.bottom;

	if (dwPageFlags == 0)	// correct pos
	{
		objrc.left = ptPagePos.x;
		objrc.top = ptPagePos.y;
	}

	// add the page, with proper attributes
	hdwp = DeferWindowPos(hdwp, *pWnd, NULL, ptPagePos.x, ptPagePos.y, objrc.Width(),
		objrc.Height(), dwPageFlags | SWP_NOZORDER | SWP_NOACTIVATE);

	// arrange buttons position
	for (int i = 0; i < 7; i++)
	{
		pWnd = GetDlgItem(_propButtons[i]);
		
		if (pWnd == NULL)
			continue;	// ignores deleted buttons

		// this should never happen, because all the buttons you
		// may activate already exist at time PresetLayout is called
		ASSERT(m_szLayoutButton[i].cx != 0 || m_szLayoutButton[i].cy != 0);
		
		pWnd->GetWindowRect(&objrc);
		ScreenToClient(&objrc);

		objrc.left = m_szLayoutButton[i].cx + wndrc.right;
		objrc.top = m_szLayoutButton[i].cy + wndrc.bottom;

		// add the control, only move
		hdwp = DeferWindowPos(hdwp, *pWnd, NULL, objrc.left, objrc.top,
			0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	// go re-arrange child windows
	EndDeferWindowPos(hdwp);
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

void CResizableSheet::OnPaint() 
{
	CPaintDC dc(this);

	DrawGrip(dc);
}

UINT CResizableSheet::OnNcHitTest(CPoint point) 
{
	// test if in size grip
	UINT ht = HitTest(point);
	if (ht != HTNOWHERE)
		return ht;
	
	return CPropertySheet::OnNcHitTest(point);
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
	int min = 0;

	// search for leftmost button
	for (int i = 0; i < 7; i++)
	{
		// left position is relative to the right border
		// of the parent window (negative value)
		if (m_szLayoutButton[i].cx < min)
			min = m_szLayoutButton[i].cx;
	}

	// sizing border width
	int border = GetSystemMetrics(SM_CXSIZEFRAME);
	
	// get tab control or wizard line left position
	CWnd* pWnd;
	CRect objrc;

	if (m_psh.dwFlags & PSH_WIZARD)
		pWnd = GetDlgItem(ID_WIZLINE);
	else
		pWnd = GetTabControl();

	pWnd->GetWindowRect(&objrc);
	ScreenToClient(&objrc);

	// add the left margin and window's border
	return -min + objrc.left + border*2;
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
