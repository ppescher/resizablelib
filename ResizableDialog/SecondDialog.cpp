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
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecondDialog message handlers

BOOL CSecondDialog::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();

	// move toolbar to its position
	// before adding it to the layout
	CSize size;
	CRect rc1, rc2;
	GetDlgItem(IDOK)->GetWindowRect(&rc1);
	GetDlgItem(IDC_LINE1)->GetWindowRect(&rc2);
	rc1.left = rc2.left;
	ScreenToClient(&rc1);

	m_tbToolbar.GetToolBarCtrl().GetMaxSize(&size);
	m_tbToolbar.MoveWindow(rc1.left,rc1.top,size.cx,size.cy,FALSE);

	// set layout
	AddAnchor(IDC_PICTURE1, TOP_LEFT, CSize(30,100));
	AddAnchor(IDC_STATIC1, CSize(30,0), CSize(80,100));
	AddAnchor(IDC_STATIC2, CSize(80,0), BOTTOM_RIGHT);

	AddAnchor(IDC_LINE1, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	// these should be equivalent, as long as you leave the
	// default control ID for the toolbar in the Create() call
	// AddAnchor(m_tbToolbar.GetSafeHwnd(), TOP_RIGHT);
	AddAnchor(AFX_IDW_TOOLBAR, BOTTOM_LEFT);

	ResetMinTrackSize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CSecondDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CResizableDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create toolbar, with just buttons and no border
	m_tbToolbar.Create(this);
	m_tbToolbar.LoadToolBar(IDR_TOOLBAR1);
	m_tbToolbar.SetBorders(0,0,0,0);
	m_tbToolbar.SetBarStyle(m_tbToolbar.GetBarStyle() & ~CBRS_BORDER_TOP);

	// tooltips don't work! (see note below)

	return 0;
}

void CSecondDialog::OnButton1() 
{
	// just for test purpose
	AfxMessageBox(_T("Hello no.1"), MB_OK, 0);

	// NOTE: for details on how you could put a toolbar in a dialog
	// box and use ON_UPDATE_COMMAND_UI, see KB article Q141751
	// "SAMPLE: Adding Control Bars to Dialog Boxes in MFC"
	// with the sample project DLGCBR32 and some hints
}
