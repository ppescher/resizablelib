// DemoView.cpp : implementation of the CDemoView class
//

#include "stdafx.h"
#include "Demo.h"

#include "DemoDoc.h"
#include "DemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoView

IMPLEMENT_DYNCREATE(CDemoView, CResizableFormView)

BEGIN_MESSAGE_MAP(CDemoView, CResizableFormView)
	//{{AFX_MSG_MAP(CDemoView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoView construction/destruction

CDemoView::CDemoView()
	: CResizableFormView(CDemoView::IDD)
{
	//{{AFX_DATA_INIT(CDemoView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDemoView::~CDemoView()
{
}

void CDemoView::DoDataExchange(CDataExchange* pDX)
{
	CResizableFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CResizableFormView::PreCreateWindow(cs);
}

void CDemoView::OnInitialUpdate()
{
	// must add controls to the layout
	// before OnInitialUpdate's base implementation
	AddAnchor(IDC_LIST1, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_GROUP1, TOP_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_RADIO1, TOP_LEFT);
	AddAnchor(IDC_RADIO2, MIDDLE_LEFT);
	AddAnchor(IDC_RADIO3, BOTTOM_LEFT);

	// use template size as min track size
	CRect rect(CPoint(0, 0), GetTotalSize());
	CalcWindowRect(rect, CWnd::adjustOutside);
	SetMinTrackSize(rect.Size());

	// complete initialization
	CResizableFormView::OnInitialUpdate();

	// these two lines are optional
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

//	SetScaleToFitSize(GetTotalSize()); // scrollbars won't appear
}

/////////////////////////////////////////////////////////////////////////////
// CDemoView diagnostics

#ifdef _DEBUG
void CDemoView::AssertValid() const
{
	CResizableFormView::AssertValid();
}

void CDemoView::Dump(CDumpContext& dc) const
{
	CResizableFormView::Dump(dc);
}

CDemoDoc* CDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemoDoc)));
	return (CDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemoView message handlers
