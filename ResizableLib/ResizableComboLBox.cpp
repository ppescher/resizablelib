// ResizableComboLBox.cpp : implementation file
//

#include "stdafx.h"
#include "ResizableComboLBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableComboLBox

CResizableComboLBox::CResizableComboLBox()
{
	m_dwAddToStyle = WS_THICKFRAME|WS_HSCROLL;
	m_dwAddToStyleEx = 0;//WS_EX_CLIENTEDGE;
	m_bClipMaxHeight = TRUE;
	m_bSizing = FALSE;
}

CResizableComboLBox::~CResizableComboLBox()
{

}


BEGIN_MESSAGE_MAP(CResizableComboLBox, CListBox)
	//{{AFX_MSG_MAP(CResizableComboLBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCHITTEST()
	ON_WM_CAPTURECHANGED()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableComboLBox message handlers

void CResizableComboLBox::PreSubclassWindow() 
{
	CListBox::PreSubclassWindow();

	InitializeControl();
}

void CResizableComboLBox::InitializeControl()
{
	CRect rect, rectNew;
	GetWindowRect(&rectNew);

	// calc new height (with new style)
	GetClientRect(&rect);
	DWORD dwStyle = GetStyle() | m_dwAddToStyle;
	DWORD dwStyleEx = GetExStyle() | m_dwAddToStyleEx;
	AdjustWindowRectEx(&rect, dwStyle, FALSE, dwStyleEx);
	if (dwStyle & WS_HSCROLL)
		rect.bottom += GetSystemMetrics(SM_CYHSCROLL);
	// calc new rect
	rectNew.bottom = rectNew.top + rect.Height();
	// save as current size
	m_sizeAfterSizing = rectNew.Size();
	// save as initial dimensions
	m_sizeMin = m_sizeAfterSizing;

	// change window's style
	// (must be done here for integral height to work)
	ModifyStyleEx(0, m_dwAddToStyleEx);
	ModifyStyle(0, m_dwAddToStyle, SWP_FRAMECHANGED);

	// set the initial size (actually only height has changed)
	SetWindowPos(NULL, 0, 0, m_sizeMin.cx, m_sizeMin.cy,
		SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOREPOSITION);
	// fix height later (needed if hscroll present)
	PostMessage(WM_SIZE, 0, 0);
}

void CResizableComboLBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	MapWindowPoints(NULL, &pt, 1);

	if (!m_bSizing)
	{
		// since mouse is captured we need to change the cursor manually
		LRESULT ht = SendMessage(WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y));
		SendMessage(WM_SETCURSOR, (WPARAM)m_hWnd, MAKELPARAM(ht, WM_MOUSEMOVE));

		CListBox::OnMouseMove(nFlags, point);
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
	MapWindowPoints(NULL, &pt, 1);

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
		CListBox::OnLButtonDown(nFlags, point);
}

void CResizableComboLBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CListBox::OnLButtonUp(nFlags, point);

	m_bSizing = FALSE;
	CRect rect;
	GetWindowRect(&rect);
	m_sizeAfterSizing = rect.Size();
}

void CResizableComboLBox::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CListBox::OnGetMinMaxInfo(lpMMI);

	// min width can't be less than combo's
	CRect rect;
	m_pOwnerCombo->GetWindowRect(&rect);
	lpMMI->ptMinTrackSize.x = rect.Width();
	lpMMI->ptMinTrackSize.y = m_sizeMin.cy;
}

UINT CResizableComboLBox::OnNcHitTest(CPoint point) 
{
	CRect rcClient;
	GetClientRect(&rcClient);
	MapWindowPoints(NULL, &rcClient);

	// ask for default hit-test value
	UINT ht = CListBox::OnNcHitTest(point);

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
		else if (!!IsRTL())
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

	return ht;
}

void CResizableComboLBox::OnCaptureChanged(CWnd *pWnd) 
{
	m_bSizing = FALSE;
	CRect rect;
	GetWindowRect(&rect);
	m_sizeAfterSizing = rect.Size();

	CListBox::OnCaptureChanged(pWnd);
}

