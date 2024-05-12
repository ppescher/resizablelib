// ResizableSplitterWnd.h : header file
//
/////////////////////////////////////////////////////////////////////////////
//
// This file is part of ResizableLib
// https://github.com/ppescher/resizablelib
//
// Copyright (C) 2000-2024 by Paolo Messina
// mailto:ppescher@hotmail.com
//
// The contents of this file are subject to the Artistic License 2.0
// http://opensource.org/licenses/Artistic-2.0
//
// If you find this code useful, credits would be nice!
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZABLESPLITTERWND_H__INCLUDED_)
#define AFX_RESIZABLESPLITTERWND_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResizableMinMax.h"

/////////////////////////////////////////////////////////////////////////////
// CResizableSplitterWnd frame with splitter

class CResizableSplitterWnd : public CSplitterWnd, public CResizableMinMax
{
	DECLARE_DYNAMIC(CResizableSplitterWnd)

// Construction
public:
	CResizableSplitterWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableSplitterWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResizableSplitterWnd();

	// Generated message map functions
	//{{AFX_MSG(CResizableSplitterWnd)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZABLESPLITTERWND_H__INCLUDED_)
