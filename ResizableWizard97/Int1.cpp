// Int1.cpp : implementation file
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
#include "Int1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInterior1 property page

IMPLEMENT_DYNCREATE(CInterior1, CResizablePageEx)

CInterior1::CInterior1() : CResizablePageEx(CInterior1::IDD, 0, IDS_HEADERTITLE, IDS_HEADERSUBTITLE)
{
	//{{AFX_DATA_INIT(CInterior1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_strHeaderTitle = "Setup Page (Test)";
	//m_psp.dwFlags |= PSP_HIDEHEADER;
}

CInterior1::~CInterior1()
{
}

void CInterior1::DoDataExchange(CDataExchange* pDX)
{
	CResizablePageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInterior1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInterior1, CResizablePageEx)
	//{{AFX_MSG_MAP(CInterior1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInterior1 message handlers

BOOL CInterior1::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT);
	return CResizablePageEx::OnSetActive();
}

BOOL CInterior1::OnInitDialog() 
{
	CResizablePageEx::OnInitDialog();
	
	AddAnchor(IDC_STATIC_TITLE, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_LIST1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_GROUP1, TOP_RIGHT, BOTTOM_RIGHT);
	AddAnchor(IDC_BUTTON1, BOTTOM_RIGHT);
	AddAnchor(IDC_STATIC1, BOTTOM_LEFT, BOTTOM_CENTER);
	AddAnchor(IDC_STATIC2, BOTTOM_CENTER, BOTTOM_RIGHT);
	AddAnchor(IDC_STATIC3, TOP_LEFT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
