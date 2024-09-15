
// DemoDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Demo.h"
#include "DemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDemoDlg dialog



CDemoDlg::CDemoDlg(CWnd* pParent /*=nullptr*/)
	: CResizableDialogEx(IDD_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_dwGripTempState = 1;
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDemoDlg, CResizableDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CDemoDlg message handlers

BOOL CDemoDlg::OnInitDialog()
{
	CResizableDialogEx::OnInitDialog();

	SetBackgroundImage(IDB_BITMAP1, BACKGR_TILE);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// preset layout
	AddAnchor(IDC_SPIN1, TOP_RIGHT);

	AddAnchor(IDC_LABEL1, TOP_LEFT);
	AddAnchor(IDC_EDIT1, TOP_LEFT, BOTTOM_RIGHT);

	AddAnchor(IDC_GROUP1, BOTTOM_LEFT, BOTTOM_CENTER);
	AddAnchor(IDC_RADIO1, BOTTOM_LEFT);
	AddAnchor(IDC_RADIO2, BOTTOM_LEFT);

	AddAnchor(IDC_GROUP2, BOTTOM_CENTER, BOTTOM_RIGHT);
	AddAnchor(IDC_RADIO3, BOTTOM_CENTER);
	AddAnchor(IDC_RADIO4, BOTTOM_CENTER);
	AddAnchor(IDC_RADIO5, BOTTOM_CENTER);

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

	SetDlgItemText(IDC_EDIT1, _T("CResizableDialogEx\r\n\r\n")
		_T("This dialog can be resized to full width and half the height of the screen.\r\n")
		_T("The minimum size is by default the one in the dialog editor.\r\n")
		_T("Size grip can be switched off.\r\n\r\n")
		_T("Try to maximize and then to restore the dialog!\r\n")
		_T("Isn't it cool?"));
	::SetWindowSubclass(GetDlgItem(IDC_EDIT1)->GetSafeHwnd(), EditSubclassProc, 12345u, NULL);


	// min/max size settings
	UpdateMaxSize();

	// save/restore
	// (for dialog based app, default is a .INI file with
	// the application's name in the Windows directory)
	EnableSaveRestore(_T("DemoDlg"));

	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CDemoDlg::UpdateMaxSize()
{
	ResetMaxTrackSize();
	ResetMaximizedRect();

	// get desktop size
	CRect rcMax;
	GetDesktopWindow()->GetClientRect(&rcMax);

	if (GetThemeAppProperties() & STAP_ALLOW_NONCLIENT)
	{
		// modern style windows use the borders for drop-shadow effects but window size appears smaller when maximized
		// try to determine the correct size for maximized state, based on the current style
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
	// use half height for tracking size limits
	int half = rcMax.Height() / 2;
	// clip maximized size to the top 100 pixels
	rcMax.bottom = 100;

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

void CDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CResizableDialogEx::OnSysCommand(nID, lParam);
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

BOOL CALLBACK CDemoDlg::SendThemeChangedProc(HWND hwnd, LPARAM /*lParam*/)
{
	::SendMessage(hwnd, WM_THEMECHANGED, 0, 0);
	return TRUE;
}

DWORD CDemoDlg::GetThemeProperties()
{
	static HMODULE hThemeLib = GetModuleHandle(_T("UxTheme.dll"));
	typedef DWORD(STDAPICALLTYPE* LPFNGETTHEMEAPPPROPERTIES)();
	static LPFNGETTHEMEAPPPROPERTIES lpfnGetThemeAppProperties = (hThemeLib == NULL) ? NULL :
		(LPFNGETTHEMEAPPPROPERTIES)GetProcAddress(hThemeLib, "GetThemeAppProperties");

	if (lpfnGetThemeAppProperties == NULL)
		return 0; // do nothing if no theme support

	// return theme settings
	return lpfnGetThemeAppProperties();
}

void CDemoDlg::SetThemeProperties(DWORD dwFlags)
{
	static HMODULE hThemeLib = GetModuleHandle(_T("UxTheme.dll"));
	typedef void (STDAPICALLTYPE *LPFNSETTHEMEAPPPROPERTIES)(DWORD);
	static LPFNSETTHEMEAPPPROPERTIES lpfnSetThemeAppProperties = (hThemeLib == NULL) ? NULL :
		(LPFNSETTHEMEAPPPROPERTIES)GetProcAddress(hThemeLib, "SetThemeAppProperties");

	if (lpfnSetThemeAppProperties == NULL)
		return; // do nothing if no theme support

	// apply new theme settings and redraw all windows
	lpfnSetThemeAppProperties(dwFlags);
	SendMessage(WM_THEMECHANGED);
	EnumChildWindows(m_hWnd, SendThemeChangedProc, 0);
	InvalidateRect(NULL);
	UpdateWindow();
	UpdateMaxSize();
}

void CDemoDlg::OnRadio3()
{
	SetThemeProperties(STAP_ALLOW_NONCLIENT | STAP_ALLOW_CONTROLS | STAP_ALLOW_WEBCONTENT);
}

void CDemoDlg::OnRadio4()
{
	SetThemeProperties(STAP_ALLOW_NONCLIENT);
}

void CDemoDlg::OnRadio5()
{
	SetThemeProperties(0);
}

LRESULT WINAPI CDemoDlg::EditSubclassProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam,
	UINT_PTR uIdSubclass,
	DWORD_PTR dwRefData
)
{
	LRESULT res = 0;
	switch (uMsg)
	{
	case WM_CUT:
	case WM_PASTE:
	case WM_CLEAR:

	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		::InvalidateRect(hWnd, NULL, TRUE);
		res = ::DefSubclassProc(hWnd, uMsg, wParam, lParam);
		return res;

	case WM_SYSKEYUP:
	case WM_KEYUP:

	case WM_VSCROLL:
	case WM_HSCROLL:
		CWnd::FromHandle(hWnd)->SetRedraw(FALSE);
		res = ::DefSubclassProc(hWnd, uMsg, wParam, lParam);
		CWnd::FromHandle(hWnd)->SetRedraw(TRUE);
		::RedrawWindow(hWnd, NULL, NULL, 0);
		return res;
	}
	return ::DefSubclassProc(hWnd, uMsg, wParam, lParam);
	//		::RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
}
