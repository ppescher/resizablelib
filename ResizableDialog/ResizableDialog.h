#if !defined(AFX_RESIZABLEDIALOG_H__21FE3CD1_979B_11D3_A80F_525400E146A5__INCLUDED_)
#define AFX_RESIZABLEDIALOG_H__21FE3CD1_979B_11D3_A80F_525400E146A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ResizableDialog.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2000 by Paolo Messina
// (ppescher@yahoo.com)
// 
// This file may be redistributed unmodified by any means PROVIDING it
// is not sold for profit without the author written consent, and 
// providing that this notice and the author name is included.
// If the code in this file is used in any commercial application 
// a simple credit would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
/////////////////////////////////////////////////////////////////////////////
// CResizableDialog window

class CResizableDialog : public CDialog
{

// Construction
public:
	CResizableDialog();
	CResizableDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

// Attributes
public:
	enum AnchorType
	{
		NOANCHOR,
		TOP_LEFT,		TOP_CENTER,		TOP_RIGHT,
		MIDDLE_LEFT,	MIDDLE_CENTER,	MIDDLE_RIGHT,
		BOTTOM_LEFT,	BOTTOM_CENTER,	BOTTOM_RIGHT
	};

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

	CSize m_szGripSize;			// set at construction time

	CRect m_rcGripRect;			// current pos of grip

	POINT m_ptMinTrackSize;		// min tracking size
	POINT m_ptMaxTrackSize;		// max tracking size
	POINT m_ptMaxPos;			// maximized position
	POINT m_ptMaxSize;			// maximized size

	CPtrList m_plLayoutList;	// list of repositionable controls

	class Layout
	{
	public:
		HWND hwnd;

		BOOL adj_hscroll;

		// upper-left corner
		AnchorType ul_type;
		SIZE ul_margin;
		
		// bottom-right corner
		AnchorType br_type;
		SIZE br_margin;
	
	public:
		Layout(HWND hw, AnchorType ul_t, SIZE ul_m, 
			AnchorType br_t, SIZE br_m, BOOL hscroll)
		{
			hwnd = hw;

			adj_hscroll = hscroll;

			ul_type = ul_t;
			ul_margin = ul_m;
			
			br_type = br_t;
			br_margin = br_m;
		};
	};

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
	void LoadWindowRect();
	void SaveWindowRect();
	void ArrangeLayout();
	void UpdateGripPos();

// callable from derived classes
protected:
	BOOL AddAnchor(CWnd* pWnd, AnchorType ul_type,
		AnchorType br_type = NOANCHOR);	// add anchors to a control
	BOOL AddAnchor(UINT ctrl_ID, AnchorType ul_type,
		AnchorType br_type = NOANCHOR)	// add anchors to a control
	{
		return AddAnchor(GetDlgItem(ctrl_ID), ul_type, br_type);
	};
	void ShowSizeGrip(BOOL bShow);				// show or hide the size grip
	void SetMaximizedRect(const CRect& rc);		// set window rect when maximized
	void ResetMaximizedRect();					// reset to default maximized rect
	void SetMinTrackSize(const CSize& size);	// set minimum tracking size
	void ResetMinTrackSize();					// reset to default minimum tracking size
	void SetMaxTrackSize(const CSize& size);	// set maximum tracking size
	void ResetMaxTrackSize();					// reset to default maximum tracking size
	void EnableSaveRestore(LPCTSTR pszSection, LPCTSTR pszEntry);	// section and entry in app's profile

// Generated message map functions
protected:
	//{{AFX_MSG(CResizableDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZABLEDIALOG_H__21FE3CD1_979B_11D3_A80F_525400E146A5__INCLUDED_)