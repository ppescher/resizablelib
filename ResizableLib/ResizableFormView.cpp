// ResizableFormView.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000-2001 by Paolo Messina
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

inline void CResizableFormView::Construct()
{
	m_bInitDone = FALSE;
}

CResizableFormView::CResizableFormView(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{
	Construct();
}

CResizableFormView::CResizableFormView(LPCTSTR lpszTemplateName)
	: CFormView(lpszTemplateName)
{
	Construct();
}

CResizableFormView::~CResizableFormView()
{
}


BEGIN_MESSAGE_MAP(CResizableFormView, CFormView)
	//{{AFX_MSG_MAP(CResizableFormView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
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
	
	if (m_bInitDone)
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

	// get scrollable size
	CSize size = GetTotalSize();

	// before initialization, "size" is dialog template size
	if (!m_bInitDone)
	{
		lpRect->right = lpRect->left + size.cx;
		lpRect->bottom = lpRect->top + size.cy;
		return;
	}

	// otherwise, give correct size if scrollbars active

	if (m_nMapMode < 0)	// scrollbars disabled
		return;

	// enlarge reported client area when needed
	CRect rect(lpRect);
	if (rect.Width() < size.cx)
		lpRect->right = lpRect->left + size.cx;
	if (rect.Height() < size.cy)
		lpRect->bottom = lpRect->top + size.cy;
}

BOOL CResizableFormView::OnEraseBkgnd(CDC* pDC) 
{
	ClipChildren(pDC);	// avoid flickering

	return CFormView::OnEraseBkgnd(pDC);
}
