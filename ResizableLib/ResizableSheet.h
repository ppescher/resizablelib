#if !defined(AFX_RESIZABLESHEET_H__INCLUDED_)
#define AFX_RESIZABLESHEET_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#include "ResizableLayout.h"
#include "ResizableGrip.h"
#include "ResizableMinMax.h"
#include "ResizableState.h"

/////////////////////////////////////////////////////////////////////////////
// ResizableSheet.h : header file
//

class CResizableSheet : public CPropertySheet, public CResizableLayout,
						public CResizableGrip, public CResizableMinMax,
						public CResizableState
{
	DECLARE_DYNAMIC(CResizableSheet)

// Construction
public:
	CResizableSheet();
	CResizableSheet(UINT nIDCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);
	CResizableSheet(LPCTSTR pszCaption, CWnd *pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
private:
	// flags
	BOOL m_bEnableSaveRestore;
	BOOL m_bRectOnly;
	BOOL m_bSavePage;

	// layout vars
	CSize m_sizePageTL, m_sizePageBR;

	// internal status
	CString m_sSection;			// section name (identifies a parent window)
	BOOL m_bInitDone;			// if all internal vars initialized

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResizableSheet();

// used internally
private:
	void PresetLayout();
	void Construct();
	void SavePage();
	void LoadPage();

	BOOL IsWizard() { return (m_psh.dwFlags & PSH_WIZARD); }

// callable from derived classes
protected:
	// section to use in app's profile
	void EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly = FALSE,
		BOOL bWithPage = FALSE);
	int GetMinWidth();	// minimum width to display all buttons


	virtual CWnd* GetResizableWnd()
	{
		// make the layout know its parent window
		return this;
	};

// Generated message map functions
protected:
	virtual BOOL ArrangeLayoutCallback(LayoutInfo& layout);
	//{{AFX_MSG(CResizableSheet)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnPageChanged();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// AFX_RESIZABLESHEET_H__INCLUDED_
