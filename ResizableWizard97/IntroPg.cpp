// IntroPg.cpp : implementation file
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
#include "IntroPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIntroPage property page

IMPLEMENT_DYNCREATE(CIntroPage, CResizablePageEx)

CIntroPage::CIntroPage() : CResizablePageEx(CIntroPage::IDD)
{
	//{{AFX_DATA_INIT(CIntroPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_psp.dwFlags |= PSP_HIDEHEADER;
}

CIntroPage::~CIntroPage()
{
}

void CIntroPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntroPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIntroPage, CResizablePageEx)
	//{{AFX_MSG_MAP(CIntroPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntroPage message handlers

BOOL CIntroPage::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons(PSWIZB_NEXT);
	return CResizablePageEx::OnSetActive();
}

BOOL CIntroPage::OnInitDialog() 
{
	CResizablePageEx::OnInitDialog();
	
	AddAnchor(IDC_BOLDTITLE, TOP_LEFT);
	AddAnchor(IDC_BIGBOLDTITLE, TOP_LEFT);
	AddAnchor(IDC_TRANSPARENT1, TOP_LEFT, MIDDLE_RIGHT);
	AddAnchor(IDC_TRANSPARENT2, MIDDLE_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TRANSPARENT3, BOTTOM_LEFT);
	AddAnchor(IDC_EDIT1, BOTTOM_LEFT, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
}
