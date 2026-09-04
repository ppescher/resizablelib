// ResizableVersion.cpp: implementation of the CResizableVersion class.
//
/////////////////////////////////////////////////////////////////////////////
//
// This file is part of ResizableLib
// https://github.com/ppescher/resizablelib
//
// Copyright (C) 2000-2024 by Paolo Messina
// mailto:ppescher@hotmail.com
//
// The contents of this file are subject to the Artistic License 2.0
// http://opensource.org/licenses/Artistic-2.0
//
// If you find this code useful, credits would be nice!
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResizableVersion.h"

//////////////////////////////////////////////////////////////////////
// Static initializer object (with macros to hide in ClassView)

// static initializer must be called before user code
#pragma warning(disable:4073)
#pragma init_seg(lib)

#ifdef _UNDEFINED_
#define BEGIN_HIDDEN {
#define END_HIDDEN }
#else
#define BEGIN_HIDDEN
#define END_HIDDEN
#endif

BEGIN_HIDDEN
struct _VersionInitializer
{
	_VersionInitializer()
	{
		InitRealVersions();
	};
};
END_HIDDEN

// The one and only version-check object
static _VersionInitializer g_version;

//////////////////////////////////////////////////////////////////////
// Private implementation

static DLLVERSIONINFO g_dviCommCtrls;
static OSVERSIONINFOEX g_osviWindows;

#pragma warning(push)
#pragma warning(disable:4996)
static void CheckOsVersion()
{
	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	ZeroMemory(&g_osviWindows, sizeof(OSVERSIONINFOEX));
	g_osviWindows.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (GetVersionEx((LPOSVERSIONINFO)&g_osviWindows))
		return;

	// If that fails, try using the OSVERSIONINFO structure.
	g_osviWindows.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
	if (GetVersionEx((LPOSVERSIONINFO)&g_osviWindows))
		return;

	// When all the above fails, set values for the worst case
	g_osviWindows.dwMajorVersion = 4;
	g_osviWindows.dwMinorVersion = 0;
	g_osviWindows.dwBuildNumber = 0;
	g_osviWindows.dwPlatformId = VER_PLATFORM_WIN32_WINDOWS;
	g_osviWindows.szCSDVersion[0] = TEXT('\0');
}
#pragma warning(pop)

static void CheckCommCtrlsVersion()
{
	// Check Common Controls version
	ZeroMemory(&g_dviCommCtrls, sizeof(DLLVERSIONINFO));
	HMODULE hMod = ::LoadLibrary(_T("comctl32.dll"));
	if (hMod != NULL)
	{
		// Get the version function
		DLLGETVERSIONPROC pfnDllGetVersion;
		pfnDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hMod, "DllGetVersion");

		if (pfnDllGetVersion != NULL)
		{
			// Obtain version information
			g_dviCommCtrls.cbSize = sizeof(DLLVERSIONINFO);
			if (SUCCEEDED(pfnDllGetVersion(&g_dviCommCtrls)))
			{
				::FreeLibrary(hMod);
				return;
			}
		}

		::FreeLibrary(hMod);
	}

	// Set values for the worst case
	g_dviCommCtrls.dwMajorVersion = 4;
	g_dviCommCtrls.dwMinorVersion = 0;
	g_dviCommCtrls.dwBuildNumber = 0;
	g_dviCommCtrls.dwPlatformID = DLLVER_PLATFORM_WINDOWS;
}


//////////////////////////////////////////////////////////////////////
// Exported global symbols

DWORD realWINVER = 0;

#ifdef _WIN32_WINDOWS
DWORD real_WIN32_WINDOWS = 0;
#endif

#ifdef _WIN32_WINNT
DWORD real_WIN32_WINNT = 0;
#endif

#ifdef _WIN32_IE
DWORD real_WIN32_IE = 0;
#endif

DWORD real_ThemeSettings = 0;

BOOL real_DpiAwarenessV2 = FALSE;

