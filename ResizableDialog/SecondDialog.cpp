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
	ON_COMMAND(ID_BUTTON2, OnButton2)
	ON_COMMAND(ID_BUTTON3, OnButton3)
	ON_COMMAND(ID_BUTTON4, OnButton4)
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
	CRect rect, rc;
	GetDlgItem(IDOK)->GetWindowRect(&rect);
	GetDlgItem(IDC_LINE1)->GetWindowRect(&rc);
	rect.left = rc.left;
	ScreenToClient(&rect);
	m_wndToolBar.GetToolBarCtrl().GetMaxSize(&size);
	rect.right = rect.left + size.cx;
	rect.bottom = rect.top + size.cy;

	m_wndToolBar.SetBarStyle(CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, 
		CWnd::reposDefault, NULL, rect);
	m_wndToolBar.SetWindowPos(&wndBottom,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	// set layout
	AddAnchor(IDC_PICTURE1, TOP_LEFT, CSize(30,100));
	AddAnchor(IDC_STATIC1, CSize(30,0), CSize(80,100));
	AddAnchor(IDC_STATIC2, CSize(80,0), BOTTOM_RIGHT);

	AddAnchor(IDC_LINE1, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	// these should be equivalent, as long as you leave the
	// default control ID for the toolbar in the Create() call
	//AddAnchor(AFX_IDW_TOOLBAR, BOTTOM_LEFT);
	AddAnchor(m_wndToolBar.GetSafeHwnd(), BOTTOM_LEFT);

	ResetMinTrackSize();

	return TRUE;  // return TRUE unless you set the focus to a control
}

int CSecondDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CResizableDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create toolbar, with just buttons and no border
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_TRANSPARENT|TBSTYLE_FLAT) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.SetBorders(0,0,0,0);
	// tooltips don't work! (see note below)

	return 0;
}

void CSecondDialog::OnButton1() 
{
	// set background color
	((CDemoApp*)AfxGetApp())->SetDialogBkColor(RGB(250,200,200));
	Invalidate();

	// NOTE: for details on how you could put a toolbar in a dialog
	// box and use ON_UPDATE_COMMAND_UI, see KB article Q141751
	// "SAMPLE: Adding Control Bars to Dialog Boxes in MFC"
	// with the sample project DLGCBR32 and some hints
}

void CSecondDialog::OnButton2() 
{
	// set background color
	((CDemoApp*)AfxGetApp())->SetDialogBkColor(RGB(200,200,250));
	Invalidate();
}

void CSecondDialog::OnButton3() 
{
	// set background color
	((CDemoApp*)AfxGetApp())->SetDialogBkColor(RGB(190,250,190));
	Invalidate();
}

void CSecondDialog::OnButton4() 
{
	// set background color
	((CDemoApp*)AfxGetApp())->SetDialogBkColor(RGB(250,250,180));
	Invalidate();
}

