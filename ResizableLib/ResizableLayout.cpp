// ResizableLayout.cpp: implementation of the CResizableLayout class.
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
#include "ResizableLayout.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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
	st.MakeUpper();

	// add the style 'clipsiblings' to a GroupBox
	// to avoid unnecessary repainting of controls inside
	if (st == "BUTTON")
	{
		DWORD style = GetWindowLong(hWnd, GWL_STYLE);
		if ((style & 0x0FL) == BS_GROUPBOX)
			SetWindowLong(hWnd, GWL_STYLE, style | WS_CLIPSIBLINGS);
	}

	// window classes that don't redraw client area correctly
	// when the hor scroll pos changes due to a resizing
	BOOL hscroll = FALSE;
	if (st == "LISTBOX")
		hscroll = TRUE;

	// window classes that need refresh when resized
	BOOL refresh = NeedsRefresh(hWnd);

	// get parent window's rect
	CRect rectParent;
	GetTotalClientRect(&rectParent);
	// and child control's rect
	CRect rectChild;
	::GetWindowRect(hWnd, &rectChild);
	pParent->ScreenToClient(&rectChild);

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

	// add to the list
	LayoutInfo layout(hWnd, sizeTypeTL, sizeMarginTL,
		sizeTypeBR, sizeMarginBR, hscroll, refresh);
	// always add to head (before callbacks)
	m_arrLayout.InsertAt(0, layout);
}

// one callback control cannot rely upon another callback control's
// size and/or position (they're updated all together at the end)
void CResizableLayout::AddAnchorCallback(UINT nCallbackID)
{
	// add to the list
	LayoutInfo layout;
	layout.nCallbackID = nCallbackID;
	// callbacks added to tail (for efficiency)
	m_arrLayout.Add(layout);
}

BOOL CResizableLayout::ArrangeLayoutCallback(LayoutInfo& /*layout*/)
{
	ASSERT(FALSE);
	// must be overridden, if callback is used
	
	return FALSE;	// no output data
}

void CResizableLayout::ArrangeLayout()
{
	CWnd* pParent = GetResizableWnd();

	// get parent window's rect
	CRect rectParent;
	GetTotalClientRect(&rectParent);

	// init some vars
	BOOL bCallbackPassed = FALSE;
	int i, count = m_arrLayout.GetSize();
	HDWP hdwp = BeginDeferWindowPos(count);

	for (i=0; i<count; ++i)
	{
		LayoutInfo layout = m_arrLayout[i];
		
		if (layout.hWnd == NULL)	// callback
		{
			if (!bCallbackPassed)	// first time only
			{
				bCallbackPassed = TRUE;
				// update previous controls
				EndDeferWindowPos(hdwp);
				// start again for callback controls
				hdwp = BeginDeferWindowPos(count-i);
			}
			// callbacks are added at the end, so that
			// you don't have multiple screen updates

			if (!ArrangeLayoutCallback(layout))	// request data
				continue;
		}

		CRect rectChild, newrc;
		CWnd* pWnd = CWnd::FromHandle(layout.hWnd); // temporary solution

		pWnd->GetWindowRect(&rectChild);
		pParent->ScreenToClient(&rectChild);
		
		// calculate new top-left corner

		newrc.left = layout.sizeMarginTL.cx + rectParent.Width() * layout.sizeTypeTL.cx / 100;
		newrc.top = layout.sizeMarginTL.cy + rectParent.Height() * layout.sizeTypeTL.cy / 100;
		
		// calculate new bottom-right corner

		newrc.right = layout.sizeMarginBR.cx + rectParent.Width() * layout.sizeTypeBR.cx / 100;
		newrc.bottom = layout.sizeMarginBR.cy + rectParent.Height() * layout.sizeTypeBR.cy / 100;

		if (!newrc.EqualRect(&rectChild))
		{
			if (layout.bAdjHScroll)
			{
				// needs repainting, due to horiz scrolling
				int diff = newrc.Width() - rectChild.Width();
				int max = pWnd->GetScrollLimit(SB_HORZ);
			
				layout.bNeedRefresh = FALSE;
				if (max > 0 && pWnd->GetScrollPos(SB_HORZ) > max - diff)
				{
					layout.bNeedRefresh = TRUE;
				}
			}

			// set flags 
			DWORD flags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION;
			//if (layout.bNeedRefresh)
			//	flags |= SWP_NOCOPYBITS;
			if (newrc.TopLeft() == rectChild.TopLeft())
				flags |= SWP_NOMOVE;
			if (newrc.Size() == rectChild.Size())
				flags |= SWP_NOSIZE;
			
			hdwp = DeferWindowPos(hdwp, layout.hWnd, NULL, newrc.left, newrc.top,
				newrc.Width(), newrc.Height(), flags);
		}
	}
	// go re-arrange child windows
	EndDeferWindowPos(hdwp);

	// refresh those that need
	for (i=0; i<count; ++i)
	{
		LayoutInfo& layout = m_arrLayout[i];
	
		if (layout.bNeedRefresh)
		{
			::RedrawWindow(layout.hWnd, NULL, NULL, RDW_INVALIDATE|RDW_NOFRAME);
		//	::InvalidateRect(layout.hWnd, NULL, TRUE);
		//	::UpdateWindow(layout.hWnd);
		}
	}
}

