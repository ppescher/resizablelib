// ResizablePageEx.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000-2004 by Paolo Messina
// (http://www.geocities.com/ppescher - ppescher@hotmail.com)
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

CResizablePageEx::CResizablePageEx()
{
}

CResizablePageEx::CResizablePageEx(UINT nIDTemplate, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle)
	: CPropertyPageEx(nIDTemplate, nIDCaption, nIDHeaderTitle, nIDHeaderSubTitle)
{
}

CResizablePageEx::CResizablePageEx(LPCTSTR lpszTemplateName, UINT nIDCaption, UINT nIDHeaderTitle, UINT nIDHeaderSubTitle)
	: CPropertyPageEx(lpszTemplateName, nIDCaption, nIDHeaderTitle, nIDHeaderSubTitle)
{
}

CResizablePageEx::~CResizablePageEx()
{
}


BEGIN_MESSAGE_MAP(CResizablePageEx, CPropertyPageEx)
	//{{AFX_MSG_MAP(CResizablePageEx)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResizablePageEx message handlers

void CResizablePageEx::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	ArrangeLayout();
}

BOOL CResizablePageEx::OnEraseBkgnd(CDC* pDC) 
{
	ClipChildren(pDC, FALSE);

	BOOL bRet = CPropertyPageEx::OnEraseBkgnd(pDC);

	ClipChildren(pDC, TRUE);

	return bRet;
}

BOOL CResizablePageEx::NeedsRefresh(const CResizableLayout::LayoutInfo& layout,
									const CRect& rectOld, const CRect& rectNew) const
{
	if (m_psp.dwFlags | PSP_HIDEHEADER)
		return TRUE;

	return CResizableLayout::NeedsRefresh(layout, rectOld, rectNew);
}

void CResizablePageEx::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	MinMaxInfo(lpMMI);
}

BOOL CResizablePageEx::OnInitDialog() 
{
	CPropertyPageEx::OnInitDialog();
	
	// set the initial size as the min track size
	CRect rc;
	GetWindowRect(&rc);
	SetMinTrackSize(rc.Size());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
