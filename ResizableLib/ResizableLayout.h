// ResizableLayout.h: interface for the CResizableLayout class.
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2000 by Paolo Messina
// (ppescher@yahoo.com)
//
// Free for non-commercial and commercial use,
// provided that the original author's name
// and copyright is quoted somewhere in the
// final executable and in the program's help
// or documentation.
// You may change the code to your needs,
// provided that credits to the original
// author are given in the modified files.
// Also a copy of your enhancements would
// be nice, but it's not required. Please,
// consider to share your work on CodeProject.
//  
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZABLELAYOUT_H__A7664CF3_1F96_4904_830F_8FB70A99F129__INCLUDED_)
#define AFX_RESIZABLELAYOUT_H__A7664CF3_1F96_4904_830F_8FB70A99F129__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// useful compatibility constants (the only one required is NOANCHOR)

const CSize NOANCHOR(-1,-1),
	TOP_LEFT(0,0), TOP_CENTER(50,0), TOP_RIGHT(100,0),
	MIDDLE_LEFT(0,50), MIDDLE_CENTER(50,50), MIDDLE_RIGHT(100,50),
	BOTTOM_LEFT(0,100), BOTTOM_CENTER(50,100), BOTTOM_RIGHT(100,100);

#define HWND_CALLBACK	HWND_BROADCAST


class CResizableLayout
{
	class LayoutInfo
	{
	public:
		HWND hWnd;
		UINT nCallbackID;

		BOOL adj_hscroll;
		BOOL need_refresh;

		// upper-left corner
		CSize tl_type;
		CSize tl_margin;
		
		// bottom-right corner
		CSize br_type;
		CSize br_margin;
	
	public:
		LayoutInfo() : hWnd(NULL), nCallbackID(0),
			adj_hscroll(FALSE), need_refresh(FALSE),
			tl_type(0,0), tl_margin(0,0),
			br_type(0,0), br_margin(0,0)
		{ }

		LayoutInfo(HWND hw, SIZE tl_t, SIZE tl_m, 
			SIZE br_t, SIZE br_m, BOOL hscroll, BOOL refresh)
		{
			hWnd = hw;
			nCallbackID = 0;

			adj_hscroll = hscroll;
			need_refresh = refresh;

			tl_type = tl_t;
			tl_margin = tl_m;
			
			br_type = br_t;
			br_margin = br_m;
		}
	};

	CArray<LayoutInfo, LayoutInfo&> m_arrLayout;	// list of repositionable controls

	static BOOL CALLBACK EnumAndClipChildWindow(HWND hWnd, LPARAM lParam);

protected:
	// exclude child windows from the clipping region
	void ClipChildren(CDC *pDC, BOOL bOnlyAnchored = TRUE);
	
	// override for scrollable or expanding parent windows
	virtual void GetTotalClientRect(LPRECT lpRect);

	// add anchors to a control, given its HWND
	void AddAnchor(HWND hWnd, CSize tl_type, CSize br_type = NOANCHOR);

	// add anchors to a control, given its ID
	void AddAnchor(UINT ctrl_ID, CSize tl_type, CSize br_type = NOANCHOR)
	{
		AddAnchor(::GetDlgItem(GetResizableWnd()->GetSafeHwnd(), ctrl_ID),
			tl_type, br_type);
	};

	// add a callback (control ID or HWND is unknown or can change)
	void AddAnchorCallback(UINT nCallbackID);

	// adjust children's layout, when parent's size changes
	void ArrangeLayout();

	// override to provide dynamic control's layout info
	virtual BOOL ArrangeLayoutCallback(LayoutInfo& layout);

	// reset layout content
	void RemoveAllAnchors()
	{
		m_arrLayout.RemoveAll();
	};

	virtual CWnd* GetResizableWnd() = 0;

public:
	CResizableLayout() {};

	virtual ~CResizableLayout()
	{
		// just for safety
		RemoveAllAnchors();
	};
};

#endif // !defined(AFX_RESIZABLELAYOUT_H__A7664CF3_1F96_4904_830F_8FB70A99F129__INCLUDED_)
