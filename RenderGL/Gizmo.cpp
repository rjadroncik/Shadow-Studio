// Gizmo.cpp: implementation of the CGizmo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gizmo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGizmo::CGizmo()
{
	Scale[0] = 1.0f;
	Scale[1] = 1.0f;
	Scale[2] = 1.0f;
}

CGizmo::~CGizmo()
{

}

void CGizmo::UpdateSelectMesh()
{
	if( Type == GT_MOVE ) { UpdateMoveMesh(); }
	if( Type == GT_SCALE ) { UpdateScaleMesh(); }
}

void CGizmo::UpdateScaleMesh()
{
//Selection Box For X Axis		
	SelectMesh.pObject[0].Color[3] = 0.2f;

	SelectMesh.pObject[0].pVertex[0] = (float)Size * 0.6f;
	SelectMesh.pObject[0].pVertex[1] = (float)Size * 0.08f;
	SelectMesh.pObject[0].pVertex[2] = (float)- Size * 0.08f;

	SelectMesh.pObject[0].pVertex[3] = (float)Size;
	SelectMesh.pObject[0].pVertex[4] = (float)Size * 0.08f;
	SelectMesh.pObject[0].pVertex[5] = (float)- Size * 0.08f;

	SelectMesh.pObject[0].pVertex[6] = (float)Size;
	SelectMesh.pObject[0].pVertex[7] = (float)Size * 0.08f;
	SelectMesh.pObject[0].pVertex[8] = (float)Size * 0.08f;

	SelectMesh.pObject[0].pVertex[9] = (float)Size * 0.6f;
	SelectMesh.pObject[0].pVertex[10] = (float)Size * 0.08f;
	SelectMesh.pObject[0].pVertex[11] = (float)Size * 0.08f;


	SelectMesh.pObject[0].pVertex[12] = (float)Size * 0.6f;
	SelectMesh.pObject[0].pVertex[13] = (float)- Size * 0.08f;
	SelectMesh.pObject[0].pVertex[14] = (float)- Size * 0.08f;

	SelectMesh.pObject[0].pVertex[15] = (float)Size;
	SelectMesh.pObject[0].pVertex[16] = (float)- Size * 0.08f;
	SelectMesh.pObject[0].pVertex[17] = (float)- Size * 0.08f;

	SelectMesh.pObject[0].pVertex[18] = (float)Size;
	SelectMesh.pObject[0].pVertex[19] = (float)- Size * 0.08f;
	SelectMesh.pObject[0].pVertex[20] = (float)Size * 0.08f;

	SelectMesh.pObject[0].pVertex[21] = (float)Size * 0.6f;
	SelectMesh.pObject[0].pVertex[22] = (float)- Size * 0.08f;
	SelectMesh.pObject[0].pVertex[23] = (float)Size * 0.08f;

//Selection Box For Y Axis		
	SelectMesh.pObject[1].Color[3] = 0.6f;

	SelectMesh.pObject[1].pVertex[0] = (float)Size * 0.08f;
	SelectMesh.pObject[1].pVertex[1] = (float)Size * 0.6f;
	SelectMesh.pObject[1].pVertex[2] = (float)- Size * 0.08f;

	SelectMesh.pObject[1].pVertex[3] = (float)Size * 0.08f;
	SelectMesh.pObject[1].pVertex[4] = (float)Size;
	SelectMesh.pObject[1].pVertex[5] = (float)- Size * 0.08f;

	SelectMesh.pObject[1].pVertex[6] = (float)Size * 0.08f;
	SelectMesh.pObject[1].pVertex[7] = (float)Size;
	SelectMesh.pObject[1].pVertex[8] = (float)Size * 0.08f;

	SelectMesh.pObject[1].pVertex[9] = (float)Size * 0.08f;
	SelectMesh.pObject[1].pVertex[10] = (float)Size * 0.6f;
	SelectMesh.pObject[1].pVertex[11] = (float)Size * 0.08f;


	SelectMesh.pObject[1].pVertex[12] = (float)- Size * 0.08f;
	SelectMesh.pObject[1].pVertex[13] = (float)Size * 0.6f;
	SelectMesh.pObject[1].pVertex[14] = (float)- Size * 0.08f;

	SelectMesh.pObject[1].pVertex[15] = (float)- Size * 0.08f;
	SelectMesh.pObject[1].pVertex[16] = (float)Size;
	SelectMesh.pObject[1].pVertex[17] = (float)- Size * 0.08f;

	SelectMesh.pObject[1].pVertex[18] = (float)- Size * 0.08f;
	SelectMesh.pObject[1].pVertex[19] = (float)Size;
	SelectMesh.pObject[1].pVertex[20] = (float)Size * 0.08f;

	SelectMesh.pObject[1].pVertex[21] = (float)- Size * 0.08f;
	SelectMesh.pObject[1].pVertex[22] = (float)Size * 0.6f;
	SelectMesh.pObject[1].pVertex[23] = (float)Size * 0.08f;

//Selection Box For Z Axis		
	SelectMesh.pObject[2].Color[3] = 0.6f;

	SelectMesh.pObject[2].pVertex[0] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[1] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[2] = (float)Size * 0.6f;

	SelectMesh.pObject[2].pVertex[3] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[4] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[5] = (float)Size;

	SelectMesh.pObject[2].pVertex[6] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[7] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[8] = (float)Size;

	SelectMesh.pObject[2].pVertex[9] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[10] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[11] = (float)Size * 0.6f;


	SelectMesh.pObject[2].pVertex[12] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[13] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[14] = (float)Size * 0.6f;

	SelectMesh.pObject[2].pVertex[15] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[16] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[17] = (float)Size;

	SelectMesh.pObject[2].pVertex[18] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[19] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[20] = (float)Size;

	SelectMesh.pObject[2].pVertex[21] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[22] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[23] = (float)Size * 0.6f;

//Selection Triangle For XY Axis		
	SelectMesh.pObject[3].Color[3] = 0.2f;

	SelectMesh.pObject[3].pVertex[0] = (float)Size * 0.5f;
	SelectMesh.pObject[3].pVertex[1] = (float)Size;
	SelectMesh.pObject[3].pVertex[2] = 0;

	SelectMesh.pObject[3].pVertex[3] = (float)Size;
	SelectMesh.pObject[3].pVertex[4] = (float)Size;
	SelectMesh.pObject[3].pVertex[5] = 0;

	SelectMesh.pObject[3].pVertex[6] = (float)Size;
	SelectMesh.pObject[3].pVertex[7] = (float)Size * 0.5f;
	SelectMesh.pObject[3].pVertex[8] = 0;

//Selection Triangle For YZ Axis		
	SelectMesh.pObject[4].Color[3] = 0.2f;

	SelectMesh.pObject[4].pVertex[0] = 0;
	SelectMesh.pObject[4].pVertex[1] = (float)Size;
	SelectMesh.pObject[4].pVertex[2] = (float)Size * 0.5f;

	SelectMesh.pObject[4].pVertex[3] = 0;
	SelectMesh.pObject[4].pVertex[4] = (float)Size;
	SelectMesh.pObject[4].pVertex[5] = (float)Size;

	SelectMesh.pObject[4].pVertex[6] = 0;
	SelectMesh.pObject[4].pVertex[7] = (float)Size * 0.5f;
	SelectMesh.pObject[4].pVertex[8] = (float)Size;

//Selection Triangle For ZX Axis		
	SelectMesh.pObject[5].Color[3] = 0.2f;

	SelectMesh.pObject[5].pVertex[0] = (float)Size * 0.5f;
	SelectMesh.pObject[5].pVertex[1] = 0;
	SelectMesh.pObject[5].pVertex[2] = (float)Size;

	SelectMesh.pObject[5].pVertex[3] = (float)Size;
	SelectMesh.pObject[5].pVertex[4] = 0;
	SelectMesh.pObject[5].pVertex[5] = (float)Size;

	SelectMesh.pObject[5].pVertex[6] = (float)Size;
	SelectMesh.pObject[5].pVertex[7] = 0;
	SelectMesh.pObject[5].pVertex[8] = (float)Size * 0.5f;


//Selection Tetrahedron All Axes		
	SelectMesh.pObject[6].Color[3] = 0.2f;

	SelectMesh.pObject[6].pVertex[0] = 0;
	SelectMesh.pObject[6].pVertex[1] = 0;
	SelectMesh.pObject[6].pVertex[2] = 0;

	SelectMesh.pObject[6].pVertex[3] = (float)Size * 0.5f;
	SelectMesh.pObject[6].pVertex[4] = 0;
	SelectMesh.pObject[6].pVertex[5] = 0;

	SelectMesh.pObject[6].pVertex[6] = 0;
	SelectMesh.pObject[6].pVertex[7] = (float)Size * 0.5f;
	SelectMesh.pObject[6].pVertex[8] = 0;

	SelectMesh.pObject[6].pVertex[9] = 0;
	SelectMesh.pObject[6].pVertex[10] = 0;
	SelectMesh.pObject[6].pVertex[11] = (float)Size * 0.5f;
}

