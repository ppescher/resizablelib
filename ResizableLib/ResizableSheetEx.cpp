// ResizableSheetEx.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// This file is part of ResizableLib
// https://github.com/ppescher/resizablelib
//
// Copyright (C) 2000-2024 by Paolo Messina
// mailto:ppescher@hotmail.com
//
// The contents of this file are subject to the Artistic License 2.0
// http://opensource.org/licenses/Artistic-2.0
//
// If you find this code useful, credits would be nice!
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResizableSheetEx.h"
#include "ResizableVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableSheetEx

IMPLEMENT_DYNAMIC(CResizableSheetEx, CPropertySheetEx)

inline void CResizableSheetEx::PrivateConstruct()
{
	m_bEnableSaveRestore = FALSE;
	m_bSavePage = FALSE;
	m_dwGripTempState = 1;
	m_bLayoutDone = FALSE;
	m_bRectOnly = FALSE;
	m_nCallbackID = 0;
}

inline BOOL CResizableSheetEx::IsWizard() const
{
	return (m_psh.dwFlags & PSH_WIZARD);
}

inline BOOL CResizableSheetEx::IsWizard97() const
{
	return (m_psh.dwFlags & (PSH_IE4WIZARD97 | PSH_IE5WIZARD97));
}

CResizableSheetEx::CResizableSheetEx()
{
	PrivateConstruct();
}

CResizableSheetEx::CResizableSheetEx(UINT nIDCaption, CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CPropertySheetEx(nIDCaption, pParentWnd, iSelectPage,
				   hbmWatermark, hpalWatermark, hbmHeader)
{
	PrivateConstruct();
}

CResizableSheetEx::CResizableSheetEx(LPCTSTR pszCaption, CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CPropertySheetEx(pszCaption, pParentWnd, iSelectPage,
				   hbmWatermark, hpalWatermark, hbmHeader)
{
	PrivateConstruct();
}


CResizableSheetEx::~CResizableSheetEx()
{
}

BEGIN_MESSAGE_MAP(CResizableSheetEx, CPropertySheetEx)
	//{{AFX_MSG_MAP(CResizableSheetEx)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_NCCREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT_EX(PSN_SETACTIVE, OnPageChanging)
	ON_REGISTERED_MESSAGE(WMU_RESIZESUPPORT, OnResizeSupport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableSheetEx message handlers

BOOL CResizableSheetEx::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CPropertySheetEx::OnNcCreate(lpCreateStruct))
		return FALSE;

	// child dialogs don't want resizable border or size grip,
	// nor they can handle the min/max size constraints
	BOOL bChild = lpCreateStruct->style & WS_CHILD;

	// create and init the size-grip
	if (!CreateSizeGrip(!bChild))
		return FALSE;

	MakeResizable(lpCreateStruct);

	return TRUE;
}

BOOL CResizableSheetEx::OnInitDialog()
{
	const BOOL bResult = CPropertySheetEx::OnInitDialog();

	// initialize layout
	PresetLayout();
	m_bLayoutDone = TRUE;

	return bResult;
}

LRESULT CResizableSheetEx::OnResizeSupport(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case RSZSUP_SHEETPAGEEXHACK:
		{
			// a window object must be still associated to the page handle
			// but MFC subclassing has been turned off to allow the system
			// to subclass it first, so we can catch all the messages
			CWnd* pWnd = CWnd::FromHandle((HWND)lParam);
			if (pWnd == NULL)
				return 0;

			// subclass the window again and refresh page and sheet
			pWnd->SubclassWindow(pWnd->Detach());
			RefreshLayout();
			pWnd->SendMessage(WM_SIZE);
			Invalidate();
			UnlockWindowUpdate();

			if (pWnd->IsWindowVisible())
			{
				// send lost PSN_SETACTIVE notification message
				CPropertyPage* pPage = DYNAMIC_DOWNCAST(CPropertyPage, pWnd);
				if (pPage != NULL)
					SetActivePage(pPage);
			}
		}
		break;

	default:
		return FALSE;
	}
	return TRUE;
}

