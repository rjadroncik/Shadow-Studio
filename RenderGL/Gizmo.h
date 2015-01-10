// Gizmo.h: interface for the CGizmo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GIZMO_H__B797390B_4133_489E_8F3E_A5C599441FCC__INCLUDED_)
#define AFX_GIZMO_H__B797390B_4133_489E_8F3E_A5C599441FCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mesh.h"
//#include "../../System.Types.h"


#define	GT_MOVE			1
#define	GT_ROTATE		2
#define	GT_SCALE		3

#define	GM_X			10
#define	GM_Y			11
#define	GM_Z			12

#define	GM_XY			13
#define	GM_YZ			14
#define	GM_ZX			15

#define COLOR_RED		1
#define COLOR_GREEN		2
#define COLOR_BLUE		3

class RENDERGL_API CGizmo  
{
public:
	CGizmo();
	virtual ~CGizmo();

	void	Create( UINT type, float size, UINT tasselation );

	void	Render( float size );

	void	RenderMove(); 
	void	RenderScale(); 

	void	UpdateSelectMesh();

	void	UpdateMoveMesh();
	void	UpdateScaleMesh();


	void	DrawArrow( float length, UINT color );

	void	Resize( float size );
	

	CMesh	SelectMesh;

	UINT	Type;

	int		ActiveObject;
	int		SelectedObject;

	Float3		Scale;

	Float4		PlaneXY;
	Float4		PlaneYZ;
	Float4		PlaneZX;

	Float3		VectorX;
	Float3		VectorY;
	Float3		VectorZ;

private:	

	float			Size;
	GLUquadricObj	*pQuadric;	

	UINT			Tasselation;

	Double3		TempVectord3a;
	Double3		TempVectord3b;
	Double3		TempVectord3c;

	double			ModelMatrix[16];
	double			ProjMatrix[16];
	int				Viewport[4];

};

#endif // !defined(AFX_GIZMO_H__B797390B_4133_489E_8F3E_A5C599441FCC__INCLUDED_)