void CGizmo::UpdateMoveMesh()
{
//Selection Box For X Axis		
	SelectMesh.pObject[0].Color[3] = 0.2f;

	SelectMesh.pObject[0].pVertex[0] = (float)Size * 0.2f;
	SelectMesh.pObject[0].pVertex[1] = (float)Size * 0.08f;
	SelectMesh.pObject[0].pVertex[2] = (float)- Size * 0.08f;

	SelectMesh.pObject[0].pVertex[3] = (float)Size;
	SelectMesh.pObject[0].pVertex[4] = (float)Size * 0.08f;
	SelectMesh.pObject[0].pVertex[5] = (float)- Size * 0.08f;

	SelectMesh.pObject[0].pVertex[6] = (float)Size;
	SelectMesh.pObject[0].pVertex[7] = (float)Size * 0.08f;
	SelectMesh.pObject[0].pVertex[8] = (float)Size * 0.08f;

	SelectMesh.pObject[0].pVertex[9] = (float)Size * 0.2f;
	SelectMesh.pObject[0].pVertex[10] = (float)Size * 0.08f;
	SelectMesh.pObject[0].pVertex[11] = (float)Size * 0.08f;


	SelectMesh.pObject[0].pVertex[12] = (float)Size * 0.2f;
	SelectMesh.pObject[0].pVertex[13] = (float)- Size * 0.08f;
	SelectMesh.pObject[0].pVertex[14] = (float)- Size * 0.08f;

	SelectMesh.pObject[0].pVertex[15] = (float)Size;
	SelectMesh.pObject[0].pVertex[16] = (float)- Size * 0.08f;
	SelectMesh.pObject[0].pVertex[17] = (float)- Size * 0.08f;

	SelectMesh.pObject[0].pVertex[18] = (float)Size;
	SelectMesh.pObject[0].pVertex[19] = (float)- Size * 0.08f;
	SelectMesh.pObject[0].pVertex[20] = (float)Size * 0.08f;

	SelectMesh.pObject[0].pVertex[21] = (float)Size * 0.2f;
	SelectMesh.pObject[0].pVertex[22] = (float)- Size * 0.08f;
	SelectMesh.pObject[0].pVertex[23] = (float)Size * 0.08f;

//Selection Box For Y Axis		
	SelectMesh.pObject[1].Color[3] = 0.2f;

	SelectMesh.pObject[1].pVertex[0] = (float)Size * 0.08f;
	SelectMesh.pObject[1].pVertex[1] = (float)Size * 0.2f;
	SelectMesh.pObject[1].pVertex[2] = (float)- Size * 0.08f;

	SelectMesh.pObject[1].pVertex[3] = (float)Size * 0.08f;
	SelectMesh.pObject[1].pVertex[4] = (float)Size;
	SelectMesh.pObject[1].pVertex[5] = (float)- Size * 0.08f;

	SelectMesh.pObject[1].pVertex[6] = (float)Size * 0.08f;
	SelectMesh.pObject[1].pVertex[7] = (float)Size;
	SelectMesh.pObject[1].pVertex[8] = (float)Size * 0.08f;

	SelectMesh.pObject[1].pVertex[9] = (float)Size * 0.08f;
	SelectMesh.pObject[1].pVertex[10] = (float)Size * 0.2f;
	SelectMesh.pObject[1].pVertex[11] = (float)Size * 0.08f;

	SelectMesh.pObject[1].pVertex[12] = (float)- Size * 0.08f;
	SelectMesh.pObject[1].pVertex[13] = (float)Size * 0.2f;
	SelectMesh.pObject[1].pVertex[14] = (float)- Size * 0.08f;

	SelectMesh.pObject[1].pVertex[15] = (float)- Size * 0.08f;
	SelectMesh.pObject[1].pVertex[16] = (float)Size;
	SelectMesh.pObject[1].pVertex[17] = (float)- Size * 0.08f;

	SelectMesh.pObject[1].pVertex[18] = (float)- Size * 0.08f;
	SelectMesh.pObject[1].pVertex[19] = (float)Size;
	SelectMesh.pObject[1].pVertex[20] = (float)Size * 0.08f;

	SelectMesh.pObject[1].pVertex[21] = (float)- Size * 0.08f;
	SelectMesh.pObject[1].pVertex[22] = (float)Size * 0.2f;
	SelectMesh.pObject[1].pVertex[23] = (float)Size * 0.08f;

//Selection Box For Z Axis		
	SelectMesh.pObject[2].Color[3] = 0.2f;

	SelectMesh.pObject[2].pVertex[0] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[1] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[2] = (float)Size * 0.2f;

	SelectMesh.pObject[2].pVertex[3] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[4] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[5] = (float)Size;

	SelectMesh.pObject[2].pVertex[6] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[7] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[8] = (float)Size;

	SelectMesh.pObject[2].pVertex[9] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[10] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[11] = (float)Size * 0.2f;

	SelectMesh.pObject[2].pVertex[12] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[13] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[14] = (float)Size * 0.2f;

	SelectMesh.pObject[2].pVertex[15] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[16] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[17] = (float)Size;

	SelectMesh.pObject[2].pVertex[18] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[19] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[20] = (float)Size;

	SelectMesh.pObject[2].pVertex[21] = (float)Size * 0.08f;
	SelectMesh.pObject[2].pVertex[22] = (float)- Size * 0.08f;
	SelectMesh.pObject[2].pVertex[23] = (float)Size * 0.2f;

//Selection Box For XY Axis		
	SelectMesh.pObject[3].Color[3] = 0.2f;

	SelectMesh.pObject[3].pVertex[0] = (float)Size * 0.5f;
	SelectMesh.pObject[3].pVertex[1] = (float)Size;
	SelectMesh.pObject[3].pVertex[2] = 0;

	SelectMesh.pObject[3].pVertex[3] = (float)Size;
	SelectMesh.pObject[3].pVertex[4] = (float)Size;
	SelectMesh.pObject[3].pVertex[5] = 0;

	SelectMesh.pObject[3].pVertex[6] = (float)Size;
	SelectMesh.pObject[3].pVertex[7] = (float)Size * 0.5f;
	SelectMesh.pObject[3].pVertex[8] = 0;

	SelectMesh.pObject[3].pVertex[9] = (float)Size * 0.5f;
	SelectMesh.pObject[3].pVertex[10] = (float)Size * 0.5f;
	SelectMesh.pObject[3].pVertex[11] = 0;

//Selection Box For YZ Axis		
	SelectMesh.pObject[4].Color[3] = 0.2f;

	SelectMesh.pObject[4].pVertex[0] = 0;
	SelectMesh.pObject[4].pVertex[1] = (float)Size;
	SelectMesh.pObject[4].pVertex[2] = (float)Size * 0.5f;

	SelectMesh.pObject[4].pVertex[3] = 0;
	SelectMesh.pObject[4].pVertex[4] = (float)Size;
	SelectMesh.pObject[4].pVertex[5] = (float)Size;

	SelectMesh.pObject[4].pVertex[6] = 0;
	SelectMesh.pObject[4].pVertex[7] = (float)Size * 0.5f;
	SelectMesh.pObject[4].pVertex[8] = (float)Size;

	SelectMesh.pObject[4].pVertex[9] = 0;
	SelectMesh.pObject[4].pVertex[10] = (float)Size * 0.5f;
	SelectMesh.pObject[4].pVertex[11] = (float)Size * 0.5f;

//Selection Box For ZX Axis		
	SelectMesh.pObject[5].Color[3] = 0.2f;

	SelectMesh.pObject[5].pVertex[0] = (float)Size * 0.5f;
	SelectMesh.pObject[5].pVertex[1] = 0;
	SelectMesh.pObject[5].pVertex[2] = (float)Size;

	SelectMesh.pObject[5].pVertex[3] = (float)Size;
	SelectMesh.pObject[5].pVertex[4] = 0;
	SelectMesh.pObject[5].pVertex[5] = (float)Size;

	SelectMesh.pObject[5].pVertex[6] = (float)Size;
	SelectMesh.pObject[5].pVertex[7] = 0;
	SelectMesh.pObject[5].pVertex[8] = (float)Size * 0.5f;

	SelectMesh.pObject[5].pVertex[9] = (float)Size * 0.5f;
	SelectMesh.pObject[5].pVertex[10] = 0;
	SelectMesh.pObject[5].pVertex[11] = (float)Size * 0.5f;
}

