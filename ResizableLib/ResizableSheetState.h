// ResizableSheetState.h: interface for the CResizableSheetState class.
//
/////////////////////////////////////////////////////////////////////////////
//
// This file is part of ResizableLib
// http://sourceforge.net/projects/resizablelib
//
// Copyright (C) 2000-2004 by Paolo Messina
// http://www.geocities.com/ppescher - mailto:ppescher@hotmail.com
//
// The contents of this file are subject to the Artistic License (the "License").
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.opensource.org/licenses/artistic-license.html
//
// If you find this code useful, credits would be nice!
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZABLESHEETSTATE_H__INCLUDED_)
#define AFX_RESIZABLESHEETSTATE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResizableWndState.h"

class CResizableSheetState : public CResizableWndState  
{
protected:
	// non-zero if successful
	BOOL LoadPage(LPCTSTR pszName);
	BOOL SavePage(LPCTSTR pszName);

	virtual CWnd* GetResizableWnd() const = 0;

public:
	CResizableSheetState();
	virtual ~CResizableSheetState();
};

#endif // !defined(AFX_RESIZABLESHEETSTATE_H__INCLUDED_)
