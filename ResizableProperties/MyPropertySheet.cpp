// MyPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MyPropertySheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet

IMPLEMENT_DYNAMIC(CMyPropertySheet, CResizableSheet)

CMyPropertySheet::CMyPropertySheet(CWnd* pWndParent)
	 : CResizableSheet(_T("CResizableSheet"), pWndParent)
{
	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
}

CMyPropertySheet::~CMyPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CMyPropertySheet, CResizableSheet)
	//{{AFX_MSG_MAP(CMyPropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet message handlers

BOOL CMyPropertySheet::OnInitDialog() 
{
	CResizableSheet::OnInitDialog();

	// ModifyStyleEx(WS_EX_DLGMODALFRAME, 0);	// enable sys menu and icon
	
	// set minimal size
	CRect rc;
	GetWindowRect(&rc);
	SetMinTrackSize(CSize(GetMinWidth(), rc.Height()));
	//ResetMinTrackSize();

	// enable save/restore, with active page
	EnableSaveRestore(_T("MyPropertySheet"), TRUE, TRUE);

	return TRUE;
}
