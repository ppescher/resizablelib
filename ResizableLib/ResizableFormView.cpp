// ResizableFormView.cpp : implementation file
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
#include "ResizableFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableFormView

IMPLEMENT_DYNAMIC(CResizableFormView, CFormView)

inline void CResizableFormView::PrivateConstruct()
{
	m_bInitDone = FALSE;
	m_bGripStatus = TRUE;
}

CResizableFormView::CResizableFormView(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{
	PrivateConstruct();
}

CResizableFormView::CResizableFormView(LPCTSTR lpszTemplateName)
	: CFormView(lpszTemplateName)
{
	PrivateConstruct();
}

CResizableFormView::~CResizableFormView()
{
}


BEGIN_MESSAGE_MAP(CResizableFormView, CFormView)
	//{{AFX_MSG_MAP(CResizableFormView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableFormView diagnostics

#ifdef _DEBUG
void CResizableFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CResizableFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CResizableFormView message handlers

void CResizableFormView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	// hide size grip when there are scrollbars
	CSize size = GetTotalSize();
	if (cx < size.cx || cy < size.cy)
		HideSizeGrip(&m_bGripStatus);
	else
		ShowSizeGrip(&m_bGripStatus);

	// update grip and layout
	UpdateSizeGrip();
	ArrangeLayout();
}

void CResizableFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	m_bInitDone = TRUE;

	// MDI child need this
	ArrangeLayout();
}

void CResizableFormView::GetTotalClientRect(LPRECT lpRect)
{
	GetClientRect(lpRect);

	// get dialog template's size
	// (this is set in CFormView::Create)
	CSize size = GetTotalSize();

	// before initialization use dialog's size
	if (!m_bInitDone)
	{
		lpRect->right = lpRect->left + size.cx;
		lpRect->bottom = lpRect->top + size.cy;
		return;
	}

	// otherwise, give the correct size if scrollbars active

	if (m_nMapMode < 0)	// scrollbars disabled
		return;

	// enlarge reported client area when needed
	CRect rect(lpRect);
	if (rect.Width() < size.cx)
		rect.right = rect.left + size.cx;
	if (rect.Height() < size.cy)
		rect.bottom = rect.top + size.cy;

	rect.OffsetRect(-GetScrollPosition());
	*lpRect = rect;
}

BOOL CResizableFormView::OnEraseBkgnd(CDC* pDC) 
{
	// Windows XP doesn't like clipping regions ...try this!
	EraseBackground(pDC);
	return TRUE;

//	ClipChildren(pDC);	// old-method (for safety)

	return CFormView::OnEraseBkgnd(pDC);
}

int CResizableFormView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create and reposition the size-grip
	if (!CreateSizeGrip())
		return -1;

	// show the grip by default only in MDI windows
	// (grip doesn't work in SDI)
	CWnd* pParentWnd = CWnd::FromHandle(lpCreateStruct->hwndParent);
	if (pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
		ShowSizeGrip();

	return 0;
}
