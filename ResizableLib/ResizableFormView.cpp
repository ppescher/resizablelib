// ResizableFormView.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000-2002 by Paolo Messina
// (http://www.geocities.com/ppescher - ppescher@yahoo.com)
//
// The contents of this file are subject to the Artistic License (the "License").
// You may not use this file except in compliance with the License. 
// You may obtain a copy of the License at:
// http://www.opensource.org/licenses/artistic-license.html
//
// If you find this code useful, credits would be nice!
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResizableFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableFormView

IMPLEMENT_DYNAMIC(CResizableFormView, CFormView)

inline void CResizableFormView::PrivateConstruct()
{
//	m_bInitDone = FALSE;
	m_dwGripTempState = GHR_SCROLLBAR | GHR_ALIGNMENT | GHR_MAXIMIZED;
}

CResizableFormView::CResizableFormView(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{
	PrivateConstruct();
}

CResizableFormView::CResizableFormView(LPCTSTR lpszTemplateName)
	: CFormView(lpszTemplateName)
{
	PrivateConstruct();
}

CResizableFormView::~CResizableFormView()
{
}


BEGIN_MESSAGE_MAP(CResizableFormView, CFormView)
	//{{AFX_MSG_MAP(CResizableFormView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableFormView diagnostics

#ifdef _DEBUG
void CResizableFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CResizableFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CResizableFormView message handlers

void CResizableFormView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);

	CWnd* pParent = GetParent();

	// hide zise grip when parent is maximized
	if (pParent->IsZoomed())
		HideSizeGrip(&m_dwGripTempState, GHR_MAXIMIZED);
	else
		ShowSizeGrip(&m_dwGripTempState, GHR_MAXIMIZED);

	// hide size grip when there are scrollbars
	CSize size = GetTotalSize();
	if ((cx < size.cx || cy < size.cy) && (m_nMapMode >= 0))
		HideSizeGrip(&m_dwGripTempState, GHR_SCROLLBAR);
	else
		ShowSizeGrip(&m_dwGripTempState, GHR_SCROLLBAR);

	// hide size grip when the parent window is not resizable
	// or the form is not bottom-right aligned (e.g. there's a statusbar)
	DWORD dwStyle = pParent->GetStyle();
	CRect rectParent, rectChild;
	GetWindowRect(rectChild);
	::MapWindowPoints(NULL, pParent->GetSafeHwnd(), (LPPOINT)&rectChild, 2);
	pParent->GetClientRect(rectParent);
	if (!(dwStyle & WS_THICKFRAME)
		|| (rectChild.BottomRight() != rectParent.BottomRight()))
		HideSizeGrip(&m_dwGripTempState, GHR_ALIGNMENT);
	else
		ShowSizeGrip(&m_dwGripTempState, GHR_ALIGNMENT);

	// update grip and layout
	UpdateSizeGrip();
	ArrangeLayout();
}

void CResizableFormView::GetTotalClientRect(LPRECT lpRect)
{
	GetClientRect(lpRect);

	// get dialog template's size
	// (this is set in CFormView::Create)
	CSize sizeTotal, sizePage, sizeLine;
	int nMapMode = 0;
	GetDeviceScrollSizes(nMapMode, sizeTotal, sizePage, sizeLine);

	// otherwise, give the correct size if scrollbars active

	if (nMapMode < 0)	// scrollbars disabled
		return;

	// enlarge reported client area when needed
	CRect rect(lpRect);
	if (rect.Width() < sizeTotal.cx)
		rect.right = rect.left + sizeTotal.cx;
	if (rect.Height() < sizeTotal.cy)
		rect.bottom = rect.top + sizeTotal.cy;

	rect.OffsetRect(-GetDeviceScrollPosition());
	*lpRect = rect;
}

BOOL CResizableFormView::OnEraseBkgnd(CDC* pDC) 
{
	ClipChildren(pDC, FALSE);

	BOOL bRet = CFormView::OnEraseBkgnd(pDC);

	ClipChildren(pDC, TRUE);

	return bRet;
}

int CResizableFormView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create and init the size-grip
	if (!CreateSizeGrip())
		return -1;

	return 0;
}

void CResizableFormView::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	MinMaxInfo(lpMMI);
}

void CResizableFormView::OnDestroy() 
{
	RemoveAllAnchors();

	CFormView::OnDestroy();
}

LRESULT CResizableFormView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_INITDIALOG)
		return (LRESULT)OnInitDialog();

	return CFormView::WindowProc(message, wParam, lParam);
}

BOOL CResizableFormView::OnInitDialog() 
{
	const MSG* pMsg = GetCurrentMessage();

	BOOL bRet = (BOOL)CFormView::WindowProc(pMsg->message, pMsg->wParam, pMsg->lParam);

	// we need to associate member variables with control IDs
	UpdateData(FALSE);
	
	// set default scroll size
	CRect rectTemplate;
	GetWindowRect(rectTemplate);
	SetScrollSizes(MM_TEXT, rectTemplate.Size());

	return bRet;
}
