// ViewHandler.h: interface for the CViewHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWHANDLER_H__ED463411_3105_44CF_A898_3AAC3D0CAE3E__INCLUDED_)
#define AFX_VIEWHANDLER_H__ED463411_3105_44CF_A898_3AAC3D0CAE3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CViewHandler  
{
public:
	CViewHandler();
	virtual ~CViewHandler();

	void	OnMouseMove(   HWND, WPARAM, LPARAM );
	void	OnLButtonDown( HWND, WPARAM, LPARAM );
	void	OnLButtonUp(   HWND, WPARAM, LPARAM );
	void	OnRButtonDown( HWND, WPARAM, LPARAM );
	void	OnRButtonUp(   HWND, WPARAM, LPARAM );
	void	OnMButtonDown( HWND, WPARAM, LPARAM );
	void	OnMButtonUp(   HWND, WPARAM, LPARAM );
};

#endif // !defined(AFX_VIEWHANDLER_H__ED463411_3105_44CF_A898_3AAC3D0CAE3E__INCLUDED_)
