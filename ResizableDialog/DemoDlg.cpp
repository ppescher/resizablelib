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
	m_hCurrentMonitor = NULL;
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
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_WM_CLOSE()
	ON_COMMAND(IDCANCEL, &CDemoDlg::OnCancel)
	ON_COMMAND(IDOK, &CDemoDlg::OnOk)
	ON_WM_NCDESTROY()
	ON_WM_WINDOWPOSCHANGED()
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
	AddAnchor(IDC_SPIN1, TOP_RIGHT);

	AddAnchor(IDC_LABEL1, TOP_LEFT);
	AddAnchor(IDC_EDIT1, TOP_LEFT, BOTTOM_RIGHT);

	AddAnchor(IDC_GROUP1, BOTTOM_LEFT, ANCHOR(33, 100));
	AddAnchor(IDC_RADIO1, BOTTOM_LEFT);
	AddAnchor(IDC_RADIO2, BOTTOM_LEFT);

	AddAnchor(IDC_GROUP2, ANCHOR(33, 100), ANCHOR(66, 100));
	AddAnchor(IDC_RADIO3, ANCHOR(33, 100));
	AddAnchor(IDC_RADIO4, ANCHOR(33, 100));
	AddAnchor(IDC_RADIO5, ANCHOR(33, 100));

	AddAnchor(IDC_GROUP3, ANCHOR(66, 100), BOTTOM_RIGHT);
	AddAnchor(IDC_RADIO6, ANCHOR(66, 100));
	AddAnchor(IDC_RADIO7, ANCHOR(66, 100));

	// other initializations

	// grip is visible by default
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
	GetDlgItem(IDC_RADIO2)->SetFocus();

	// setup theme support
	if (GetModuleHandle(_T("UxTheme.dll")) != NULL)
	{
		CheckRadioButton(IDC_RADIO3, IDC_RADIO5, IDC_RADIO3);
	}
	else
	{
		CheckRadioButton(IDC_RADIO3, IDC_RADIO5, IDC_RADIO5);
		GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO5)->EnableWindow(FALSE);
	}

	// setup DWM support
	m_hDwmLib = LoadLibrary(_T("dwmapi.dll"));
	if (m_hDwmLib != NULL)
	{
		CheckRadioButton(IDC_RADIO6, IDC_RADIO7, IDC_RADIO6);
	}
	else
	{
		CheckRadioButton(IDC_RADIO6, IDC_RADIO7, IDC_RADIO7);
		GetDlgItem(IDC_RADIO6)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO7)->EnableWindow(FALSE);
	}

	SetDlgItemText(IDC_EDIT1, _T("CResizableDialog\r\n\r\n")
		_T("This dialog can be resized to full width and half the height of the screen.\r\n")
		_T("The minimum size is by default the one in the dialog editor.\r\n")
		_T("Size grip can be switched off. Visual styles and theme can be switched at run-time.\r\n\r\n")
		_T("Try to maximize and then to restore the dialog!\r\n")
		_T("Isn't it cool?"));

	// save/restore
	// (for dialog based app, default is a .INI file with
	// the application's name in the Windows directory)
	EnableSaveRestore(_T("DemoDlg"));

	// min/max size settings
	UpdateCurrentMonitor();
	UpdateMaxSize();

	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CDemoDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CResizableDialog::OnWindowPosChanged(lpwndpos);

	// update min/max size settings when monitor changes
	if (UpdateCurrentMonitor())
		UpdateMaxSize();
}

BOOL CDemoDlg::UpdateCurrentMonitor()
{
	HMONITOR hMon = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	if (m_hCurrentMonitor != hMon)
	{
		m_hCurrentMonitor = hMon;
		return TRUE;
	}
	return FALSE;
}

void CDemoDlg::UpdateMaxSize()
{
	ResetMaxTrackSize();
	ResetMaximizedRect();

	// get desktop size
	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(m_hCurrentMonitor, &mi);

	CRect rcMax = mi.rcMonitor; // correct area based on monitor DPI and app DPI awareness
	rcMax.MoveToXY(0, 0); // maximized rect wants zero offset (relative to current monitor)

	// use half height for tracking size limits
	int half = rcMax.Height() / 2;
	// clip maximized size to the top 100 pixels
	rcMax.bottom = 100;

	// do not apply correction for old style themed or classic window borders
	if (!IsDlgButtonChecked(IDC_RADIO5) && !IsDlgButtonChecked(IDC_RADIO7))
	{
		// modern style windows leave space around the client area when maximized (ugly transparent border)
		// try to determine the correct size to fill the target rect, based on the current style
		CRect rcWnd, rcClient, rcMargins;
		GetWindowRect(&rcWnd);
		GetClientRect(&rcClient);
		::MapWindowPoints(GetSafeHwnd(), NULL, (LPPOINT)&rcClient, 2);

		rcMargins.left = rcClient.left - rcWnd.left;
		rcMargins.top = rcClient.top - rcWnd.top - GetSystemMetrics(GetExStyle() & WS_EX_TOOLWINDOW ? SM_CYSMCAPTION : SM_CYCAPTION);
		rcMargins.right = rcWnd.right - rcClient.right;
		rcMargins.bottom = rcWnd.bottom - rcClient.bottom;

		rcMax.InflateRect(&rcMargins);
	}

	// maximized position and size on top of the screen
	SetMaximizedRect(rcMax);

	// limit max tracking size to half a screen vertically
	SetMaxTrackSize(CSize(rcMax.Width(), half));

	if (IsZoomed())
	{
		// window already maximized needs to be refreshed, but we try to avoid flickering
		// when you disable redraw the window loses WS_VISIBLE but stay displayed on the screen
		// so the only chance is to use SW_HIDE to change the maximized state without affecting visibility
		// (any other show command would make the window appear again and flash, or it won't refresh the maximized state)
		// then we maximize again, which implies the window will also be visible and then we restore redraw
		SetRedraw(FALSE);
		ShowWindow(SW_HIDE);
		ShowWindow(SW_SHOWMAXIMIZED);
		SetRedraw(TRUE);
	}
}

