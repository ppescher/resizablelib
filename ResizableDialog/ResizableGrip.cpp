// ResizableGrip.cpp: implementation of the CResizableGrip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo.h"
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
	GetLayoutParent()->ScreenToClient(&pt);

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

	GetLayoutParent()->InvalidateRect(&m_rcGripRect);	// del old grip

	GetLayoutParent()->GetClientRect(&m_rcGripRect);

	m_rcGripRect.left = m_rcGripRect.right - m_szGripSize.cx;
	m_rcGripRect.top = m_rcGripRect.bottom - m_szGripSize.cy;

	GetLayoutParent()->InvalidateRect(&m_rcGripRect);	// paint new grip
}

void CResizableGrip::ShowSizeGrip(BOOL bShow)
{
	if (m_bShowGrip != bShow)
	{
		m_bShowGrip = bShow;
		GetLayoutParent()->InvalidateRect(&m_rcGripRect);
	}
}

void CResizableGrip::DrawGrip(CDC &dc)
{
	if (m_bShowGrip && !GetLayoutParent()->IsZoomed())
	{
		// draw size-grip
		dc.DrawFrameControl(&m_rcGripRect, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
	}
}
