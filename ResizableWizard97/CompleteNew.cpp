// CompleteNew.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "Wizard97.h"
#include "CompleteNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompletionNew property page

IMPLEMENT_DYNCREATE(CCompletionNew, CResizablePageEx)

CCompletionNew::CCompletionNew() : CResizablePageEx(CCompletionNew::IDD)
{
	//{{AFX_DATA_INIT(CCompletionNew)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_psp.dwFlags |= PSP_HIDEHEADER;
}

CCompletionNew::~CCompletionNew()
{
}

void CCompletionNew::DoDataExchange(CDataExchange* pDX)
{
	CResizablePageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompletionNew)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCompletionNew, CResizablePageEx)
	//{{AFX_MSG_MAP(CCompletionNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompletionNew message handlers

BOOL CCompletionNew::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_FINISH);
	return CResizablePageEx::OnSetActive();
}

BOOL CCompletionNew::OnInitDialog() 
{
	CResizablePageEx::OnInitDialog();

	AddAnchor(IDC_BOLDTITLE, TOP_LEFT);
	AddAnchor(IDC_BIGBOLDTITLE, TOP_LEFT);
	AddAnchor(IDC_TRANSPARENT1, TOP_LEFT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