void CGizmo::Resize( float size )
{
	Size = size;
	UpdateSelectMesh();
}

void CGizmo::Create( UINT type, float size, UINT tasselation )
{
	ActiveObject = -1;
	SelectedObject = -1;
	Tasselation = tasselation;
	Type = type;

	pQuadric = gluNewQuadric();
	gluQuadricNormals( pQuadric, GLU_NONE );
	gluQuadricTexture( pQuadric, GL_FALSE );
	gluQuadricDrawStyle( pQuadric, GLU_FILL );

	PlaneXY[0] = 0.0f;
	PlaneXY[1] = 0.0f;
	PlaneXY[2] = 1.0f;
	PlaneXY[3] = 0.0f;

	PlaneYZ[0] = 1.0f;
	PlaneYZ[1] = 0.0f;
	PlaneYZ[2] = 0.0f;
	PlaneYZ[3] = 0.0f;
	
	PlaneZX[0] = 0.0f;
	PlaneZX[1] = 1.0f;
	PlaneZX[2] = 0.0f;
	PlaneZX[3] = 0.0f;

	if( type == GT_MOVE )
	{
		for( UINT n = 0; n < 3; n++ )
		{
			SelectMesh.AddObject();

			SelectMesh.pObject[n].HasTexture = FALSE;
			SelectMesh.pObject[n].HasVertexColors = FALSE;

			SelectMesh.AddVertices( n, 8 );

			SelectMesh.AddTriangle( n, 3, 1, 0 );
			SelectMesh.AddTriangle( n, 3, 2, 1 );

			SelectMesh.AddTriangle( n, 7, 2, 3 );
			SelectMesh.AddTriangle( n, 7, 6, 2 );

			SelectMesh.AddTriangle( n, 6, 1, 2 );
			SelectMesh.AddTriangle( n, 6, 5, 1 );

			SelectMesh.AddTriangle( n, 4, 3, 0 );
			SelectMesh.AddTriangle( n, 4, 7, 3 );

			SelectMesh.AddTriangle( n, 5, 0, 1 );
			SelectMesh.AddTriangle( n, 5, 4, 0 );

			SelectMesh.AddTriangle( n, 4, 6, 7 );
			SelectMesh.AddTriangle( n, 4, 5, 6 );
		}

		for( UINT m = 3; m < 6; m++ )
		{
			SelectMesh.AddObject();

			SelectMesh.pObject[m].HasTexture = FALSE;
			SelectMesh.pObject[m].HasVertexColors = FALSE;

			SelectMesh.AddVertices( m, 4 );

			SelectMesh.AddTriangle( m, 3, 1, 0 );
			SelectMesh.AddTriangle( m, 3, 2, 1 );

			SelectMesh.AddTriangle( m, 0, 1, 3 );
			SelectMesh.AddTriangle( m, 1, 2, 3 );
		}
	}

	if( type == GT_SCALE )
	{
		for( UINT n = 0; n < 3; n++ )
		{
			SelectMesh.AddObject();

			SelectMesh.pObject[n].HasTexture = FALSE;
			SelectMesh.pObject[n].HasVertexColors = FALSE;

			SelectMesh.AddVertices( n, 8 );

			SelectMesh.AddTriangle( n, 3, 1, 0 );
			SelectMesh.AddTriangle( n, 3, 2, 1 );

			SelectMesh.AddTriangle( n, 7, 2, 3 );
			SelectMesh.AddTriangle( n, 7, 6, 2 );

			SelectMesh.AddTriangle( n, 6, 1, 2 );
			SelectMesh.AddTriangle( n, 6, 5, 1 );

			SelectMesh.AddTriangle( n, 4, 3, 0 );
			SelectMesh.AddTriangle( n, 4, 7, 3 );

			SelectMesh.AddTriangle( n, 5, 0, 1 );
			SelectMesh.AddTriangle( n, 5, 4, 0 );

			SelectMesh.AddTriangle( n, 4, 6, 7 );
			SelectMesh.AddTriangle( n, 4, 5, 6 );
		}

		for( UINT m = 3; m < 6; m++ )
		{
			SelectMesh.AddObject();

			SelectMesh.pObject[m].HasTexture = FALSE;
			SelectMesh.pObject[m].HasVertexColors = FALSE;

			SelectMesh.AddVertices( m, 3 );

			SelectMesh.AddTriangle( m, 2, 1, 0 );
			SelectMesh.AddTriangle( m, 0, 1, 2 );
		}

		SelectMesh.AddObject();

		SelectMesh.pObject[6].Color[0] = 1.0f;
		SelectMesh.pObject[6].Color[1] = 1.0f;
		SelectMesh.pObject[6].Color[2] = 0.0f;

		SelectMesh.pObject[6].HasTexture = FALSE;
		SelectMesh.pObject[6].HasVertexColors = FALSE;

		SelectMesh.AddVertices( 6, 4 );

		SelectMesh.AddTriangle( 6, 1, 2, 3 );
		SelectMesh.AddTriangle( 6, 0, 2, 1 );

		SelectMesh.AddTriangle( 6, 0, 3, 2 );
		SelectMesh.AddTriangle( 6, 0, 1, 3 );
	}

	Resize( size );
}

