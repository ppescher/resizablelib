// ResizablePage.cpp : implementation file
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
#include "ResizablePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizablePage

IMPLEMENT_DYNCREATE(CResizablePage, CPropertyPage)

inline void CResizablePage::Construct()
{
	m_bInitDone = FALSE;
}

CResizablePage::CResizablePage()
{
	Construct();
}

CResizablePage::CResizablePage(UINT nIDTemplate, UINT nIDCaption)
	: CPropertyPage(nIDTemplate, nIDCaption)
{
	Construct();
}

CResizablePage::CResizablePage(LPCTSTR lpszTemplateName, UINT nIDCaption)
	: CPropertyPage(lpszTemplateName, nIDCaption)
{
	Construct();
}

CResizablePage::~CResizablePage()
{
}


BEGIN_MESSAGE_MAP(CResizablePage, CPropertyPage)
	//{{AFX_MSG_MAP(CResizablePage)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResizablePage message handlers


BOOL CResizablePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
/*	// gets the initial size as the min track size
	CRect rc;
	GetWindowRect(&rc);
*/
	m_bInitDone = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResizablePage::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_bInitDone)
		ArrangeLayout();
}
