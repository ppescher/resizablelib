// ResizableGrip.h: interface for the CResizableGrip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZABLEGRIP_H__86501E7F_B2F8_4928_A421_25C6F041AD1A__INCLUDED_)
#define AFX_RESIZABLEGRIP_H__86501E7F_B2F8_4928_A421_25C6F041AD1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WS_EX_LAYOUT_RTL	0x00400000

class CResizableGrip  
{
private:
	SIZE m_sizeGrip;		// holds grip size

	CScrollBar m_wndGrip;	// grip control

private:
	static BOOL IsRTL(HWND hwnd)
	{
		DWORD dwExStyle = ::GetWindowLong(hwnd, GWL_EXSTYLE);
		return (dwExStyle & WS_EX_LAYOUT_RTL);
	}

	static LRESULT CALLBACK GripWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	BOOL InitGrip();
	void UpdateGripPos();
	void ShowSizeGrip(BOOL bShow = TRUE);	// show or hide the size grip

	virtual CWnd* GetResizableWnd() = 0;

public:
	CResizableGrip();
	virtual ~CResizableGrip();

};

#endif // !defined(AFX_RESIZABLEGRIP_H__86501E7F_B2F8_4928_A421_25C6F041AD1A__INCLUDED_)