BOOL CResizableLayout::EnumAndClipChildWindow(HWND hWnd, LPARAM lParam)
{
	CResizableLayout* pThis = (CResizableLayout*)lParam;

	// only direct children
	if (::GetParent(hWnd) != pThis->GetResizableWnd()->GetSafeHwnd())
		return TRUE;

	if (!::IsWindowVisible(hWnd))
		return TRUE;	// skip invisible windows

	// go clipping?
	if (!pThis->LikesClipping(hWnd))
		return TRUE;

	// obtain window position
	CRect rect;
	::GetWindowRect(hWnd, &rect);
	pThis->GetResizableWnd()->ScreenToClient(&rect);

	// use window region if any
	CRgn rgn;
	rgn.CreateRectRgn(0,0,0,0);
	if (COMPLEXREGION == ::GetWindowRgn(hWnd, rgn))
	{
		rgn.OffsetRgn(rect.TopLeft());
		pThis->m_pClipDC->SelectClipRgn(&rgn, RGN_DIFF);
	}
	else
	{
		pThis->m_pClipDC->ExcludeClipRect(&rect);
	}

	return TRUE;
}

void CResizableLayout::ClipChildren(CDC *pDC, BOOL bOnlyAnchored)
{
	m_pClipDC = pDC;

	if (bOnlyAnchored)
	{
		for (int i=0; i<m_arrLayout.GetSize(); ++i)
			if (m_arrLayout[i].hWnd != NULL)
				EnumAndClipChildWindow(m_arrLayout[i].hWnd, (LPARAM)this);
	}
	else
		EnumChildWindows(GetResizableWnd()->GetSafeHwnd(),
			EnumAndClipChildWindow, (LPARAM)this);

	m_pClipDC = NULL; // just to be sure
}

void CResizableLayout::GetTotalClientRect(LPRECT lpRect)
{
	GetResizableWnd()->GetClientRect(lpRect);
}

BOOL CResizableLayout::NeedsRefresh(HWND hWnd)
{
	// get window class
	CString st;
	GetClassName(hWnd, st.GetBufferSetLength(MAX_PATH), MAX_PATH);
	st.ReleaseBuffer();
	st.MakeUpper();

	// window classes that need refresh when resized
	BOOL refresh = FALSE;
	if (st == "STATIC")
	{
		DWORD style = GetWindowLong(hWnd, GWL_STYLE);

		switch (style & SS_TYPEMASK)
		{
		case SS_LEFT:
		case SS_CENTER:
		case SS_RIGHT:
		case SS_ENHMETAFILE:
			// word-wrapped text needs refresh
			refresh = TRUE;
			break;

		case SS_ICON:
		case SS_SIMPLE:
		case SS_ETCHEDHORZ:
		case SS_ETCHEDVERT:
			break;

		default:
			// centered images or text need refresh
			if (style & SS_CENTERIMAGE)
				refresh = TRUE;
		}
	}

	return refresh;
}

BOOL CResizableLayout::LikesClipping(HWND hWnd)
{
	// check child type
	CString st;
	GetClassName(hWnd, st.GetBufferSetLength(MAX_PATH), MAX_PATH);
	st.ReleaseBuffer();
	st.MakeUpper();

	DWORD style = GetWindowLong(hWnd, GWL_STYLE);

	// skip windows that wants background repainted
	if (st == "BUTTON" && (style & 0x0FL) == BS_GROUPBOX)
		return FALSE;
	if (st == "STATIC")
	{
		switch (style & SS_TYPEMASK)
		{
		case SS_BLACKRECT:
		case SS_GRAYRECT:
		case SS_WHITERECT:
		case SS_ETCHEDHORZ:
		case SS_ETCHEDVERT:
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
	return TRUE;
}