void CResizableSheetEx::OnDestroy()
{
	if (m_bEnableSaveRestore)
	{
		SaveWindowRect(m_sSection, m_bRectOnly);
		if (m_bSavePage)
			SavePage(m_sSection);
	}

	// reset instance data
	RemoveAllAnchors();
	ResetAllRects();
	PrivateConstruct();

	CPropertySheetEx::OnDestroy();
}

// maps an index to a button ID and vice-versa
static UINT _propButtons[] =
{
	IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP,
	ID_WIZBACK, ID_WIZNEXT, ID_WIZFINISH
};
const int _propButtonsCount = sizeof(_propButtons)/sizeof(UINT);

// horizontal line in wizard mode
#define ID_WIZLINE		ID_WIZFINISH+1
#define ID_WIZLINEHDR	ID_WIZFINISH+2

void CResizableSheetEx::PresetLayout()
{
	// set the initial size as the min track size
	CRect rc;
	GetWindowRect(&rc);
	SetMinTrackSize(rc.Size());

	// use *total* parent size to have correct margins
	CRect rectPage, rectSheet;
	GetTotalClientRect(&rectSheet);

	// get page area
	if (IsWizard() || IsWizard97())
	{
		HWND hPage = PropSheet_GetCurrentPageHwnd(m_hWnd);
		::GetWindowRect(hPage, &rectPage);
	}
	else
	{
		GetTabControl()->GetWindowRect(&rectPage);
	}
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rectPage, 2);

	// calculate margins
	CRect rect;
	int cxDiff = rectSheet.right - rectPage.right;
	int cyDiff = 0;

	// try all possible buttons
	for (int i = 0; i < _propButtonsCount; i++)
	{
		CWnd* pWnd = GetDlgItem(_propButtons[i]);
		if (NULL != pWnd)
		{
			// move buttons if necessary
			if (GetStyle() & WS_CHILD)
			{
				pWnd->GetWindowRect(&rect);
				::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rect, 2);

				cyDiff = rectSheet.bottom - rect.bottom;
				rect.OffsetRect(cxDiff, cyDiff);

				pWnd->MoveWindow(&rect);
			}
			// add buttons to the layout manager
			AddAnchor(_propButtons[i], BOTTOM_RIGHT);
		}
	}

	// setup pages area
	if (IsWizard() || IsWizard97())
	{
		// move line and pages if necessary
		if (GetStyle() & WS_CHILD)
		{
			GetDlgItem(ID_WIZLINE)->GetWindowRect(&rect);
			::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rect, 2);

			rect.OffsetRect(0, cyDiff);
			rect.InflateRect(cxDiff, 0);

			GetDlgItem(ID_WIZLINE)->MoveWindow(&rect);

			rectPage.bottom += cyDiff;
			rectPage.left = 0;
			rectPage.top = 0;
			rectPage.right = rectSheet.right;
		}

		AddAnchor(ID_WIZLINE, BOTTOM_LEFT, BOTTOM_RIGHT);

		if (IsWizard97())	// add header line for wizard97 dialogs
			AddAnchor(ID_WIZLINEHDR, TOP_LEFT, TOP_RIGHT);

		// hide tab control and keep it hidden
		GetTabControl()->ShowWindow(SW_HIDE);
		AddAnchor(AFX_IDC_TAB_CONTROL, TOP_LEFT);

		// pre-calculate margins
		m_sizePageTL = rectPage.TopLeft() - rectSheet.TopLeft();
		m_sizePageBR = rectPage.BottomRight() - rectSheet.BottomRight();
	}
	else
	{
		// grow tab to the available sheet space
		if (cyDiff > 0)
			rectSheet.bottom = rectPage.bottom + cyDiff;

		if (GetStyle() & WS_CHILD)
			GetTabControl()->MoveWindow(&rectSheet);

		AddAnchor(AFX_IDC_TAB_CONTROL, TOP_LEFT, BOTTOM_RIGHT);
	}

	// add a callback for active page (which can change at run-time)
	m_nCallbackID = AddAnchorCallback();

	// prevent flickering
	GetTabControl()->ModifyStyle(0, WS_CLIPSIBLINGS);
}

