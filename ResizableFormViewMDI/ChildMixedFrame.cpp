// ChildMixedFrame.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "ChildMixedFrame.h"
#include "DemoDoc.h"
#include "DemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildMixedFrame

IMPLEMENT_DYNCREATE(CChildMixedFrame, CResizableMDIChild)

CChildMixedFrame::CChildMixedFrame()
{
}

CChildMixedFrame::~CChildMixedFrame()
{
}


BEGIN_MESSAGE_MAP(CChildMixedFrame, CResizableMDIChild)
	//{{AFX_MSG_MAP(CChildMixedFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildMixedFrame message handlers

BOOL CChildMixedFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	// CG: The following block was added by the Splitter Bar component.
	{
		if (!m_wndSplitter.CreateStatic(this, 2, 1))
		{
			TRACE0("Failed to create splitter bar ");
			return FALSE;    // failed to create
		}
		if (!m_wndSplitter.CreateView(0, 0,
			RUNTIME_CLASS(CEditView), CSize(0,100), pContext))
		{
			TRACE0("Failed to create splitter's first pane ");
			return FALSE;    // failed to create
		}
		if (!m_wndSplitter.CreateView(1, 0,
			RUNTIME_CLASS(CDemoView), CSize(0,0), pContext))
		{
			TRACE0("Failed to create splitter's second pane ");
			return FALSE;    // failed to create
		}

		m_wndSplitter.SetRowInfo(0, 100, 30);
		m_wndSplitter.SetRowInfo(1, 100, 30);

		return TRUE;
	}
}

void CChildMixedFrame::ActivateFrame(int nCmdShow) 
{
	if ( EnableSaveRestore(_T("Demo MDI MixedFrame")) )
	{
		// if correctly restored, activate window and return
		MDIActivate();
		return;
	}

	CResizableMDIChild::ActivateFrame(nCmdShow);
}
