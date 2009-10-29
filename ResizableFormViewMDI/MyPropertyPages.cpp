// MyPropertyPages.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MyPropertyPages.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage1, CResizablePageEx)
IMPLEMENT_DYNCREATE(CMyPropertyPage2, CResizablePageEx)
IMPLEMENT_DYNCREATE(CMyPropertyPage3, CResizablePageEx)


/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage1 property page

CMyPropertyPage1::CMyPropertyPage1() : CResizablePageEx(CMyPropertyPage1::IDD)
{
	//{{AFX_DATA_INIT(CMyPropertyPage1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyPropertyPage1::~CMyPropertyPage1()
{
}

void CMyPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CResizablePageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage1, CResizablePageEx)
	//{{AFX_MSG_MAP(CMyPropertyPage1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMyPropertyPage1::OnInitDialog() 
{
	CResizablePageEx::OnInitDialog();

	// preset layout
	AddAnchor(IDC_EDIT1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_BUTTON1, TOP_RIGHT, BOTTOM_RIGHT);

	SetDlgItemText(IDC_EDIT1, _T("Pages have been designed for a property")
		_T(" sheet dialog, so they have bad margins in wizard mode.\r\n\r\n")
		_T("The active page can be saved together with dialog's size and")
		_T(" position. Select another page and try to close and open again."));

	return TRUE;
}

BOOL CMyPropertyPage1::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_NEXT);
	
	return CResizablePageEx::OnSetActive();
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage2 property page

CMyPropertyPage2::CMyPropertyPage2() : CResizablePageEx(CMyPropertyPage2::IDD)
{
	//{{AFX_DATA_INIT(CMyPropertyPage2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyPropertyPage2::~CMyPropertyPage2()
{
}

void CMyPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CResizablePageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage2)
	DDX_Control(pDX, IDC_LIST1, m_ctlEdit1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage2, CResizablePageEx)
	//{{AFX_MSG_MAP(CMyPropertyPage2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMyPropertyPage2::OnInitDialog() 
{
	CResizablePageEx::OnInitDialog();
	
	// preset layout
	AddAnchor(IDC_LIST1, TOP_LEFT, ANCHOR(50,70));
	AddAnchor(IDC_PICTURE1, ANCHOR(50,0), ANCHOR(100,70));
	AddAnchor(IDC_GROUP1, ANCHOR(0,70), BOTTOM_RIGHT);
	AddAnchor(IDC_CHECK1, ANCHOR(0,85));
	AddAnchor(IDC_RADIO4, ANCHOR(100,85));
	AddAnchor(IDC_COMBO1, ANCHOR(100,70));
	AddAnchor(IDC_BUTTON1, BOTTOM_RIGHT);

	m_ctlEdit1.AddString(_T("Just a single item to test the ")
		_T("listbox behavior with very long lines..."));
	m_ctlEdit1.SetHorizontalExtent(300);

	return TRUE;
}

BOOL CMyPropertyPage2::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	
	return CResizablePageEx::OnSetActive();
}


/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage3 property page

CMyPropertyPage3::CMyPropertyPage3() : CResizablePageEx(CMyPropertyPage3::IDD)
{
	//{{AFX_DATA_INIT(CMyPropertyPage3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyPropertyPage3::~CMyPropertyPage3()
{
}

void CMyPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CResizablePageEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage3, CResizablePageEx)
	//{{AFX_MSG_MAP(CMyPropertyPage3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMyPropertyPage3::OnInitDialog() 
{
	CResizablePageEx::OnInitDialog();
	
	// preset layout
	AddAnchor(IDC_LABEL1, TOP_LEFT);
	AddAnchor(IDC_EDIT1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_SLIDER1, BOTTOM_LEFT, BOTTOM_RIGHT);
	
	return TRUE;
}

BOOL CMyPropertyPage3::OnSetActive() 
{
	((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_FINISH);
	
	return CResizablePageEx::OnSetActive();
}

