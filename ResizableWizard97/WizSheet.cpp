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
// CWizard97Sheet

IMPLEMENT_DYNAMIC(CWizard97Sheet, CResizableSheetEx)

CWizard97Sheet::CWizard97Sheet(BOOL bOldStyle)
{
	if (bOldStyle)
	{
		VERIFY(m_bmpWatermark.LoadBitmap(IDB_WATERMARK_OLD));
		VERIFY(m_bmpHeader.LoadBitmap(IDB_BANNER_OLD));
	}
	else
	{
		VERIFY(m_bmpWatermark.LoadBitmap(IDB_WATERMARK_NEW));
		VERIFY(m_bmpHeader.LoadBitmap(IDB_BANNER_NEW));
	}

	Construct(IDS_SAMPLEWIZARD, NULL, 0, m_bmpWatermark, NULL, m_bmpHeader);

	// add all the pages of the wizard
	if (bOldStyle)
		AddPage(&m_Intro);
	else
		AddPage(&m_IntroNew);
	AddPage(&m_Interior1);
	AddPage(&m_Interior2);
	if (bOldStyle)
		AddPage(&m_Completion);
	else
		AddPage(&m_CompletionNew);

	// use the right flag for Wizard97 style
	m_psh.dwFlags |= bOldStyle ? PSH_IE4WIZARD97|PSH_STRETCHWATERMARK : PSH_IE5WIZARD97;
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
