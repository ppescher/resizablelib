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
		CSizeGrip() {}

		BOOL IsRTL();

		CWnd* m_pResizableWnd;

		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	};

	CSizeGrip m_wndGrip;	// grip control
	SIZE m_sizeGrip;		// holds grip size
	int m_nShowCount;		// support for hiding the grip

protected:
	BOOL CreateSizeGrip();	// create a size grip, initially visible
	void UpdateSizeGrip();	// update the grip's visibility and position
	void ShowSizeGrip(BOOL* pbStatus = NULL);	// show the size grip
	void HideSizeGrip(BOOL* pbStatus = NULL);	// hide the size grip
	BOOL IsSizeGripVisible();	// TRUE if grip is set to be visible

	virtual CWnd* GetResizableWnd() = 0;

public:
	CResizableGrip();
	virtual ~CResizableGrip();

};

#endif // !defined(AFX_RESIZABLEGRIP_H__INCLUDED_)
