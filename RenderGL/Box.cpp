         // Box.cpp: implementation of the CBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Box.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBox::CBox() { Initialize(); }
CBox::~CBox() {}

void CBox::Initialize()
{
	pMesh = NULL;

	Size[0] = 100.0f;
	Size[1] = 100.0f;
	Size[2] = 100.0f;

	Tassellation[0] = 10;
	Tassellation[1] = 10;
	Tassellation[2] = 10;
}

void CBox::Delete() {}

void CBox::SetMesh( CMesh *pmesh )
{
	Object = -1;
	pMesh = pmesh; 
}

void CBox::DeleteMesh()
{
	if( !pMesh ) { return; }

	for( n = 0; n < pMesh->ObjectCount; n++ )
	{
		pMesh->DeleteObject( n ); 
	} 

	Object = -1;
}

bool CBox::GenerateMesh()
{
	if( pMesh ) { DeleteMesh(); }

	Object = pMesh->AddObject(); 

	pMesh->pObject[Object].HasVertexColors = FALSE; 
	pMesh->pObject[Object].Color[0] = 0.3f;
	pMesh->pObject[Object].Color[1] = 0.3f;
	pMesh->pObject[Object].Color[2] = 0.3f;

	pMesh->AddVertices( 0,  ( Tassellation[0] + 1 ) * ( Tassellation[1] + 1 ) * 2 + 
							( Tassellation[1] + 1 ) * ( Tassellation[2] + 1 ) * 2 + 
							( Tassellation[2] + 1 ) * ( Tassellation[0] + 1 ) * 2 );
	
	pMesh->AddTriangles( 0, Tassellation[0] * Tassellation[1] * 4 + 
							Tassellation[1] * Tassellation[2] * 4 + 
							Tassellation[2] * Tassellation[0] * 4 );
	return TRUE;
}

void CBox::UpdateMesh( bool updateData )
{
	if( !pMesh || ( Object < 0 ) ) { return; }

	Vertex = 0;
	Triangle = 0;

	Step[0] = Size[0] / Tassellation[0];
	Step[1] = Size[1] / Tassellation[1];
	Step[2] = Size[2] / Tassellation[2];

//Top Side
	for( y = 0; y < (WORD)Tassellation[2] + 1; y++ )
	{
		for( x = 0; x < (WORD)Tassellation[0] + 1; x++ )
		{
			pMesh->pObject[Object].pVertex[Vertex]		= (float)( Step[0] * x - Size[0] / 2 );
			pMesh->pObject[Object].pVertex[Vertex + 1]	= Size[1] / 2;
			pMesh->pObject[Object].pVertex[Vertex + 2]	= (float)( Step[2] * y - Size[2] / 2 );
			Vertex += 3;
		}
	}

	if( ( Size[1] * Size[0] * Size[2] ) >= 0 )
	{
		for( y = 0; y < (WORD)Tassellation[2]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[0]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= x + y * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2] = x + y * ( (WORD)Tassellation[0] + 1 ) + 1;    
		
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;

				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 ) + 1;    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
			}
		}
	}
	else
	{
		for( y = 0; y < (WORD)Tassellation[2]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[0]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]	    = x + y * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1] = x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
	
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]	    = x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2] = x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= FALSE;
				Triangle++;
			}
		}
	}

	VertexCount = Vertex / 3;

//Bottom Side
	for( y = 0; y < (WORD)Tassellation[2] + 1; y++ )
	{
		for( x = 0; x < (WORD)Tassellation[0] + 1; x++ )
		{
			pMesh->pObject[Object].pVertex[Vertex]		= (float)( Step[0] * x - Size[0] / 2 );
			pMesh->pObject[Object].pVertex[Vertex + 1]	= - Size[1] / 2;
			pMesh->pObject[Object].pVertex[Vertex + 2]	= (float)( Step[2] * y - Size[2] / 2 );
			Vertex += 3;
		}
	}

	if( ( Size[1] * Size[0] * Size[2] ) >= 0 )
	{
		for( y = 0; y < (WORD)Tassellation[2]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[0]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 ) + 1;    
		
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;

				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 ) + 1;    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
			}
		}
	}
	else
	{
		for( y = 0; y < (WORD)Tassellation[2]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[0]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
	
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= FALSE;
				Triangle++;
			}
		}
	}

	VertexCount = Vertex / 3;

