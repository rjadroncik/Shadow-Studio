// Plane.cpp: implementation of the CPlane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Plane.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlane::CPlane() { Initialize(); }
CPlane::~CPlane() {}

void CPlane::Initialize()
{
	pMesh = NULL;

	Size[0] = 100.0f;
	Size[1] = 100.0f;

	Tassellation[0] = 10;
	Tassellation[1] = 10;
}

void CPlane::Delete() {}

void CPlane::SetMesh( CMesh *pmesh )
{
	Object = -1;
	pMesh = pmesh; 
}

void CPlane::DeleteMesh()
{
	if( !pMesh ) { return; }

	for( n = 0; n < pMesh->ObjectCount; n++ )
	{
		pMesh->DeleteObject( n ); 
	} 

	Object = -1;
}

bool CPlane::GenerateMesh()
{
	if( !pMesh ) { return FALSE; }

	DeleteMesh(); 

	Object = pMesh->AddObject();

	pMesh->pObject[Object].HasVertexColors = FALSE; 
	pMesh->pObject[Object].Color[0] = 0.3f;
	pMesh->pObject[Object].Color[1] = 0.3f;
	pMesh->pObject[Object].Color[2] = 0.3f;

	Step[0] = Size[0] / Tassellation[0];
	Step[1] = Size[1] / Tassellation[1];

	pMesh->AddVertices( Object,  ( Tassellation[0] + 1 ) * ( Tassellation[1] + 1 ) ); 
	pMesh->AddTriangles( Object, Tassellation[0] * Tassellation[1] * 2 );

	return TRUE;
}

void CPlane::UpdateMesh( bool updateData )
{
	if( !pMesh || ( Object < 0 ) ) { return; }

	Vertex = 0;
	Triangle = 0;

	Step[0] = Size[0] / Tassellation[0];
	Step[1] = Size[1] / Tassellation[1];

	for( y = 0; y < (WORD)Tassellation[1] + 1; y++ )
	{
		for( x = 0; x < (WORD)Tassellation[0] + 1; x++ )
		{
			pMesh->pObject[Object].pVertex[Vertex] = (float)( Step[0] * x - Size[0] / 2 );
			pMesh->pObject[Object].pVertex[Vertex + 2] = (float)( Step[1] * y - Size[1] / 2 );
			Vertex += 3;
		}
	}

	if( ( Size[0] * Size[1] ) >= 0 )
	{
		for( y = 0; y < (WORD)Tassellation[1]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[0]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]	   = x + y * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2] = x + y * ( (WORD)Tassellation[0] + 1 ) + 1;    
		
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]	 = TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1] = FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2] = TRUE;
				Triangle++;

				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]	   = x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 ) + 1;    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]	 = FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1] = TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2] = TRUE;
				Triangle++;
			}
		}
	}
	else
	{
		for( y = 0; y < (WORD)Tassellation[1]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[0]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]	   = x + y * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1] = x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]	 = TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1] = FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2] = TRUE;
				Triangle++;
	
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]	   = x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]	 = TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1] = TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2] = FALSE;
				Triangle++;
			}
		}
	}

	if( updateData )
	{
		pMesh->UpdateConnectivity();
		pMesh->UpdateFacePlanes();
		pMesh->UpdateNormals(); 
	}
}
