// Grid.cpp: implementation of the CGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Grid.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrid::CGrid() { Initialize(); }
CGrid::~CGrid() {}

void CGrid::Initialize()
{
	pRenderMesh = NULL;

	Size[0] = 100.0;
	Size[1] = 100.0;

	Tassellation[0] = 20;
	Tassellation[1] = 20;
}

void CGrid::Delete() {}

void CGrid::SetMesh( CMesh *pMesh ) { pRenderMesh = pMesh; }
void CGrid::DeleteMesh() { if( pRenderMesh ) { pRenderMesh->DeleteObject( 0 ); } }

bool CGrid::GenerateMesh()
{
	DeleteMesh();

	UINT object = pRenderMesh->AddObject();

	pRenderMesh->pObject[object].HasVertexColors = TRUE; 

	Step[0] = Size[0] / Tassellation[0];
	Step[1] = Size[1] / Tassellation[1];

	UINT VertexCount = 0;

	for( int y = 0; y < Tassellation[1] + 1; y++ )
	{
		pRenderMesh->AddVertex( object,   Size[0] / 2,  0, Step[1] * y - Size[1] / 2 );    
		pRenderMesh->AddVertex( object,   -Size[0] / 2, 0, Step[1] * y - Size[1] / 2 );

		pRenderMesh->AddLine( object, VertexCount, VertexCount + 1 );
 
		if( ( Tassellation[1] > 9 ) && ( y == ( Tassellation[1] / 2 ) ) && ( ( Tassellation[1] % 2 ) == 0 ) )
		{ 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 1] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 2] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 3] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 1] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 2] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 3] = 0; 
		}
		else
		{ 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4] = 0.404f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 1] = 0.402f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 2] = 0.4f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 3] = 1.0f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4] = 0.404f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 1] = 0.402f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 2] = 0.4f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 3] = 1.0f; 
		}

		VertexCount += 2;
	}

	for( int x = 0; x < Tassellation[0] + 1; x++ )
	{
		pRenderMesh->AddVertex( object,   Step[0] * x - Size[0] / 2,  0, Size[1] / 2  );    
		pRenderMesh->AddVertex( object,   Step[0] * x - Size[0] / 2,  0, -Size[1] / 2  );    

		pRenderMesh->AddLine( object, VertexCount, VertexCount + 1 );

		if( ( Tassellation[0] > 9 ) && ( x == ( Tassellation[0] / 2 ) ) && ( ( Tassellation[0] % 2 ) == 0 ) )
		{ 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 1] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 2] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 3] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 1] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 2] = 0; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 3] = 0; 
		}
		else
		{ 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4] = 0.404f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 1] = 0.402f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 2] = 0.4f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 1 ) * 4 + 3] = 1.0f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4] = 0.404f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 1] = 0.402f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 2] = 0.4f; 
			pRenderMesh->pObject[0].pColor[( pRenderMesh->pObject[0].VertexCount - 2 ) * 4 + 3] = 1.0f; 
		}

		VertexCount += 2;
	}

	return TRUE;
}
