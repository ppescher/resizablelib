#if !defined(AFX_RESIZABLECOMBOLBOX_H__INCLUDED_)
#define AFX_RESIZABLECOMBOLBOX_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ResizableComboLBox.h : header file
//
/////////////////////////////////////////////////////////////////////////////
//
// This file is part of ResizableLib
// https://github.com/ppescher/resizablelib
//
// Copyright (C) 2000-2015 by Paolo Messina
// mailto:ppescher@hotmail.com
//
// The contents of this file are subject to the Artistic License 2.0
// http://opensource.org/licenses/Artistic-2.0
//
// If you find this code useful, credits would be nice!
//
/////////////////////////////////////////////////////////////////////////////

#include "ResizableGrip.h"

/////////////////////////////////////////////////////////////////////////////
// CResizableComboLBox window

class CResizableComboBox;

class CResizableComboLBox : public CWnd, public CResizableGrip
{
	friend class CResizableComboBox;

// Construction
public:
	CResizableComboLBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableComboLBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResizableComboLBox();

private:
	CPoint m_ptBeforeSizing;	// screen coords
	CRect m_rcBeforeSizing;		// screen coords
	CSize m_sizeAfterSizing;	// screen coords
	UINT m_nHitTest;			// current resize operation
	BOOL m_bSizing;

	void InitializeControl();

protected:
	DWORD m_dwAddToStyle;
	DWORD m_dwAddToStyleEx;
	CSize m_sizeMin;			// initial size (minimum)
	CResizableComboBox* m_pOwnerCombo;	// owner combobox

	void ApplyLimitsToPos(WINDOWPOS* lpwndpos);
	void EndSizing();

	BOOL IsRTL();

	virtual CWnd* GetResizableWnd() const
	{
		// make the layout know its parent window
		return CWnd::FromHandlePermanent(m_hWnd);
	};

	// Generated message map functions
protected:
	//{{AFX_MSG(CResizableComboLBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZABLECOMBOLBOX_H__INCLUDED_)
