// ResizableWndState.h: interface for the CResizableWndState class.
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

#if !defined(AFX_RESIZABLEWNDSTATE_H__INCLUDED_)
#define AFX_RESIZABLEWNDSTATE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResizableState.h"

class CResizableWndState : public CResizableState  
{
protected:
	// non-zero if successful
	BOOL LoadWindowRect(LPCTSTR pszName, BOOL bRectOnly);
	BOOL SaveWindowRect(LPCTSTR pszName, BOOL bRectOnly);

	virtual CWnd* GetResizableWnd() const = 0;

public:
	CResizableWndState();
	virtual ~CResizableWndState();
};

#endif // !defined(AFX_RESIZABLEWNDSTATE_H__INCLUDED_)
