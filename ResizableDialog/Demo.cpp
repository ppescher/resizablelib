// Demo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Demo.h"
#include "DemoDlg.h"
#include "SecondDialog.h"

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
	CDemoDlg dlg;
	CSecondDialog dlg2;

	dlg.DoModal();
	dlg2.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
