// ResizableDialog.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2000 by Paolo Messina
// (ppescher@yahoo.com)
// 
// This file may be redistributed unmodified by any means PROVIDING it
// is not sold for profit without the author written consent, and 
// providing that this notice and the author name is included.
// If the code in this file is used in any commercial application 
// a simple credit would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResizableDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableDialog

CResizableDialog::CResizableDialog()
{
	m_bInitDone = FALSE;

	m_bUseMinTrack = TRUE;
	m_bUseMaxTrack = FALSE;
	m_bUseMaxRect = FALSE;

	m_bShowGrip = TRUE;
	
	m_bEnableSaveRestore = FALSE;

	m_szGripSize = CSize(GetSystemMetrics(SM_CXVSCROLL),
		GetSystemMetrics(SM_CYHSCROLL));
}

CResizableDialog::CResizableDialog(UINT nIDTemplate, CWnd* pParentWnd)
	: CDialog(nIDTemplate, pParentWnd)
{
	m_bInitDone = FALSE;

	m_bUseMinTrack = TRUE;
	m_bUseMaxTrack = FALSE;
	m_bUseMaxRect = FALSE;

	m_bShowGrip = TRUE;

	m_bEnableSaveRestore = FALSE;

	m_szGripSize = CSize(GetSystemMetrics(SM_CXVSCROLL),
		GetSystemMetrics(SM_CYHSCROLL));
}

CResizableDialog::~CResizableDialog()
{
}


BEGIN_MESSAGE_MAP(CResizableDialog, CDialog)
	//{{AFX_MSG_MAP(CResizableDialog)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResizableDialog message handlers


BOOL CResizableDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateGripPos();

	// gets the template size as the min track size
	CRect rc;
	GetWindowRect(&rc);
	m_ptMinTrackSize.x = rc.Width();
	m_ptMinTrackSize.y = rc.Height();

	m_bInitDone = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResizableDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_bEnableSaveRestore)
		SaveWindowRect();
}

