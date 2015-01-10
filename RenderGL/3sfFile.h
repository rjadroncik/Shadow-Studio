// 3sfFile.h: interface for the C3sfFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3SFFILE_H__8157F7E7_472E_492D_8B9D_AA5AF3A05413__INCLUDED_)
#define AFX_3SFFILE_H__8157F7E7_472E_492D_8B9D_AA5AF3A05413__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderGL.h"
#include "Scene.h"

struct t3sfHeader
{



};


class RENDERGL_API C3sfFile  
{
public:
	C3sfFile();
	virtual ~C3sfFile();

	bool Load( CScene *pscene, LPSTR fileName );
	bool Save( LPSTR fileName, CScene *pscene );

	bool Import( CScene *pscene, LPSTR fileName );
};

#endif // !defined(AFX_3SFFILE_H__8157F7E7_472E_492D_8B9D_AA5AF3A05413__INCLUDED_)
