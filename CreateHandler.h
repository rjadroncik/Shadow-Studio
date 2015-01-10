// CreateHandler.h: interface for the CCreateHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEHANDLER_H__17EF371F_BBCC_4030_ABC8_0AA3858A1B3D__INCLUDED_)
#define AFX_CREATEHANDLER_H__17EF371F_BBCC_4030_ABC8_0AA3858A1B3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCreateHandler  
{
public:
	CCreateHandler();
	virtual ~CCreateHandler();

	void	OnMouseMove(   HWND, WPARAM, LPARAM );
	void	OnLButtonDown( HWND, WPARAM, LPARAM );
	void	OnLButtonUp(   HWND, WPARAM, LPARAM );

	void	OnRButtonDown( HWND, WPARAM, LPARAM );
	void	OnRButtonUp( HWND, WPARAM, LPARAM );
	
	void	UpdateCreationDialog( WORD model );
	void	AssignRandomColor( WORD model );

private:

	WORD model;
};

#endif // !defined(AFX_CREATEHANDLER_H__17EF371F_BBCC_4030_ABC8_0AA3858A1B3D__INCLUDED_)
