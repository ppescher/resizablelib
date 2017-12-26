// DemoDlg.h : header file
//

#if !defined(AFX_DEMODLG_H__21FE3CC9_979B_11D3_A80F_525400E146A5__INCLUDED_)
#define AFX_DEMODLG_H__21FE3CC9_979B_11D3_A80F_525400E146A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog
#include "ResizableDialog.h"

class CDemoDlg : public CResizableDialog
{
// Construction
public:
	CDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDemoDlg)
	enum { IDD = IDD_DEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// support for temporarily hiding the grip
	DWORD m_dwGripTempState;

	HICON m_hIcon;

	static BOOL CALLBACK SendThemeChangedProc(HWND hwnd, LPARAM lParam);
	void SetThemeProperties(DWORD dwFlags);

	// Generated message map functions
	//{{AFX_MSG(CDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMODLG_H__21FE3CC9_979B_11D3_A80F_525400E146A5__INCLUDED_)
