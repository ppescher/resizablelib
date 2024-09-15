
// DemoDlg.h : header file
//

#pragma once

#include "ResizableDialogEx.h"

// CDemoDlg dialog
class CDemoDlg : public CResizableDialogEx
{
// Construction
public:
	CDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// support for temporarily hiding the grip
	DWORD m_dwGripTempState;

	static BOOL CALLBACK SendThemeChangedProc(HWND hwnd, LPARAM lParam);
	static DWORD GetThemeProperties();
	void SetThemeProperties(DWORD dwFlags);
	void UpdateMaxSize();

	static LRESULT WINAPI EditSubclassProc(
		HWND hWnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		UINT_PTR uIdSubclass,
		DWORD_PTR dwRefData
	);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
