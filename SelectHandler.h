// SelectHandler.h: interface for the CSelectHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTIONHANDLER_H__B46FA831_A25C_4AA7_97EE_C066E1170645__INCLUDED_)
#define AFX_SELECTIONHANDLER_H__B46FA831_A25C_4AA7_97EE_C066E1170645__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSelectHandler  
{
public:
	CSelectHandler();
	virtual ~CSelectHandler();

	void	UpdateRegion( bool draw );

	void	OnMouseMove(   HWND, WPARAM, LPARAM );
	void	OnLButtonDown( HWND, WPARAM, LPARAM );
	void	OnLButtonUp(   HWND, WPARAM, LPARAM );
	void	OnRButtonDown( HWND, WPARAM, LPARAM );
	void	OnRButtonUp(   HWND, WPARAM, LPARAM );
	void	OnMButtonDown( HWND, WPARAM, LPARAM );
	void	OnMButtonUp(   HWND, WPARAM, LPARAM );
};

#endif // !defined(AFX_SELECTIONHANDLER_H__B46FA831_A25C_4AA7_97EE_C066E1170645__INCLUDED_)