BOOL CResizableSheetEx::ArrangeLayoutCallback(LAYOUTINFO &layout) const
{
	if (layout.nCallbackID != m_nCallbackID)	// we only added 1 callback
		return CResizableLayout::ArrangeLayoutCallback(layout);

	// set layout info for active page
	layout.hWnd = (HWND)::SendMessage(GetSafeHwnd(), PSM_GETCURRENTPAGEHWND, 0, 0);
	if (!::IsWindow(layout.hWnd))
		return FALSE;

	// set margins
	if (IsWizard())	// wizard mode
	{
		// use pre-calculated margins
		layout.marginTopLeft = m_sizePageTL;
		layout.marginBottomRight = m_sizePageBR;
	}
	else if (IsWizard97())	// wizard 97
	{
		// use pre-calculated margins
		layout.marginTopLeft = m_sizePageTL;
		layout.marginBottomRight = m_sizePageBR;

		if (!(GetActivePage()->m_psp.dwFlags & PSP_HIDEHEADER))
		{
			// add header vertical offset
			CRect rectLine, rectSheet;
			GetTotalClientRect(&rectSheet);
			GetAnchorPosition(ID_WIZLINEHDR, rectSheet, rectLine);

			layout.marginTopLeft.cy = rectLine.bottom;
		}
	}
	else	// tab mode
	{
		CTabCtrl* pTab = GetTabControl();
		ASSERT(pTab != NULL);

		// get tab position after resizing and calc page rect
		CRect rectPage, rectSheet;
		GetTotalClientRect(&rectSheet);

		if (!GetAnchorPosition(pTab->m_hWnd, rectSheet, rectPage))
			return FALSE; // no page yet

		// temporarily resize the tab control to calc page size
		CRect rectSave;
		pTab->GetWindowRect(rectSave);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rectSave, 2);
		pTab->SetRedraw(FALSE);
		pTab->MoveWindow(rectPage, FALSE);
		pTab->AdjustRect(FALSE, &rectPage);
		pTab->MoveWindow(rectSave, FALSE);
		pTab->SetRedraw(TRUE);

		// set margins
		layout.marginTopLeft = rectPage.TopLeft() - rectSheet.TopLeft();
		layout.marginBottomRight = rectPage.BottomRight() - rectSheet.BottomRight();
	}

	// set anchor types
	layout.anchorTopLeft = TOP_LEFT;
	layout.anchorBottomRight = BOTTOM_RIGHT;

	// use this layout info
	return TRUE;
}

void CResizableSheetEx::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW)
		return;		// arrangement not needed

	if (nType == SIZE_MAXIMIZED)
		HideSizeGrip(&m_dwGripTempState);
	else
		ShowSizeGrip(&m_dwGripTempState);

	// update grip and layout
	UpdateSizeGrip();
	ArrangeLayout();

	if (IsWizard97())
	{
		// refresh header area (erase needed if stretched)
		CRect rect;
		GetHeaderRect(rect);
		InvalidateRect(rect, (m_psh.dwFlags & PSH_STRETCHWATERMARK) ? TRUE : FALSE);
	}
}

BOOL CResizableSheetEx::OnPageChanging(NMHDR* /*pNotifyStruct*/, LRESULT* /*pResult*/)
{
	// update new wizard page
	// active page changes after this notification
	PostMessage(WM_SIZE);

	return FALSE;	// continue routing
}

