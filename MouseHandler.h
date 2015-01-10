// MouseHandler.h: interface for the CMouseHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSEHANDLER_H__328ACBD1_1C85_452C_A693_6115485B31CC__INCLUDED_)
#define AFX_MOUSEHANDLER_H__328ACBD1_1C85_452C_A693_6115485B31CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMouseHandler  
{
public:
	CMouseHandler();
	virtual ~CMouseHandler();

	void	OnMouseMove(   HWND, WPARAM, LPARAM );
	void	OnLButtonDown( HWND, WPARAM, LPARAM );
	void	OnLButtonUp(   HWND, WPARAM, LPARAM );
	void	OnRButtonDown( HWND, WPARAM, LPARAM );
	void	OnRButtonUp(   HWND, WPARAM, LPARAM );
	void	OnMButtonDown( HWND, WPARAM, LPARAM );
	void	OnMButtonUp(   HWND, WPARAM, LPARAM );

	void	AutoPan();
};

#endif // !defined(AFX_MOUSEHANDLER_H__328ACBD1_1C85_452C_A693_6115485B31CC__INCLUDED_)
