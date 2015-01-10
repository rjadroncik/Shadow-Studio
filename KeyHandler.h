// KeyHandler.h: interface for the CKeyHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYHANDLER_H__5DA700B0_88CB_4534_97ED_79E9F23630A7__INCLUDED_)
#define AFX_KEYHANDLER_H__5DA700B0_88CB_4534_97ED_79E9F23630A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKeyHandler  
{
public:
	CKeyHandler();
	virtual ~CKeyHandler();

	void OnKeyDown(	HWND, WPARAM, LPARAM );
};

#endif // !defined(AFX_KEYHANDLER_H__5DA700B0_88CB_4534_97ED_79E9F23630A7__INCLUDED_)
