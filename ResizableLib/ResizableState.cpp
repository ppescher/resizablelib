// ResizableState.cpp: implementation of the CResizableState class.
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000-2004 by Paolo Messina
// (http://www.geocities.com/ppescher - ppescher@hotmail.com)
//
// The contents of this file are subject to the Artistic License (the "License").
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.opensource.org/licenses/artistic-license.html
//
// If you find this code useful, credits would be nice!
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
#define PLACEMENT_FMT 	_T("%d,%d,%d,%d,%d,%d,%d,%d")

BOOL CResizableState::SaveWindowRect(LPCTSTR pszSection, BOOL bRectOnly)
{
	CString data;
	WINDOWPLACEMENT wp;

	ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
	wp.length = sizeof(WINDOWPLACEMENT);
	if (!GetResizableWnd()->GetWindowPlacement(&wp))
		return FALSE;
	
	// use workspace coordinates
	RECT& rc = wp.rcNormalPosition;

	if (bRectOnly)	// save size/pos only (normal state)
	{
		data.Format(PLACEMENT_FMT, rc.left, rc.top,
			rc.right, rc.bottom, SW_SHOWNORMAL, 0, 0, 0);
	}
	else	// save also min/max state
	{
		data.Format(PLACEMENT_FMT, rc.left, rc.top,
			rc.right, rc.bottom, wp.showCmd, wp.flags,
			wp.ptMinPosition.x, wp.ptMinPosition.y);
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

	// use workspace coordinates
	RECT& rc = wp.rcNormalPosition;

	if (_stscanf(data, PLACEMENT_FMT, &rc.left, &rc.top,
		&rc.right, &rc.bottom, &wp.showCmd, &wp.flags,
		&wp.ptMinPosition.x, &wp.ptMinPosition.y) == 8)
	{
		if (bRectOnly)	// restore size/pos only
		{
			wp.showCmd = SW_SHOWNORMAL;
			wp.flags = 0;
			return GetResizableWnd()->SetWindowPlacement(&wp);
		}
		else	// restore also min/max state
		{
			return GetResizableWnd()->SetWindowPlacement(&wp);
		}
	}
	return FALSE;
}