void CGizmo::DrawArrow( float length, UINT color )
{
	glTranslatef( 0, 0, Size * ( 1 - length ) );
	gluCylinder( pQuadric, Size * 0.02f, Size * 0.02f,	Size * ( length - 0.2f ), Tasselation, 1 );

	if( color == COLOR_RED )	{ glColor3f( 1, 0, 0 ); }
	if( color == COLOR_GREEN )	{ glColor3f( 0, 1, 0 ); }
	if( color == COLOR_BLUE )	{ glColor3f( 0, 0, 1 ); }

	glTranslatef( 0, 0, Size * ( length - 0.2f ) );
	gluCylinder( pQuadric, Size * 0.06f, 0,				Size * 0.2f, Tasselation, 1 );
}


void CGizmo::Render( float size )
{
//	SelectMesh.Render( RENDER_VERTEX | RENDER_FACES );

	Size = size;

	glPushAttrib( GL_ENABLE_BIT );

	glDisable( GL_CULL_FACE );
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_FOG );
	glDisable( GL_TEXTURE_2D );


	if( Type == GT_MOVE ) { RenderMove(); }
	if( Type == GT_SCALE ) { RenderScale(); }


	glPopAttrib();
}

void CGizmo::RenderScale()
{

//	for( UINT n = 0; n < 6; n++ ) {	SelectMesh.RenderObject( n ); }

	glScalef( Scale[0], Scale[1], Scale[2] );

//Select Color
	if( ( ActiveObject == 6 ) || ( SelectedObject == 6 ) ) { SelectMesh.pObject[6].Color[3] = 0.5f; }
	else { SelectMesh.pObject[6].Color[3] = 0.1f; }

	glEnable( GL_CULL_FACE );

	SelectMesh.RenderObject( 6 );   

	glDisable( GL_CULL_FACE );

	if( ( ActiveObject == 6 ) || ( SelectedObject == 6 ) ) { glColor4f( 1.0f, 1.0f, 0.0f, 0.8f ); }
	else { glColor4f( 1.0f, 1.0f, 0.0f, 0.2f ); }

	glBegin( GL_LINE_LOOP );

	glVertex3fv( &SelectMesh.pObject[6].pVertex[0] );
	glVertex3fv( &SelectMesh.pObject[6].pVertex[3] );
	glVertex3fv( &SelectMesh.pObject[6].pVertex[6] );

	glEnd();

	glDisable( GL_BLEND );

	glBegin( GL_LINES );

	glColor3f( 1.0f, 0.0f, 0.0f );
	glVertex3fv( &SelectMesh.pObject[6].pVertex[0] );
	glVertex3fv( &SelectMesh.pObject[6].pVertex[3] );
	glColor3f( 0.0f, 1.0f, 0.0f );
	glVertex3fv( &SelectMesh.pObject[6].pVertex[0] );
	glVertex3fv( &SelectMesh.pObject[6].pVertex[6] );
	glColor3f( 0.0f, 0.0f, 1.0f );
	glVertex3fv( &SelectMesh.pObject[6].pVertex[0] );
	glVertex3fv( &SelectMesh.pObject[6].pVertex[9] );

	glEnd();



//Draw X Axis Arrow
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

	glRotatef( 90, 0, 1, 0 );

//Select Color
	if( ( ActiveObject == 0 ) || ( SelectedObject == 0 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 1, 0, 0 ); }

	DrawArrow( 0.5f, COLOR_RED );

	glPopMatrix();

//Draw Y Axis Arrow
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

	glRotatef( -90, 1, 0, 0 );

//Select Color
	if( ( ActiveObject == 1 ) || ( SelectedObject == 1 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 0, 1, 0 ); }

	DrawArrow( 0.5f, COLOR_GREEN );

	glPopMatrix();

//Draw Z Axis Arrow
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

//	glRotatef( -90, 1, 0, 0 );

//Select Color
	if( ( ActiveObject == 2 ) || ( SelectedObject == 2 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 0, 0, 1 ); }

	DrawArrow( 0.5f, COLOR_BLUE );

	glPopMatrix();


//Draw XY Axis Plane
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

//	glRotatef( 90, 0, 1, 0 );

//Select Color
	if( ( ActiveObject == 3 ) || ( SelectedObject == 3 ) ) { glColor4f( 1.0f, 1.0f, 0, 0.5f ); }
	else { glColor4f( 1.0f, 1.0f, 0, 0.1f ); }

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_DST_ALPHA );

	glBegin( GL_TRIANGLES );

	glVertex3f( Size * 0.5f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 0.5f,	0 );

	glEnd();

	glDisable( GL_BLEND );

	glBegin( GL_LINES );

	glColor3f( 0, 1, 0 ); 
	glVertex3f( Size * 0.45f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );

	glColor3f( 1, 0, 0 ); 
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 0.45f, 0 );

	glEnd();

	glPopMatrix();

