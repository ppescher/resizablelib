#if !defined(AFX_RESIZABLELISTBOX_H__06867B74_5C72_483A_8FC5_C4846EAE83CB__INCLUDED_)
#define AFX_RESIZABLELISTBOX_H__06867B74_5C72_483A_8FC5_C4846EAE83CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResizableComboLBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResizableComboLBox window

class CResizableComboLBox : public CWnd
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
	int m_iExtent;

	void InitializeControl();

protected:
	DWORD m_dwAddToStyle;
	DWORD m_dwAddToStyleEx;
	BOOL m_bClipMaxHeight;
	CSize m_sizeMin;			// initial size (minimum)
	CComboBox* m_pOwnerCombo;	// owner combobox

	int MakeIntegralHeight(const int height);
	void ApplyLimitsToPos(WINDOWPOS* lpwndpos);
	void EndSizing();
	void InitHorizontalExtent();

	BOOL IsRTL() { return (GetExStyle() & WS_EX_LAYOUTRTL); }

	// Generated message map functions
protected:
	void UpdateHorizontalExtent(LPCTSTR szText);
	//{{AFX_MSG(CResizableComboLBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESIZABLELISTBOX_H__06867B74_5C72_483A_8FC5_C4846EAE83CB__INCLUDED_)
