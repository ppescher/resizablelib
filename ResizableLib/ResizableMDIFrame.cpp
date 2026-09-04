// ResizableMDIFrame.cpp : implementation file
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
#include "ResizableMDIFrame.h"
#include "ResizableVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableMDIFrame

IMPLEMENT_DYNCREATE(CResizableMDIFrame, CMDIFrameWnd)

CResizableMDIFrame::CResizableMDIFrame()
{
	m_bEnableSaveRestore = FALSE;
	m_bRectOnly = FALSE;
}

CResizableMDIFrame::~CResizableMDIFrame()
{
}


BEGIN_MESSAGE_MAP(CResizableMDIFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CResizableMDIFrame)
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_WM_NCCREATE()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableMDIFrame message handlers

void CResizableMDIFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	// MDI should call default implementation
	CMDIFrameWnd::OnGetMinMaxInfo(lpMMI);

	MinMaxInfo(lpMMI);

	BOOL bMaximized = FALSE;
	CMDIChildWnd* pChild = MDIGetActive(&bMaximized);
	if (pChild != NULL && bMaximized)
		ChainMinMaxInfo(lpMMI, this, pChild);
}

// NOTE: this must be called after setting the layout
//       to have the view and its controls displayed properly
BOOL CResizableMDIFrame::EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly)
{
	m_sSection = pszSection;

	m_bEnableSaveRestore = TRUE;
	m_bRectOnly = bRectOnly;

	// restore immediately
	return LoadWindowRect(pszSection, bRectOnly);
}

void CResizableMDIFrame::OnDestroy()
{
	if (m_bEnableSaveRestore)
		SaveWindowRect(m_sSection, m_bRectOnly);

	// reset instance data
	RemoveAllAnchors();
	ResetAllRects();
	m_bEnableSaveRestore = FALSE;

	CMDIFrameWnd::OnDestroy();
}

BOOL CResizableMDIFrame::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CMDIFrameWnd::OnNcCreate(lpCreateStruct))
		return FALSE;

	MakeResizable(lpCreateStruct);

	return TRUE;
}

LRESULT CResizableMDIFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DPICHANGED:
		// forward to the view
		for (HWND hWnd = ::GetWindow(MDIGetActive()->GetSafeHwnd(), GW_HWNDFIRST);
			hWnd != NULL; hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT))
		{
			::SendMessage(hWnd, message, wParam, lParam);
		}
		break;

	case WM_NCCALCSIZE:
		// improve client area validation to reduce flickering
		if (wParam != FALSE)
		{
			LRESULT lResult = 0;
			HandleNcCalcSize(FALSE, (LPNCCALCSIZE_PARAMS)lParam, lResult);
			lResult = CMDIFrameWnd::WindowProc(message, wParam, lParam);
			HandleNcCalcSize(TRUE, (LPNCCALCSIZE_PARAMS)lParam, lResult);
			return lResult;
		}
		break;
	}
	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}

void CResizableMDIFrame::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	CMDIFrameWnd::OnWindowPosChanging(lpwndpos);

	// since this window class doesn't have the style CS_HREDRAW|CS_VREDRAW
	// the client area is not invalidated during a resize operation and
	// this prevents the system from using WM_NCCALCSIZE to validate rects
	Invalidate();
}
