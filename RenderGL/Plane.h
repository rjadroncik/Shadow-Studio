// Plane.h: interface for the CPlane class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANE_H__758D51C2_15FB_11D8_A845_0002440D03A4__INCLUDED_)
#define AFX_PLANE_H__758D51C2_15FB_11D8_A845_0002440D03A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderGL.h"
#include "Mesh.h"


class RENDERGL_API CPlane  
{
public:

	CPlane();
	virtual ~CPlane();

	void	Initialize();
	void	Delete();

	void	UpdateMesh( bool updateData );
	bool	GenerateMesh();
	void	DeleteMesh();

	void	SetMesh( CMesh	*pmesh );

	Float2	Size;
	Int2	Tassellation;

	CMesh		*pMesh;
	short		Object;

private:

	Float2	Step;

	WORD		n, x, y;
	WORD		Triangle, Vertex;
};

#endif // !defined(AFX_PLANE_H__758D51C2_15FB_11D8_A845_0002440D03A4__INCLUDED_)
