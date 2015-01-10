// Mesh.h: interface for the CMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESH_H__FE45ACA0_139A_11D8_A845_0002440D03A4__INCLUDED_)
#define AFX_MESH_H__FE45ACA0_139A_11D8_A845_0002440D03A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "RenderGL.h"

#ifdef RENDERGL_EXPORTS
#define RENDERGL_API __declspec(dllexport)
#else
#define RENDERGL_API __declspec(dllimport)
#endif

#define RENDER_VERTEX		0x0001	
#define RENDER_EDGES		0x0002
#define RENDER_FACES		0x0004

#define ENABLE_LIGHTING		0x0100

#define HIGHLIGHT_EDGES		0x0010


struct tTriangle
{
//Neighbour Triangle Indexes
	int			Neighbour[3];

//Plane Equation
	Float4	Plane;

//Light Visibility ( for Shadows )	
	bool		Visible; 
};


struct  tMaterial
{
	Float4	Ambient;
	Float4	Diffuse;

	Float4	Specular;
	Float4	Emission;

	float		Shininess;
};
/*
struct  tSmoothGroup
{
	WORD	*pTriangle;
	WORD	TriangleCount;
};*/

struct  tObject
{
//General Information
	bool		Visible;
	tMaterial	Material;
	Float4	Color;

//Transformation Information
	Float3	Position;
	Float3	Rotation;
//	Float3	Scale;
//	Float3	Direction;

//Vertex Information
	WORD		VertexCount;
	GLfloat		*pVertex;			//[3]
	GLfloat		*pNormal;			//[3]
	GLfloat		*pTexCoord;			//[2]
	GLfloat		*pColor;			//[4]		
	
	bool		HasVertexColors;

//Texture Information
	UINT		TextureID;
	CHAR		TextureFile[96];
	bool		HasTexture;

//Triangle Information
	WORD		TriangleCount;
	tTriangle	*pTriangle;

	GLboolean 	*pEdgeFlag;			//[3]
	GLushort	*pTriangleIndice;	//[3]

	bool		HasTriangles;

//Line Information
	WORD		LineCount;
	GLushort	*pLineIndice;		//[2]

	bool		HasLines;

//Smoothing Information
/*	tSmoothGroup	*pSmoothGroup;
	WORD			SmoothGroupCount;*/
};


class RENDERGL_API CMesh  
{
public:

	CMesh();
	virtual ~CMesh();

//Mesh Manipulation
	void	Reset();
	void	Initialize();
	void	Delete();

	void	Render( WORD renderStyle );

	void	UpdateNormals();
	void	UpdateConnectivity();
	void	UpdateFacePlanes();
	void	UpdateVisibility( Float4 *lightPos );

//Object Manipulation
	WORD	AddObject();
	bool	DeleteObject( WORD index );

//Pre-Rendering Setup
	void	UpdateObjectFacePlanes( WORD index );
	void	UpdateObjectNormals( WORD index );
	void	UpdateObjectConnectivity( WORD index );
	void	UpdateObjectVisibility( WORD index, Float4 *lightPos );


	void	RenderObject( WORD object );
	void	RenderObjectEdges( WORD object, bool highlight );

	void	SetObjectRotation( WORD index, float x, float y, float z );
	void	SetObjectPosition( WORD index, float x, float y, float z );

//	void	SetObjectDirection( WORD index, float x, float y, float z );
	void	SetObjectTexture( WORD index, LPSTR textureFile );

//Triangle Manipulation
	void	AddTriangle( WORD object, WORD vertex1, WORD vertex2, WORD vertex3 );
	void	AddTriangles( WORD object, WORD count );
	void	DeleteTriangle( WORD object, WORD triangle );
	void	EvaluateTriangleNeighbour( WORD object, WORD triangle, float s, float t );

//Line Manipulation
	void	AddLine( WORD object, WORD vertex1, WORD vertex2 );
	void	DeleteLine( WORD object, WORD line );

//Vertex Manipulation
	void	AddVertex( WORD object, float x, float y, float z );
	void	AddVertices( WORD object, WORD count );
	void	DeleteVertex( WORD object, WORD vertex );

//Object Information
	WORD		ObjectCount;
	tObject		*pObject;

//Texture Information
	WORD		TextureCount;

private:

	bool	UsingVertexBuffers;

	WORD		n, m, i; 
	Float3	TempVector3a, TempVector3b, TempVector3c;

};

#endif // !defined(AFX_MESH_H__FE45ACA0_139A_11D8_A845_0002440D03A4__INCLUDED_)


