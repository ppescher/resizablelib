// Complete.cpp : implementation file
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
#include "Complete.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompletion property page

IMPLEMENT_DYNCREATE(CCompletion, CResizablePageEx)

CCompletion::CCompletion() : CResizablePageEx(CCompletion::IDD)
{
	//{{AFX_DATA_INIT(CCompletion)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_psp.dwFlags |= PSP_HIDEHEADER;
}

CCompletion::~CCompletion()
{
}

void CCompletion::DoDataExchange(CDataExchange* pDX)
{
	CResizablePageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompletion)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCompletion, CResizablePageEx)
	//{{AFX_MSG_MAP(CCompletion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompletion message handlers

BOOL CCompletion::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons( PSWIZB_BACK | PSWIZB_NEXT | PSWIZB_FINISH);
	return CResizablePageEx::OnSetActive();
}

BOOL CCompletion::OnInitDialog() 
{
	CResizablePageEx::OnInitDialog();

	AddAnchor(IDC_BOLDTITLE, TOP_LEFT);
	AddAnchor(IDC_BIGBOLDTITLE, TOP_LEFT);
	AddAnchor(IDC_TRANSPARENT1, TOP_LEFT);

	LOGFONT lf;
	GetDlgItem(IDC_BOLDTITLE)->GetFont()->GetLogFont(&lf);
	
	lf.lfWeight = FW_BOLD;
	m_fontTitle.CreateFontIndirect(&lf);
	GetDlgItem(IDC_BOLDTITLE)->SetFont(&m_fontTitle);

	lf.lfWeight = FW_BOLD;
	lf.lfHeight *= 2;
	m_fontBig.CreateFontIndirect(&lf);
	GetDlgItem(IDC_BIGBOLDTITLE)->SetFont(&m_fontBig);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
