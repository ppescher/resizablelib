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
			&& (m_ctrlListBox.m_hWnd == NULL))
		{
			TRACE("ComboLBox: 0x%08X\n", pWnd->m_hWnd);

			// attach to the owned listbox
			m_ctrlListBox.m_pOwnerCombo = this;
			m_ctrlListBox.SubclassWindow(pWnd->m_hWnd);
		}
	}

	return hbr;
}

LRESULT CResizableComboBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT lResult = CComboBox::WindowProc(message, wParam, lParam);

	// if listbox is attached, update horizontal extent

	switch (message)
	{
	case CB_INSERTSTRING:
	case CB_ADDSTRING:
		if (lResult != CB_ERR && lResult != CB_ERRSPACE)
			UpdateHorizontalExtent((LPCTSTR)lParam);
		break;

	case CB_DIR:
		if (lResult != CB_ERR && lResult != CB_ERRSPACE)
			InitHorizontalExtent();
		break;

	case CB_RESETCONTENT:
		InitHorizontalExtent();
		break;
	}

	return lResult;
}

void CResizableComboBox::InitHorizontalExtent()
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(GetFont());

	CString str;
	
	m_iExtent = 0;
	int n = GetCount();
	for (int i=0; i<n; i++)
	{
		GetLBText(i, str);
		int cx = dc.GetTextExtent(str).cx;
		if (cx > m_iExtent)
			m_iExtent = cx;
	}

	SetHorizontalExtent(m_iExtent
		+ LOWORD(GetDialogBaseUnits()));

	dc.SelectObject(pOldFont);
}

void CResizableComboBox::UpdateHorizontalExtent(LPCTSTR szText)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(GetFont());

	int cx = dc.GetTextExtent(szText, lstrlen(szText)).cx;
	if (cx > m_iExtent)
	{
		m_iExtent = cx;

		SetHorizontalExtent(m_iExtent
			+ LOWORD(GetDialogBaseUnits()));
	}

	dc.SelectObject(pOldFont);
}

void CResizableComboBox::PreSubclassWindow() 
{
	InitHorizontalExtent();
	
	CComboBox::PreSubclassWindow();
}
