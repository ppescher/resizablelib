// ResizableLayout.cpp: implementation of the CResizableLayout class.
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000-2002 by Paolo Messina
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

// In August 2002 Platform SDK, some guy at MS thought it was time to
// add the missing symbol BS_TYPEMASK, but forgot its original meaning
// and so now he's telling us not to use that symbol because its
// value is likely to change in the future SDK releases, including all
// the BS_* style bits in the mask, not just the button's type as the
// symbol's name suggests. So now we're forced to use another symbol!
#define _BS_TYPEMASK 0x0000000FL

void CResizableLayout::AddAnchor(HWND hWnd, ANCHOR sizeTypeTL, ANCHOR sizeTypeBR)
{
	CWnd* pParent = GetResizableWnd();

	// child window must be valid
	ASSERT(::IsWindow(hWnd));
	// must be child of parent window
	ASSERT(::IsChild(pParent->GetSafeHwnd(), hWnd));

	// get parent window's rect
	CRect rectParent;
	GetTotalClientRect(&rectParent);
	// and child control's rect
	CRect rectChild;
	::GetWindowRect(hWnd, &rectChild);
	::MapWindowPoints(NULL, pParent->m_hWnd, (LPPOINT)&rectChild, 2);

	// adjust position, if client area has been scrolled
	rectChild.OffsetRect(-rectParent.TopLeft());

	// go calculate margins
	CSize sizeMarginTL, sizeMarginBR;

	// calculate margin for the top-left corner

	sizeMarginTL.cx = rectChild.left - rectParent.Width() * sizeTypeTL.cx / 100;
	sizeMarginTL.cy = rectChild.top - rectParent.Height() * sizeTypeTL.cy / 100;
	
	// calculate margin for the bottom-right corner

	sizeMarginBR.cx = rectChild.right - rectParent.Width() * sizeTypeBR.cx / 100;
	sizeMarginBR.cy = rectChild.bottom - rectParent.Height() * sizeTypeBR.cy / 100;

	// prepare the structure
	LayoutInfo layout(hWnd, sizeTypeTL, sizeMarginTL,
		sizeTypeBR, sizeMarginBR);

	// get control's window class
	GetClassName(hWnd, layout.sWndClass, MAX_PATH);

	// initialize resize properties (overridable)
	InitResizeProperties(layout);

	// must not be already there!
	// (this is probably due to a duplicate call to AddAnchor)
	POSITION pos;
	ASSERT(!m_mapLayout.Lookup(hWnd, pos));

	// add to the list and the map
	pos = m_listLayout.AddTail(layout);
	m_mapLayout.SetAt(hWnd, pos);
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

BOOL CResizableLayout::ArrangeLayoutCallback(CResizableLayout::LayoutInfo& /*layout*/) const
{
	ASSERT(FALSE);
	// must be overridden, if callback is used
	
	return FALSE;	// no output data
}

void CResizableLayout::ArrangeLayout() const
{
	// common vars
	UINT uFlags;
	LayoutInfo layout;
	CRect rectParent, rectChild;
	int count = m_listLayout.GetCount();
	int countCB = m_listLayoutCB.GetCount();

	if (count + countCB == 0)
		return;

	// get parent window's rect
	GetTotalClientRect(&rectParent);

	// reposition child windows
	HDWP hdwp = ::BeginDeferWindowPos(count + countCB);
	
	POSITION pos = m_listLayout.GetHeadPosition();
	while (pos != NULL)
	{
		// get layout info
		layout = m_listLayout.GetNext(pos);
		
		// calculate new child's position, size and flags for SetWindowPos
		CalcNewChildPosition(layout, rectParent, rectChild, uFlags);

		// only if size or position changed
		if ((uFlags & (SWP_NOMOVE|SWP_NOSIZE)) != (SWP_NOMOVE|SWP_NOSIZE))
		{
			hdwp = ::DeferWindowPos(hdwp, layout.hWnd, NULL, rectChild.left,
				rectChild.top, rectChild.Width(), rectChild.Height(), uFlags);
		}
	}

	// for callback items you may use GetAnchorPosition to know the
	// new position and size of a non-callback item after resizing

	pos = m_listLayoutCB.GetHeadPosition();
	while (pos != NULL)
	{
		// get layout info
		layout = m_listLayoutCB.GetNext(pos);
		// request layout data
		if (!ArrangeLayoutCallback(layout))
			continue;

		// calculate new child's position, size and flags for SetWindowPos
		CalcNewChildPosition(layout, rectParent, rectChild, uFlags);

		// only if size or position changed
		if ((uFlags & (SWP_NOMOVE|SWP_NOSIZE)) != (SWP_NOMOVE|SWP_NOSIZE))
		{
			hdwp = ::DeferWindowPos(hdwp, layout.hWnd, NULL, rectChild.left,
				rectChild.top, rectChild.Width(), rectChild.Height(), uFlags);
		}
	}

	// finally move all the windows at once
	::EndDeferWindowPos(hdwp);
}

void CResizableLayout::ClipChildWindow(const CResizableLayout::LayoutInfo& layout,
									   CRgn* pRegion) const
{
	// obtain window position
	CRect rect;
	::GetWindowRect(layout.hWnd, &rect);
	::MapWindowPoints(NULL, GetResizableWnd()->m_hWnd, (LPPOINT)&rect, 2);

	// use window region if any
	CRgn rgn;
	rgn.CreateRectRgn(0,0,0,0);
	switch (::GetWindowRgn(layout.hWnd, rgn))
	{
	case COMPLEXREGION:
	case SIMPLEREGION:
		rgn.OffsetRgn(rect.TopLeft());
		break;

	default:
		rgn.SetRectRgn(&rect);
	}

	// get the clipping property
	BOOL bClipping = layout.properties.bAskClipping ?
		LikesClipping(layout) : layout.properties.bCachedLikesClipping;

	// modify region accordingly
	if (bClipping)
		pRegion->CombineRgn(pRegion, &rgn, RGN_DIFF);
	else
		pRegion->CombineRgn(pRegion, &rgn, RGN_OR);
}

void CResizableLayout::GetClippingRegion(CRgn* pRegion) const
{
	CWnd* pWnd = GetResizableWnd();

	// System's default clipping area is screen's size,
	// not enough for max track size, for example:
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
	POSITION pos = m_listLayout.GetHeadPosition();
	while (pos != NULL)
	{
		// get layout info
		layout = m_listLayout.GetNext(pos);
		
		if (::IsWindowVisible(layout.hWnd))
			ClipChildWindow(layout, pRegion);
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
			ClipChildWindow(layout, pRegion);
	}

	// fix for RTL layouts (1 pixel of horz offset)
	if (pWnd->GetExStyle() & WS_EX_LAYOUTRTL)
		pRegion->OffsetRgn(-1,0);
}

// enable/restore clipping on the specified DC when appropriate
void CResizableLayout::ClipChildren(CDC* pDC, BOOL bUndo)
{
	HDC hDC = pDC->GetSafeHdc();
	HWND hWnd = GetResizableWnd()->GetSafeHwnd();

	m_nOldClipRgn = -1; // invalid region by default

	// Some controls (such as transparent toolbars and standard controls
	// with XP theme enabled) send a WM_ERASEBKGND msg to the parent
	// to draw themselves, in which case we must not enable clipping.

	// We check that the window associated with the DC is the
	// resizable window and not a child control.

	if (!bUndo && (hWnd == ::WindowFromDC(hDC)))
	{
		// save old DC clipping region
		m_nOldClipRgn = ::GetClipRgn(hDC, m_hOldClipRgn);

		// clip out supported child windows
		CRgn rgnClip;
		GetClippingRegion(&rgnClip);
		::ExtSelectClipRgn(hDC, rgnClip, RGN_AND);
	}

	// restore old clipping region, only if modified and valid
	if (bUndo && m_nOldClipRgn >= 0)
	{
		if (m_nOldClipRgn == 1)
			::SelectClipRgn(hDC, m_hOldClipRgn);
		else
			::SelectClipRgn(hDC, NULL);
	}
}

void CResizableLayout::GetTotalClientRect(LPRECT lpRect) const
{
	GetResizableWnd()->GetClientRect(lpRect);
}

BOOL CResizableLayout::NeedsRefresh(const CResizableLayout::LayoutInfo& layout,
								const CRect& rectOld, const CRect& rectNew) const
{
	if (layout.bMsgSupport)
	{
		REFRESHPROPERTY refresh;
		refresh.rcOld = rectOld;
		refresh.rcNew = rectNew;
		if (Send_NeedsRefresh(layout.hWnd, &refresh))
			return refresh.bNeedsRefresh;
	}

	int nDiffWidth = (rectNew.Width() - rectOld.Width());
	int nDiffHeight = (rectNew.Height() - rectOld.Height());

	// is the same size?
	if (nDiffWidth == 0 && nDiffHeight == 0)
		return FALSE;

	// optimistic, no need to refresh
	BOOL bRefresh = FALSE;

	// window classes that need refresh when resized
	if (0 == lstrcmp(layout.sWndClass, WC_STATIC))
	{
		DWORD style = ::GetWindowLong(layout.hWnd, GWL_STYLE);

		switch (style & SS_TYPEMASK)
		{
		case SS_LEFT:
		case SS_CENTER:
		case SS_RIGHT:
			// word-wrapped text
			bRefresh = bRefresh || (nDiffWidth != 0);
			// vertically centered text
			if (style & SS_CENTERIMAGE)
				bRefresh = bRefresh || (nDiffHeight != 0);
			break;

		case SS_LEFTNOWORDWRAP:
			// text with ellipsis
			if (style & SS_ELLIPSISMASK)
				bRefresh = bRefresh || (nDiffWidth != 0);
			// vertically centered text
			if (style & SS_CENTERIMAGE)
				bRefresh = bRefresh || (nDiffHeight != 0);
			break;

		case SS_ENHMETAFILE:
		case SS_BITMAP:
		case SS_ICON:
			// images
		case SS_BLACKFRAME:
		case SS_GRAYFRAME:
		case SS_WHITEFRAME:
		case SS_ETCHEDFRAME:
			// and frames
			bRefresh = TRUE;
			break;
		}
	}

	// window classes that don't redraw client area correctly
	// when the hor scroll pos changes due to a resizing
	BOOL bHScroll = FALSE;
	if (0 == lstrcmp(layout.sWndClass, WC_LISTBOX))
		bHScroll = TRUE;

	// fix for horizontally scrollable windows
	if (bHScroll && (nDiffWidth > 0))
	{
		// get max scroll position
		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
		if (::GetScrollInfo(layout.hWnd, SB_HORZ, &info))
		{
			// subtract the page size
			info.nMax -= __max(info.nPage-1,0);
		}

		// resizing will cause the text to scroll on the right
		// because the scrollbar is going beyond the right limit
		if ((info.nMax > 0) && (info.nPos + nDiffWidth > info.nMax))
		{
			// needs repainting, due to horiz scrolling
			bRefresh = TRUE;
		}
	}

	return bRefresh;
}

BOOL CResizableLayout::LikesClipping(const CResizableLayout::LayoutInfo& layout) const
{
	if (layout.bMsgSupport)
	{
		CLIPPINGPROPERTY clipping;
		if (Send_LikesClipping(layout.hWnd, &clipping))
			return clipping.bLikesClipping;
	}

	DWORD style = ::GetWindowLong(layout.hWnd, GWL_STYLE);

	// skip windows that wants background repainted
	if (0 == lstrcmp(layout.sWndClass, WC_BUTTON))
	{
		CRect rect;
		switch (style & _BS_TYPEMASK)
		{
		case BS_GROUPBOX:
			return FALSE;

		case BS_OWNERDRAW:
			// ownerdraw buttons must return correct hittest code
			// to notify their transparency to the system and this library
			::GetWindowRect(layout.hWnd, &rect);
			::SendMessage(layout.hWnd, WM_NCCALCSIZE, FALSE, (LPARAM)&rect);
			if ( HTTRANSPARENT == ::SendMessage(layout.hWnd,
				WM_NCHITTEST, 0, MAKELPARAM(rect.left, rect.top)) )
				return FALSE;
			break;
		}
		return TRUE;
	}
	else if (0 == lstrcmp(layout.sWndClass, WC_STATIC))
	{
		switch (style & SS_TYPEMASK)
		{
		case SS_LEFT:
		case SS_CENTER:
		case SS_RIGHT:
		case SS_SIMPLE:
		case SS_LEFTNOWORDWRAP:
			// text
		case SS_BLACKRECT:
		case SS_GRAYRECT:
		case SS_WHITERECT:
			// filled rects
		case SS_ETCHEDHORZ:
		case SS_ETCHEDVERT:
			// etched lines
		case SS_BITMAP:
			// bitmaps
			return TRUE;
			break;

		case SS_ICON:
		case SS_ENHMETAFILE:
			if (style & SS_CENTERIMAGE)
				return FALSE;
			return TRUE;
			break;

		default:
			return FALSE;
		}
	}

	// assume the others like clipping
	return TRUE;
}

void CResizableLayout::CalcNewChildPosition(const CResizableLayout::LayoutInfo& layout,
						const CRect &rectParent, CRect &rectChild, UINT& uFlags) const
{
	CWnd* pParent = GetResizableWnd();

	::GetWindowRect(layout.hWnd, &rectChild);
	::MapWindowPoints(NULL, pParent->m_hWnd, (LPPOINT)&rectChild, 2);
	
	CRect rectNew;

	// calculate new top-left corner
	rectNew.left = layout.sizeMarginTL.cx + rectParent.Width() * layout.sizeTypeTL.cx / 100;
	rectNew.top = layout.sizeMarginTL.cy + rectParent.Height() * layout.sizeTypeTL.cy / 100;
	
	// calculate new bottom-right corner
	rectNew.right = layout.sizeMarginBR.cx + rectParent.Width() * layout.sizeTypeBR.cx / 100;
	rectNew.bottom = layout.sizeMarginBR.cy + rectParent.Height() * layout.sizeTypeBR.cy / 100;

	// adjust position, if client area has been scrolled
	rectNew.OffsetRect(rectParent.TopLeft());

	// get the refresh property
	BOOL bRefresh = layout.properties.bAskRefresh ?
		NeedsRefresh(layout, rectChild, rectNew) : layout.properties.bCachedNeedsRefresh;

	// set flags 
	uFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION;
	if (bRefresh)
		uFlags |= SWP_NOCOPYBITS;
	if (rectNew.TopLeft() == rectChild.TopLeft())
		uFlags |= SWP_NOMOVE;
	if (rectNew.Size() == rectChild.Size())
		uFlags |= SWP_NOSIZE;

	// update rect
	rectChild = rectNew;
}

BOOL CResizableLayout::GetAnchorMargins(HWND hWnd, const CSize &sizeChild, CRect &rectMargins) const
{
	POSITION pos;
	if (!m_mapLayout.Lookup(hWnd, pos))
		return FALSE;

	const CResizableLayout::LayoutInfo& layout = m_listLayout.GetAt(pos);

	// augmented size, relative to anchor points
	CSize size = sizeChild + layout.sizeMarginTL - layout.sizeMarginBR;

	// percent of parent size occupied by this control
	CSize percent(layout.sizeTypeBR.cx - layout.sizeTypeTL.cx,
		layout.sizeTypeBR.cy - layout.sizeTypeTL.cy);

	// calculate total margins
	rectMargins.left = size.cx * layout.sizeTypeTL.cx / percent.cx + layout.sizeMarginTL.cx;
	rectMargins.top = size.cy * layout.sizeTypeTL.cy / percent.cy + layout.sizeMarginTL.cy;
	rectMargins.right = size.cx * (100 - layout.sizeTypeBR.cx) / percent.cx - layout.sizeMarginBR.cx;
	rectMargins.bottom = size.cy * (100 - layout.sizeTypeBR.cy) / percent.cy - layout.sizeMarginBR.cy;

	return TRUE;
}

void CResizableLayout::InitResizeProperties(CResizableLayout::LayoutInfo &layout) const
{
	// check if custom window supports this library
	// (properties must be correctly set by the window)
	layout.bMsgSupport = Send_QueryProperties(layout.hWnd, &layout.properties);

	// default properties
	if (!layout.bMsgSupport)
	{
		// clipping property is assumed as static
		layout.properties.bAskClipping = FALSE;
		layout.properties.bCachedLikesClipping = LikesClipping(layout);
		// refresh property is assumed as dynamic
		layout.properties.bAskRefresh = TRUE;
	}
}
