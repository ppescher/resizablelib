// IntroPgNew.cpp : implementation file
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
#include "IntroPgNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIntroPageNew property page

IMPLEMENT_DYNCREATE(CIntroPageNew, CResizablePageEx)

CIntroPageNew::CIntroPageNew() : CResizablePageEx(CIntroPageNew::IDD)
{
	//{{AFX_DATA_INIT(CIntroPageNew)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_psp.dwFlags |= PSP_HIDEHEADER;
}

CIntroPageNew::~CIntroPageNew()
{
}

void CIntroPageNew::DoDataExchange(CDataExchange* pDX)
{
	CResizablePageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntroPageNew)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIntroPageNew, CResizablePageEx)
	//{{AFX_MSG_MAP(CIntroPageNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntroPageNew message handlers

BOOL CIntroPageNew::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons(PSWIZB_NEXT);
	return CResizablePageEx::OnSetActive();
}

BOOL CIntroPageNew::OnInitDialog() 
{
	CResizablePageEx::OnInitDialog();
	
	AddAnchor(IDC_BOLDTITLE, TOP_LEFT);
	AddAnchor(IDC_BIGBOLDTITLE, TOP_LEFT);
	AddAnchor(IDC_TRANSPARENT1, TOP_LEFT, MIDDLE_RIGHT);
	AddAnchor(IDC_TRANSPARENT2, MIDDLE_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_TRANSPARENT3, BOTTOM_LEFT);
	AddAnchor(IDC_EDIT1, BOTTOM_LEFT, BOTTOM_RIGHT);

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
}
