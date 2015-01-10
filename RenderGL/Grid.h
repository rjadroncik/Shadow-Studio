// Grid.h: interface for the CGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_H__E75860EB_1242_11D8_A845_0002440D03A4__INCLUDED_)
#define AFX_GRID_H__E75860EB_1242_11D8_A845_0002440D03A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../ModifierStack.h"
#include "RenderGL.h"


class RENDERGL_API CGrid  
{
public:

	CGrid();
	virtual ~CGrid();

	void	Initialize();
	void	Delete();

	bool	GenerateMesh();
	void	DeleteMesh();

	void	SetMesh( CMesh	*pMesh );


	Float2	Size;
	Int2	Tassellation;

	CMesh		*pRenderMesh;

private:

	Float2	Step;
};

#endif // !defined(AFX_GRID_H__E75860EB_1242_11D8_A845_0002440D03A4__INCLUDED_)
