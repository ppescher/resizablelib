// DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DemoDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog


CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CDemoDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg message handlers

void CDemoDlg::OnButton1() 
{
	// wizard mode (with help)
	propSheet.m_psh.dwFlags |= PSH_HASHELP;
	propSheet.m_psh.dwFlags |= PSH_WIZARD;
	propSheet.DoModal();
}

void CDemoDlg::OnButton2() 
{
	// property sheet (without help)
	propSheet.m_psh.dwFlags &= ~PSH_HASHELP;
	propSheet.m_psh.dwFlags &= ~PSH_WIZARD;
//	propSheet.EnableStackedTabs(FALSE);
	propSheet.DoModal();
}

