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
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dwGripTempState = 1;
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemoDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CDemoDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg message handlers

BOOL CDemoDlg::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// preset layout
	AddAnchor(IDOK, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);
	
	AddAnchor(IDC_SPIN1, TOP_RIGHT);

	AddAnchor(IDC_LABEL1, TOP_LEFT);
	AddAnchor(IDC_EDIT1, TOP_LEFT, BOTTOM_RIGHT);

	AddAnchor(IDC_GROUP1, BOTTOM_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_RADIO1, BOTTOM_LEFT);
	AddAnchor(IDC_RADIO2, BOTTOM_LEFT);

	// other initializations

	// grip is visible by default
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
	GetDlgItem(IDC_RADIO2)->SetFocus();

	SetDlgItemText(IDC_EDIT1, _T("CResizableDialog\r\n\r\n"
		"This dialog can be resized to full width and half the height of the screen.\r\n"
		"The minimum size is by default the one in the dialog editor.\r\n"
		"Size grip can be switched off.\r\n\r\n"
		"Try to maximize and then to restore the dialog!\r\n"
		"Isn't it cool?"));

	// min/max size settings

	// get desktop size
	CRect rc;
	GetDesktopWindow()->GetClientRect(&rc);

	// set max tracking size to half a screen
	SetMaxTrackSize(CSize(rc.Width(), rc.Height()/2));

	// maximized position and size on top of the screen
	rc.bottom = 100;
	SetMaximizedRect(rc);

	// save/restore
	// (for dialog based app, default is a .INI file with
	// the application's name in the Windows directory)
	EnableSaveRestore(_T("DemoDlg"));

	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CDemoDlg::OnRadio1() 
{
	HideSizeGrip(&m_dwGripTempState);
	UpdateSizeGrip();
}

void CDemoDlg::OnRadio2() 
{
 	ShowSizeGrip(&m_dwGripTempState);
	UpdateSizeGrip();
}


#define WS_EX_LAYOUT_RTL	0x00400000

int CDemoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
//	ModifyStyleEx(0, WS_EX_LAYOUT_RTL);

	if (CResizableDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
