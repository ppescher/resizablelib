// ResizableDialog.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000 by Paolo Messina
// (ppescher@yahoo.com)
//
// Free for non-commercial and commercial use,
// provided that the original author's name
// and copyright is quoted somewhere in the
// final executable and in the program's help
// or documentation.
// You may change the code to your needs,
// provided that credits to the original
// author are given in the modified files.
// Also a copy of your enhancements would
// be nice, but it's not required. Please,
// consider to share your work on CodeProject.
//  
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResizableDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableDialog

inline void CResizableDialog::Construct()
{
	m_bInitDone = FALSE;

	m_bEnableSaveRestore = FALSE;
}

CResizableDialog::CResizableDialog()
{
	Construct();
}

CResizableDialog::CResizableDialog(UINT nIDTemplate, CWnd* pParentWnd)
	: CDialog(nIDTemplate, pParentWnd)
{
	Construct();
}

CResizableDialog::CResizableDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
	: CDialog(lpszTemplateName, pParentWnd)
{
	Construct();
}

CResizableDialog::~CResizableDialog()
{
}


BEGIN_MESSAGE_MAP(CResizableDialog, CDialog)
	//{{AFX_MSG_MAP(CResizableDialog)
	ON_WM_NCHITTEST()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CResizableDialog message handlers

int CResizableDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// automatically set resizable style
	ModifyStyle(DS_MODALFRAME, WS_POPUP | WS_THICKFRAME);

	return 0;
}

BOOL CResizableDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// gets the template size as the min track size
	CRect rc;
	GetWindowRect(&rc);
	SetMinTrackSize(rc.Size());

	// init

	UpdateGripPos();

	m_bInitDone = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CResizableDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_bEnableSaveRestore)
		SaveWindowRect();

	// remove old windows
	RemoveAllAnchors();
}

void CResizableDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	DrawGrip(dc);
}

void CResizableDialog::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW)
		return;		// arrangement not needed

	if (m_bInitDone)
	{
		// update size-grip
		UpdateGripPos();

		ArrangeLayout();
	}
}

UINT CResizableDialog::OnNcHitTest(CPoint point) 
{
	// test if in size grip
	UINT ht = HitTest(point);
	if (ht != HTNOWHERE)
		return ht;
	
	return CDialog::OnNcHitTest(point);
}

void CResizableDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (!m_bInitDone)
		return;

	MinMaxInfo(lpMMI);
}

// NOTE: this must be called after all the other settings
//       to have the dialog and its controls displayed properly
void CResizableDialog::EnableSaveRestore(LPCTSTR pszSection, LPCTSTR pszEntry)
{
	m_sSection = pszSection;
	m_sEntry = pszEntry;

	m_bEnableSaveRestore = TRUE;

	LoadWindowRect();
}


// used to save/restore window's size and position
// either in the registry or a private .INI file
// depending on your application settings

#define PROFILE_FMT 	_T("%d,%d,%d,%d,%d,%d")

void CResizableDialog::SaveWindowRect()
{
	CString data;
	WINDOWPLACEMENT wp;

	ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);
	
	RECT& rc = wp.rcNormalPosition;	// alias

	data.Format(PROFILE_FMT, rc.left, rc.top,
		rc.right, rc.bottom, wp.showCmd, wp.flags);

	AfxGetApp()->WriteProfileString(m_sSection, m_sEntry, data);
}

void CResizableDialog::LoadWindowRect()
{
	CString data;
	WINDOWPLACEMENT wp;

	data = AfxGetApp()->GetProfileString(m_sSection, m_sEntry);
	
	if (data.IsEmpty())	// never saved before
		return;
	
	ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);

	RECT& rc = wp.rcNormalPosition;	// alias

	if (_stscanf(data, PROFILE_FMT, &rc.left, &rc.top,
		&rc.right, &rc.bottom, &wp.showCmd, &wp.flags) == 6)
	{
		SetWindowPlacement(&wp);
	}
}
