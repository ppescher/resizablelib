// ResizableGrip.cpp: implementation of the CResizableGrip class.
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
	m_bShowGrip = TRUE;
	
	m_szGripSize.cx = GetSystemMetrics(SM_CXVSCROLL);
	m_szGripSize.cy = GetSystemMetrics(SM_CYHSCROLL);
}

CResizableGrip::~CResizableGrip()
{

}

UINT CResizableGrip::HitTest(CPoint point)
{
	// size-grip goes bottom right in the client area
	// (any right-to-left adjustment should go here)

	CPoint pt = point;
	GetResizableWnd()->ScreenToClient(&pt);

	// if in size grip and in client area
	if (m_bShowGrip && m_rcGripRect.PtInRect(pt) &&
		pt.x >= 0 && pt.y >= 0)
		return HTBOTTOMRIGHT;

	return HTNOWHERE;
}

void CResizableGrip::UpdateGripPos()
{
	// size-grip goes bottom right in the client area
	// (any right-to-left adjustment should go here)

	GetResizableWnd()->InvalidateRect(&m_rcGripRect);	// del old grip

	GetResizableWnd()->GetClientRect(&m_rcGripRect);

	m_rcGripRect.left = m_rcGripRect.right - m_szGripSize.cx;
	m_rcGripRect.top = m_rcGripRect.bottom - m_szGripSize.cy;

	GetResizableWnd()->InvalidateRect(&m_rcGripRect);	// paint new grip
}

void CResizableGrip::ShowSizeGrip(BOOL bShow)
{
	if (m_bShowGrip != bShow)
	{
		m_bShowGrip = bShow;
		GetResizableWnd()->InvalidateRect(&m_rcGripRect);
	}
}

void CResizableGrip::DrawGrip(CDC &dc)
{
	if (m_bShowGrip && !GetResizableWnd()->IsZoomed())
	{
		// draw size-grip
		dc.DrawFrameControl(&m_rcGripRect, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
	}
}
