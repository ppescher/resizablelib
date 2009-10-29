// PropertyFormView.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "PropertyFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyFormView

IMPLEMENT_DYNCREATE(CPropertyFormView, CResizableFormView)

CPropertyFormView::CPropertyFormView()
	: CResizableFormView(CPropertyFormView::IDD)
{
	//{{AFX_DATA_INIT(CPropertyFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPropertyFormView::~CPropertyFormView()
{
}

void CPropertyFormView::DoDataExchange(CDataExchange* pDX)
{
	CResizableFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyFormView, CResizableFormView)
	//{{AFX_MSG_MAP(CPropertyFormView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyFormView diagnostics

#ifdef _DEBUG
void CPropertyFormView::AssertValid() const
{
	CResizableFormView::AssertValid();
}

void CPropertyFormView::Dump(CDumpContext& dc) const
{
	CResizableFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPropertyFormView message handlers

BOOL CPropertyFormView::OnInitDialog() 
{
	CResizableFormView::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_SHEETHOLDER);
	
	CRect rc;
	pWnd->GetWindowRect(&rc);
	pWnd->DestroyWindow();

	::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);

	//m_Sheet.SetWizardMode();
	//m_Sheet.m_psh.dwFlags |= PSH_HASHELP;

	m_Sheet.Create(this, WS_CHILD, 0);
	m_Sheet.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	m_Sheet.SetDlgCtrlID(IDC_SHEETHOLDER);
	
	AddAnchor(IDC_TEXT, TOP_LEFT, TOP_LEFT);
	AddAnchor(IDC_SHEETHOLDER, TOP_LEFT, BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
