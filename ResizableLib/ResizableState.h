// ResizableState.h: interface for the CResizableState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZABLESTATE_H__9B226426_F27A_4F1E_BA45_88CD8A5A1B9E__INCLUDED_)
#define AFX_RESIZABLESTATE_H__9B226426_F27A_4F1E_BA45_88CD8A5A1B9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResizableState  
{
protected:
	// non-zero if successful
	BOOL LoadWindowRect(LPCTSTR pszSection, BOOL bRectOnly);
	BOOL SaveWindowRect(LPCTSTR pszSection);

	virtual CWnd* GetResizableWnd() = 0;

public:
	CResizableState();
	virtual ~CResizableState();
};

#endif // !defined(AFX_RESIZABLESTATE_H__9B226426_F27A_4F1E_BA45_88CD8A5A1B9E__INCLUDED_)
