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

	rect.left = rect.right - m_wndGrip.m_size.cx;
	rect.top = rect.bottom - m_wndGrip.m_size.cy;

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
	// create grip
	CRect rect(0 , 0, m_wndGrip.m_size.cx, m_wndGrip.m_size.cy);
	BOOL bRet = m_wndGrip.Create(WS_CHILD | WS_CLIPSIBLINGS | SBS_SIZEGRIP,
		rect, GetResizableWnd(), 0);

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
	switch (message)
	{
	case WM_NCHITTEST:
		// choose proper cursor shape
		if (IsRTL())
			return HTBOTTOMLEFT;
		else
			return HTBOTTOMRIGHT;
		break;

	case WM_DESTROY:
		if (m_bTransparent)
			SetTransparency(FALSE);
		break;

	case WM_PAINT:
		if (m_bTransparent)
		{
			CPaintDC dc(this);

			// select bitmaps
			CBitmap *pOldGrip, *pOldMask;

			pOldGrip = m_dcGrip.SelectObject(&m_bmGrip);
			pOldMask = m_dcMask.SelectObject(&m_bmMask);

			// obtain original grip bitmap, make the mask and prepare masked bitmap
			CScrollBar::WindowProc(WM_PAINT, (WPARAM)m_dcGrip.GetSafeHdc(), lParam);
			m_dcGrip.SetBkColor(m_dcGrip.GetPixel(0, 0));
			m_dcMask.BitBlt(0, 0, m_size.cx, m_size.cy, &m_dcGrip, 0, 0, SRCCOPY);
			m_dcGrip.BitBlt(0, 0, m_size.cx, m_size.cy, &m_dcMask, 0, 0, 0x00220326);
			
			// draw transparently
			dc.BitBlt(0, 0, m_size.cx, m_size.cy, &m_dcMask, 0, 0, SRCAND);
			dc.BitBlt(0, 0, m_size.cx, m_size.cy, &m_dcGrip, 0, 0, SRCPAINT);

			// unselect bitmaps
			m_dcGrip.SelectObject(pOldGrip);
			m_dcMask.SelectObject(pOldMask);

			return 0;
		}
		break;
	}

	return CScrollBar::WindowProc(message, wParam, lParam);
}

void CResizableGrip::CSizeGrip::SetTransparency(BOOL bActivate)
{
	if (bActivate && !m_bTransparent)
	{
		m_bTransparent = TRUE;

		CClientDC dc(this);

		// create memory DCs and bitmaps
		m_dcGrip.CreateCompatibleDC(&dc);
		m_bmGrip.CreateCompatibleBitmap(&dc, m_size.cx, m_size.cy);

		m_dcMask.CreateCompatibleDC(&dc);
		m_bmMask.CreateBitmap(m_size.cx, m_size.cy, 1, 1, NULL);
	}
	else if (!bActivate && m_bTransparent)
	{
		m_bTransparent = FALSE;

		// destroy memory DCs and bitmaps
		m_dcGrip.DeleteDC();
		m_bmGrip.DeleteObject();

		m_dcMask.DeleteDC();
		m_bmMask.DeleteObject();
	}
}

BOOL CResizableGrip::SetSizeGripBkMode(int nBkMode)
{
	if (::IsWindow(m_wndGrip.m_hWnd))
	{
		if (nBkMode == OPAQUE)
			m_wndGrip.SetTransparency(FALSE);
		else if (nBkMode == TRANSPARENT)
			m_wndGrip.SetTransparency(TRUE);
		else
			return FALSE;
		return TRUE;
	}
	return FALSE;
}
