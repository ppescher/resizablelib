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
	BOOL LoadWindowRect(HWND hWnd, LPCTSTR pszSection);	// non-zero if successful
	BOOL SaveWindowRect(HWND hWnd, LPCTSTR pszSection);	// non-zero if successful

public:
	CResizableState();
	virtual ~CResizableState();
};

#endif // !defined(AFX_RESIZABLESTATE_H__9B226426_F27A_4F1E_BA45_88CD8A5A1B9E__INCLUDED_)
