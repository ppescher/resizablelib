// ResizableSplitterWnd.cpp : implementation file
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
#include "ResizableSplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableSplitterWnd

IMPLEMENT_DYNAMIC(CResizableSplitterWnd, CSplitterWnd)

CResizableSplitterWnd::CResizableSplitterWnd()
{
}

CResizableSplitterWnd::~CResizableSplitterWnd()
{
}

BEGIN_MESSAGE_MAP(CResizableSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CResizableSplitterWnd)
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableSplitterWnd message handlers

void CResizableSplitterWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CWnd* pPane = GetActivePane();
	if (pPane != NULL)
	{
		// get the extra size from child to parent
		CRect rectClient, rectWnd;
		GetWindowRect(rectWnd);
		pPane->GetWindowRect(rectClient);
		CSize sizeExtra = rectWnd.Size() - rectClient.Size();

		// ask the view for track size
		MINMAXINFO mmiChild = *lpMMI;
		pPane->SendMessage(WM_GETMINMAXINFO, 0, (LPARAM)&mmiChild);
		mmiChild.ptMaxTrackSize = sizeExtra + mmiChild.ptMaxTrackSize;
		mmiChild.ptMinTrackSize = sizeExtra + mmiChild.ptMinTrackSize;

		// min size is the largest
		lpMMI->ptMinTrackSize.x = __max(lpMMI->ptMinTrackSize.x,
			mmiChild.ptMinTrackSize.x);
		lpMMI->ptMinTrackSize.y = __max(lpMMI->ptMinTrackSize.y,
			mmiChild.ptMinTrackSize.y);

		// max size is the shortest
		lpMMI->ptMaxTrackSize.x = __min(lpMMI->ptMaxTrackSize.x,
			mmiChild.ptMaxTrackSize.x);
		lpMMI->ptMaxTrackSize.y = __min(lpMMI->ptMaxTrackSize.y,
			mmiChild.ptMaxTrackSize.y);
	}

	CSplitterWnd::OnGetMinMaxInfo(lpMMI);
}
