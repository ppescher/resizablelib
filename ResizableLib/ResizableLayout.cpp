// ResizableLayout.cpp: implementation of the CResizableLayout class.
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
#include "ResizableLayout.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define BS_TYPEMASK SS_TYPEMASK

void CResizableLayout::AddAnchor(HWND hWnd, CSize sizeTypeTL, CSize sizeTypeBR)
{
	CWnd* pParent = GetResizableWnd();

	// child window must be valid
	ASSERT(::IsWindow(hWnd));
	// must be child of parent window
	ASSERT(::IsChild(pParent->GetSafeHwnd(), hWnd));
	// top-left anchor must be valid
	ASSERT(sizeTypeTL != NOANCHOR);

	// get control's window class
	CString st;
	GetClassName(hWnd, st.GetBufferSetLength(MAX_PATH), MAX_PATH);
	st.ReleaseBuffer();

	// add the style 'clipsiblings' to a GroupBox
	// to avoid unnecessary repainting of controls inside
	if (st == "Button")
	{
		DWORD style = GetWindowLong(hWnd, GWL_STYLE);
		if ((style & BS_TYPEMASK) == BS_GROUPBOX)
			SetWindowLong(hWnd, GWL_STYLE, style | WS_CLIPSIBLINGS);
	}

	// window classes that don't redraw client area correctly
	// when the hor scroll pos changes due to a resizing
	BOOL bHScroll = FALSE;
	if (st == "ListBox")
		bHScroll = TRUE;

	// window classes that need refresh when resized
	BOOL bRefresh = NeedsRefresh(hWnd);

	// get parent window's rect
	CRect rectParent;
	GetTotalClientRect(&rectParent);
	// and child control's rect
	CRect rectChild;
	::GetWindowRect(hWnd, &rectChild);
	::MapWindowPoints(NULL, pParent->m_hWnd, (LPPOINT)&rectChild, 2);

	// go calculate margins
	CSize sizeMarginTL, sizeMarginBR;

	if (sizeTypeBR == NOANCHOR)
		sizeTypeBR = sizeTypeTL;
	
	// calculate margin for the top-left corner

	sizeMarginTL.cx = rectChild.left - rectParent.Width() * sizeTypeTL.cx / 100;
	sizeMarginTL.cy = rectChild.top - rectParent.Height() * sizeTypeTL.cy / 100;
	
	// calculate margin for the bottom-right corner

	sizeMarginBR.cx = rectChild.right - rectParent.Width() * sizeTypeBR.cx / 100;
	sizeMarginBR.cy = rectChild.bottom - rectParent.Height() * sizeTypeBR.cy / 100;

	// add to the map
	LayoutInfo layout(hWnd, sizeTypeTL, sizeMarginTL,
		sizeTypeBR, sizeMarginBR, bHScroll, bRefresh);
	m_mapLayout.SetAt(hWnd, layout);
}

void CResizableLayout::AddAnchorCallback(UINT nCallbackID)
{
	// one callback control cannot rely upon another callback control's
	// size and/or position (they're updated all together at the end)
	// it can however use a non-callback control, which is updated before

	// add to the list
	LayoutInfo layout;
	layout.nCallbackID = nCallbackID;
	m_listLayoutCB.AddTail(layout);
}

BOOL CResizableLayout::ArrangeLayoutCallback(LayoutInfo& /*layout*/)
{
	ASSERT(FALSE);
	// must be overridden, if callback is used
	
	return FALSE;	// no output data
}

void CResizableLayout::ArrangeLayout()
{
	// common vars
	UINT uFlags;
	LayoutInfo layout;
	CRect rectParent, rectChild;
	GetTotalClientRect(&rectParent); // get parent window's rect
	int count = m_mapLayout.GetCount();
	int countCB = m_listLayoutCB.GetCount();

	// reposition child windows
	HDWP hdwp = ::BeginDeferWindowPos(count);
	
	HWND hWnd;
	POSITION pos = m_mapLayout.GetStartPosition();
	while (pos != NULL)
	{
		// get layout info
		m_mapLayout.GetNextAssoc(pos, hWnd, layout);
		
		// calculate new child's position, size and flags for SetWindowPos
		CalcNewChildPosition(rectParent, layout, rectChild, uFlags);

		// only if size or position changed
		if ((uFlags & (SWP_NOMOVE|SWP_NOSIZE)) != (SWP_NOMOVE|SWP_NOSIZE))
		{
			hdwp = ::DeferWindowPos(hdwp, layout.hWnd, NULL, rectChild.left, rectChild.top,
				rectChild.Width(), rectChild.Height(), uFlags);
		}
	}
	::EndDeferWindowPos(hdwp);

	// force updating of non-callback windows
	// since the others may need it

	// reposition callback windows
	hdwp = ::BeginDeferWindowPos(countCB);

	pos = m_listLayoutCB.GetHeadPosition();
	while (pos != NULL)
	{
		// get layout info
		layout = m_listLayoutCB.GetNext(pos);
		// request data
		if (!ArrangeLayoutCallback(layout))
			continue;

		// calculate new child's position, size and flags for SetWindowPos
		CalcNewChildPosition(rectParent, layout, rectChild, uFlags);

		// only if size or position changed
		if ((uFlags & (SWP_NOMOVE|SWP_NOSIZE)) != (SWP_NOMOVE|SWP_NOSIZE))
		{
			hdwp = ::DeferWindowPos(hdwp, layout.hWnd, NULL, rectChild.left, rectChild.top,
				rectChild.Width(), rectChild.Height(), uFlags);
		}
	}
	::EndDeferWindowPos(hdwp);
}

