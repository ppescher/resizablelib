// ResizableVersion.h: interface for the CResizableVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZABLEVERSION_H__F7B06FFD_5074_4165_A82F_B4DA2BC4AF95__INCLUDED_)
#define AFX_RESIZABLEVERSION_H__F7B06FFD_5074_4165_A82F_B4DA2BC4AF95__INCLUDED_

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

// called automatically by a static initializer
// (if not appropriate can be called later)
// to setup global version numbers

void InitRealVersions();


#endif // !defined(AFX_RESIZABLEVERSION_H__F7B06FFD_5074_4165_A82F_B4DA2BC4AF95__INCLUDED_)
