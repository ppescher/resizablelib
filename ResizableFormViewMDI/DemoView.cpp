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
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
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
	DDX_Control(pDX, IDC_ICON1, m_icon1);
	//}}AFX_DATA_MAP
}

void CDemoView::OnInitialUpdate()
{
	// complete initialization
	CResizableFormView::OnInitialUpdate();

	// use template size as min track size
	CRect rect(CPoint(0, 0), GetTotalSize());
	CalcWindowRect(rect, CWnd::adjustOutside);
	SetMinTrackSize(rect.Size());

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);

	// change icon and disable buttons
	m_icon1.SetIcon((HICON)::GetClassLong(GetParentFrame()->GetSafeHwnd(), GCL_HICON));
	if (GetParentFrame() != GetParent())
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	}
	else // only if parent is a frame (not for splitters)!
	{
		// these two lines are optional
		GetParentFrame()->RecalcLayout();
		ResizeParentToFit();
	}
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

void CDemoView::OnButton2() 
{
	FormViewToDialog();
	UpdateSizeGrip();
}

void CDemoView::OnButton1() 
{
	DialogToFormView();
	UpdateSizeGrip();
}

void CDemoView::FormViewToDialog()
{
	CWnd* pParent = GetParent();
	// estimate new window's size
	CRect rect;
	GetTotalClientRect(&rect);	// uses GetTotalSize()
	DWORD style = pParent->GetStyle() & ~WS_THICKFRAME | WS_DLGFRAME;
	::AdjustWindowRect(&rect, style, ::IsMenu(::GetMenu(pParent->GetSafeHwnd())));
	// change style and size
	pParent->ModifyStyle(WS_THICKFRAME, WS_DLGFRAME);
	pParent->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
		SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED);
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
}

void CDemoView::DialogToFormView()
{
	CWnd* pParent = GetParent();
	// estimate new window's size
	CRect rect;
	GetTotalClientRect(&rect);	// uses GetTotalSize()
	DWORD style = pParent->GetStyle() & ~WS_DLGFRAME | WS_THICKFRAME|WS_CAPTION;
	::AdjustWindowRectEx(&rect, style, ::IsMenu(::GetMenu(pParent->GetSafeHwnd())),
		WS_EX_CLIENTEDGE);
	// change style and size
	pParent->ModifyStyle(WS_DLGFRAME, WS_THICKFRAME|WS_CAPTION);
	pParent->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(),
		SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_FRAMECHANGED);
	ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED);
}

void CDemoView::OnRadio3() 
{
	// scrollbars won't appear
	SetScaleToFitSize(GetTotalSize());
}

void CDemoView::OnRadio2() 
{
	// allow any size
	ResetMinTrackSize();
	// reactivate scrollbars
	SetScrollSizes(MM_TEXT, GetTotalSize());
}

void CDemoView::OnRadio1() 
{
	// use template size as min track size
	CRect rect(CPoint(0, 0), GetTotalSize());
	CalcWindowRect(rect, CWnd::adjustOutside);
	SetMinTrackSize(rect.Size());
	// reactivate scrollbars
	SetScrollSizes(MM_TEXT, GetTotalSize());
}

BOOL CDemoView::OnInitDialog() 
{
	CResizableFormView::OnInitDialog();
	
	// preset layout
	AddAnchor(IDC_EDIT1, TOP_LEFT, CSize(40,100));
	AddAnchor(IDC_BUTTON1, BOTTOM_LEFT, CSize(40,100));
	AddAnchor(IDC_BUTTON2, BOTTOM_LEFT, CSize(40,100));
	AddAnchor(IDC_GROUP1, CSize(40,0), BOTTOM_RIGHT);
	AddAnchor(IDC_COMBO1, CSize(40,0), TOP_RIGHT);
	AddAnchor(IDC_RADIO1, CSize(70,0), CSize(70,0));
	AddAnchor(IDC_RADIO2, CSize(70,0), CSize(70,0));
	AddAnchor(IDC_RADIO3, CSize(70,0), CSize(70,0));
	AddAnchor(IDC_STATIC1, CSize(40,0), BOTTOM_RIGHT);
	AddAnchor(IDC_ICON1, CSize(40,0), CSize(70,0));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