BOOL CResizableSheetEx::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	if (m_psh.dwFlags & PSH_IE4WIZARD97)
	{
		// get header area
		GetHeaderRect(rect);
		// implement header redraw for compatibility (missing with Common Controls >= 6.00)
		if (rect.Width() > 0 && (real_WIN32_IE >= 0x0600) && (real_ThemeSettings & STAP_ALLOW_CONTROLS))
		{
			if (m_psh.dwFlags & PSH_USEHBMHEADER)
			{
				CBitmap *pHdr = CBitmap::FromHandle(m_psh.hbmHeader);
				if (m_psh.dwFlags & PSH_STRETCHWATERMARK)
				{
					BITMAP bmp;
					if (pHdr->GetBitmap(&bmp))
					{
						CDC dc;
						dc.CreateCompatibleDC(pDC);
						CBitmap *pOld = dc.SelectObject(pHdr);
						int nOld = pDC->SetStretchBltMode(COLORONCOLOR);
						pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
						pDC->SetStretchBltMode(nOld);
						dc.SelectObject(pOld);
					}
				}
				else
				{
					CBrush brush(pHdr);
					pDC->FillRect(&rect, &brush);
				}
			}
			else
			{
				pDC->FillSolidRect(&rect, ::GetSysColor(COLOR_WINDOW));
			}
			// get system font
			NONCLIENTMETRICS ncm = {sizeof(NONCLIENTMETRICS)};
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
			// create fonts, with bold variant
			CFont fontTitle, fontSubTitle;
			fontSubTitle.CreateFontIndirect(&ncm.lfMenuFont);
			ncm.lfMenuFont.lfWeight += 300;
			fontTitle.CreateFontIndirect(&ncm.lfMenuFont);
			// draw title and sub-title
			CRect rc(14, 6, 0, 0);
			MapDialogRect(rc);
			rc.right = rect.right;
			rc.bottom = rect.bottom;
			pDC->SetBkMode(TRANSPARENT);
			int idx = GetActiveIndex();
			CFont* pOld = pDC->SelectObject(&fontTitle);
			pDC->DrawText(m_psh.ppsp[idx].pszHeaderTitle, rc, DT_LEFT | DT_CALCRECT);
			pDC->DrawText(m_psh.ppsp[idx].pszHeaderTitle, rc, DT_LEFT);
			rc.OffsetRect(rc.left, rc.Height());
			pDC->SelectObject(&fontSubTitle);
			pDC->DrawText(m_psh.ppsp[idx].pszHeaderSubTitle, rc, DT_LEFT | DT_CALCRECT);
			pDC->DrawText(m_psh.ppsp[idx].pszHeaderSubTitle, rc, DT_LEFT);
			pDC->SelectObject(pOld);
		}
	}
	if (ClipChildren(pDC, FALSE))
	{
		// when clipping, remove header from clipping area
		if (IsWizard97())
		{
			// clip header area out
			pDC->ExcludeClipRect(rect);
		}
	}

	BOOL bRet = CPropertySheetEx::OnEraseBkgnd(pDC);

	ClipChildren(pDC, TRUE);

	return bRet;
}

BOOL CResizableSheetEx::CalcSizeExtra(HWND /*hWndChild*/, const CSize& sizeChild, CSize& sizeExtra)
{
	CTabCtrl* pTab = GetTabControl();
	if (!pTab)
		return FALSE;

	// get margins of tabcontrol
	CRect rectMargins;
	if (!GetAnchorMargins(pTab->m_hWnd, sizeChild, rectMargins))
		return FALSE;

	// get tab position after resizing and calc page rect
	CRect rectPage, rectSheet;
	GetTotalClientRect(&rectSheet);

	if (!GetAnchorPosition(pTab->m_hWnd, rectSheet, rectPage))
		return FALSE; // no page yet

	// temporarily resize the tab control to calc page size
	CRect rectSave, rectTabMargins;
	pTab->GetWindowRect(rectSave);
	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rectSave, 2);
	pTab->SetRedraw(FALSE);
	pTab->MoveWindow(rectPage, FALSE);
	pTab->AdjustRect(TRUE, &rectTabMargins);
	pTab->MoveWindow(rectSave, FALSE);
	pTab->SetRedraw(TRUE);

	// add non-client size
	const DWORD dwStyle = GetStyle();
	::AdjustWindowRectEx(&rectTabMargins, dwStyle, !(dwStyle & WS_CHILD) &&
		::IsMenu(GetMenu()->GetSafeHmenu()), GetExStyle());
	// compute extra size
	sizeExtra = rectMargins.TopLeft() + rectMargins.BottomRight() +
		rectTabMargins.Size();
	return TRUE;
}

