// ResizableComboLBox.cpp : implementation file
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
#include "ResizableComboLBox.h"
#include "ResizableComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableComboLBox

CResizableComboLBox::CResizableComboLBox()
	: m_nHitTest(0), m_pOwnerCombo(NULL)
{
	m_bSizing = FALSE;
}

CResizableComboLBox::~CResizableComboLBox()
{

}


BEGIN_MESSAGE_MAP(CResizableComboLBox, CWnd)
	//{{AFX_MSG_MAP(CResizableComboLBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST()
	ON_WM_CAPTURECHANGED()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableComboLBox message handlers

void CResizableComboLBox::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	InitializeControl();
}

BOOL CResizableComboLBox::IsRTL()
{
	return (GetExStyle() & WS_EX_LAYOUTRTL);
}

void CResizableComboLBox::InitializeControl()
{
	CreateSizeGrip(FALSE);

	CRect rect;
	m_pOwnerCombo->GetWindowRect(&rect);
	m_sizeAfterSizing.cx = rect.Width();
	m_sizeAfterSizing.cy = -rect.Height();
	m_pOwnerCombo->GetDroppedControlRect(&rect);
	m_sizeAfterSizing.cy += rect.Height();
	m_sizeMin.cy = m_sizeAfterSizing.cy-2;

	// count hscroll if present
	if (GetStyle() & WS_HSCROLL)
		m_sizeAfterSizing.cy += GetSystemMetrics(SM_CYHSCROLL);

	// apply size constraints
	WINDOWPOS wp = {};
	wp.cx = m_sizeAfterSizing.cx;
	wp.cy = m_sizeAfterSizing.cy;
	ApplyLimitsToPos(&wp);

	SetWindowPos(NULL, 0, 0, wp.cx, wp.cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
}

void CResizableComboLBox::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint pt = point;
	MapWindowPoints(NULL, &pt, 1);	// to screen coord

	if (!m_bSizing)
	{
		// since mouse is captured we need to change the cursor manually
		LRESULT ht = SendMessage(WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y));
		SendMessage(WM_SETCURSOR, (WPARAM)m_hWnd, MAKELPARAM(ht, WM_MOUSEMOVE));

		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	// during resize
	CRect rect = m_rcBeforeSizing;
	CSize relMove = pt - m_ptBeforeSizing;

	switch (m_nHitTest)
	{
	case HTBOTTOM:
		rect.bottom += relMove.cy;
		break;
	case HTBOTTOMRIGHT:
		rect.bottom += relMove.cy;
		rect.right += relMove.cx;
		break;
	case HTRIGHT:
		rect.right += relMove.cx;
		break;
	case HTBOTTOMLEFT:
		rect.bottom += relMove.cy;
		rect.left += relMove.cx;
		break;
	case HTLEFT:
		rect.left += relMove.cx;
		break;
	}

	// move window (if right-aligned it needs refresh)
	UINT nCopyFlag = (GetExStyle() & WS_EX_RIGHT) ? SWP_NOCOPYBITS : 0;
	SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(),
		SWP_NOACTIVATE|SWP_NOZORDER|nCopyFlag);
}

void CResizableComboLBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint pt = point;
	MapWindowPoints(NULL, &pt, 1);	// to screen coord

	LRESULT ht = SendMessage(WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y));

	if (ht == HTBOTTOM || ht == HTRIGHT || ht == HTBOTTOMRIGHT
		|| ht == HTLEFT || ht == HTBOTTOMLEFT)
	{
		// start resizing
		m_bSizing = TRUE;
		m_nHitTest = ht;
		GetWindowRect(&m_rcBeforeSizing);
		m_ptBeforeSizing = pt;
	}
	else
		CWnd::OnLButtonDown(nFlags, point);
}

void CResizableComboLBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

	EndSizing();
}

LRESULT CResizableComboLBox::OnNcHitTest(CPoint point)
{
	CRect rcClient, rcWindow;
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);
	MapWindowPoints(NULL, &rcClient);

	// ask for default hit-test value
	LRESULT ht = CWnd::OnNcHitTest(point);

	// disable improper resizing (based on layout setting)
	switch (ht)
	{
	case HTTOPRIGHT:
		if (!IsRTL() && point.y > rcClient.top)
			ht = HTRIGHT;
		else
			ht = HTBORDER;
		break;
	case HTTOPLEFT:
		if (IsRTL() && point.y > rcClient.top)
			ht = HTLEFT;
		else
			ht = HTBORDER;
		break;

	case HTBOTTOMLEFT:
		if (!IsRTL() && point.y > rcClient.bottom)
			ht = HTBOTTOM;
		else if (!IsRTL())
			ht = HTBORDER;
		break;
	case HTBOTTOMRIGHT:
		if (IsRTL() && point.y > rcClient.bottom)
			ht = HTBOTTOM;
		else if (IsRTL())
			ht = HTBORDER;
		break;

	case HTLEFT:
		if (!IsRTL())
			ht = HTBORDER;
		break;
	case HTRIGHT:
		if (IsRTL())
			ht = HTBORDER;
		break;

	case HTTOP:
		ht = HTBORDER;
	}

	CSize border(GetSystemMetrics(SM_CXFRAME), GetSystemMetrics(SM_CYFRAME));
	if (!IsRTL())
		rcWindow.right += border.cx;
	else
		rcWindow.left += border.cx;
	rcWindow.bottom += border.cy;

	if (point.x > rcClient.right && point.y > rcClient.bottom &&
		point.x <= rcWindow.right && point.y <= rcWindow.bottom)
	{
		ht = IsRTL() ? HTBOTTOMLEFT : HTBOTTOMRIGHT;
	}
	else
	{
		if (rcWindow.PtInRect(point))
		{
			if (point.y >= rcWindow.bottom - border.cy && point.y <= rcWindow.bottom)
			{
				ht = HTBOTTOM;
			}
			else
			{
				if (!IsRTL())
				{
					if (point.x >= rcWindow.right - border.cx && point.x <= rcWindow.right)
						ht = HTRIGHT;
				}
				else
				{
					if (point.x >= rcWindow.left - border.cx && point.x <= rcWindow.left)
						ht = HTLEFT;
				}
			}
		}
	}
	return ht;
}