void CResizableLayout::ClipChildWindow(HWND hWnd, CRgn* pRegion)
{
	// obtain window position
	CRect rect;
	::GetWindowRect(hWnd, &rect);
	::MapWindowPoints(NULL, GetResizableWnd()->m_hWnd, (LPPOINT)&rect, 2);

	// use window region if any
	CRgn rgn;
	rgn.CreateRectRgn(0,0,0,0);
	switch (::GetWindowRgn(hWnd, rgn))
	{
	case COMPLEXREGION:
	case SIMPLEREGION:
		rgn.OffsetRgn(rect.TopLeft());
		break;

	default:
		rgn.SetRectRgn(&rect);
	}

	// go clipping?
	if (LikesClipping(hWnd))
		pRegion->CombineRgn(pRegion, &rgn, RGN_DIFF);
	else
		pRegion->CombineRgn(pRegion, &rgn, RGN_OR);
}

void CResizableLayout::GetClippingRegion(CRgn* pRegion)
{
	CWnd* pWnd = GetResizableWnd();

	// System's default clipping area is screen's size,
	// not enough for max track size:
	// if screen is 1024 x 768 and resizing border is 4 pixels,
	// maximized size is 1024+4*2=1032 x 768+4*2=776,
	// but max track size is 4 pixels bigger 1036 x 780 (don't ask me why!)
	// So, if you resize the window to maximum size, the last 4 pixels
	// are clipped out by the default clipping region, that gets created
	// as soon as you call clipping functions (my guess).

	// reset clipping region to the whole client area
	CRect rect;
	pWnd->GetClientRect(&rect);
	pRegion->CreateRectRgnIndirect(&rect);

	// clip only anchored controls
	LayoutInfo layout;
	HWND hWnd;
	POSITION pos = m_mapLayout.GetStartPosition();
	while (pos != NULL)
	{
		// get layout info
		m_mapLayout.GetNextAssoc(pos, hWnd, layout);
		
		if (::IsWindowVisible(layout.hWnd))
			ClipChildWindow(layout.hWnd, pRegion);
	}
	pos = m_listLayoutCB.GetHeadPosition();
	while (pos != NULL)
	{
		// get layout info
		layout = m_listLayoutCB.GetNext(pos);
		// request data
		if (!ArrangeLayoutCallback(layout))
			continue;

		if (::IsWindowVisible(layout.hWnd))
			ClipChildWindow(layout.hWnd, pRegion);
	}

	// fix for RTL layouts (1 pixel too-much)
	if (pWnd->GetExStyle() & WS_EX_LAYOUTRTL)
		pRegion->OffsetRgn(-1,0);
}

void CResizableLayout::EraseBackground(CDC* pDC)
{
	HWND hWnd = GetResizableWnd()->GetSafeHwnd();

	// retrieve the background brush
	HBRUSH hBrush = NULL;
	// is this a dialog box?
	ATOM atomWndClass = (ATOM)::GetClassLong(hWnd, GCW_ATOM);
	if (WC_DIALOG == MAKEINTATOM(atomWndClass))
	{
		// send a message to the dialog box
		hBrush = (HBRUSH)::SendMessage(hWnd, WM_CTLCOLORDLG,
			(WPARAM)pDC->GetSafeHdc(), (LPARAM)hWnd);
	}
	else
	{
		// take the background brush from the window's class
		hBrush = (HBRUSH)::GetClassLong(hWnd, GCL_HBRBACKGROUND);
	}

	CRgn rgn;
	GetClippingRegion(&rgn);

	::FillRgn(pDC->GetSafeHdc(), rgn, hBrush);
}

