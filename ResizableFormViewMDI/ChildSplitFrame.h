#if !defined(AFX_CHILDSPLITFRAME_H__81BE110F_51B4_4BA7_A681_A0683177BDA6__INCLUDED_)
#define AFX_CHILDSPLITFRAME_H__81BE110F_51B4_4BA7_A681_A0683177BDA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildSplitFrame.h : header file
//

#include "ResizableMDIChild.h"
#include "ResizableSplitterWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CChildSplitFrame frame

class CChildSplitFrame : public CResizableMDIChild
{
	DECLARE_DYNCREATE(CChildSplitFrame)
protected:
	CChildSplitFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildSplitFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CResizableSplitterWnd m_wndSplitter;
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual ~CChildSplitFrame();

	// Generated message map functions
	//{{AFX_MSG(CChildSplitFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDSPLITFRAME_H__81BE110F_51B4_4BA7_A681_A0683177BDA6__INCLUDED_)
