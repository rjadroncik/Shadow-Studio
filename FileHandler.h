// FileHandler.h: interface for the CFileHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEHANDLER_H__8B94AB0B_0FF1_4020_9B21_6E0B8D8FA134__INCLUDED_)
#define AFX_FILEHANDLER_H__8B94AB0B_0FF1_4020_9B21_6E0B8D8FA134__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileHandler  
{
public:
	CFileHandler();
	virtual ~CFileHandler();

	void	OnFileLoad();
	void	OnFileSave( bool newFile );
	void	OnFileNew( bool force );
	void	OnFileClose( bool reset );

};

#endif // !defined(AFX_FILEHANDLER_H__8B94AB0B_0FF1_4020_9B21_6E0B8D8FA134__INCLUDED_)
