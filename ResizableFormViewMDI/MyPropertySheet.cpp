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

IMPLEMENT_DYNAMIC(CMyPropertySheet, CResizableSheetEx)

CMyPropertySheet::CMyPropertySheet(CWnd* pWndParent)
	 : CResizableSheetEx(_T("CResizableSheetEx"), pWndParent)
{
	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
}

CMyPropertySheet::~CMyPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CMyPropertySheet, CResizableSheetEx)
	//{{AFX_MSG_MAP(CMyPropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet message handlers

BOOL CMyPropertySheet::OnInitDialog() 
{
	// prevent removal of buttons (hack)
	BOOL bOldState = m_bModeless;
	m_bModeless = FALSE;
	// recover first active page (lost with hack)
	m_nFlags |= WF_CONTINUEMODAL;

	CResizableSheetEx::OnInitDialog();
	
	m_nFlags &= ~WF_CONTINUEMODAL;
	m_bModeless = bOldState;

	return TRUE;
}
