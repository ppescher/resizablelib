#if !defined(AFX_RESIZABLECOMBOBOX_H__CAEDF06D_C4F5_49E4_A783_AD7D894CF9A0__INCLUDED_)
#define AFX_RESIZABLECOMBOBOX_H__CAEDF06D_C4F5_49E4_A783_AD7D894CF9A0__INCLUDED_

#include "ResizableComboLBox.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResizableComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResizableComboBox window

class CResizableComboBox : public CComboBox
{
// Construction
public:
	CResizableComboBox();

// Attributes
public:

protected:
	CResizableComboLBox m_ctrlListBox;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableComboBox)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CResizableComboBox();

// Generated message map functions
protected:
	//{{AFX_MSG(CResizableComboBox)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZABLECOMBOBOX_H__CAEDF06D_C4F5_49E4_A783_AD7D894CF9A0__INCLUDED_)