//Right Side
	for( y = 0; y < (WORD)Tassellation[2] + 1; y++ )
	{
		for( x = 0; x < (WORD)Tassellation[1] + 1; x++ )
		{
			pMesh->pObject[Object].pVertex[Vertex]		= Size[0] / 2;
			pMesh->pObject[Object].pVertex[Vertex + 1]	= (float)( Step[1] * x - Size[1] / 2 );
			pMesh->pObject[Object].pVertex[Vertex + 2]	= (float)( Step[2] * y - Size[2] / 2 );
			Vertex += 3;
		}
	}

	if( ( Size[0] * Size[1] * Size[2] ) >= 0 )
	{
		for( y = 0; y < (WORD)Tassellation[2]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[1]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;    
		
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;

				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 ) + 1;    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
			}
		}
	}
	else
	{
		for( y = 0; y < (WORD)Tassellation[2]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[1]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
	
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= FALSE;
				Triangle++;
			}
		}
	}

	VertexCount = Vertex / 3;

//Left Side
	for( y = 0; y < (WORD)Tassellation[2] + 1; y++ )
	{
		for( x = 0; x < (WORD)Tassellation[1] + 1; x++ )
		{
			pMesh->pObject[Object].pVertex[Vertex]		= - Size[0] / 2;
			pMesh->pObject[Object].pVertex[Vertex + 1]	= (float)( Step[1] * x - Size[1] / 2 );
			pMesh->pObject[Object].pVertex[Vertex + 2]	= (float)( Step[2] * y - Size[2] / 2 );
			Vertex += 3;
		}
	}

	if( ( Size[0] * Size[1] * Size[2] ) >= 0 )
	{
		for( y = 0; y < (WORD)Tassellation[2]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[1]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;    
		
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;

				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 ) + 1;    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
			}
		}
	}
	else
	{
		for( y = 0; y < (WORD)Tassellation[2]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[1]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
	
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= FALSE;
				Triangle++;
			}
		}
	}

	VertexCount = Vertex / 3;

//Front Side
	for( y = 0; y < (WORD)Tassellation[0] + 1; y++ )
	{
		for( x = 0; x < (WORD)Tassellation[1] + 1; x++ )
		{
			pMesh->pObject[Object].pVertex[Vertex]		= (float)( Step[0] * y - Size[0] / 2 );
			pMesh->pObject[Object].pVertex[Vertex + 1]	= (float)( Step[1] * x - Size[1] / 2 );
			pMesh->pObject[Object].pVertex[Vertex + 2]	= Size[2] / 2;
			Vertex += 3;
		}
	}

	if( ( Size[2] * Size[1] * Size[0] ) >= 0 )
	{
		for( y = 0; y < (WORD)Tassellation[0]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[1]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;    
		
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;

				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 ) + 1;    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
			}
		}
	}
	else
	{
		for( y = 0; y < (WORD)Tassellation[0]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[1]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
	
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + y * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[0] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= FALSE;
				Triangle++;
			}
		}
	}

	VertexCount = Vertex / 3;

//Back Side
	for( y = 0; y < (WORD)Tassellation[0] + 1; y++ )
	{
		for( x = 0; x < (WORD)Tassellation[1] + 1; x++ )
		{
			pMesh->pObject[Object].pVertex[Vertex]		= (float)( Step[0] * y - Size[0] / 2 );
			pMesh->pObject[Object].pVertex[Vertex + 1]	= (float)( Step[1] * x - Size[1] / 2 );
			pMesh->pObject[Object].pVertex[Vertex + 2]	= - Size[2] / 2;
			Vertex += 3;
		}
	}

	if( ( Size[2] * Size[1] * Size[0] ) >= 0 )
	{
		for( y = 0; y < (WORD)Tassellation[0]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[1]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;    
		
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;

				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 ) + 1;    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
			}
		}
	}
	else
	{
		for( y = 0; y < (WORD)Tassellation[0]; y++ )
		{
			for( x = 0; x < (WORD)Tassellation[1]; x++ )
			{
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 );
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= FALSE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= TRUE;
				Triangle++;
	
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 2]	= VertexCount + x + y * ( (WORD)Tassellation[1] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3 + 1]	= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 ) + 1;
				pMesh->pObject[Object].pTriangleIndice[Triangle * 3]		= VertexCount + x + ( y + 1 ) * ( (WORD)Tassellation[1] + 1 );    

				pMesh->pObject[Object].pEdgeFlag[Triangle * 3]		= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 1]	= TRUE;
				pMesh->pObject[Object].pEdgeFlag[Triangle * 3 + 2]	= FALSE;
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
