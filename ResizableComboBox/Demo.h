// Demo.h : main header file for the TESTCOMBOBOX application
//

#if !defined(AFX_TESTCOMBOBOX_H__AD8B91BB_D15A_4B88_B140_6D0F92474AED__INCLUDED_)
#define AFX_TESTCOMBOBOX_H__AD8B91BB_D15A_4B88_B140_6D0F92474AED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDemoApp:
// See Demo.cpp for the implementation of this class
//

class CDemoApp : public CWinApp
{
public:
	CDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDemoApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTCOMBOBOX_H__AD8B91BB_D15A_4B88_B140_6D0F92474AED__INCLUDED_)
