#if !defined(AFX_RESIZABLEDIALOG_H__INCLUDED_)
#define AFX_RESIZABLEDIALOG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ResizableDialog.h : header file
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

#include "ResizableLayout.h"

/////////////////////////////////////////////////////////////////////////////
// CResizableDialog window

class CResizableDialog : public CDialog, public CResizableLayout
{

// Construction
public:
	CResizableDialog();
	CResizableDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	CResizableDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);

// Attributes
private:
	// flags
	BOOL m_bShowGrip;
	BOOL m_bUseMaxTrack;
	BOOL m_bUseMinTrack;
	BOOL m_bUseMaxRect;
	BOOL m_bEnableSaveRestore;

	// internal status
	CString m_sSection;			// section name and
	CString m_sEntry;			// entry for save/restore

	BOOL m_bInitDone;			// if all internal vars initialized

	SIZE m_szGripSize;			// set at construction time

	CRect m_rcGripRect;			// current pos of grip

	POINT m_ptMinTrackSize;		// min tracking size
	POINT m_ptMaxTrackSize;		// max tracking size
	POINT m_ptMaxPos;			// maximized position
	POINT m_ptMaxSize;			// maximized size

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableDialog)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResizableDialog();

// used internally
private:
	void Construct();
	void LoadWindowRect();
	void SaveWindowRect();
	void UpdateGripPos();

// callable from derived classes
protected:
	void ShowSizeGrip(BOOL bShow);				// show or hide the size grip
	void SetMaximizedRect(const CRect& rc);		// set window rect when maximized
	void ResetMaximizedRect();					// reset to default maximized rect
	void SetMinTrackSize(const CSize& size);	// set minimum tracking size
	void ResetMinTrackSize();					// reset to default minimum tracking size
	void SetMaxTrackSize(const CSize& size);	// set maximum tracking size
	void ResetMaxTrackSize();					// reset to default maximum tracking size
	void EnableSaveRestore(LPCTSTR pszSection, LPCTSTR pszEntry);	// section and entry in app's profile

	virtual CWnd* GetLayoutParent()
	{
		// make the layout know its parent window
		return this;
	};

// Generated message map functions
protected:
	//{{AFX_MSG(CResizableDialog)
	virtual BOOL OnInitDialog();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZABLEDIALOG_H__INCLUDED_)