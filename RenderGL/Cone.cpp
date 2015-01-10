// Plane.cpp: implementation of the CCone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cone.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCone::CCone() { Initialize(); }
CCone::~CCone() {}

void CCone::Initialize()
{
	pRenderMesh = NULL;

	Size[0] = 100.0;
	Size[1] = 100.0;

	Tassellation[0] = 10;
	Tassellation[1] = 10;
}

void CCone::Delete() {}

void CCone::SetMesh( CMesh *pMesh ) { pRenderMesh = pMesh; }
void CCone::DeleteMesh() { if( pRenderMesh ) { pRenderMesh->DeleteObject( 0 ); } }

bool CCone::GenerateMesh()
{
	DeleteMesh();

	UINT	object = pRenderMesh->AddObject();

	pRenderMesh->pObject[object].HasLines = FALSE; 
	pRenderMesh->pObject[object].HasTriangles = TRUE; 
	pRenderMesh->pObject[object].HasVertexColors = FALSE; 
	pRenderMesh->pObject[object].Color[0] = 0.3f;
	pRenderMesh->pObject[object].Color[1] = 0.3f;
	pRenderMesh->pObject[object].Color[2] = 0.3f;

	Step[0] = Size[0] / (UINT)Tassellation[0];
	Step[1] = Size[1] / (UINT)Tassellation[1];

	pRenderMesh->AddVertices( object,  (UINT)( ( Tassellation[0] + 1 ) * ( Tassellation[1] + 1 ) ) ); 
	pRenderMesh->AddTriangles( object, (UINT)( Tassellation[0] * Tassellation[1] * 2 ) );

	return TRUE;
}


void CCone::UpdateMesh()
{
/*	UINT VertexCount = 0;
	UINT TriangleCount = 0;
	UINT t, s;

	Step[0] = Size[0] / (UINT)Tassellation[0];
	Step[1] = Size[1] / (UINT)Tassellation[1];

	for( UINT y = 0; y < (UINT)Tassellation[1] + 1; y++ )
	{
		for( UINT x = 0; x < (UINT)Tassellation[0] + 1; x++ )
		{
			pRenderMesh->pObject[0].pVertex[VertexCount][0] = Step[0] * x - Size[0] / 2;
			pRenderMesh->pObject[0].pVertex[VertexCount][2] = Step[1] * y - Size[1] / 2;
			VertexCount++;
		}
	}

	if( ( Size[0] * Size[1] ) >= 0 )
	{
		for( t = 0; t < (UINT)Tassellation[1]; t++ )
		{
			for( s = 0; s < (UINT)Tassellation[0]; s++ )
			{
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[0] = s + t * ( (UINT)Tassellation[0] + 1 ),
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[1] = s + ( t + 1 ) * ( (UINT)Tassellation[0] + 1 ),
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[2] = s + t * ( (UINT)Tassellation[0] + 1 ) + 1;    
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[0] = TRUE;
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[1] = FALSE;
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[2] = TRUE;
				TriangleCount++;
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[0] = s + t * ( (UINT)Tassellation[0] + 1 ) + 1,
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[1] = s + ( t + 1 ) * ( (UINT)Tassellation[0] + 1 ),
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[2] = s + ( t + 1 ) * ( (UINT)Tassellation[0] + 1 ) + 1;    
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[0] = FALSE;
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[1] = TRUE;
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[2] = TRUE;
				TriangleCount++;
			}
		}
	}
	else
	{
		for( t = 0; t < (UINT)Tassellation[1]; t++ )
		{
			for( s = 0; s < (UINT)Tassellation[0]; s++ )
			{
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[0] = s + t * ( (UINT)Tassellation[0] + 1 ),
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[1] = s + t * ( (UINT)Tassellation[0] + 1 ) + 1,
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[2] = s + ( t + 1 ) * ( (UINT)Tassellation[0] + 1 );    
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[0] = TRUE;
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[1] = FALSE;
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[2] = TRUE;
				TriangleCount++;
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[0] = s + t * ( (UINT)Tassellation[0] + 1 ) + 1,
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[1] = s + ( t + 1 ) * ( (UINT)Tassellation[0] + 1 ) + 1,
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].Vertex[2] = s + ( t + 1 ) * ( (UINT)Tassellation[0] + 1 );    
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[0] = TRUE;
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[1] = TRUE;
				pRenderMesh->pObject[0].pTriangleSmooth[TriangleCount].EdgeFlag[2] = FALSE;
				TriangleCount++;
			}
		}
	}
*/
}
