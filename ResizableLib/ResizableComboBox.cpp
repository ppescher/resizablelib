// ResizableComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "ResizableComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableComboBox

CResizableComboBox::CResizableComboBox()
{
}

CResizableComboBox::~CResizableComboBox()
{
	if (m_ctrlListBox.GetSafeHwnd() != NULL)
		m_ctrlListBox.UnsubclassWindow();
}


BEGIN_MESSAGE_MAP(CResizableComboBox, CComboBox)
	//{{AFX_MSG_MAP(CResizableComboBox)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableComboBox message handlers

HBRUSH CResizableComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		if (!(GetStyle() & CBS_SIMPLE)
			&& (m_ctrlListBox.GetSafeHwnd() == NULL))
		{
			TRACE("Combo: 0x%08X\n", pWnd->m_hWnd);

			// attach to the owned listbox
			m_ctrlListBox.m_pOwnerCombo = this;
			m_ctrlListBox.SubclassWindow(pWnd->m_hWnd);
		}
	}

	return hbr;
}

