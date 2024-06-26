// ResizableMDIFrame.h : header file
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

#if !defined(AFX_RESIZABLEMDIFRAME_H__INCLUDED_)
#define AFX_RESIZABLEMDIFRAME_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResizableMinMax.h"
#include "ResizableWndState.h"
#include "ResizableLayout.h"

/////////////////////////////////////////////////////////////////////////////
// CResizableMDIFrame frame

class CResizableMDIFrame : public CMDIFrameWnd, public CResizableMinMax,
						public CResizableWndState, public CResizableLayout
{
	DECLARE_DYNCREATE(CResizableMDIFrame)
protected:
	CResizableMDIFrame();		// protected constructor used by dynamic creation

// Attributes
protected:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableMDIFrame)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CResizableMDIFrame();

	BOOL EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly = FALSE);

	virtual CWnd* GetResizableWnd() const
	{
		// make the layout know its parent window
		return CWnd::FromHandle(m_hWnd);
	};

private:
	// flags
	BOOL m_bEnableSaveRestore;
	BOOL m_bRectOnly;

	// internal status
	CString m_sSection;			// section name (identifies a parent window)

protected:
	// Generated message map functions
	//{{AFX_MSG(CResizableMDIFrame)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDestroy();
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZABLEMDIFRAME_H__INCLUDED_)
