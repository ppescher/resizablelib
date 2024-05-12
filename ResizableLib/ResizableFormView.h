#if !defined(AFX_RESIZABLEFORMVIEW_H__INCLUDED_)
#define AFX_RESIZABLEFORMVIEW_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ResizableFormView.h : header file
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

#include "ResizableLayout.h"
#include "ResizableGrip.h"
#include "ResizableMinMax.h"

/////////////////////////////////////////////////////////////////////////////
// CResizableFormView form view

#include <afxext.h>

class CResizableFormView : public CFormView, public CResizableLayout,
						   public CResizableGrip, public CResizableMinMax
{
	DECLARE_DYNAMIC(CResizableFormView)

// Construction
protected:      // must derive your own class
	explicit CResizableFormView(UINT nIDTemplate);
	explicit CResizableFormView(LPCTSTR lpszTemplateName);
	virtual ~CResizableFormView();

private:
	void PrivateConstruct();

	// support for temporarily hiding the grip
	DWORD m_dwGripTempState;
	enum GripHideReason		// bitmask
	{
		GHR_MAXIMIZED = 0x01,
		GHR_SCROLLBAR = 0x02,
		GHR_ALIGNMENT = 0x04,
	};

// called from base class
protected:

	virtual void GetTotalClientRect(LPRECT lpRect) const;

	virtual CWnd* GetResizableWnd() const
	{
		// make the layout know its parent window
		return CWnd::FromHandle(m_hWnd);
	};


// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnInitDialog();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableFormView)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CResizableFormView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDestroy();
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZABLEFORMVIEW_H__INCLUDED_)
