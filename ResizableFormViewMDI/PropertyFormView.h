#if !defined(AFX_PROPERTYFORMVIEW_H__38E47C7A_8F05_4E08_8765_01C48BD9DEA9__INCLUDED_)
#define AFX_PROPERTYFORMVIEW_H__38E47C7A_8F05_4E08_8765_01C48BD9DEA9__INCLUDED_

#include "MyPropertySheet.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ResizableFormView.h"

class CPropertyFormView : public CResizableFormView
{
protected:
	CPropertyFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPropertyFormView)

// Form Data
public:
	//{{AFX_DATA(CPropertyFormView)
	enum { IDD = IDD_NEWFORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CMyPropertySheet m_Sheet;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyFormView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPropertyFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPropertyFormView)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYFORMVIEW_H__38E47C7A_8F05_4E08_8765_01C48BD9DEA9__INCLUDED_)