//Draw YZ Axis Plane
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

	glRotatef( -90, 0, 1, 0 );

//Select Color
	if( ( ActiveObject == 4 ) || ( SelectedObject == 4 ) ) { glColor4f( 1.0f, 1.0f, 0, 0.5f ); }
	else { glColor4f( 1.0f, 1.0f, 0, 0.1f ); }

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_DST_ALPHA );

	glBegin( GL_TRIANGLES );

	glVertex3f( Size * 0.5f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 0.5f,	0 );

	glEnd();

	glDisable( GL_BLEND );

	glBegin( GL_LINES );

	glColor3f( 0, 1, 0 ); 
	glVertex3f( Size * 0.45f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );

	glColor3f( 0, 0, 1 ); 
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 0.45f, 0 );

	glEnd();

	glPopMatrix();


//Draw ZX Axis Plane
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

	glRotatef( 90, 1, 0, 0 );

//Select Color
	if( ( ActiveObject == 5 ) || ( SelectedObject == 5 ) ) { glColor4f( 1.0f, 1.0f, 0, 0.5f ); }
	else { glColor4f( 1.0f, 1.0f, 0, 0.1f ); }

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_DST_ALPHA );

	glBegin( GL_TRIANGLES );

	glVertex3f( Size * 0.5f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 0.5f,	0 );

	glEnd();

	glDisable( GL_BLEND );

	glBegin( GL_LINES );

	glColor3f( 0, 0, 1 ); 
	glVertex3f( Size * 0.45f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );

	glColor3f( 1, 0, 0 ); 
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 0.45f, 0 );

	glEnd();

	glPopMatrix();

	glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );
	glGetDoublev( GL_PROJECTION_MATRIX, ProjMatrix );
	glGetIntegerv( GL_VIEWPORT, Viewport );

