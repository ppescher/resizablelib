// ResizableGrip.cpp: implementation of the CResizableGrip class.
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
#include "ResizableGrip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResizableGrip::CResizableGrip()
{
	m_sizeGrip.cx = GetSystemMetrics(SM_CXVSCROLL);
	m_sizeGrip.cy = GetSystemMetrics(SM_CYHSCROLL);
	m_nShowCount = 0;
}

CResizableGrip::~CResizableGrip()
{

}

void CResizableGrip::UpdateSizeGrip()
{
	ASSERT(::IsWindow(m_wndGrip.m_hWnd));

	// size-grip goes bottom right in the client area
	// (any right-to-left adjustment should go here)

	RECT rect;
	GetResizableWnd()->GetClientRect(&rect);

	rect.left = rect.right - m_sizeGrip.cx;
	rect.top = rect.bottom - m_sizeGrip.cy;

	// must stay below other children
	m_wndGrip.SetWindowPos(&CWnd::wndBottom, rect.left, rect.top, 0, 0,
		SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOREPOSITION);

	// maximized windows cannot be resized
	if (GetResizableWnd()->IsZoomed())
		m_wndGrip.EnableWindow(FALSE);
	else
		m_wndGrip.EnableWindow(TRUE);

	m_wndGrip.ShowWindow(IsSizeGripVisible() ? SW_SHOW : SW_HIDE);
}

// pbStatus points to a variable, maintained by the caller, that
// holds its visibility status. Initialize the variable with TRUE
// to allow to temporarily hide the grip, FALSE to allow to
// temporarily show the grip.
// NULL to force showing/hiding the grip

// NB: visibility is effective only after an update

void CResizableGrip::ShowSizeGrip(BOOL* pbStatus /* = NULL */)
{
	if (pbStatus == NULL)
	{
		m_nShowCount = 1;
	}
	else if ( !(*pbStatus) )
	{
		m_nShowCount++;
		(*pbStatus) = TRUE;
	}
}

void CResizableGrip::HideSizeGrip(BOOL* pbStatus /* = NULL */)
{
	if (pbStatus == NULL)
	{
		m_nShowCount = 0;
	}
	else if (*pbStatus)
	{
		m_nShowCount--;
		(*pbStatus) = FALSE;
	}
}

BOOL CResizableGrip::IsSizeGripVisible()
{
	// NB: visibility is effective only after an update
	return (m_nShowCount > 0);
}

BOOL CResizableGrip::CreateSizeGrip()
{
	CRect rect(0 , 0, m_sizeGrip.cx, m_sizeGrip.cy);

	m_wndGrip.m_pResizableWnd = GetResizableWnd();
	BOOL bRet = m_wndGrip.Create(WS_CHILD | WS_CLIPSIBLINGS | SBS_SIZEGRIP,
		rect, m_wndGrip.m_pResizableWnd, 0);

	if (bRet)
	{
		// set a triangular window region
		rect.OffsetRect(-rect.TopLeft());
		POINT aPoints[] =
		{
			{ rect.left, rect.bottom },
			{ rect.right, rect.bottom },
			{ rect.right, rect.top }
		};
		CRgn rgnGrip;
		rgnGrip.CreatePolygonRgn(aPoints, 3, WINDING);
		m_wndGrip.SetWindowRgn((HRGN)rgnGrip.Detach(), FALSE);

		// update position
		UpdateSizeGrip();
	}

	return bRet;
}

BOOL CResizableGrip::CSizeGrip::IsRTL()
{
	return GetExStyle() & WS_EX_LAYOUTRTL;
}

LRESULT CResizableGrip::CSizeGrip::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CWnd* pParentWnd = m_pResizableWnd->GetParent();

	switch (message)
	{
	case WM_NCHITTEST:
		// choose proper cursor shape
		if (IsRTL())
			return HTBOTTOMLEFT;
		else
			return HTBOTTOMRIGHT;
		break;

/*	case WM_MOVE:

		if ((m_pResizableWnd->GetStyle() & WS_CHILD)
			&& (pParentWnd != NULL))
		{
			CRect rectClient, rectCalc;
			pParentWnd->GetClientRect(rectClient);
			pParentWnd->RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST,
				reposQuery, rectCalc, rectClient);
			CSize sizeOffset = rectClient.Size() - rectCalc.Size();
			pParentWnd->GetWindowRect(rectCalc);
			CSize sizeWnd = rectCalc.Size() + sizeOffset;
			pParentWnd->SetWindowPos(NULL, 0, 0, sizeWnd.cx, sizeWnd.cy,
				SWP_NOZORDER | SWP_NOREPOSITION | SWP_NOACTIVATE | SWP_NOMOVE);
		}
		break;*/
	}

	return CScrollBar::WindowProc(message, wParam, lParam);
}
