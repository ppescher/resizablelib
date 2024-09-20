// Demo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Demo.h"
#include "DemoDlg.h"
#include "SecondDialog.h"
#include "TestDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoApp

BEGIN_MESSAGE_MAP(CDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CDemoApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoApp construction

CDemoApp::CDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDemoApp object

CDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDemoApp initialization

BOOL CDemoApp::InitInstance()
{
	SetRegistryKey(_T("Demo"));

	CDemoDlg* dlg = new CDemoDlg();
	CSecondDialog dlg2;
	CTestDialog dlg3;

	// first dialog is created modeless and initially hidden
	dlg->Create(dlg->IDD);
	// after the second modal dialog is dismissed, show the first
	dlg2.DoModal();
	dlg->ShowWindow(SW_SHOW);
	// show the last modal dialog
	dlg3.DoModal();
	// keep running until the first modeless dialog is closed
	m_pMainWnd = dlg;
	// start the application's message pump
	return TRUE;
}
