// DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Demo.h"
#include "DemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog

CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoDlg)
	DDX_Control(pDX, IDC_LIST1, m_ctrlList1);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlCombo1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemoDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CDemoDlg)
	ON_WM_NCCREATE()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg message handlers

BOOL CDemoDlg::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	AddAnchor(IDOK, TOP_RIGHT);
	AddAnchor(IDC_COMBO1, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT1, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_LIST1, TOP_RIGHT, BOTTOM_RIGHT);

	m_ctrlList1.AddString(_T("a very very very loooong item text"));
	m_ctrlList1.SetHorizontalExtent(400);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

#ifndef WS_EX_LAYOUTRTL
#pragma message("Please update your Windows header files, get the latest SDK")
#define WS_EX_LAYOUTRTL	0x00400000
#endif

BOOL CDemoDlg::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
//	ModifyStyleEx(0, WS_EX_LAYOUTRTL);
	
	if (!CResizableDialog::OnNcCreate(lpCreateStruct))
		return FALSE;
	
	return TRUE;
}

void CDemoDlg::OnButton1() 
{
	CString str;
	GetDlgItemText(IDC_EDIT1, str);
	m_ctrlCombo1.AddString(str);
}

void CDemoDlg::OnButton2() 
{
	m_ctrlCombo1.ResetContent();
}

void CDemoDlg::OnButton3() 
{
	CString str;
	GetDlgItemText(IDC_EDIT1, str);
	m_ctrlCombo1.Dir(0, str);
}