void CResizableSheetEx::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	MinMaxInfo(lpMMI);

	if (!GetTabControl())
		return;

	int idx = GetPageCount();
	if (!IsWizard() && !IsWizard97())	// tab mode
	{
		while (--idx >= 0)
			ChainMinMaxInfoCB(lpMMI, *GetPage(idx));
	}
	else	// wizard mode
	{
		while (--idx >= 0)
		{
			// use pre-calculated margins
			CRect rectExtra(-CPoint(m_sizePageTL), -CPoint(m_sizePageBR));

			if (IsWizard97() && !(GetPage(idx)->m_psp.dwFlags & PSP_HIDEHEADER))	// wizard97 mode
			{
				// add header vertical offset
				CRect rectLine, rectSheet;
				GetTotalClientRect(&rectSheet);
				GetAnchorPosition(ID_WIZLINEHDR, rectSheet, rectLine);

				rectExtra.top = -rectLine.bottom;
			}

			// add non-client size
			const DWORD dwStyle = GetStyle();
			::AdjustWindowRectEx(&rectExtra, dwStyle, !(dwStyle & WS_CHILD) &&
				::IsMenu(GetMenu()->GetSafeHmenu()), GetExStyle());
			ChainMinMaxInfo(lpMMI, *GetPage(idx), rectExtra.Size());
		}
	}
}

// protected members

void CResizableSheetEx::GetHeaderRect(LPRECT lpRect)
{
	const CWnd* pWizLineHdr = GetDlgItem(ID_WIZLINEHDR);
	if (pWizLineHdr != NULL && pWizLineHdr->IsWindowVisible())
	{
		pWizLineHdr->GetWindowRect(lpRect);
		::MapWindowPoints(NULL, m_hWnd, (LPPOINT)lpRect, 2);
		LONG bottom = lpRect->top;
		GetClientRect(lpRect);
		lpRect->bottom = bottom;
	}
	else
		::SetRectEmpty(lpRect);
}

int CResizableSheetEx::GetMinWidth()
{
	CRect rectWnd, rectSheet;
	GetTotalClientRect(&rectSheet);

	int max = 0, min = rectSheet.Width();
	// search for leftmost and rightmost button margins
	for (int i = 0; i < 7; ++i)
	{
		const CWnd* pWnd = GetDlgItem(_propButtons[i]);
		// exclude not present or hidden buttons
		if (pWnd != NULL && (pWnd->GetStyle() & WS_VISIBLE))
		{
			// left position is relative to the right border
			// of the parent window (negative value)
			pWnd->GetWindowRect(&rectWnd);
			::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rectWnd, 2);
			const int left = rectSheet.right - rectWnd.left;
			const int right = rectSheet.right - rectWnd.right;

			if (left > max)
				max = left;
			if (right < min)
				min = right;
		}
	}

	// sizing border width
	const int border = GetSystemMetrics(SM_CXSIZEFRAME);

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
	if (bWithPage)
	{
		LoadPage(pszSection);
		ArrangeLayout();	// needs refresh
	}
}

void CResizableSheetEx::RefreshLayout()
{
	SendMessage(WM_SIZE);
}

LRESULT CResizableSheetEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message != WM_NCCALCSIZE || wParam == 0 || !m_bLayoutDone)
		return CPropertySheetEx::WindowProc(message, wParam, lParam);

	// specifying valid rects needs controls already anchored
	LRESULT lResult = 0;
	HandleNcCalcSize(FALSE, (LPNCCALCSIZE_PARAMS)lParam, lResult);
	lResult = CPropertySheetEx::WindowProc(message, wParam, lParam);
	HandleNcCalcSize(TRUE, (LPNCCALCSIZE_PARAMS)lParam, lResult);
	return lResult;
}
