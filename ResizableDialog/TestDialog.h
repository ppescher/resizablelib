#if !defined(AFX_TESTDIALOG_H__094DC281_1962_4794_B482_15DEAA29829D__INCLUDED_)
#define AFX_TESTDIALOG_H__094DC281_1962_4794_B482_15DEAA29829D__INCLUDED_

#include "ResizableDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestDialog dialog

class CTestDialog : public CResizableDialog
{
// Construction
public:
	CTestDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDialog)
	enum { IDD = IDD_TEST_DIALOG };
	CStatic	m_ctrlTest;
	int		m_nStaticType;
	BOOL	m_bCenterImage;
	BOOL	m_bEndEllipsis;
	BOOL	m_bPathEllipsis;
	BOOL	m_bRealSizeImage;
	BOOL	m_bRightJust;
	CString	m_sStaticText;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestDialog)
	afx_msg void OnButtonUpdate();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDIALOG_H__094DC281_1962_4794_B482_15DEAA29829D__INCLUDED_)