//Calculate Corner Vectors
	if( gluProject(	Size, 0, 0,	ModelMatrix, ProjMatrix, Viewport,
					&TempVectord3a[0], &TempVectord3a[1], &TempVectord3a[2] ) )
	{
	}

	if( gluProject(	0, Size, 0,	ModelMatrix, ProjMatrix, Viewport,
					&TempVectord3b[0], &TempVectord3b[1], &TempVectord3b[2] ) )
	{
	}

	if( gluProject(	0, 0, Size,	ModelMatrix, ProjMatrix, Viewport,
					&TempVectord3c[0], &TempVectord3c[1], &TempVectord3c[2] ) )
	{
	}

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	glOrtho( 0, Viewport[2], 0, Viewport[3], -1000, 1000 );

	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();
	glLoadIdentity();

	glBegin( GL_LINES ); 

	if( ( ActiveObject == 0 ) || ( ActiveObject == 3 ) || ( ActiveObject == 5 ) || 
		( SelectedObject == 0 ) || ( SelectedObject == 3 ) || ( SelectedObject == 5 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 1, 0, 0 ); }

//X
	glVertex2d( TempVectord3a[0] + 1, TempVectord3a[1] + 1 );
	glVertex2d( TempVectord3a[0] + 6, TempVectord3a[1] + 8 );

	glVertex2d( TempVectord3a[0] + 6, TempVectord3a[1] + 1 );
	glVertex2d( TempVectord3a[0] + 1, TempVectord3a[1] + 8 );

	if( ( ActiveObject == 1 ) || ( ActiveObject == 3 ) || ( ActiveObject == 4 ) || 
		( SelectedObject == 1 ) || ( SelectedObject == 3 ) || ( SelectedObject == 4 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 0, 1, 0 ); }

