// ResizableLayout.h: interface for the CResizableLayout class.
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

#if !defined(AFX_RESIZABLELAYOUT_H__INCLUDED_)
#define AFX_RESIZABLELAYOUT_H__INCLUDED_

#include <afxtempl.h>
#include "ResizableMsgSupport.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// special type for layout anchors
struct ANCHOR : public SIZE
{
	ANCHOR() { }

	ANCHOR(int x, int y)
	{
		cx = x;
		cy = y;
	}
};

const ANCHOR
	TOP_LEFT(0,0), TOP_CENTER(50,0), TOP_RIGHT(100,0),
	MIDDLE_LEFT(0,50), MIDDLE_CENTER(50,50), MIDDLE_RIGHT(100,50),
	BOTTOM_LEFT(0,100), BOTTOM_CENTER(50,100), BOTTOM_RIGHT(100,100);


class CResizableLayout
{
protected:
	class LayoutInfo
	{
	public:
		HWND hWnd;
		UINT nCallbackID;

		TCHAR sWndClass[MAX_PATH];

		// upper-left corner
		ANCHOR sizeTypeTL;
		SIZE sizeMarginTL;
		
		// bottom-right corner
		ANCHOR sizeTypeBR;
		SIZE sizeMarginBR;

		// custom window support
		BOOL bMsgSupport;
		RESIZEPROPERTIES properties;
	
	public:
		LayoutInfo() : hWnd(NULL), nCallbackID(0), bMsgSupport(FALSE)
		{ }

		LayoutInfo(HWND hwnd, ANCHOR tl_t, SIZE tl_m, 
			ANCHOR br_t, SIZE br_m)
			: hWnd(hwnd), nCallbackID(0), bMsgSupport(FALSE),
			sizeTypeTL(tl_t), sizeMarginTL(tl_m),
			sizeTypeBR(br_t), sizeMarginBR(br_m)
		{
			sWndClass[0] = 0;
		}
	};

private:
	// list of repositionable controls
	CMap<HWND, HWND, POSITION, POSITION> m_mapLayout;
	CList<LayoutInfo, LayoutInfo&> m_listLayout;
	CList<LayoutInfo, LayoutInfo&> m_listLayoutCB;

	// used for clipping
	HRGN m_hOldClipRgn;
	int m_nOldClipRgn;

	void ClipChildWindow(const CResizableLayout::LayoutInfo &layout, CRgn* pRegion) const;

	void CalcNewChildPosition(const CResizableLayout::LayoutInfo &layout,
		const CRect &rectParent, CRect &rectChild, UINT& uFlags) const;

protected:
	// override to initialize resize properties (clipping, refresh)
	virtual void InitResizeProperties(CResizableLayout::LayoutInfo& layout) const;

	// override to specify clipping for unsupported windows
	virtual BOOL LikesClipping(const CResizableLayout::LayoutInfo &layout) const;

	// override to specify refresh for unsupported windows
	virtual BOOL NeedsRefresh(const CResizableLayout::LayoutInfo &layout,
		const CRect &rectOld, const CRect &rectNew) const;

	// paint the background on the given DC (for XP theme's compatibility)

	// clip out child windows from the given DC
	void ClipChildren(CDC* pDC, BOOL bUndo);

	// get the clipping region (without clipped child windows)
	void GetClippingRegion(CRgn* pRegion) const;
	
	// override for scrollable or expanding parent windows
	virtual void GetTotalClientRect(LPRECT lpRect) const;

	// add anchors to a control, given its HWND
	void AddAnchor(HWND hWnd, ANCHOR sizeTypeTL, ANCHOR sizeTypeBR);

	// add anchors to a control, given its HWND
	void AddAnchor(HWND hWnd, ANCHOR sizeTypeTL)
	{
		AddAnchor(hWnd, sizeTypeTL, sizeTypeTL);
	}

	// add anchors to a control, given its ID
	void AddAnchor(UINT nID, ANCHOR sizeTypeTL, ANCHOR sizeTypeBR)
	{
		AddAnchor(::GetDlgItem(GetResizableWnd()->GetSafeHwnd(), nID),
			sizeTypeTL, sizeTypeBR);
	}

	// add anchors to a control, given its ID
	void AddAnchor(UINT nID, ANCHOR sizeTypeTL)
	{
		AddAnchor(::GetDlgItem(GetResizableWnd()->GetSafeHwnd(), nID),
			sizeTypeTL, sizeTypeTL);
	}

	// add a callback (control ID or HWND is unknown or may change)
	void AddAnchorCallback(UINT nCallbackID);

	// get rect of an anchored window, given the parent's client area
	BOOL GetAnchorPosition(HWND hWnd, const CRect &rectParent,
		CRect &rectChild, UINT* lpFlags = NULL) const
	{
		POSITION pos;
		if (!m_mapLayout.Lookup(hWnd, pos))
			return FALSE;

		UINT uTmpFlags;
		CalcNewChildPosition(m_listLayout.GetAt(pos), rectParent, rectChild,
			(lpFlags != NULL) ? (*lpFlags) : uTmpFlags);
		return TRUE;
	}

	// get rect of an anchored window, given the parent's client area
	BOOL GetAnchorPosition(UINT nID, const CRect &rectParent,
		CRect &rectChild, UINT* lpFlags = NULL) const
	{
		return GetAnchorPosition(::GetDlgItem(GetResizableWnd()->GetSafeHwnd(), nID),
			rectParent, rectChild, lpFlags);
	}

	// get margins surrounding a child window at the given size
	BOOL GetAnchorMargins(HWND hWnd, const CSize &sizeChild, CRect &rectMargins) const;

	// get margins surrounding a child window at the given size
	BOOL GetAnchorMargins(UINT nID, const CSize &sizeChild, CRect &rectMargins) const
	{
		return GetAnchorMargins(::GetDlgItem(GetResizableWnd()->GetSafeHwnd(), nID),
			sizeChild, rectMargins);
	}

	// remove an anchored control from the layout, given its HWND
	BOOL RemoveAnchor(HWND hWnd)
	{
		POSITION pos;
		if (!m_mapLayout.Lookup(hWnd, pos))
			return FALSE;

		m_listLayout.RemoveAt(pos);
		return m_mapLayout.RemoveKey(hWnd);
	}

	// remove an anchored control from the layout, given its HWND
	BOOL RemoveAnchor(UINT nID)
	{
		return RemoveAnchor(::GetDlgItem(GetResizableWnd()->GetSafeHwnd(), nID));
	}

	// reset layout content
	void RemoveAllAnchors()
	{
		m_mapLayout.RemoveAll();
		m_listLayout.RemoveAll();
		m_listLayoutCB.RemoveAll();
	}

	// adjust children's layout, when parent's size changes
	void ArrangeLayout() const;

	// override to provide dynamic control's layout info
	virtual BOOL ArrangeLayoutCallback(CResizableLayout::LayoutInfo& layout) const;

	// override to provide the parent window
	virtual CWnd* GetResizableWnd() const = 0;

public:
	CResizableLayout()
	{
		m_hOldClipRgn = ::CreateRectRgn(0,0,0,0);
	}

	virtual ~CResizableLayout()
	{
		// just for safety
		RemoveAllAnchors();
	}
};

#endif // !defined(AFX_RESIZABLELAYOUT_H__INCLUDED_)