// macro to convert version numbers to hex format
#define CNV_OS_VER(x) ((BYTE)(((BYTE)(x) / 10 * 16) | ((BYTE)(x) % 10)))

void InitRealVersions()
{
	CheckCommCtrlsVersion();
	CheckOsVersion();

	// set real version values

	realWINVER = MAKEWORD(CNV_OS_VER(g_osviWindows.dwMinorVersion),
		CNV_OS_VER(g_osviWindows.dwMajorVersion));

#ifdef _WIN32_WINDOWS
	if (g_osviWindows.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		real_WIN32_WINDOWS = realWINVER;
	else
		real_WIN32_WINDOWS = 0;
#endif

#ifdef _WIN32_WINNT
	if (g_osviWindows.dwPlatformId == VER_PLATFORM_WIN32_NT)
		real_WIN32_WINNT = realWINVER;
	else
		real_WIN32_WINNT = 0;
#endif

#ifdef _WIN32_IE
	switch (g_dviCommCtrls.dwMajorVersion)
	{
	case 4:
		switch (g_dviCommCtrls.dwMinorVersion)
		{
		case 70:
			real_WIN32_IE = 0x0300;
			break;
		case 71:
			real_WIN32_IE = 0x0400;
			break;
		case 72:
			real_WIN32_IE = 0x0401;
			break;
		default:
			real_WIN32_IE = 0x0200;
		}
		break;
	case 5:
		if (g_dviCommCtrls.dwMinorVersion > 80)
			real_WIN32_IE = 0x0501;
		else
			real_WIN32_IE = 0x0500;
		break;
	case 6:
		if (g_dviCommCtrls.dwMinorVersion > 0)
			real_WIN32_IE = 0x0700;	// since Vista
		else
			real_WIN32_IE = 0x0600;	// includes checks for 0x0560 (IE6)
		break;
	default:
		real_WIN32_IE = 0;
	}
#endif
}

// Whether non-client area is using XP Visual Style
void InitAppSettings()
{
	real_ThemeSettings = 0;
	real_DpiAwarenessV2 = FALSE;

	typedef BOOL (STDAPICALLTYPE * IS_APP_THEMED)(VOID);
	typedef DWORD (STDAPICALLTYPE * GET_THEME_APP_PROPERTIES)(VOID);
	typedef INT_PTR (STDAPICALLTYPE* GET_THREAD_DPI_AWARENESS_CONTEXT)(VOID);
	typedef BOOL (STDAPICALLTYPE* ARE_DPI_AWARENESS_CONTEXTS_EQUAL)(INT_PTR, INT_PTR);

	// check DPI awareness (assume per process)
	static HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
	static GET_THREAD_DPI_AWARENESS_CONTEXT pfnGetThreadDpiAwarenessContext = 
		(GET_THREAD_DPI_AWARENESS_CONTEXT) GetProcAddress(hUser32, "GetThreadDpiAwarenessContext");
	static ARE_DPI_AWARENESS_CONTEXTS_EQUAL pfnAreDpiAwarenessContextsEqual =
		(ARE_DPI_AWARENESS_CONTEXTS_EQUAL) GetProcAddress(hUser32, "AreDpiAwarenessContextsEqual");

	if (pfnGetThreadDpiAwarenessContext != NULL && pfnAreDpiAwarenessContextsEqual != NULL)
	{
		INT_PTR nDpiAwareCtx = pfnGetThreadDpiAwarenessContext();
		// Is Per Monitor V2?
		real_DpiAwarenessV2 = pfnAreDpiAwarenessContextsEqual(nDpiAwareCtx, (INT_PTR)(-4));
	}

	// check DLL is in place, themes can't work without
	static HMODULE hThemeLib = GetModuleHandle(_T("uxtheme.dll"));
	static IS_APP_THEMED pfnIsAppThemed =
		(IS_APP_THEMED) GetProcAddress(hThemeLib, "IsAppThemed");
	static GET_THEME_APP_PROPERTIES pfnGetThemeAppProperties =
		(GET_THEME_APP_PROPERTIES) GetProcAddress(hThemeLib, "GetThemeAppProperties");

	// check calling process has themes enabled
	// check application theme includes non-client area
	if (pfnIsAppThemed != NULL && pfnGetThemeAppProperties != NULL)
	{
		if (pfnIsAppThemed())
			real_ThemeSettings = pfnGetThemeAppProperties();
	}
}

//////////////////////////////////////////////////////////////////////
// DPI awareness support
//////////////////////////////////////////////////////////////////////

CSize GetSizeGripMetrics(HWND hWnd)
{
	typedef UINT(WINAPI* PFNGETDPIFORWINDOW)(HWND);
	typedef int (WINAPI* PFNGETSYSTEMMETRICSFORDPI)(int, UINT);

	static HMODULE hUser32 = ::GetModuleHandle(_T("user32.dll"));
	static PFNGETDPIFORWINDOW pfnGetDpiForWindow =
		(PFNGETDPIFORWINDOW)::GetProcAddress(hUser32, "GetDpiForWindow");
	static PFNGETSYSTEMMETRICSFORDPI pfnGetSystemMetricsForDpi =
		(PFNGETSYSTEMMETRICSFORDPI)::GetProcAddress(hUser32, "GetSystemMetricsForDpi");

	if (hWnd != NULL && pfnGetDpiForWindow != NULL && pfnGetSystemMetricsForDpi != NULL)
	{
		const UINT nDpi = pfnGetDpiForWindow(hWnd);
		if (nDpi != 0)
			return CSize(pfnGetSystemMetricsForDpi(SM_CXVSCROLL, nDpi),
				pfnGetSystemMetricsForDpi(SM_CYHSCROLL, nDpi));
	}

	return CSize(::GetSystemMetrics(SM_CXVSCROLL), ::GetSystemMetrics(SM_CYHSCROLL));
}

UINT GetWindowDpi(HWND hWnd)
{
	typedef UINT(WINAPI* PFNGETDPIFORWINDOW)(HWND);
	typedef int (WINAPI* PFNGETSYSTEMMETRICSFORDPI)(int, UINT);

	static HMODULE hUser32 = ::GetModuleHandle(_T("user32.dll"));
	static PFNGETDPIFORWINDOW pfnGetDpiForWindow =
		(PFNGETDPIFORWINDOW)::GetProcAddress(hUser32, "GetDpiForWindow");

	if (hWnd != NULL && pfnGetDpiForWindow != NULL)
	{
		const UINT nDpi = pfnGetDpiForWindow(hWnd);
		if (nDpi != 0)
			return nDpi;
	}

	return USER_DEFAULT_SCREEN_DPI;
}

BOOL AdjustWindowForDpi(HWND hWnd, LPRECT lpRect, UINT nDpi)
{
	typedef UINT(WINAPI* PFNGETDPIFORWINDOW)(HWND);
	typedef BOOL(WINAPI* PFNADJUSTWINDOWRECTEXFORDPI)(
		LPRECT lpRect,
		DWORD  dwStyle,
		BOOL   bMenu,
		DWORD  dwExStyle,
		UINT   dpi
		);

	static HMODULE hUser32 = ::GetModuleHandle(_T("user32.dll"));
	static PFNADJUSTWINDOWRECTEXFORDPI pfnAdjustWindowRectExForDpi =
		(PFNADJUSTWINDOWRECTEXFORDPI)::GetProcAddress(hUser32, "AdjustWindowRectExForDpi");

	if (hWnd != NULL && pfnAdjustWindowRectExForDpi != NULL)
	{
		if (nDpi == 0)
			nDpi = GetWindowDpi(hWnd);
		if (pfnAdjustWindowRectExForDpi(lpRect, ::GetWindowLong(hWnd, GWL_STYLE),
			::IsMenu(::GetMenu(hWnd)), ::GetWindowLong(hWnd, GWL_EXSTYLE), nDpi))
			return TRUE;
	}

	return ::AdjustWindowRectEx(lpRect, ::GetWindowLong(hWnd, GWL_STYLE),
		::IsMenu(::GetMenu(hWnd)), ::GetWindowLong(hWnd, GWL_EXSTYLE));
}

CRect CalcResizedWindowForDpi(HWND hWnd, UINT nNewDpi, UINT nCurDpi)
{
	CWnd* pWnd = CWnd::FromHandle(hWnd);
	DWORD dwStyleEx = pWnd->GetExStyle();
	DWORD dwChildStyleEx = ::GetWindowLong(pWnd->GetWindow(GW_CHILD)->GetSafeHwnd(), GWL_EXSTYLE);

	CRect rect;
	CRect rcBorder;
	// fix automatic resize done by V2 DPI system
	if (real_DpiAwarenessV2 && nNewDpi > nCurDpi)
	{
		// non-client area has been resized already, but we want to know
		// the original client area before DPI changed, to correct calculation
		AdjustWindowForDpi(hWnd, rect, nNewDpi);
		AdjustWindowForDpi(hWnd, rcBorder, nCurDpi);
		rcBorder.SetRect(rcBorder.left - rect.left, rcBorder.top - rect.top,
			rect.right - rcBorder.right, rect.bottom - rcBorder.bottom);
	}

	// get position relative to parent
	pWnd->GetWindowRect(rect);
	if (dwStyleEx & WS_EX_MDICHILD)
		::MapWindowPoints(NULL, ::GetParent(hWnd), &rect.TopLeft(), 2);
	// keep aligned to top-left corner
	CPoint pt = rect.TopLeft();

	// get client area (correct for MDI frame client edge)
	pWnd->GetClientRect(rect);
	if ((dwStyleEx & WS_EX_MDICHILD) && (dwChildStyleEx & WS_EX_CLIENTEDGE))
		rect.DeflateRect(2, 2);

	// apply correction only to MDI child windows
	if (dwStyleEx & WS_EX_MDICHILD)
		rect.InflateRect(rcBorder);

	// rescale for target DPI
	CSize size = rect.Size();
	size.cx = MulDiv(size.cx, nNewDpi, nCurDpi);
	size.cy = MulDiv(size.cy, nNewDpi, nCurDpi);
	rect = CRect(rect.TopLeft(), size);

	// add frame borders for top-level window or MDI child
	if (!(pWnd->GetStyle() & WS_CHILD))
	{
		::MapWindowPoints(hWnd, NULL, &rect.TopLeft(), 2);
		::AdjustWindowRectEx(&rect, pWnd->GetStyle(),
			::IsMenu(::GetMenu(hWnd)), pWnd->GetExStyle());
	}
	else
	{
		if (dwStyleEx & WS_EX_MDICHILD)
		{
			// correct for MDI frame client edge
			if (dwChildStyleEx & WS_EX_CLIENTEDGE)
				rect.InflateRect(2, 2);
			// add borders
			::AdjustWindowRectEx(&rect, pWnd->GetStyle(),
				::IsMenu(::GetMenu(hWnd)), dwStyleEx);
			// keep top left position
			rect.MoveToXY(pt);
		}
		else
			::MapWindowPoints(hWnd, ::GetParent(hWnd), &rect.TopLeft(), 2);
	}
	return rect;
}

void ResizeWindowForDpi(HWND hWnd, UINT nNewDpi, UINT nCurDpi)
{
	CWnd* pWnd = CWnd::FromHandle(hWnd);

	BOOL bMaximized = pWnd->IsZoomed();
	if (bMaximized)
	{
		// window already maximized needs to be resized
		// but we need to do it on the normal rect, so that it looks
		// good also when restored to normal
		pWnd->SetRedraw(FALSE);
		pWnd->ShowWindow(SW_RESTORE);
	}

	CRect rect = CalcResizedWindowForDpi(hWnd, nNewDpi, nCurDpi);

	// resize/move to new scaled rect
	pWnd->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(),
		SWP_NOSENDCHANGING | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION);

	if (bMaximized)
	{
		// back to maximized state if required
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->SetRedraw(TRUE);
	}
}