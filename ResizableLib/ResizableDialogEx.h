#if !defined(AFX_RESIZABLEDIALOGEX_H__INCLUDED_)
#define AFX_RESIZABLEDIALOGEX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ResizableDialogEx.h : header file
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

#include "afxdialogex.h"

#include "ResizableLayout.h"
#include "ResizableGrip.h"
#include "ResizableMinMax.h"
#include "ResizableWndState.h"

/////////////////////////////////////////////////////////////////////////////
// CResizableDialogEx window

class CResizableDialogEx : public CDialogEx, public CResizableLayout,
						 public CResizableGrip, public CResizableMinMax,
						 public CResizableWndState
{

// Construction
public:
	CResizableDialogEx();
	explicit CResizableDialogEx(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	explicit CResizableDialogEx(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);

// Attributes
private:
	// support for temporarily hiding the grip
	DWORD m_dwGripTempState;

	// flags
	BOOL m_bEnableSaveRestore;
	BOOL m_bRectOnly;
	BOOL m_bRestoreAfterVisible;

	// internal status
	CString m_sSection;			// section name (identifies a parent window)

	CBrush m_brushBack;

// Operations
public:
	void SetBackgroundImage(HBITMAP hBitmap, BackgroundLocation location = BACKGR_TILE, BOOL bAutoDestroy = TRUE, BOOL bRepaint = TRUE);
	BOOL SetBackgroundImage(UINT uiBmpResId, BackgroundLocation location = BACKGR_TILE, BOOL bRepaint = TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableDialogEx)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	virtual BOOL LikesClipping(const LAYOUTINFO &layout) const;
	virtual BOOL NeedsRefresh(const LAYOUTINFO& layout,
		const CRect& rectOld, const CRect& rectNew) const;

// Implementation
public:
	virtual ~CResizableDialogEx();

// used internally
private:
	void PrivateConstruct();

// callable from derived classes
protected:
	// section to use in app's profile
	void EnableSaveRestore(LPCTSTR pszSection, BOOL bRectOnly = FALSE);

	virtual CWnd* GetResizableWnd() const
	{
		// make the layout know its parent window
		return CWnd::FromHandle(m_hWnd);
	};

// Generated message map functions
protected:
	//{{AFX_MSG(CResizableDialogEx)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZABLEDIALOGEX_H__INCLUDED_)
