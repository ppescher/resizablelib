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

void CResizableLayout::AddAnchor(HWND hWnd, CSize tl_type, CSize br_type)
{
	CWnd* pParent = GetResizableWnd();

	// child window must be valid
	ASSERT(::IsWindow(hWnd));
	// must be child of parent window
	ASSERT(::IsChild(pParent->GetSafeHwnd(), hWnd));
	// top-left anchor must be valid
	ASSERT(tl_type != NOANCHOR);

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
	BOOL refresh = FALSE;
	if (st == "STATIC")
	{
		DWORD style = GetWindowLong(hWnd, GWL_STYLE);

		switch (style & SS_TYPEMASK)
		{
		case SS_LEFT:
		case SS_CENTER:
		case SS_RIGHT:
			// word-wrapped text needs refresh
			refresh = TRUE;
		}

		// centered images or text need refresh
		if (style & SS_CENTERIMAGE)
			refresh = TRUE;

		// simple text or icon never needs refresh
		switch (style & SS_TYPEMASK)
		{
		case SS_SIMPLE:
		case SS_ICON:
			refresh = FALSE;
		}
	}

	// get parent window's rect
	CRect rectParent;
	GetTotalClientRect(&rectParent);
	// and child control's rect
	CRect rectChild;
	::GetWindowRect(hWnd, &rectChild);
	pParent->ScreenToClient(&rectChild);

	// go calculate margins
	CSize tl_margin, br_margin;

	if (br_type == NOANCHOR)
		br_type = tl_type;
	
	// calculate margin for the top-left corner

	tl_margin.cx = rectChild.left - rectParent.Width() * tl_type.cx / 100;
	tl_margin.cy = rectChild.top - rectParent.Height() * tl_type.cy / 100;
	
	// calculate margin for the bottom-right corner

	br_margin.cx = rectChild.right - rectParent.Width() * br_type.cx / 100;
	br_margin.cy = rectChild.bottom - rectParent.Height() * br_type.cy / 100;

	// add to the list
	LayoutInfo layout(hWnd, tl_type, tl_margin,
		br_type, br_margin, hscroll, refresh);
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

BOOL CResizableLayout::ArrangeLayoutCallback(LayoutInfo &layout)
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
	int i, count = m_arrLayout.GetSize();
	HDWP hdwp = BeginDeferWindowPos(count);

	for (i=0; i<count; ++i)
	{
		LayoutInfo layout = m_arrLayout[i];
		
		if (layout.hWnd == NULL)	// callback
		{
			// update previous controls
			EndDeferWindowPos(hdwp);
			hdwp = BeginDeferWindowPos(count-i+1);
// --> NEED OPTIMIZATION ^
			// callbacks should be added at the end, so that
			// you don't have multiple screen updates (max 2 times)

			if (!ArrangeLayoutCallback(layout))	// request data
				continue;
		}

		CRect rectChild, newrc;
		CWnd* pWnd = CWnd::FromHandle(layout.hWnd); // temporary solution

		pWnd->GetWindowRect(&rectChild);
		pParent->ScreenToClient(&rectChild);
		
		// calculate new top-left corner

		newrc.left = layout.tl_margin.cx + rectParent.Width() * layout.tl_type.cx / 100;
		newrc.top = layout.tl_margin.cy + rectParent.Height() * layout.tl_type.cy / 100;
		
		// calculate new bottom-right corner

		newrc.right = layout.br_margin.cx + rectParent.Width() * layout.br_type.cx / 100;
		newrc.bottom = layout.br_margin.cy + rectParent.Height() * layout.br_type.cy / 100;

		if (!newrc.EqualRect(&rectChild))
		{
			if (layout.adj_hscroll)
			{
				// needs repainting, due to horiz scrolling
				int diff = newrc.Width() - rectChild.Width();
				int max = pWnd->GetScrollLimit(SB_HORZ);
			
				layout.need_refresh = FALSE;
				if (max > 0 && pWnd->GetScrollPos(SB_HORZ) > max - diff)
				{
					layout.need_refresh = TRUE;
				}
			}

			// set flags 
			DWORD flags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION;
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
	
		if (layout.need_refresh)
		{
			::InvalidateRect(layout.hWnd, NULL, TRUE);
			::UpdateWindow(layout.hWnd);
		}
	}
}


BOOL CResizableLayout::EnumAndClipChildWindow(HWND hWnd, LPARAM lParam)
{
	CString st;
	GetClassName(hWnd, st.GetBufferSetLength(MAX_PATH), MAX_PATH);
	st.ReleaseBuffer();
	st.MakeUpper();

	DWORD style = GetWindowLong(hWnd, GWL_STYLE);

	if (st == "BUTTON" && (style & 0x0FL) == BS_GROUPBOX)
		return TRUE;
	if (st == "STATIC")
		switch (style & SS_TYPEMASK)
		{
		case SS_BLACKRECT:
		case SS_GRAYRECT:
		case SS_WHITERECT:
			break;
		case SS_ICON:
		case SS_ENHMETAFILE:
			if (style & SS_CENTERIMAGE)
				return TRUE;
			break;
		default:
			return TRUE;
		}

	// clipping
	CDC* pDC = (CDC*)lParam;
	CWnd* pParent = pDC->GetWindow();

	RECT rect;
	::GetWindowRect(hWnd, &rect);
	pParent->ScreenToClient(&rect);
	pDC->ExcludeClipRect(&rect);

	return TRUE;
}

void CResizableLayout::ClipChildren(CDC *pDC, BOOL bOnlyAnchored)
{
	if (bOnlyAnchored)
	{
		for (int i=0; i<m_arrLayout.GetSize(); ++i)
			EnumAndClipChildWindow(m_arrLayout[i].hWnd, (LPARAM)pDC);
	}
	else
		EnumChildWindows(GetResizableWnd()->GetSafeHwnd(),
			EnumAndClipChildWindow, (LPARAM)pDC);
}

void CResizableLayout::GetTotalClientRect(LPRECT lpRect)
{
	GetResizableWnd()->GetClientRect(lpRect);
}