void CDemoDlg::OnRadio1() 
{
	ModifyStyle(WS_THICKFRAME, 0, SWP_FRAMECHANGED | SWP_DRAWFRAME);
	HideSizeGrip(&m_dwGripTempState);
	UpdateSizeGrip();
	UpdateMaxSize();
}

void CDemoDlg::OnRadio2() 
{
	ModifyStyle(0, WS_THICKFRAME, SWP_FRAMECHANGED | SWP_DRAWFRAME);
	ShowSizeGrip(&m_dwGripTempState);
	UpdateSizeGrip();
	UpdateMaxSize();
}

BOOL CDemoDlg::SetDwmAttribute(DWORD attr, int value)
{
	typedef HRESULT(WINAPI* LPFN_DWM_SET_WINDOW_ATTRIBUTE)
		(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);

	if (!m_hDwmLib)
		return FALSE; // XP or earlier, DWM not present

	LPFN_DWM_SET_WINDOW_ATTRIBUTE lpfnDwmSetWindowAttribute =
		(LPFN_DWM_SET_WINDOW_ATTRIBUTE) GetProcAddress(m_hDwmLib, "DwmSetWindowAttribute");

	if (!lpfnDwmSetWindowAttribute)
		return FALSE;

	HRESULT hr = lpfnDwmSetWindowAttribute(m_hWnd, attr, &value, sizeof(value));
	return SUCCEEDED(hr);
}

BOOL CALLBACK CDemoDlg::SetChildThemeProc(HWND hwnd, LPARAM lParam)
{
	if (!(lParam & STAP_ALLOW_CONTROLS))
		::SetWindowTheme(hwnd, L"", L""); // set classic look for child controls (include embedded scrollbars)
	else
		::SetWindowTheme(hwnd, NULL, NULL); // reset to default style
	return TRUE;
}

void CDemoDlg::SetThemeProperties(DWORD dwFlags)
{
	// apply new theme settings and redraw all windows (this dialog only)
	if (!(dwFlags & STAP_ALLOW_NONCLIENT))
		::SetWindowTheme(m_hWnd, L"", L""); // set classic look for parent window
	else
		::SetWindowTheme(m_hWnd, NULL, NULL); // reset to default style

	EnumChildWindows(m_hWnd, SetChildThemeProc, dwFlags);

	SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOREPOSITION);
	UpdateMaxSize();
}

void CDemoDlg::OnRadio3()
{
	SetThemeProperties(STAP_ALLOW_NONCLIENT | STAP_ALLOW_CONTROLS | STAP_ALLOW_WEBCONTENT);

	if (m_hDwmLib != NULL)
	{
		GetDlgItem(IDC_RADIO6)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO7)->EnableWindow(TRUE);

		if (IsDlgButtonChecked(IDC_RADIO7))
			OnRadio7();
		else
			OnRadio6();
	}
}

void CDemoDlg::OnRadio4()
{
	SetThemeProperties(STAP_ALLOW_NONCLIENT);

	if (m_hDwmLib != NULL)
	{
		GetDlgItem(IDC_RADIO6)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO7)->EnableWindow(TRUE);

		if (IsDlgButtonChecked(IDC_RADIO7))
			OnRadio7();
		else
			OnRadio6();
	}
}

void CDemoDlg::OnRadio5()
{
	SetThemeProperties(0);

	GetDlgItem(IDC_RADIO6)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO7)->EnableWindow(FALSE);
}

void CDemoDlg::OnRadio6()
{
	SetDwmAttribute(DWMWA_NCRENDERING_POLICY, DWMNCRP_ENABLED);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOREPOSITION);
	UpdateMaxSize();
}

void CDemoDlg::OnRadio7()
{
	SetDwmAttribute(DWMWA_NCRENDERING_POLICY, DWMNCRP_DISABLED);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOREPOSITION);
	UpdateMaxSize();
}

afx_msg void CDemoDlg::OnClose()
{
	DestroyWindow();
}

void CDemoDlg::OnCancel()
{
	DestroyWindow();
}

void CDemoDlg::OnOk()
{
	DestroyWindow();
}

void CDemoDlg::OnNcDestroy()
{
	FreeLibrary(m_hDwmLib);

	// we are the main window if we were created modeless
	// in that case we need to free memory allocated by new
	// but main window is cleared inside default message handler
	// so we catch it here and delete later
	BOOL needsDelete = (AfxGetMainWnd() == this);

	CResizableDialog::OnNcDestroy();
	
	if (needsDelete)
		delete this;
}
