// ResizableGrip.h: interface for the CResizableGrip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZABLEGRIP_H__86501E7F_B2F8_4928_A421_25C6F041AD1A__INCLUDED_)
#define AFX_RESIZABLEGRIP_H__86501E7F_B2F8_4928_A421_25C6F041AD1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResizableGrip  
{
// Attributes
private:
	// flags
	BOOL m_bShowGrip;

	SIZE m_szGripSize;			// set at construction time

	CRect m_rcGripRect;			// current pos of grip

protected:
	void DrawGrip(CDC& dc);
	void ShowSizeGrip(BOOL bShow);	// show or hide the size grip
	void UpdateGripPos();
	UINT HitTest(CPoint point);		// in screen coord

	virtual CWnd* GetLayoutParent() = 0;

public:
	CResizableGrip();
	virtual ~CResizableGrip();

};

#endif // !defined(AFX_RESIZABLEGRIP_H__86501E7F_B2F8_4928_A421_25C6F041AD1A__INCLUDED_)