// support legacy code (will disappear in future versions)
void CResizableLayout::ClipChildren(CDC* pDC)
{
	CRgn rgn;
	GetClippingRegion(&rgn);
	// the clipping region is in device units
	rgn.OffsetRgn(-pDC->GetWindowOrg());
	pDC->SelectClipRgn(&rgn);
}

void CResizableLayout::GetTotalClientRect(LPRECT lpRect)
{
	GetResizableWnd()->GetClientRect(lpRect);
}

BOOL CResizableLayout::NeedsRefresh(HWND hWnd)
{
	// get window class
	CString str;
	::GetClassName(hWnd, str.GetBufferSetLength(MAX_PATH), MAX_PATH);
	str.ReleaseBuffer();

	// optimistic, no need to refresh
	BOOL bRefresh = FALSE;

	// window classes that need refresh when resized
	if (str == "Static")
	{
		DWORD style = ::GetWindowLong(hWnd, GWL_STYLE);

		switch (style & SS_TYPEMASK)
		{
		case SS_LEFT:
		case SS_CENTER:
		case SS_RIGHT:
		case SS_ENHMETAFILE:
		case SS_BITMAP:
			// word-wrapped text needs bRefresh
			bRefresh = TRUE;
			break;

		case SS_ICON:
		case SS_SIMPLE:
		case SS_ETCHEDHORZ:
		case SS_ETCHEDVERT:
			break;

		default:
			// centered images or text need bRefresh
			if (style & SS_CENTERIMAGE)
				bRefresh = TRUE;
		}
	}

	return bRefresh;
}

BOOL CResizableLayout::LikesClipping(HWND hWnd)
{
	// check child type
	CString str;
	::GetClassName(hWnd, str.GetBufferSetLength(MAX_PATH), MAX_PATH);
	str.ReleaseBuffer();

	DWORD style = ::GetWindowLong(hWnd, GWL_STYLE);

	// skip windows that wants background repainted
	if (str == TOOLBARCLASSNAME && (style & TBSTYLE_TRANSPARENT))
		return FALSE;
	if (str == "Button" && (style & BS_TYPEMASK) == BS_GROUPBOX)
		return FALSE;
	if (str == "Static")
	{
		switch (style & SS_TYPEMASK)
		{
		case SS_BLACKRECT:
		case SS_GRAYRECT:
		case SS_WHITERECT:
		case SS_ETCHEDHORZ:
		case SS_ETCHEDVERT:
		case SS_BITMAP:
			break;
		case SS_ICON:
		case SS_ENHMETAFILE:
			if (style & SS_CENTERIMAGE)
				return FALSE;
			break;
		default:
			return FALSE;
		}
	}

	// assume the others like clipping
	return TRUE;
}

void CResizableLayout::CalcNewChildPosition(const CRect &rectParent, CResizableLayout::LayoutInfo &layout, CRect &rectChild, UINT& uFlags)
{
	CWnd* pParent = GetResizableWnd();

	CRect rectNew;
	CWnd* pWnd = CWnd::FromHandle(layout.hWnd); // temporary solution

	pWnd->GetWindowRect(&rectChild);
	::MapWindowPoints(NULL, pParent->m_hWnd, (LPPOINT)&rectChild, 2);
	
	// calculate new top-left corner
	rectNew.left = layout.sizeMarginTL.cx + rectParent.Width() * layout.sizeTypeTL.cx / 100;
	rectNew.top = layout.sizeMarginTL.cy + rectParent.Height() * layout.sizeTypeTL.cy / 100;
	
	// calculate new bottom-right corner
	rectNew.right = layout.sizeMarginBR.cx + rectParent.Width() * layout.sizeTypeBR.cx / 100;
	rectNew.bottom = layout.sizeMarginBR.cy + rectParent.Height() * layout.sizeTypeBR.cy / 100;

	// fix for horizontally scrollable windows
	if (layout.bAdjHScroll && !rectNew.EqualRect(&rectChild))
	{
		int diff = rectNew.Width() - rectChild.Width();
		int max = pWnd->GetScrollLimit(SB_HORZ);
	
		layout.bNeedRefresh = FALSE;
		if (max > 0 && pWnd->GetScrollPos(SB_HORZ) > max - diff)
		{
			// needs repainting, due to horiz scrolling
			layout.bNeedRefresh = TRUE;
		}
	}

	// set flags 
	uFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION;
	if (layout.bNeedRefresh)
		uFlags |= SWP_NOCOPYBITS;
	if (rectNew.TopLeft() == rectChild.TopLeft())
		uFlags |= SWP_NOMOVE;
	if (rectNew.Size() == rectChild.Size())
		uFlags |= SWP_NOSIZE;

	// update rect
	rectChild = rectNew;
}
