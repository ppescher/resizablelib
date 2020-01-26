// ResizableDialog.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// This file is part of ResizableLib
// https://github.com/ppescher/resizablelib
//
// Copyright (C) 2000-2015 by Paolo Messina
// mailto:ppescher@hotmail.com
//
// The contents of this file are subject to the Artistic License 2.0
// http://opensource.org/licenses/Artistic-2.0
//
// If you find this code useful, credits would be nice!
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

inline void CResizableDialog::PrivateConstruct()
{
	m_bEnableSaveRestore = FALSE;
	m_dwGripTempState = 1;
	m_bRectOnly = FALSE;
}

CResizableDialog::CResizableDialog()
{
	PrivateConstruct();
}

CResizableDialog::CResizableDialog(UINT nIDTemplate, CWnd* pParentWnd)
	: BASE_DIALOG_CLASS(nIDTemplate, pParentWnd)
{
	PrivateConstruct();
}

CResizableDialog::CResizableDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
	: BASE_DIALOG_CLASS(lpszTemplateName, pParentWnd)
{
	PrivateConstruct();
}

CResizableDialog::~CResizableDialog()
{
}


BEGIN_MESSAGE_MAP(CResizableDialog, BASE_DIALOG_CLASS)
	//{{AFX_MSG_MAP(CResizableDialog)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_NCCREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableDialog message handlers

BOOL CResizableDialog::OnNcCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (!BASE_DIALOG_CLASS::OnNcCreate(lpCreateStruct))
		return FALSE;

	// child dialogs don't want resizable border or size grip,
	// nor they can handle the min/max size constraints
	BOOL bChild = lpCreateStruct->style & WS_CHILD;

	// create and init the size-grip
	if (!CreateSizeGrip(!bChild))
		return FALSE;

	// Moved from behind if (!bChild) because user could resize the dialog smaller as in resource defined and that causes some static text to be clipped or dissapear.
	MakeResizable(lpCreateStruct);

	if (!bChild)
	{
		// set the initial size as the min track size
		SetMinTrackSize(CSize(lpCreateStruct->cx, lpCreateStruct->cy));
	}
	
	return TRUE;
}

void CResizableDialog::OnDestroy() 
{
	if (m_bEnableSaveRestore)
		SaveWindowRect(m_sSection, m_bRectOnly);

	// reset instance data
	RemoveAllAnchors();
	ResetAllRects();
	PrivateConstruct();

	BASE_DIALOG_CLASS::OnDestroy();
}

void CResizableDialog::OnSize(UINT nType, int cx, int cy) 
{
	BASE_DIALOG_CLASS::OnSize(nType, cx, cy);
	
	if (nType == SIZE_MAXHIDE || nType == SIZE_MAXSHOW)
		return;		// arrangement not needed

	if (nType == SIZE_MAXIMIZED)
		HideSizeGrip(&m_dwGripTempState);
	else
		ShowSizeGrip(&m_dwGripTempState);

	// update grip and layout
	UpdateSizeGrip();
	ArrangeLayout();
}

void CResizableDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	MinMaxInfo(lpMMI);
}

// NOTE: this must be called after setting the layout
//       to have the dialog and its controls displayed properly
void CResizableDialog::EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly)
{
	m_sSection = pszSection;

	m_bEnableSaveRestore = TRUE;
	m_bRectOnly = bRectOnly;

	// restore immediately
	LoadWindowRect(pszSection, bRectOnly);
}

BOOL CResizableDialog::OnEraseBkgnd(CDC* pDC) 
{
	ClipChildren(pDC, FALSE);

	BOOL bRet = BASE_DIALOG_CLASS::OnEraseBkgnd(pDC);

	ClipChildren(pDC, TRUE);

	return bRet;
}

LRESULT CResizableDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message != WM_NCCALCSIZE || wParam == 0)
		return BASE_DIALOG_CLASS::WindowProc(message, wParam, lParam);

	LRESULT lResult = 0;
	HandleNcCalcSize(FALSE, (LPNCCALCSIZE_PARAMS)lParam, lResult);
	lResult = BASE_DIALOG_CLASS::WindowProc(message, wParam, lParam);
	HandleNcCalcSize(TRUE, (LPNCCALCSIZE_PARAMS)lParam, lResult);
	return lResult;
}
