#if !defined(AFX_CHILDMIXEDFRAME_H__05BBB9E7_433A_412B_80FC_FF81188E5D8D__INCLUDED_)
#define AFX_CHILDMIXEDFRAME_H__05BBB9E7_433A_412B_80FC_FF81188E5D8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildMixedFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildMixedFrame frame

class CChildMixedFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildMixedFrame)
protected:
	CChildMixedFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildMixedFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSplitterWnd m_wndSplitter;
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual ~CChildMixedFrame();

	// Generated message map functions
	//{{AFX_MSG(CChildMixedFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDMIXEDFRAME_H__05BBB9E7_433A_412B_80FC_FF81188E5D8D__INCLUDED_)
