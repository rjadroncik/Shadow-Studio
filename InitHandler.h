// InitHandler.h: interface for the CInitHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITHANDLER1_H__7CB50E02_AADB_4819_AFCA_377BBB80560B__INCLUDED_)
#define AFX_INITHANDLER1_H__7CB50E02_AADB_4819_AFCA_377BBB80560B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInitHandler  
{
public:
	CInitHandler();
	virtual ~CInitHandler();

	void	InitSystem( HINSTANCE hInstance );
	void	InitMainBar();
	void	InitTrackBar();
	void	InitMainToolBar();
	void	InitBottomToolBar();
	void	InitMenu();

	void	InitDialogs();

	void	InitTools();
};

#endif // !defined(AFX_INITHANDLER1_H__7CB50E02_AADB_4819_AFCA_377BBB80560B__INCLUDED_)
