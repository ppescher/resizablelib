// ResizablePageEx.cpp : implementation file
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
#include "ResizablePageEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizablePageEx

IMPLEMENT_DYNCREATE(CResizablePageEx, CPropertyPageEx)

inline void CResizablePageEx::Construct()
{
	m_bInitDone = FALSE;
}

CResizablePageEx::CResizablePageEx()
{
	Construct();
}

CResizablePageEx::CResizablePageEx(UINT nIDTemplate, UINT nIDCaption)
	: CPropertyPageEx(nIDTemplate, nIDCaption)
{
	Construct();
}

CResizablePageEx::CResizablePageEx(LPCTSTR lpszTemplateName, UINT nIDCaption)
	: CPropertyPageEx(lpszTemplateName, nIDCaption)
{
	Construct();
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
