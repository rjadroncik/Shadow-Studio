// Cone.h: interface for the CCone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONE_H__758D51C2_15FB_11D8_A845_0002440D03A4__INCLUDED_)
#define AFX_CONE_H__758D51C2_15FB_11D8_A845_0002440D03A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderGL.h"
#include "Mesh.h"


class RENDERGL_API CCone  
{
public:

	CCone();
	virtual ~CCone();

	void	Initialize();
	void	Delete();

	void	UpdateMesh();
	bool	GenerateMesh();
	void	DeleteMesh();

	void	SetMesh( CMesh	*pMesh );


	Float2	Size;
	Int2	Tassellation;

	CMesh		*pRenderMesh;


private:

	Float2	Step;

};

#endif // !defined(AFX_CONE_H__758D51C2_15FB_11D8_A845_0002440D03A4__INCLUDED_)