BOOL CResizableDialog::AddAnchor(CWnd *pWnd, AnchorType ul_type, AnchorType br_type)
{
	// pWnd is a temporary pointer, should not be stored
	// store the window handle instead (HWND)
	
	if (pWnd == NULL)
		return FALSE;

	CString st;
	GetClassName(*pWnd, st.GetBufferSetLength(MAX_PATH), MAX_PATH);
	st.ReleaseBuffer();
	st.MakeUpper();

	// add the style 'clipsiblings' to a GroupBox
	// to avoid unnecessary repainting of controls inside
	if (st == "BUTTON")
	{
		DWORD style = GetWindowLong(*pWnd, GWL_STYLE);
		if (style & BS_GROUPBOX)
			SetWindowLong(*pWnd, GWL_STYLE, style | WS_CLIPSIBLINGS);
	}

	// wnd classes that don't redraw client area correctly
	// when the hor scroll pos changes due to a resizing
	BOOL hscroll = FALSE;
	if (st == "LISTBOX")
		hscroll = TRUE;

	CRect wndrc, objrc;

	GetClientRect(&wndrc);
	ClientToScreen(&wndrc);
	pWnd->GetWindowRect(&objrc);
	
	CSize ul_margin(0,0), br_margin(0,0);
	
	// calculate margin for the upper-left corner

	switch(ul_type)
	{
	case TOP_LEFT:
		ul_margin.cx = objrc.left - wndrc.left;
		ul_margin.cy = objrc.top - wndrc.top;
		break;
	case TOP_CENTER:
		ul_margin.cx = objrc.left - (wndrc.left + wndrc.right)/2;
		ul_margin.cy = objrc.top - wndrc.top;
		break;
	case MIDDLE_LEFT:
		ul_margin.cx = objrc.left - wndrc.left;
		ul_margin.cy = objrc.top - (wndrc.top + wndrc.bottom)/2;
		break;

	case BOTTOM_RIGHT:
		ul_margin.cx = wndrc.right - objrc.left;
		ul_margin.cy = wndrc.bottom - objrc.top;
		break;
	case BOTTOM_CENTER:
		ul_margin.cx = objrc.left - (wndrc.left + wndrc.right)/2;
		ul_margin.cy = wndrc.bottom - objrc.top;
		break;
	case MIDDLE_RIGHT:
		ul_margin.cx = wndrc.right - objrc.left;
		ul_margin.cy = objrc.top - (wndrc.top + wndrc.bottom)/2;
		break;

	case BOTTOM_LEFT:
		ul_margin.cx = objrc.left - wndrc.left;
		ul_margin.cy = wndrc.bottom - objrc.top;
		break;
	case TOP_RIGHT:
		ul_margin.cx = wndrc.right - objrc.left;
		ul_margin.cy = objrc.top - wndrc.top;
		break;
	case MIDDLE_CENTER:
		ul_margin.cx = objrc.left - (wndrc.left + wndrc.right)/2;
		ul_margin.cy = objrc.top - (wndrc.top + wndrc.bottom)/2;
		break;

	default:
		ASSERT(FALSE);	// cannot be NOANCHOR !!
		return FALSE;
	}
	
	// calculate margin for the upper-left corner

	switch(br_type)
	{
	case TOP_LEFT:
		br_margin.cx = objrc.right - wndrc.left;
		br_margin.cy = objrc.bottom - wndrc.top;
		break;
	case TOP_CENTER:
		br_margin.cx = objrc.right - (wndrc.left + wndrc.right)/2;
		br_margin.cy = objrc.bottom - wndrc.top;
		break;
	case MIDDLE_LEFT:
		br_margin.cx = objrc.right - wndrc.left;
		br_margin.cy = objrc.bottom - (wndrc.top + wndrc.bottom)/2;
		break;

	case BOTTOM_RIGHT:
		br_margin.cx = wndrc.right - objrc.right;
		br_margin.cy = wndrc.bottom - objrc.bottom;
		break;
	case BOTTOM_CENTER:
		br_margin.cx = objrc.right - (wndrc.left + wndrc.right)/2;
		br_margin.cy = wndrc.bottom - objrc.bottom;
		break;
	case MIDDLE_RIGHT:
		br_margin.cx = wndrc.right - objrc.right;
		br_margin.cy = objrc.bottom - (wndrc.top + wndrc.bottom)/2;
		break;

	case BOTTOM_LEFT:
		br_margin.cx = objrc.right - wndrc.left;
		br_margin.cy = wndrc.bottom - objrc.bottom;
		break;
	case TOP_RIGHT:
		br_margin.cx = wndrc.right - objrc.right;
		br_margin.cy = objrc.bottom - wndrc.top;
		break;
	case MIDDLE_CENTER:
		br_margin.cx = objrc.right - (wndrc.left + wndrc.right)/2;
		br_margin.cy = objrc.bottom - (wndrc.top + wndrc.bottom)/2;
		break;

	default:
		ASSERT(br_type == NOANCHOR);
	}

	// add to the list
	m_plLayoutList.AddTail(new Layout(pWnd->GetSafeHwnd(),
		ul_type, ul_margin,	br_type, br_margin, hscroll));

	return TRUE;
}

