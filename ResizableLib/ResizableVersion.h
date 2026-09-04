// ResizableVersion.h: interface for the CResizableVersion class.
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

#if !defined(AFX_RESIZABLEVERSION_H__INCLUDED_)
#define AFX_RESIZABLEVERSION_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// global variables that hold actual version numbers
// retrieved and adapted at run-time to be equivalent
// to preprocessor macros that set the target platform

extern DWORD realWINVER;

#ifdef _WIN32_WINDOWS
extern DWORD real_WIN32_WINDOWS;
#endif

#ifdef _WIN32_WINNT
extern DWORD real_WIN32_WINNT;
#endif

#ifdef _WIN32_IE
extern DWORD real_WIN32_IE;
#endif

extern DWORD real_ThemeSettings;

extern BOOL real_DpiAwarenessV2;

// called automatically by a static initializer
// (if not appropriate can be called later)
// to setup global version numbers

void InitRealVersions();

// check for visual style and DPI settings

void InitAppSettings();

// DPI Awareness helpers

#ifndef WM_DPICHANGED
#define WM_DPICHANGED				0x02E0
#define WM_GETDPISCALEDSIZE			0x02E4
#endif

// sent to child windows after the parent handled WM_DPICHANGED
#ifndef WM_DPICHANGED_AFTERPARENT
#define WM_DPICHANGED_AFTERPARENT	0x02E3
#endif

#ifndef USER_DEFAULT_SCREEN_DPI
#define USER_DEFAULT_SCREEN_DPI		96
#endif

UINT GetWindowDpi(HWND hWnd);
BOOL AdjustWindowForDpi(HWND hWnd, LPRECT lpRect, UINT nDpi = 0);
CSize GetSizeGripMetrics(HWND hWnd);
CRect CalcResizedWindowForDpi(HWND hWnd, UINT nNewDpi, UINT nCurDpi);
void ResizeWindowForDpi(HWND hWnd, UINT nNewDpi, UINT nCurDpi);

#endif // !defined(AFX_RESIZABLEVERSION_H__INCLUDED_)
