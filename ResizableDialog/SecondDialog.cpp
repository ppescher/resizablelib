// SecondDialog.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "SecondDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSecondDialog dialog


CSecondDialog::CSecondDialog(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CSecondDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSecondDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSecondDialog::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSecondDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSecondDialog, CResizableDialog)
	//{{AFX_MSG_MAP(CSecondDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecondDialog message handlers

BOOL CSecondDialog::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	AddAnchor(IDC_PICTURE1, TOP_LEFT, CSize(30,100));
	AddAnchor(IDC_STATIC1, CSize(30,0), CSize(80,100));
	AddAnchor(IDC_STATIC2, CSize(80,0), BOTTOM_RIGHT);

	AddAnchor(IDC_LINE1, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
