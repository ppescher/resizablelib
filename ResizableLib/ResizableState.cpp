// ResizableState.cpp: implementation of the CResizableState class.
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
#include "ResizableState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResizableState::CResizableState()
{

}

CResizableState::~CResizableState()
{

}


// used to save/restore window's size and position
// either in the registry or a private .INI file
// depending on your application settings

#define PLACEMENT_ENT	_T("WindowPlacement")
#define PLACEMENT_FMT 	_T("%d,%d,%d,%d,%d,%d")

BOOL CResizableState::SaveWindowRect(LPCTSTR pszSection, BOOL bRectOnly)
{
	CString data;
	WINDOWPLACEMENT wp;

	ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
	wp.length = sizeof(WINDOWPLACEMENT);
	if (!GetResizableWnd()->GetWindowPlacement(&wp))
		return FALSE;
	
	RECT& rc = wp.rcNormalPosition;	// alias

	if (bRectOnly)	// save size/pos only (normal state)
	{
		// use screen coordinates
		GetResizableWnd()->GetWindowRect(&rc);

		data.Format(PLACEMENT_FMT, rc.left, rc.top,
			rc.right, rc.bottom, SW_NORMAL, 0);
	}
	else	// save also min/max state
	{
		// use workspace coordinates
		data.Format(PLACEMENT_FMT, rc.left, rc.top,
			rc.right, rc.bottom, wp.showCmd, wp.flags);
	}

	return AfxGetApp()->WriteProfileString(pszSection, PLACEMENT_ENT, data);
}

BOOL CResizableState::LoadWindowRect(LPCTSTR pszSection, BOOL bRectOnly)
{
	CString data;
	WINDOWPLACEMENT wp;

	data = AfxGetApp()->GetProfileString(pszSection, PLACEMENT_ENT);
	
	if (data.IsEmpty())	// never saved before
		return FALSE;
	
	ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
	wp.length = sizeof(WINDOWPLACEMENT);
	if (!GetResizableWnd()->GetWindowPlacement(&wp))
		return FALSE;

	RECT& rc = wp.rcNormalPosition;	// alias

	if (_stscanf(data, PLACEMENT_FMT, &rc.left, &rc.top,
		&rc.right, &rc.bottom, &wp.showCmd, &wp.flags) == 6)
	{
		if (bRectOnly)	// restore size/pos only
		{
			CRect rect(rc);
			return GetResizableWnd()->SetWindowPos(NULL, rect.left, rect.top,
				rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER |
				SWP_NOREPOSITION);
		}
		else	// restore also min/max state
		{
			return GetResizableWnd()->SetWindowPlacement(&wp);
		}
	}
	return FALSE;
}
