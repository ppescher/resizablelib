// DemoView.h : interface of the CDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMOVIEW_H__20781705_5C0F_4E72_ACAE_50F448A9B9DC__INCLUDED_)
#define AFX_DEMOVIEW_H__20781705_5C0F_4E72_ACAE_50F448A9B9DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResizableFormView.h"

class CDemoView : public CResizableFormView
{
protected: // create from serialization only
	CDemoView();
	DECLARE_DYNCREATE(CDemoView)

public:
	//{{AFX_DATA(CDemoView)
	enum{ IDD = IDD_DEMO_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemoView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DemoView.cpp
inline CDemoDoc* CDemoView::GetDocument()
   { return (CDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOVIEW_H__20781705_5C0F_4E72_ACAE_50F448A9B9DC__INCLUDED_)
