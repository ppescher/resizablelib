// Wizard97.cpp : Defines the class behaviors for the application.
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
// CWizard97App

BEGIN_MESSAGE_MAP(CWizard97App, CWinApp)
	//{{AFX_MSG_MAP(CWizard97App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizard97App construction

CWizard97App::CWizard97App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWizard97App object

CWizard97App theApp;

/////////////////////////////////////////////////////////////////////////////
// CWizard97App initialization

BOOL CWizard97App::InitInstance()
{
	// use old style wizard (ala Wizard97 with IE4)
	{
		CWizard97Sheet dlg(TRUE);
		dlg.DoModal();
	}

	// display a new style wizard (ala Windows 2000)
	{
		CWizard97Sheet dlg(FALSE);
		dlg.DoModal();
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