void CResizableComboLBox::OnCaptureChanged(CWnd *pWnd)
{
	EndSizing();

	CWnd::OnCaptureChanged(pWnd);
}

void CResizableComboLBox::EndSizing()
{
	m_bSizing = FALSE;
	CRect rect;
	GetWindowRect(&rect);
	m_sizeAfterSizing = rect.Size();
}

void CResizableComboLBox::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	if (!m_bSizing)
	{
		// restore the size when the drop-down list becomes visible
		lpwndpos->cx = m_sizeAfterSizing.cx;
		lpwndpos->cy = m_sizeAfterSizing.cy;
	}
	ApplyLimitsToPos(lpwndpos);

	CWnd::OnWindowPosChanging(lpwndpos);
}

void CResizableComboLBox::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	// default implementation sends a WM_SIZE message
	// that can change the size again to force integral height

	// since we do that manually during resize, we should also
	// update the horizontal scrollbar
	SendMessage(WM_HSCROLL, SB_ENDSCROLL, 0);

	GetWindowRect(&m_pOwnerCombo->m_rectDropDown);
	::MapWindowPoints(NULL, m_pOwnerCombo->GetSafeHwnd(),
		(LPPOINT)&m_pOwnerCombo->m_rectDropDown, 2);

	CWnd::OnWindowPosChanged(lpwndpos);

	if (!m_bSizing)
	{
		// draw missing size grip
		CDC *pDC = GetWindowDC();
		PaintSizeGrip(pDC);
		ReleaseDC(pDC);
	}
}

void CResizableComboLBox::ApplyLimitsToPos(WINDOWPOS* lpwndpos)
{
	//TRACE(">H w(%d)\n", lpwndpos->cy);
	// to adjust horizontally, use window rect

	// min width can't be less than combo's
	CRect rect;
	m_pOwnerCombo->GetWindowRect(&rect);
	m_sizeMin.cx = rect.Width();

	// apply horizontal limits
	if (lpwndpos->cx < m_sizeMin.cx)
		lpwndpos->cx = m_sizeMin.cx;

	// fix horizontal alignment
	rect = CRect(0, 0, lpwndpos->cx, lpwndpos->cy);
	m_pOwnerCombo->MapWindowPoints(NULL, &rect);
	lpwndpos->x = rect.left;

	// to adjust vertically, use client rect

	// get client rect
	rect = CRect(CPoint(lpwndpos->x, lpwndpos->y),
		CSize(lpwndpos->cx, lpwndpos->cy));
	SendMessage(WM_NCCALCSIZE, FALSE, (LPARAM)&rect);
	CSize sizeClient = rect.Size();

	// apply vertical limits
	if (sizeClient.cy < m_sizeMin.cy)
		sizeClient.cy = m_sizeMin.cy;

	//TRACE(">H c(%d)\n", sizeClient.cy);
	// adjust height, if needed
	sizeClient.cy = m_pOwnerCombo->MakeIntegralHeight(sizeClient.cy);
	//TRACE(">H c(%d)\n", sizeClient.cy);

	// back to window rect
	rect = CRect(0, 0, 1, sizeClient.cy);
	const DWORD dwStyle = GetStyle();
	::AdjustWindowRectEx(&rect, dwStyle, FALSE, GetExStyle());
	lpwndpos->cy = rect.Height();
	if (dwStyle & WS_HSCROLL)
		lpwndpos->cy += GetSystemMetrics(SM_CYHSCROLL);

	//TRACE("H c(%d) w(%d)\n", sizeClient.cy, lpwndpos->cy);
}

void CResizableComboLBox::OnNcPaint()
{
	CWnd::OnNcPaint();

	// draw missing size grip
	CDC *pDC = GetWindowDC();
	PaintSizeGrip(pDC);
	ReleaseDC(pDC);
}

void CResizableComboLBox::PaintSizeGrip(CDC * pDC)
{
	CRect rc;
	GetClientRect(&rc);
	CPoint ptOld = pDC->OffsetViewportOrg(rc.right, rc.bottom);
	GetSizeGripWnd()->SendMessage(WM_PRINTCLIENT, (WPARAM)pDC->GetSafeHdc(), PRF_CLIENT | PRF_NONCLIENT | PRF_ERASEBKGND);
	pDC->SetViewportOrg(ptOld);
}

LRESULT CResizableComboLBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PRINTCLIENT:
		CWnd::WindowProc(message, wParam, lParam);
		if (lParam & PRF_NONCLIENT)
			PaintSizeGrip(CDC::FromHandle((HDC)wParam));
		return 0;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}
