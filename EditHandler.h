// EditHandler.h: interface for the CEditHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITHANDLER_H__EAE42F10_750E_4C52_8F7C_B274F8E187D3__INCLUDED_)
#define AFX_EDITHANDLER_H__EAE42F10_750E_4C52_8F7C_B274F8E187D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEditHandler  
{
public:
	CEditHandler();
	virtual ~CEditHandler();

	void	OnMouseMove(   HWND, WPARAM, LPARAM );
	void	OnLButtonDown( HWND, WPARAM, LPARAM );
	void	OnLButtonUp(   HWND, WPARAM, LPARAM );
	void	OnRButtonDown( HWND, WPARAM, LPARAM );
	void	OnRButtonUp(   HWND, WPARAM, LPARAM );
	void	OnMButtonDown( HWND, WPARAM, LPARAM );
	void	OnMButtonUp(   HWND, WPARAM, LPARAM );
};

#endif // !defined(AFX_EDITHANDLER_H__EAE42F10_750E_4C52_8F7C_B274F8E187D3__INCLUDED_)