//Y
	glVertex2d( TempVectord3b[0] + 2, TempVectord3b[1] + 1 );
	glVertex2d( TempVectord3b[0] + 6, TempVectord3b[1] + 8 );

	glVertex2d( TempVectord3b[0] + 3, TempVectord3b[1] + 3 );
	glVertex2d( TempVectord3b[0] + 1, TempVectord3b[1] + 8 );

	if( ( ActiveObject == 2 ) || ( ActiveObject == 5 ) || ( ActiveObject == 4 ) || 
		( SelectedObject == 2 ) || ( SelectedObject == 5 ) || ( SelectedObject == 4 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 0, 0, 1 ); }

//Z
	glVertex2d( TempVectord3c[0] + 1, TempVectord3c[1] + 1 );
	glVertex2d( TempVectord3c[0] + 6, TempVectord3c[1] + 7 );

	glVertex2d( TempVectord3c[0] + 1, TempVectord3c[1] + 7 );
	glVertex2d( TempVectord3c[0] + 6, TempVectord3c[1] + 7 );

	glVertex2d( TempVectord3c[0] + 1, TempVectord3c[1] + 1 );
	glVertex2d( TempVectord3c[0] + 6, TempVectord3c[1] + 1 );

	glEnd();

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
}

void CGizmo::RenderMove()
{
//Draw X Axis Arrow
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

	glRotatef( 90, 0, 1, 0 );

//Select Color
	if( ( ActiveObject == 0 ) || ( SelectedObject == 0 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 1, 0, 0 ); }

	DrawArrow( 0.8f, COLOR_RED );

	glPopMatrix();

//Draw Y Axis Arrow
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

	glRotatef( -90, 1, 0, 0 );

//Select Color
	if( ( ActiveObject == 1 ) || ( SelectedObject == 1 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 0, 1, 0 ); }

	DrawArrow( 0.8f, COLOR_GREEN );

	glPopMatrix();

//Draw Z Axis Arrow
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

//	glRotatef( -90, 1, 0, 0 );

//Select Color
	if( ( ActiveObject == 2 ) || ( SelectedObject == 2 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 0, 0, 1 ); }

	DrawArrow( 0.8f, COLOR_BLUE );

	glPopMatrix();


//Draw XY Axis Plane
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

//	glRotatef( 90, 0, 1, 0 );

//Select Color
	if( ( ActiveObject == 3 ) || ( SelectedObject == 3 ) ) { glColor4f( 1.0f, 1.0f, 0, 0.5f ); }
	else { glColor4f( 1.0f, 1.0f, 0, 0.1f ); }

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_DST_ALPHA );

	glBegin( GL_QUADS );

	glVertex3f( Size * 0.5f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 0.5f,	0 );
	glVertex3f( Size * 0.5f,	Size * 0.5f,	0 );

	glEnd();

	glDisable( GL_BLEND );

	glBegin( GL_LINES );

	glColor3f( 0, 1, 0 ); 
	glVertex3f( Size * 0.45f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );

	glColor3f( 1, 0, 0 ); 
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 0.45f, 0 );

	glEnd();

	glPopMatrix();

//Draw YZ Axis Plane
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

	glRotatef( -90, 0, 1, 0 );

