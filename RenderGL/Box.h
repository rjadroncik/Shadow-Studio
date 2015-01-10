// Box.h: interface for the CBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOX_H__98F9184C_12E2_11D8_A845_0002440D03A4__INCLUDED_)
#define AFX_BOX_H__98F9184C_12E2_11D8_A845_0002440D03A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderGL.h"
#include "Mesh.h"

class RENDERGL_API CBox  
{
public:
	CBox();
	virtual ~CBox();

	void	Initialize();
	void	Delete();

	void	UpdateMesh( bool updateData );

	bool	GenerateMesh();
	void	DeleteMesh();

	void	SetMesh( CMesh	*pmesh );

	Float3	Size;
	Int3	Tassellation;

	CMesh		*pMesh;
	short		Object;

private:

	Float3	Step;

	WORD		n, x, y;
	WORD		Triangle, Vertex;

	WORD		VertexCount;
};

#endif // !defined(AFX_BOX_H__98F9184C_12E2_11D8_A845_0002440D03A4__INCLUDED_)