void CResizableDialog::ArrangeLayout()
{
	// init some vars
	CRect wndrc;
	GetClientRect(&wndrc);

	CPoint center;
	center.x = wndrc.Width()/2;
	center.y = wndrc.Height()/2;

	Layout *pl;
	POSITION pos = m_plLayoutList.GetHeadPosition();

	HDWP hdwp = BeginDeferWindowPos(m_plLayoutList.GetCount());

	while (pos != NULL)
	{
		pl = (Layout*)m_plLayoutList.GetNext(pos);
	
		CRect objrc, newrc;
		CWnd* wnd = CWnd::FromHandle(pl->hwnd); // temporary solution

		wnd->GetWindowRect(&objrc);
		ScreenToClient(&objrc);
		
		// upper-left
		switch(pl->ul_type)
		{
		case TOP_LEFT:
			newrc.left = pl->ul_margin.cx;
			newrc.top = pl->ul_margin.cy;
			break;
		case TOP_CENTER:
			newrc.left = center.x + pl->ul_margin.cx;
			newrc.top = pl->ul_margin.cy;
			break;
		case MIDDLE_LEFT:
			newrc.left = pl->ul_margin.cx;
			newrc.top = center.y + pl->ul_margin.cy;
			break;
		case BOTTOM_RIGHT:
			newrc.left = wndrc.right - pl->ul_margin.cx;
			newrc.top = wndrc.bottom - pl->ul_margin.cy;
			break;
		case BOTTOM_CENTER:
			newrc.left = center.x + pl->ul_margin.cx;
			newrc.top = wndrc.bottom - pl->ul_margin.cy;
			break;
		case MIDDLE_RIGHT:
			newrc.left = wndrc.right - pl->ul_margin.cx;
			newrc.top = center.y + pl->ul_margin.cy;
			break;
		case BOTTOM_LEFT:
			newrc.left = pl->ul_margin.cx;
			newrc.top = wndrc.bottom - pl->ul_margin.cy;
			break;
		case TOP_RIGHT:
			newrc.left = wndrc.right - pl->ul_margin.cx;
			newrc.top = pl->ul_margin.cy;
			break;
		case MIDDLE_CENTER:
			newrc.left = center.x + pl->ul_margin.cx;
			newrc.top = center.y + pl->ul_margin.cy;
		}

		// bottom-right
		switch(pl->br_type)
		{
		case TOP_LEFT:
			newrc.right = pl->br_margin.cx;
			newrc.bottom = pl->br_margin.cy;
			break;
		case TOP_CENTER:
			newrc.right = center.x + pl->br_margin.cx;
			newrc.bottom = pl->br_margin.cy;
			break;
		case MIDDLE_LEFT:
			newrc.right = pl->br_margin.cx;
			newrc.bottom = center.y + pl->br_margin.cy;
			break;
		case BOTTOM_RIGHT:
			newrc.right = wndrc.right - pl->br_margin.cx;
			newrc.bottom = wndrc.bottom - pl->br_margin.cy;
			break;
		case BOTTOM_CENTER:
			newrc.right = center.x + pl->br_margin.cx;
			newrc.bottom = wndrc.bottom - pl->br_margin.cy;
			break;
		case MIDDLE_RIGHT:
			newrc.right = wndrc.right - pl->br_margin.cx;
			newrc.bottom = center.y + pl->br_margin.cy;
			break;
		case BOTTOM_LEFT:
			newrc.right = pl->br_margin.cx;
			newrc.bottom = wndrc.bottom - pl->br_margin.cy;
			break;
		case TOP_RIGHT:
			newrc.right = wndrc.right - pl->br_margin.cx;
			newrc.bottom = pl->br_margin.cy;
			break;
		case MIDDLE_CENTER:
			newrc.right = center.x + pl->br_margin.cx;
			newrc.bottom = center.y + pl->br_margin.cy;
			break;
		case NOANCHOR:
			newrc.right = newrc.left + objrc.Width();
			newrc.bottom = newrc.top + objrc.Height();
		}

		if (!newrc.EqualRect(&objrc))
		{
		    DeferWindowPos(hdwp, pl->hwnd, NULL, newrc.left, newrc.top,
				newrc.Width(), newrc.Height(), SWP_NOZORDER |
				SWP_NOACTIVATE);

			if (pl->adj_hscroll)
			{
				// needs repainting, due to horiz scrolling
				int diff = newrc.Width() - objrc.Width();
				int max = wnd->GetScrollLimit(SB_HORZ);
			
				if (max > 0 && wnd->GetScrollPos(SB_HORZ) > max - diff)
					wnd->Invalidate();
			}
		}
	}

	// update size-grip
	InvalidateRect(&m_rcGripRect);
	UpdateGripPos();
	InvalidateRect(&m_rcGripRect);

	// go re-arrange child windows
	EndDeferWindowPos(hdwp);
}

void CResizableDialog::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW)
		return;		// arrangement not needed

	ArrangeLayout();
}