void CResizableComboLBox::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	if (!m_bSizing)
	{
		// restore the size when the drop-down list becomes visible

		lpwndpos->cy = m_sizeAfterSizing.cy;
		
		int diff = m_sizeAfterSizing.cx - lpwndpos->cx;
		if (diff > 0)	// only if greater than combo's width
		{
			if (IsRTL())
				lpwndpos->x -= diff;
			lpwndpos->cx += diff;
		}
	}
	ApplyLimitsToPos(lpwndpos);
	return;

	CListBox::OnWindowPosChanging(lpwndpos);
}

void CResizableComboLBox::ApplyLimitsToPos(WINDOWPOS* lpwndpos)
{
	MINMAXINFO mmi;
	ZeroMemory(&mmi, sizeof(MINMAXINFO));
	SendMessage(WM_GETMINMAXINFO, 0, (LPARAM)&mmi);
	if (lpwndpos->cx < mmi.ptMinTrackSize.x)
	{
		if (IsRTL())
		{
			CPoint pt(0,0);
			m_pOwnerCombo->MapWindowPoints(NULL, &pt, 1);
			lpwndpos->x = pt.x - mmi.ptMinTrackSize.x;
		}
		lpwndpos->cx = mmi.ptMinTrackSize.x;
	}
	if (lpwndpos->cy < mmi.ptMinTrackSize.y)
		lpwndpos->cy = mmi.ptMinTrackSize.y;

	lpwndpos->cy = MakeIntegralHeight(lpwndpos->cy);
}

int CResizableComboLBox::MakeIntegralHeight(const int height)
{
	int inth = height;	// integral height (result)

	DWORD dwStyle = GetStyle();
	if (dwStyle & LBS_NOINTEGRALHEIGHT)
		return inth;

	CRect rcWnd, rcClient;
	GetWindowRect(&rcWnd);
	GetClientRect(&rcClient);

	int border = rcWnd.Height() - rcClient.Height();

	int availh = height - border;	// available height
	int n = GetCount();
	
	if (dwStyle & LBS_OWNERDRAWVARIABLE)
	{
		inth = 0;	// try to reach availh by integral steps

		// use items below the first visible
		for (int i=GetTopIndex(); availh>0 && i<n; i++)
		{
			int h = GetItemHeight(i);
			if (h == LB_ERR)
				break;

			inth += h;
			availh -= h;
		}
		// to fill the remaining height, use items above
		for (i=GetTopIndex()-1; availh>0 && i>=0; i--)
		{
			int h = GetItemHeight(i);
			if (h == LB_ERR)
				break;

			inth += h;
			availh -= h;
		}
		// scroll into view
		SetTopIndex(i);

		if (!m_bClipMaxHeight) // it can be higher than all the items
		{
			// to fill the remaining height, use last item
			int h = GetItemHeight(n-1);
			if (h != LB_ERR)
			{
				inth += availh - availh % h;
			}
		}
	}
	else
	{
		// every item has the same height (take the first)
		int h = GetItemHeight(0);
		if (h != LB_ERR && n != LB_ERR)
		{
			int rows = availh / h;
			// can't be higher than all the items
			if (m_bClipMaxHeight && rows > n)
				rows = n;
			inth = rows * h;
			// scroll into view
			if (n - rows < GetTopIndex())
				SetTopIndex(n-rows);
		}
	}

	return border + inth;
}

LRESULT CResizableComboLBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
//	TRACE("Msg: 0x%04X (%d)\n", message, message);
	switch (message)
	{
	case WM_SIZE:
		if (wParam == 0 && lParam == 0)
		{
			// set the initial size
			SetWindowPos(NULL, 0, 0, m_sizeMin.cx, m_sizeMin.cy,
				SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOREPOSITION);
		}
		return ::DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	return CListBox::WindowProc(message, wParam, lParam);
}

