// DemoDlg.h : header file
//

#if !defined(AFX_TESTCOMBOBOXDLG_H__CA544F9E_0749_4281_8E00_EED3872CDCAC__INCLUDED_)
#define AFX_TESTCOMBOBOXDLG_H__CA544F9E_0749_4281_8E00_EED3872CDCAC__INCLUDED_

#include "ResizableComboBox.h"
#include "ResizableDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog

class CDemoDlg : public CResizableDialog
{
// Construction
public:
	CDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDemoDlg)
	enum { IDD = IDD_TESTCOMBOBOX_DIALOG };
	CResizableComboBox	m_ctrlCombo1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCOMBOBOXDLG_H__CA544F9E_0749_4281_8E00_EED3872CDCAC__INCLUDED_)
