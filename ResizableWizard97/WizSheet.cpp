// SampWizP.cpp : implementation file
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
#include "WizSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampWizP

IMPLEMENT_DYNAMIC(CWizard97Sheet, CResizableSheetEx)

CWizard97Sheet::CWizard97Sheet(UINT nIDCaption, CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CResizableSheetEx(nIDCaption, pParentWnd, iSelectPage,
				  hbmWatermark, hpalWatermark, hbmHeader)
{
	// add all the pages of the wizard
	AddPage(&m_Intro);
	AddPage(&m_Interior1);
	AddPage(&m_Interior2);
	AddPage(&m_Completion);

	// set the WIZARD97 flag so we'll get the new look
	m_psh.dwFlags |= PSH_WIZARD97;
}

CWizard97Sheet::CWizard97Sheet(LPCTSTR pszCaption, CWnd* pParentWnd,
	UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
: CResizableSheetEx(pszCaption, pParentWnd, iSelectPage,
					  hbmWatermark, hpalWatermark, hbmHeader)

{
	// add all the pages of the wizard
	AddPage(&m_Intro);
	AddPage(&m_Interior1);
	AddPage(&m_Interior2);
	AddPage(&m_Completion);

	// set the WIZARD97 flag so we'll get the new look
	m_psh.dwFlags |= PSH_WIZARD97;
}

CWizard97Sheet::~CWizard97Sheet()
{
}


BEGIN_MESSAGE_MAP(CWizard97Sheet, CResizableSheetEx)
	//{{AFX_MSG_MAP(CWizard97Sheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizard97Sheet message handlers

BOOL CWizard97Sheet::OnInitDialog() 
{
	CResizableSheetEx::OnInitDialog();
	
	CRect rect;
	GetWindowRect(&rect);
	SetMinTrackSize(CSize(GetMinWidth(), rect.Height()));
	
	//EnableSaveRestore("Wizard97");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