void CResizableDialog::OnPaint() 
{
	CPaintDC dc(this);

	if (m_bShowGrip)
	{
		// draw size-grip
		dc.DrawFrameControl(&m_rcGripRect, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
	}
}

UINT CResizableDialog::OnNcHitTest(CPoint point) 
{
	CPoint pt = point;
	ScreenToClient(&pt);

	if (m_rcGripRect.PtInRect(pt))
		return HTBOTTOMRIGHT;
	
	return CDialog::OnNcHitTest(point);
}

void CResizableDialog::UpdateGripPos()
{
	// size-grip goes bottom right in the client area

	GetClientRect(&m_rcGripRect);

	m_rcGripRect.left = m_rcGripRect.right - m_szGripSize.cx;
	m_rcGripRect.top = m_rcGripRect.bottom - m_szGripSize.cy;
}

void CResizableDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (!m_bInitDone)
		return;

	if (m_bUseMinTrack)
		lpMMI->ptMinTrackSize = m_ptMinTrackSize;

	if (m_bUseMaxTrack)
		lpMMI->ptMaxTrackSize = m_ptMaxTrackSize;

	if (m_bUseMaxRect)
	{
		lpMMI->ptMaxPosition = m_ptMaxPos;
		lpMMI->ptMaxSize = m_ptMaxSize;
	}
}

void CResizableDialog::SetMaximizedRect(const CRect& rc)
{
	m_bUseMaxRect = TRUE;
	m_ptMaxPos = rc.TopLeft();
	m_ptMaxSize = *(POINT*)&rc.Size();
}

void CResizableDialog::ResetMaximizedRect()
{
	m_bUseMaxRect = FALSE;
}

void CResizableDialog::ShowSizeGrip(BOOL bShow)
{
	if (m_bShowGrip != bShow)
	{
		m_bShowGrip = bShow;
		InvalidateRect(&m_rcGripRect);
	}
}

void CResizableDialog::SetMinTrackSize(const CSize& size)
{
	m_bUseMinTrack = TRUE;
	m_ptMinTrackSize = *(POINT*)&size;
}

void CResizableDialog::ResetMinTrackSize()
{
	m_bUseMinTrack = FALSE;
}

void CResizableDialog::SetMaxTrackSize(const CSize& size)
{
	m_bUseMaxTrack = TRUE;
	m_ptMaxTrackSize = *(POINT*)&size;
}

void CResizableDialog::ResetMaxTrackSize()
{
	m_bUseMaxTrack = FALSE;
}

// NOTE: this must be called after all the other settings
//       to have the dialog and its controls displayed properly
void CResizableDialog::EnableSaveRestore(LPCTSTR pszSection, LPCTSTR pszEntry)
{
	m_sSection = pszSection;
	m_sEntry = pszEntry;

	m_bEnableSaveRestore = TRUE;

	LoadWindowRect();
}


// used to save/restore window's size and position
// either in the registry or a private .INI file
// depending on your application settings

#define PROFILE_FMT 	_T("%d,%d,%d,%d,%d,%d")

void CResizableDialog::SaveWindowRect()
{
	CString data;
	WINDOWPLACEMENT wp;

	ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);
	
	RECT& rc = wp.rcNormalPosition;	// alias

	data.Format(PROFILE_FMT, rc.left, rc.top,
		rc.right, rc.bottom, wp.showCmd, wp.flags);

	AfxGetApp()->WriteProfileString(m_sSection, m_sEntry, data);
}

void CResizableDialog::LoadWindowRect()
{
	CString data;
	WINDOWPLACEMENT wp;

	data = AfxGetApp()->GetProfileString(m_sSection, m_sEntry);
	
	if (data.IsEmpty())	// never saved before
		return;
	
	ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
	wp.length = sizeof(WINDOWPLACEMENT);

	RECT& rc = wp.rcNormalPosition;	// alias

	if (_stscanf(data, PROFILE_FMT, &rc.left, &rc.top,
		&rc.right, &rc.bottom, &wp.showCmd, &wp.flags) == 6)
	{
		SetWindowPlacement(&wp);
	}
}
