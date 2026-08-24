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

/*!
 *  @file
 *  @brief Implementation of the CResizableGrip class.
 */

#include "stdafx.h"
#include "ResizableGrip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// DPI awareness support
//////////////////////////////////////////////////////////////////////

// sent to child windows after the parent handled WM_DPICHANGED
#ifndef WM_DPICHANGED_AFTERPARENT
#define WM_DPICHANGED_AFTERPARENT 0x02E3
#endif

// System metrics are only available per-monitor through the DPI aware API added
// in Windows 10, version 1607. Bind to it at run-time, so that older systems keep
// using GetSystemMetrics, which is the best answer available there.
static CSize GetSizeGripMetrics(HWND hWnd)
{
	typedef UINT (WINAPI * PFNGETDPIFORWINDOW)(HWND);
	typedef int (WINAPI * PFNGETSYSTEMMETRICSFORDPI)(int, UINT);

	static HMODULE hUser32 = ::GetModuleHandle(_T("user32.dll"));
	static PFNGETDPIFORWINDOW pfnGetDpiForWindow =
		(PFNGETDPIFORWINDOW)::GetProcAddress(hUser32, "GetDpiForWindow");
	static PFNGETSYSTEMMETRICSFORDPI pfnGetSystemMetricsForDpi =
		(PFNGETSYSTEMMETRICSFORDPI)::GetProcAddress(hUser32, "GetSystemMetricsForDpi");

	if (hWnd != NULL && pfnGetDpiForWindow != NULL && pfnGetSystemMetricsForDpi != NULL)
	{
		const UINT nDpi = pfnGetDpiForWindow(hWnd);
		if (nDpi != 0)
			return CSize(pfnGetSystemMetricsForDpi(SM_CXVSCROLL, nDpi),
				pfnGetSystemMetricsForDpi(SM_CYHSCROLL, nDpi));
	}

	return CSize(::GetSystemMetrics(SM_CXVSCROLL), ::GetSystemMetrics(SM_CYHSCROLL));
}

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
	if (!::IsWindow(m_wndGrip.m_hWnd))
		return;

	// size-grip goes bottom right in the client area
	// (any right-to-left adjustment should go here)

	RECT rect;
	GetResizableWnd()->GetClientRect(&rect);

	rect.left = rect.right - m_wndGrip.m_size.cx;
	rect.top = rect.bottom - m_wndGrip.m_size.cy;

	// must stay below other children
	m_wndGrip.SetWindowPos(&CWnd::wndBottom, rect.left, rect.top,
		m_wndGrip.m_size.cx, m_wndGrip.m_size.cy,
		SWP_NOACTIVATE | SWP_NOREPOSITION
		| (IsSizeGripVisible() ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
}

// pStatus points to a variable, maintained by the caller, that
// holds its visibility status. Initialize the variable with 1
// to allow to temporarily hide the grip, 0 to allow to
// temporarily show the grip (with respect to the dwMask bit).

// NB: visibility is effective only after an update

void CResizableGrip::ShowSizeGrip(DWORD* pStatus, DWORD dwMask /*= 1*/)
{
	ASSERT(pStatus != NULL);

	if (!(*pStatus & dwMask))
	{
		m_nShowCount++;
		(*pStatus) |= dwMask;
	}
}

void CResizableGrip::HideSizeGrip(DWORD* pStatus, DWORD dwMask /*= 1*/)
{
	ASSERT(pStatus != NULL);

	if (*pStatus & dwMask)
	{
		m_nShowCount--;
		(*pStatus) &= ~dwMask;
	}
}

BOOL CResizableGrip::IsSizeGripVisible() const
{
	// NB: visibility is effective only after an update
	return (m_nShowCount > 0);
}

void CResizableGrip::SetSizeGripVisibility(BOOL bVisible)
{
	if (bVisible)
		m_nShowCount = 1;
	else
		m_nShowCount = 0;
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

void CResizableGrip::SetSizeGripShape(BOOL bTriangular)
{
	if (::IsWindow(m_wndGrip.m_hWnd))
		m_wndGrip.SetTriangularShape(bTriangular);
}

BOOL CResizableGrip::CreateSizeGrip(BOOL bVisible /*= TRUE*/,
		BOOL bTriangular /*= TRUE*/, BOOL bTransparent /*= FALSE*/)
{
	// create grip
	CRect rect(0 , 0, m_wndGrip.m_size.cx, m_wndGrip.m_size.cy);
	BOOL bRet = m_wndGrip.Create(WS_CHILD | WS_CLIPSIBLINGS
		| SBS_SIZEGRIP, rect, GetResizableWnd(), 0);

	if (bRet)
	{
		// set options
		m_wndGrip.SetTriangularShape(bTriangular);
		m_wndGrip.SetTransparency(bTransparent);
		SetSizeGripVisibility(bVisible);

		// update position
		UpdateSizeGrip();
	}

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// CSizeGrip implementation

BOOL CResizableGrip::CSizeGrip::IsRTL()
{
	return GetExStyle() & WS_EX_LAYOUTRTL;
}

BOOL CResizableGrip::CSizeGrip::PreCreateWindow(CREATESTRUCT& cs)
{
	// set window size
	m_size = GetSizeGripMetrics(cs.hwndParent);

	cs.cx = m_size.cx;
	cs.cy = m_size.cy;

	return CScrollBar::PreCreateWindow(cs);
}

LRESULT CResizableGrip::CSizeGrip::WindowProc(UINT message,
											  WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_GETDLGCODE:
		// fix to prevent the control to gain focus, using arrow keys
		// (standard grip returns DLGC_WANTARROWS, like any standard scrollbar)
		return DLGC_STATIC;

	case WM_SETFOCUS:
		// fix to prevent the control to gain focus, if set directly
		// (for example when it's the only one control in a dialog)
		return 0;

	case WM_NCHITTEST:
		// choose proper cursor shape
		return IsRTL() ? HTBOTTOMLEFT : HTBOTTOMRIGHT;

	case WM_SIZE:
		// the shape follows the window size, and the owner may resize the grip
		// itself (for example to match the DPI of the monitor it is on)
		if (m_bTriangular)
			SetTriangularShape(m_bTriangular);
		// the transparency bitmaps must follow the size too
		if (m_bTransparent)
			CreateTransparencyBitmaps();
		break;

	case WM_DPICHANGED_AFTERPARENT:
	case WM_SETTINGCHANGE:
		{
			// update grip's size
			m_size = GetSizeGripMetrics(m_hWnd);

			// resize the grip, keeping the bottom right corner where it is: the
			// owner may already have resized it, so set the size instead of
			// growing the window by the difference
			CRect rect;
			GetWindowRect(rect);
			rect.left = rect.right - m_size.cx;
			rect.top = rect.bottom - m_size.cy;
			::MapWindowPoints(NULL, GetParent()->GetSafeHwnd(), &rect.TopLeft(), 2);
			MoveWindow(rect, TRUE);
		}
		break;

	case WM_DESTROY:
		// perform clean up
		if (m_bTransparent)
			SetTransparency(FALSE);
		break;

	case WM_PAINT:
	case WM_PRINTCLIENT:
		if (m_bTransparent)
		{
			PAINTSTRUCT ps;
			CDC* pDC = (message == WM_PAINT && wParam == 0) ?
				BeginPaint(&ps) : CDC::FromHandle((HDC)wParam);

			// the owner may have given us a window larger than the size box the
			// control draws, which always goes in the bottom right corner, so work
			// out where it is instead of assuming it fills the client area
			CRect rectClient;
			GetClientRect(rectClient);

			const CSize size(__min(rectClient.Width(), m_size.cx),
				__min(rectClient.Height(), m_size.cy));
			const CPoint pt(rectClient.right - size.cx, rectClient.bottom - size.cy);

			// select bitmaps
			CBitmap *pOldGrip = m_dcGrip.SelectObject(&m_bmGrip);
			CBitmap *pOldMask = m_dcMask.SelectObject(&m_bmMask);

			// obtain original grip bitmap, make the mask and prepare masked bitmap
			CScrollBar::WindowProc(message, (WPARAM)m_dcGrip.GetSafeHdc(), lParam);
			m_dcGrip.SetBkColor(m_dcGrip.GetPixel(pt.x, pt.y));
			m_dcMask.BitBlt(pt.x, pt.y, size.cx, size.cy, &m_dcGrip, pt.x, pt.y, SRCCOPY);
			m_dcGrip.BitBlt(pt.x, pt.y, size.cx, size.cy, &m_dcMask, pt.x, pt.y, 0x00220326);

			// draw transparently
			pDC->BitBlt(pt.x, pt.y, size.cx, size.cy, &m_dcMask, pt.x, pt.y, SRCAND);
			pDC->BitBlt(pt.x, pt.y, size.cx, size.cy, &m_dcGrip, pt.x, pt.y, SRCPAINT);

			// unselect bitmaps
			m_dcGrip.SelectObject(pOldGrip);
			m_dcMask.SelectObject(pOldMask);

			if (message == WM_PAINT && wParam == 0)
				EndPaint(&ps);
			return 0;
		}
	}

	return CScrollBar::WindowProc(message, wParam, lParam);
}

void CResizableGrip::CSizeGrip::SetTransparency(BOOL bActivate)
{
	// creates or deletes DCs and Bitmaps used for
	// implementing a transparent size grip

	if (bActivate && !m_bTransparent)
	{
		m_bTransparent = TRUE;

		CClientDC dc(this);

		// create memory DCs and bitmaps
		m_dcGrip.CreateCompatibleDC(&dc);
		m_dcMask.CreateCompatibleDC(&dc);

		CreateTransparencyBitmaps();
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

void CResizableGrip::CSizeGrip::CreateTransparencyBitmaps()
{
	CClientDC dc(this);

	// bitmaps cover the whole client area, because the size box is drawn
	// wherever it falls when the owner resizes the grip itself
	CRect rectClient;
	GetClientRect(rectClient);

	// destroy bitmaps
	m_bmGrip.DeleteObject();
	m_bmMask.DeleteObject();

	// re-create bitmaps
	m_bmGrip.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
	m_bmMask.CreateBitmap(rectClient.Width(), rectClient.Height(), 1, 1, NULL);
}

void CResizableGrip::CSizeGrip::SetTriangularShape(BOOL bEnable)
{
	m_bTriangular = bEnable;

	if (bEnable)
	{
		// set a triangular window region
		CRect rect;
		GetWindowRect(rect);
		rect.OffsetRect(-rect.TopLeft());
		POINT arrPoints[] =
		{
			{ rect.left, rect.bottom },
			{ rect.right, rect.bottom },
			{ rect.right, rect.top }
		};
		CRgn rgnGrip;
		rgnGrip.CreatePolygonRgn(arrPoints, 3, WINDING);
		SetWindowRgn((HRGN)rgnGrip.Detach(), IsWindowVisible());
	}
	else
	{
		SetWindowRgn((HRGN)NULL, IsWindowVisible());
	}
}