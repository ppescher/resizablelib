// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__979888BD_FD55_4384_A5BE_44E267193FEE__INCLUDED_)
#define AFX_CHILDFRM_H__979888BD_FD55_4384_A5BE_44E267193FEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResizableMDIChild.h"

class CChildFrame : public CResizableMDIChild
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__979888BD_FD55_4384_A5BE_44E267193FEE__INCLUDED_)