//Select Color
	if( ( ActiveObject == 4 ) || ( SelectedObject == 4 ) ) { glColor4f( 1.0f, 1.0f, 0, 0.5f ); }
	else { glColor4f( 1.0f, 1.0f, 0, 0.1f ); }

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_DST_ALPHA );

	glBegin( GL_QUADS );

	glVertex3f( Size * 0.5f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 0.5f,	0 );
	glVertex3f( Size * 0.5f,	Size * 0.5f,	0 );

	glEnd();

	glDisable( GL_BLEND );

	glBegin( GL_LINES );

	glColor3f( 0, 1, 0 ); 
	glVertex3f( Size * 0.45f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );

	glColor3f( 0, 0, 1 ); 
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 0.45f, 0 );

	glEnd();

	glPopMatrix();


//Draw ZX Axis Plane
	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

	glRotatef( 90, 1, 0, 0 );

//Select Color
	if( ( ActiveObject == 5 ) || ( SelectedObject == 5 ) ) { glColor4f( 1.0f, 1.0f, 0, 0.5f ); }
	else { glColor4f( 1.0f, 1.0f, 0, 0.1f ); }

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_DST_ALPHA );

	glBegin( GL_QUADS );

	glVertex3f( Size * 0.5f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 1.0f,	0 );
	glVertex3f( Size * 1.0f,	Size * 0.5f,	0 );
	glVertex3f( Size * 0.5f,	Size * 0.5f,	0 );

	glEnd();

	glDisable( GL_BLEND );

	glBegin( GL_LINES );

	glColor3f( 0, 0, 1 ); 
	glVertex3f( Size * 0.45f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );

	glColor3f( 1, 0, 0 ); 
	glVertex3f( Size * 1.0f, Size * 1.0f, 0 );
	glVertex3f( Size * 1.0f, Size * 0.45f, 0 );

	glEnd();

	glPopMatrix();

	glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );
	glGetDoublev( GL_PROJECTION_MATRIX, ProjMatrix );
	glGetIntegerv( GL_VIEWPORT, Viewport );

//Calculate Corner Vectors
	if( gluProject(	Size, 0, 0,	ModelMatrix, ProjMatrix, Viewport,
					&TempVectord3a[0], &TempVectord3a[1], &TempVectord3a[2] ) )
	{
	}

	if( gluProject(	0, Size, 0,	ModelMatrix, ProjMatrix, Viewport,
					&TempVectord3b[0], &TempVectord3b[1], &TempVectord3b[2] ) )
	{
	}

	if( gluProject(	0, 0, Size,	ModelMatrix, ProjMatrix, Viewport,
					&TempVectord3c[0], &TempVectord3c[1], &TempVectord3c[2] ) )
	{
	}

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	glOrtho( 0, Viewport[2], 0, Viewport[3], -1000, 1000 );

	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();
	glLoadIdentity();

	glBegin( GL_LINES ); 

	if( ( ActiveObject == 0 ) || ( ActiveObject == 3 ) || ( ActiveObject == 5 ) || 
		( SelectedObject == 0 ) || ( SelectedObject == 3 ) || ( SelectedObject == 5 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 1, 0, 0 ); }

//X
	glVertex2d( TempVectord3a[0] + 1, TempVectord3a[1] + 1 );
	glVertex2d( TempVectord3a[0] + 6, TempVectord3a[1] + 8 );

	glVertex2d( TempVectord3a[0] + 6, TempVectord3a[1] + 1 );
	glVertex2d( TempVectord3a[0] + 1, TempVectord3a[1] + 8 );

	if( ( ActiveObject == 1 ) || ( ActiveObject == 3 ) || ( ActiveObject == 4 ) || 
		( SelectedObject == 1 ) || ( SelectedObject == 3 ) || ( SelectedObject == 4 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 0, 1, 0 ); }

//Y
	glVertex2d( TempVectord3b[0] + 2, TempVectord3b[1] + 1 );
	glVertex2d( TempVectord3b[0] + 6, TempVectord3b[1] + 8 );

	glVertex2d( TempVectord3b[0] + 3, TempVectord3b[1] + 3 );
	glVertex2d( TempVectord3b[0] + 1, TempVectord3b[1] + 8 );

	if( ( ActiveObject == 2 ) || ( ActiveObject == 5 ) || ( ActiveObject == 4 ) || 
		( SelectedObject == 2 ) || ( SelectedObject == 5 ) || ( SelectedObject == 4 ) ) { glColor3f( 1, 1, 0 ); }
	else { glColor3f( 0, 0, 1 ); }

//Z
	glVertex2d( TempVectord3c[0] + 1, TempVectord3c[1] + 1 );
	glVertex2d( TempVectord3c[0] + 6, TempVectord3c[1] + 7 );

	glVertex2d( TempVectord3c[0] + 1, TempVectord3c[1] + 7 );
	glVertex2d( TempVectord3c[0] + 6, TempVectord3c[1] + 7 );

	glVertex2d( TempVectord3c[0] + 1, TempVectord3c[1] + 1 );
	glVertex2d( TempVectord3c[0] + 6, TempVectord3c[1] + 1 );

	glEnd();

	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
}