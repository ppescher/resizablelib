// ResizableDialogEx.cpp : implementation file
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
#include "ResizableDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableDialogEx

inline void CResizableDialogEx::PrivateConstruct()
{
	m_bEnableSaveRestore = FALSE;
	m_dwGripTempState = 1;
	m_bRectOnly = FALSE;
	m_bRestoreAfterVisible = TRUE;
}

CResizableDialogEx::CResizableDialogEx()
{
	PrivateConstruct();
}

CResizableDialogEx::CResizableDialogEx(UINT nIDTemplate, CWnd* pParentWnd)
	: CDialogEx(nIDTemplate, pParentWnd)
{
	PrivateConstruct();
}

CResizableDialogEx::CResizableDialogEx(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
	: CDialogEx(lpszTemplateName, pParentWnd)
{
	PrivateConstruct();
}

CResizableDialogEx::~CResizableDialogEx()
{
}

void CResizableDialogEx::SetBackgroundImage(HBITMAP hBitmap, BackgroundLocation location, BOOL bAutoDestroy, BOOL bRepaint)
{
	CDialogEx::SetBackgroundImage(hBitmap, location, bAutoDestroy, bRepaint);
	if (m_hBkgrBitmap)
		m_brushBack.CreatePatternBrush(CBitmap::FromHandle(m_hBkgrBitmap));
	else
		m_brushBack.CreateSysColorBrush(COLOR_3DFACE);
}

BOOL CResizableDialogEx::SetBackgroundImage(UINT uiBmpResId, BackgroundLocation location, BOOL bRepaint)
{
	if (CDialogEx::SetBackgroundImage(uiBmpResId, location, bRepaint))
	{
		m_brushBack.CreatePatternBrush(CBitmap::FromHandle(m_hBkgrBitmap));
		return TRUE;
	}
	m_brushBack.CreateSysColorBrush(COLOR_3DFACE);
	return FALSE;
}


BEGIN_MESSAGE_MAP(CResizableDialogEx, CDialogEx)
	//{{AFX_MSG_MAP(CResizableDialogEx)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_NCCREATE()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableDialogEx message handlers

BOOL CResizableDialogEx::OnNcCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (!CDialogEx::OnNcCreate(lpCreateStruct))
		return FALSE;

	// child dialogs don't want resizable border or size grip,
	// nor they can handle the min/max size constraints
	BOOL bChild = lpCreateStruct->style & WS_CHILD;

	// create and init the size-grip
	if (!CreateSizeGrip(!bChild))
		return FALSE;

	// Moved from behind if (!bChild) because user could resize the dialog smaller as in resource defined and that causes some static text to be clipped or dissapear.
	MakeResizable(lpCreateStruct);

	if (!bChild)
	{
		// set the initial size as the min track size
		SetMinTrackSize(CSize(lpCreateStruct->cx, lpCreateStruct->cy));
	}
	
	return TRUE;
}

void CResizableDialogEx::OnDestroy() 
{
	if (m_bEnableSaveRestore)
		SaveWindowRect(m_sSection, m_bRectOnly);

	// reset instance data
	RemoveAllAnchors();
	ResetAllRects();
	PrivateConstruct();

	CDialogEx::OnDestroy();
}

void CResizableDialogEx::OnSize(UINT nType, int cx, int cy) 
{
	CDialogEx::OnSize(nType, cx, cy);
	
	if (nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW)
		return;		// arrangement not needed

	if (nType == SIZE_MAXIMIZED)
		HideSizeGrip(&m_dwGripTempState);
	else
		ShowSizeGrip(&m_dwGripTempState);

	// update grip and layout
	UpdateSizeGrip();
	ArrangeLayout();
}

void CResizableDialogEx::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	MinMaxInfo(lpMMI);
}

// NOTE: this must be called after setting the layout
//       to have the dialog and its controls displayed properly
void CResizableDialogEx::EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly)
{
	m_sSection = pszSection;

	m_bEnableSaveRestore = TRUE;
	m_bRectOnly = bRectOnly;

	// do not restore immediately, but only after the window is made visible
}

void CResizableDialogEx::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (m_bEnableSaveRestore && m_bRestoreAfterVisible && bShow)
	{
		// restore when the window is made visible for the first time
		m_bRestoreAfterVisible = FALSE;
		LoadWindowRect(m_sSection, m_bRectOnly);
	}
	CDialog::OnShowWindow(bShow, nStatus);
}

BOOL CResizableDialogEx::OnEraseBkgnd(CDC* pDC) 
{
	ClipChildren(pDC, FALSE);

	BOOL bRet = CDialogEx::OnEraseBkgnd(pDC);

	ClipChildren(pDC, TRUE);

	return bRet;
}

LRESULT CResizableDialogEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (m_hBkgrBitmap != NULL || message != WM_NCCALCSIZE || wParam == 0)
		return CDialogEx::WindowProc(message, wParam, lParam);

	LRESULT lResult = 0;
	HandleNcCalcSize(FALSE, (LPNCCALCSIZE_PARAMS)lParam, lResult);
	lResult = CDialogEx::WindowProc(message, wParam, lParam);
	HandleNcCalcSize(TRUE, (LPNCCALCSIZE_PARAMS)lParam, lResult);
	return lResult;
}

HBRUSH CResizableDialogEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT /*nCtlColor*/)
{
	if (m_hBkgrBitmap)
	{
		// reposition origin of background brush
		// used for transparent effect on page controls
		// (needed with double-buffering and XP themes)
		CRect rect;
		pWnd->GetWindowRect(rect);
		pWnd->SendMessage(WM_NCCALCSIZE, FALSE, (LPARAM)&rect);
		ScreenToClient(rect);
		CPoint pt(-rect.TopLeft());
		HDC hDC = pDC->GetSafeHdc();
		::LPtoDP(hDC, &pt, 1);
		::UnrealizeObject(m_brushBack);
		::SetBrushOrgEx(hDC, pt.x, pt.y, NULL);
	}
	pDC->SetBkMode(TRANSPARENT);
	return m_brushBack;
}

BOOL CResizableDialogEx::LikesClipping(const LAYOUTINFO& layout) const
{
	if (0 == lstrcmp(layout.sWndClass, WC_EDIT))
		return FALSE;
	return CResizableLayout::LikesClipping(layout);
}

BOOL CResizableDialogEx::NeedsRefresh(const LAYOUTINFO& layout,
	const CRect& rectOld, const CRect& rectNew) const
{
	if (0 == lstrcmp(layout.sWndClass, WC_BUTTON))
	{
		LONG_PTR style = ::GetWindowLongPtr(layout.hWnd, GWL_STYLE);

		switch (style & BS_TYPEMASK)
		{
		case BS_GROUPBOX:
			return TRUE;
		}
	}
	return CResizableLayout::NeedsRefresh(layout, rectOld, rectNew);
}
