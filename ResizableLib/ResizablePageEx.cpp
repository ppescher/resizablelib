// ResizablePageEx.cpp : implementation file
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
#include "ResizablePageEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizablePageEx

IMPLEMENT_DYNCREATE(CResizablePageEx, CPropertyPageEx)

inline void CResizablePageEx::PrivateConstruct()
{
	m_bInitDone = FALSE;
}

CResizablePageEx::CResizablePageEx()
{
	PrivateConstruct();
}

CResizablePageEx::CResizablePageEx(UINT nIDTemplate, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle)
	: CPropertyPageEx(nIDTemplate, nIDCaption, nIDHeaderTitle, nIDHeaderSubTitle)
{
	PrivateConstruct();
}

CResizablePageEx::CResizablePageEx(LPCTSTR lpszTemplateName, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle)
	: CPropertyPageEx(lpszTemplateName, nIDCaption, nIDHeaderTitle, nIDHeaderSubTitle)
{
	PrivateConstruct();
}

CResizablePageEx::~CResizablePageEx()
{
}


BEGIN_MESSAGE_MAP(CResizablePageEx, CPropertyPageEx)
	//{{AFX_MSG_MAP(CResizablePageEx)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResizablePageEx message handlers


BOOL CResizablePageEx::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
/*	// gets the initial size as the min track size
	CRect rc;
	GetWindowRect(&rc);
*/
	m_bInitDone = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResizablePageEx::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_bInitDone)
		ArrangeLayout();
}

BOOL CResizablePageEx::OnEraseBkgnd(CDC* pDC) 
{
	ClipChildren(pDC);
	
	return CPropertyPageEx::OnEraseBkgnd(pDC);
}

BOOL CResizablePageEx::NeedsRefresh(HWND hWnd)
{
	if (m_psp.dwFlags | PSP_HIDEHEADER)
		return TRUE;

	return CResizableLayout::NeedsRefresh(hWnd);
}
