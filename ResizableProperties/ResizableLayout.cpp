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

void CResizableLayout::AddAnchor(HWND wnd, CSize tl_type, CSize br_type)
{
	CWnd* pParent = GetLayoutParent();

	// child window must be valid
	ASSERT(wnd != NULL && ::IsWindow(wnd));
	// must be child of parent window
	ASSERT(::IsChild(pParent->GetSafeHwnd(), wnd));
	// top-left anchor must be valid
	ASSERT(tl_type != NOANCHOR);

	// get control's window class
	
	CString st;
	GetClassName(wnd, st.GetBufferSetLength(MAX_PATH), MAX_PATH);
	st.ReleaseBuffer();
	st.MakeUpper();

	// add the style 'clipsiblings' to a GroupBox
	// to avoid unnecessary repainting of controls inside
	if (st == "BUTTON")
	{
		DWORD style = GetWindowLong(wnd, GWL_STYLE);
		if ((style & 0x0FL) == BS_GROUPBOX)
			SetWindowLong(wnd, GWL_STYLE, style | WS_CLIPSIBLINGS);
	}

	// wnd classes that don't redraw client area correctly
	// when the hor scroll pos changes due to a resizing
	BOOL hscroll = FALSE;
	if (st == "LISTBOX")
		hscroll = TRUE;

	// wnd classes that need refresh when resized
	BOOL refresh = FALSE;
	if (st == "STATIC")
	{
		DWORD style = GetWindowLong(wnd, GWL_STYLE);

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

		// simple text never needs refresh
		if (style & SS_TYPEMASK == SS_SIMPLE)
			refresh = FALSE;
	}

	// get dialog's and control's rect
	CRect wndrc, objrc;

	pParent->GetClientRect(&wndrc);
	::GetWindowRect(wnd, &objrc);
	pParent->ScreenToClient(&objrc);
	
	CSize tl_margin, br_margin;

	if (br_type == NOANCHOR)
		br_type = tl_type;
	
	// calculate margin for the top-left corner

	tl_margin.cx = objrc.left - wndrc.Width() * tl_type.cx / 100;
	tl_margin.cy = objrc.top - wndrc.Height() * tl_type.cy / 100;
	
	// calculate margin for the bottom-right corner

	br_margin.cx = objrc.right - wndrc.Width() * br_type.cx / 100;
	br_margin.cy = objrc.bottom - wndrc.Height() * br_type.cy / 100;

	// add to the list
	Layout obj(wnd, tl_type, tl_margin,	br_type, br_margin, hscroll, refresh);
	m_arrLayout.Add(obj);
}

void CResizableLayout::ArrangeLayout()
{
	CWnd* pParent = GetLayoutParent();

	// init some vars
	CRect wndrc;
	pParent->GetClientRect(&wndrc);

	int i, count = m_arrLayout.GetSize();
	HDWP hdwp = BeginDeferWindowPos(count);

	for (i=0; i<count; ++i)
	{
		Layout& obj = m_arrLayout[i];

		CRect objrc, newrc;
		CWnd* wnd = CWnd::FromHandle(obj.hwnd); // temporary solution

		wnd->GetWindowRect(&objrc);
		pParent->ScreenToClient(&objrc);
		
		// calculate new top-left corner

		newrc.left = obj.tl_margin.cx + wndrc.Width() * obj.tl_type.cx / 100;
		newrc.top = obj.tl_margin.cy + wndrc.Height() * obj.tl_type.cy / 100;
		
		// calculate new bottom-right corner

		newrc.right = obj.br_margin.cx + wndrc.Width() * obj.br_type.cx / 100;
		newrc.bottom = obj.br_margin.cy + wndrc.Height() * obj.br_type.cy / 100;

		if (!newrc.EqualRect(&objrc))
		{
			if (obj.adj_hscroll)
			{
				// needs repainting, due to horiz scrolling
				int diff = newrc.Width() - objrc.Width();
				int max = wnd->GetScrollLimit(SB_HORZ);
			
				obj.need_refresh = FALSE;
				if (max > 0 && wnd->GetScrollPos(SB_HORZ) > max - diff)
				{
					obj.need_refresh = TRUE;
				}
			}

			// set flags 
			DWORD flags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION;
			if (newrc.TopLeft() == objrc.TopLeft())
				flags |= SWP_NOMOVE;
			if (newrc.Size() == objrc.Size())
				flags |= SWP_NOSIZE;
			
			DeferWindowPos(hdwp, obj.hwnd, NULL, newrc.left, newrc.top,
				newrc.Width(), newrc.Height(), flags);
		}
	}
	// go re-arrange child windows
	EndDeferWindowPos(hdwp);

	// refresh those that need
	for (i=0; i<count; ++i)
	{
		Layout& obj = m_arrLayout[i];
		CWnd* wnd = CWnd::FromHandle(obj.hwnd); // temporary solution
	
		if (obj.need_refresh)
		{
			wnd->Invalidate();
			wnd->UpdateWindow();
		}
	}
}

