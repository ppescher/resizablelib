// ResizableGrip.h: interface for the CResizableGrip class.
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

#if !defined(AFX_RESIZABLEGRIP_H__INCLUDED_)
#define AFX_RESIZABLEGRIP_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResizableGrip  
{
private:
	class CSizeGrip : public CScrollBar
	{
	public:
		CSizeGrip()
		{
			m_bTransparent = FALSE;
			m_size.cx = GetSystemMetrics(SM_CXVSCROLL);
			m_size.cy = GetSystemMetrics(SM_CYHSCROLL);
		}

		void SetTransparency(BOOL bActivate);

		BOOL IsRTL();			// right-to-left layout support

		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

		SIZE m_size;			// holds grip size

	protected:
		BOOL m_bTransparent;	// transparency active

		// memory DCs and bitmaps for transparent grip
		CDC m_dcGrip, m_dcMask;
		CBitmap m_bmGrip, m_bmMask;
	};

	CSizeGrip m_wndGrip;		// grip control
	int m_nShowCount;			// support for hiding the grip

protected:
	BOOL CreateSizeGrip();		// create a size grip, initially visible
	BOOL IsSizeGripVisible();	// TRUE if grip is set to be visible
	void SetSizeGripVisibility(BOOL bVisible);	// set default visibility
	void UpdateSizeGrip();		// update the grip's visibility and position
	void ShowSizeGrip(DWORD* pStatus, DWORD dwMask = 1);	// temp show the size grip
	void HideSizeGrip(DWORD* pStatus, DWORD dwMask = 1);	// temp hide the size grip
	BOOL SetSizeGripBkMode(int nBkMode);		// like CDC::SetBkMode

	virtual CWnd* GetResizableWnd() = 0;

public:
	CResizableGrip();
	virtual ~CResizableGrip();
};

#endif // !defined(AFX_RESIZABLEGRIP_H__INCLUDED_)
