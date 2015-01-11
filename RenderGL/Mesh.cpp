// Mesh.cpp: implementation of the CMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mesh.h"
#include "../Math.h"
#include "../ImgLoad.h"
#include "Assert.h"

extern CMath Math;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMesh::CMesh()
{
	pObject = NULL;
	ObjectCount = 0;

	TextureCount = 0;
//	RenderMode = RENDER_FACES | RENDER_TRIANGLES;
}

CMesh::~CMesh()
{
	for( WORD s = 0; s < ObjectCount; s++ )
	{
		free( pObject[s].pVertex );
		free( pObject[s].pTriangle );
	}

	free( pObject );
}

void CMesh::Initialize()
{
	pObject = NULL;
	ObjectCount = 0;

	TextureCount = 0;
	//RenderMode = RENDER_FACES | RENDER_TRIANGLES;
}

void CMesh::Delete()
{
	if( pObject )
	{
		for( WORD n = 0; n < ObjectCount; n++ )
		{
			if( pObject[n].pVertex )			{ free( pObject[n].pVertex ); }
			if( pObject[n].pNormal )			{ free( pObject[n].pNormal ); }
			if( pObject[n].pTexCoord )			{ free( pObject[n].pTexCoord ); }
			if( pObject[n].pColor )				{ free( pObject[n].pColor ); }

			if( pObject[n].pTriangle )			{ free( pObject[n].pTriangle ); }
			if( pObject[n].pTriangleIndice )			{ free( pObject[n].pTriangleIndice ); }
			if( pObject[n].pEdgeFlag )			{ free( pObject[n].pEdgeFlag ); }

			if( pObject[n].pLineIndice )			{ free( pObject[n].pLineIndice ); }

/*			if( pObject[n].pSmoothGroup )
			{
				for( WORD m = 0; m < pObject[n].SmoothGroupCount; m++ )
				{
					free( pObject[n].pSmoothGroup[m].pTriangle );
				}
			}*/
		}
		
		free( pObject );
	}

	pObject = NULL;
	ObjectCount = 0;
}

void CMesh::Reset()
{
	for( WORD s = 0; s < ObjectCount; s++ )
	{
		free( pObject[s].pVertex );
		free( pObject[s].pTriangle );
	}

	free( pObject );
}

WORD CMesh::AddObject()
{
	ObjectCount++;

//Safely Realloc Object Array
	tObject *pTempObject = (tObject*)realloc( pObject, sizeof( tObject ) * ObjectCount ); 

//Check for Success
	if( !pTempObject )
	{
	//Update Object Count
		ObjectCount--;
		
		MessageBox( NULL, "Out of Memory! -> Not Creating object.", "CMesh::AddObject()", MB_ICONERROR | MB_OK ); 
		return -1; 
	}

//Copy Pointer
	pObject = pTempObject; 

//Initialize Object Material
	pObject[ObjectCount - 1].Material.Ambient[0] = 0.2f;
	pObject[ObjectCount - 1].Material.Ambient[1] = 0.2f;
	pObject[ObjectCount - 1].Material.Ambient[2] = 0.2f;
	pObject[ObjectCount - 1].Material.Ambient[3] = 1.0f;

	pObject[ObjectCount - 1].Material.Diffuse[0] = 0.8f;
	pObject[ObjectCount - 1].Material.Diffuse[1] = 0.8f;
	pObject[ObjectCount - 1].Material.Diffuse[2] = 0.8f;
	pObject[ObjectCount - 1].Material.Diffuse[3] = 1.0f;

	pObject[ObjectCount - 1].Material.Specular[0] = 0.0f;
	pObject[ObjectCount - 1].Material.Specular[1] = 0.0f;
	pObject[ObjectCount - 1].Material.Specular[2] = 0.0f;
	pObject[ObjectCount - 1].Material.Specular[3] = 1.0f;

	pObject[ObjectCount - 1].Material.Emission[0] = 0.0f;
	pObject[ObjectCount - 1].Material.Emission[1] = 0.0f;
	pObject[ObjectCount - 1].Material.Emission[2] = 0.0f;
	pObject[ObjectCount - 1].Material.Emission[3] = 1.0f;

	pObject[ObjectCount - 1].Material.Shininess = 0.0f;

	pObject[ObjectCount - 1].Color[0] = 1.0f;
	pObject[ObjectCount - 1].Color[1] = 1.0f;
	pObject[ObjectCount - 1].Color[2] = 1.0f;
	pObject[ObjectCount - 1].Color[3] = 1.0f;

//Initialize Object Transformations
/*	pObject[ObjectCount - 1].Scale[0] = 1.0f;
	pObject[ObjectCount - 1].Scale[1] = 1.0f;
	pObject[ObjectCount - 1].Scale[2] = 1.0f;
*/
	pObject[ObjectCount - 1].Rotation[0] = 0.0f;
	pObject[ObjectCount - 1].Rotation[1] = 0.0f;
	pObject[ObjectCount - 1].Rotation[2] = 0.0f;

	pObject[ObjectCount - 1].Position[0] = 0.0f;
	pObject[ObjectCount - 1].Position[1] = 0.0f;
	pObject[ObjectCount - 1].Position[2] = 0.0f;

/*	pObject[ObjectCount - 1].Direction[0] = 0.0f;
	pObject[ObjectCount - 1].Direction[1] = 0.0f;
	pObject[ObjectCount - 1].Direction[2] = -1.0f;*/

//Initialize Vertex Data
	pObject[ObjectCount - 1].VertexCount = 0;

	pObject[ObjectCount - 1].pVertex = NULL;
	pObject[ObjectCount - 1].pNormal = NULL;
	pObject[ObjectCount - 1].pColor = NULL;
	pObject[ObjectCount - 1].pTexCoord = NULL;

	pObject[ObjectCount - 1].HasVertexColors = FALSE;

//Initialize Polygon Data
	pObject[ObjectCount - 1].pTriangle = NULL;
	pObject[ObjectCount - 1].pTriangleIndice = NULL;
	pObject[ObjectCount - 1].pEdgeFlag = NULL;
	pObject[ObjectCount - 1].TriangleCount = 0;

	pObject[ObjectCount - 1].HasTriangles = FALSE;

//Initialize Line Data
	pObject[ObjectCount - 1].pLineIndice = NULL;
	pObject[ObjectCount - 1].LineCount = 0;
	pObject[ObjectCount - 1].HasLines = FALSE;

//Initialize Smoothing
/*	pObject[ObjectCount - 1].pSmoothGroup = (tSmoothGroup*)malloc( sizeof( tSmoothGroup ) );
	pObject[ObjectCount - 1].SmoothGroupCount = 1;

	pObject[ObjectCount - 1].pSmoothGroup[0].pTriangle = NULL;
	pObject[ObjectCount - 1].pSmoothGroup[0].TriangleCount = 0;
*/
	pObject[ObjectCount - 1].Visible = TRUE;
	pObject[ObjectCount - 1].HasTexture = FALSE;

	pObject[ObjectCount - 1].TextureID = 0;
	strcpy( pObject[ObjectCount - 1].TextureFile, "" );
//	memset( pObject[ObjectCount - 1].TextureFile, ' ', 96 );


	return ObjectCount - 1;
}


bool CMesh::DeleteObject( WORD index )
{
	if( ObjectCount == 0 ) return FALSE;

	ObjectCount--;

	if( ObjectCount == 0 )
	{
		free( pObject );
		pObject = NULL;
	}
	else
	{
		pObject[index] = pObject[ObjectCount]; 
		pObject = (tObject*)realloc( pObject, sizeof( tObject ) * ObjectCount ); 
	}

	return TRUE;
}


void CMesh::SetObjectRotation( WORD index, float x, float y, float z )
{
	pObject[index].Rotation[0] = x;
	pObject[index].Rotation[1] = y;
	pObject[index].Rotation[2] = z;

//	Math.RotationToDirection3( &pObject[index].Rotation, &pObject[index].Direction );
}

/*
void CMesh::SetObjectDirection( WORD index, float x, float y, float z )
{
	pObject[index].Direction[0] = x;
	pObject[index].Direction[1] = y;
	pObject[index].Direction[2] = z;

	Math.DirectionToRotation3( &pObject[index].Direction, &pObject[index].Rotation );
}*/

void CMesh::SetObjectTexture( WORD index, LPSTR textureFile )
{
//Delete Odl Texture, if There is One
	if( pObject[index].HasTexture )
	{
		glDeleteTextures( 1, &pObject[index].TextureID );
		pObject[index].TextureID = 0;
		pObject[index].HasTexture = FALSE;
		memset( pObject[index].TextureFile, ' ', 96 );
	}

//Load the Image and Create an OpenGL Texture + Store its ID 
	tImageInfo *pImage = NULL;	

	if( strstr( textureFile, ".jpg" ) || strstr( textureFile, ".JPG" ) ) { LoadJPG( textureFile, pImage ); }
	if( strstr( textureFile, ".tga" ) || strstr( textureFile, ".TGA" ) ) { LoadTGA( textureFile, pImage ); }

	if( pImage )
	{
		glGenTextures( 1, &pObject[index].TextureID );
		glBindTexture( GL_TEXTURE_2D, pObject[index].TextureID );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); //GL_LINEAR_MIPMAP_NEAREST
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );		// nastavuje za textury sa v smere u (vodorovnom) neopakuju
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );		// nastavuje za textury sa v smere v (zvislom) neopakuju

		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB8, pImage->Width, pImage->Height, GL_RGB, GL_UNSIGNED_BYTE, pImage->pData );
	}
	else { MessageBox( NULL, "OpenGL couldn't Create a Texture !", textureFile, MB_ICONERROR ); }

//Vacuum Cleaning
	if( pImage )
	{
		if( pImage->pData ) { free( pImage->pData ); }
		free( pImage );								
	}

//Update Object Information
	strcpy( pObject[index].TextureFile, textureFile );
	pObject[index].HasTexture = TRUE;
}


void CMesh::AddVertices( WORD object, WORD count )
{
	pObject[object].VertexCount += count;

//Safely Realloc Arrays
	GLfloat *pTempVertex =		(GLfloat*)realloc( pObject[object].pVertex,		sizeof( GLfloat ) * 3 * pObject[object].VertexCount ); 
	GLfloat *pTempNormal =		(GLfloat*)realloc( pObject[object].pNormal,		sizeof( GLfloat ) * 3 * pObject[object].VertexCount ); 
	GLfloat *pTempTexCoord =	(GLfloat*)realloc( pObject[object].pTexCoord,	sizeof( GLfloat ) * 2 * pObject[object].VertexCount ); 

	GLfloat *pTempColor;
	if( pObject[object].HasVertexColors ) { pTempColor = (GLfloat*)realloc( pObject[object].pColor, sizeof( GLfloat )* 4 * pObject[object].VertexCount ); }

//Check for Success
	if( ( !pTempVertex || !pTempNormal || !pTempTexCoord ) || ( pObject[object].HasVertexColors && !pTempColor ) )
	{
	//Update Triangle Count	
		pObject[object].VertexCount -= count;
		
		MessageBox( NULL, "Out of Memory! -> Not Adding Vertices.", "CMesh::AddVertices()", MB_ICONERROR | MB_OK );
		return; 
	}

//Copy Pointers
	pObject[object].pVertex = pTempVertex; 
	pObject[object].pNormal = pTempNormal;
	pObject[object].pTexCoord = pTempTexCoord;

	if( pObject[object].HasVertexColors ) { pObject[object].pColor = pTempColor; }

//Initialize Data
	for( WORD n = pObject[object].VertexCount - count; n < pObject[object].VertexCount; n++ )
	{
		pObject[object].pVertex[3 * n]	   = 0.0f;
		pObject[object].pVertex[3 * n + 1] = 0.0f;
		pObject[object].pVertex[3 * n + 2] = 0.0f;

		pObject[object].pNormal[3 * n]	   = 0.0f;
		pObject[object].pNormal[3 * n + 1] = 1.0f;
		pObject[object].pNormal[3 * n + 2] = 0.0f;

		pObject[object].pTexCoord[2 * n]	 = 0.0f;
		pObject[object].pTexCoord[2 * n + 1] = 0.0f;

		if( pObject[object].HasVertexColors )
		{
			pObject[object].pColor[n * 4]	  = 1.0f;
			pObject[object].pColor[n * 4 + 1] = 1.0f;
			pObject[object].pColor[n * 4 + 2] = 1.0f;
			pObject[object].pColor[n * 4 + 3] = 1.0f;
		}
	}
}


void CMesh::AddVertex( WORD object, float x, float y, float z )
{
	pObject[object].VertexCount++;

//Safely Realloc Arrays
	GLfloat *pTempVertex =		(GLfloat*)realloc( pObject[object].pVertex,		sizeof( GLfloat ) * 3 * pObject[object].VertexCount ); 
	GLfloat *pTempNormal =		(GLfloat*)realloc( pObject[object].pNormal,		sizeof( GLfloat ) * 3 * pObject[object].VertexCount ); 
	GLfloat *pTempTexCoord =	(GLfloat*)realloc( pObject[object].pTexCoord,	sizeof( GLfloat ) * 2 * pObject[object].VertexCount ); 

	GLfloat *pTempColor;
	if( pObject[object].HasVertexColors ) { pTempColor = (GLfloat*)realloc( pObject[object].pColor, sizeof( GLfloat )* 4 * pObject[object].VertexCount ); }

//Check for Success
	if( ( !pTempVertex || !pTempNormal || !pTempTexCoord ) || ( pObject[object].HasVertexColors && !pTempColor ) )
	{
	//Update Triangle Count	
		pObject[object].VertexCount--;
		
		MessageBox( NULL, "Out of Memory! -> Not Adding Vertex.", "CMesh::AddVertex()", MB_ICONERROR | MB_OK );
		return; 
	}

//Copy Pointers
	pObject[object].pVertex = pTempVertex; 
	pObject[object].pNormal = pTempNormal;
	pObject[object].pTexCoord = pTempTexCoord;

	if( pObject[object].HasVertexColors ) { pObject[object].pColor = pTempColor; }

//Initialize Data
	pObject[object].pVertex[( pObject[object].VertexCount - 1 ) * 3]	 = x;
	pObject[object].pVertex[( pObject[object].VertexCount - 1 ) * 3 + 1] = y;
	pObject[object].pVertex[( pObject[object].VertexCount - 1 ) * 3 + 2] = z;

	pObject[object].pNormal[( pObject[object].VertexCount - 1 ) * 3]	 = 0.0f;
	pObject[object].pNormal[( pObject[object].VertexCount - 1 ) * 3 + 1] = 1.0f;
	pObject[object].pNormal[( pObject[object].VertexCount - 1 ) * 3 + 2] = 0.0f;

	pObject[object].pTexCoord[( pObject[object].VertexCount - 1 ) * 2]	   = 0.0f;
	pObject[object].pTexCoord[( pObject[object].VertexCount - 1 ) * 2 + 1] = 0.0f;

	if( pObject[object].HasVertexColors )
	{
		pObject[object].pColor = (GLfloat*)realloc( pObject[object].pColor, sizeof( GLfloat ) * 4 * pObject[object].VertexCount ); 

		pObject[object].pColor[( pObject[object].VertexCount - 1 ) * 4]		= 1.0f;
		pObject[object].pColor[( pObject[object].VertexCount - 1 ) * 4 + 1] = 1.0f;
		pObject[object].pColor[( pObject[object].VertexCount - 1 ) * 4 + 2] = 1.0f;
		pObject[object].pColor[( pObject[object].VertexCount - 1 ) * 4 + 3] = 1.0f;
	}
}

//!!!! WARNING : Deleting a Vertex Causes Confusion + Errors -> Need To Fix !!!!
void CMesh::DeleteVertex( WORD object, WORD vertex )
{
	if( pObject[object].VertexCount == 0 ) { return; }

	if( pObject[object].VertexCount == 1 ) 
	{
		pObject[object].VertexCount--;

	//Delete Vertices
		free( pObject[object].pVertex ); 
		pObject[object].pVertex = NULL;

		free( pObject[object].pNormal ); 
		pObject[object].pNormal = NULL;

		free( pObject[object].pTexCoord ); 
		pObject[object].pTexCoord = NULL;

		free( pObject[object].pColor ); 
		pObject[object].pColor = NULL;

	//Delete Lines
		free( pObject[object].pLineIndice ); 
		pObject[object].pLineIndice = NULL;
		pObject[object].HasLines = FALSE;

	//Delete Triangles
		free( pObject[object].pEdgeFlag ); 
		pObject[object].pEdgeFlag = NULL;

		free( pObject[object].pTriangleIndice ); 
		pObject[object].pTriangleIndice = NULL;

		free( pObject[object].pTriangle ); 
		pObject[object].pTriangle = NULL;
		pObject[object].HasTriangles = FALSE;
	
	}	
	else
	{
		WORD n;

		if( pObject[object].HasTriangles )
		{
		//Delete Now Invalid Triangles
 			for( n = 0; n < pObject[object].TriangleCount; n++ )
			{
				if( ( pObject[object].pTriangleIndice[n * 3] == vertex ) || 
					( pObject[object].pTriangleIndice[n * 3 + 1] == vertex ) ||
					( pObject[object].pTriangleIndice[n * 3 + 2] == vertex ) ) { DeleteTriangle( object, n ); }
			}

		//Update Vertex Indexes in Triangles, that use the Moved Vertex
 			for( n = 0; n < pObject[object].TriangleCount; n++ )
			{
				if( ( pObject[object].pTriangleIndice[n * 3]	 == ( pObject[object].VertexCount - 1 ) ) ) { pObject[object].pTriangleIndice[n * 3]	 = vertex; } 
				if( ( pObject[object].pTriangleIndice[n * 3 + 1] == ( pObject[object].VertexCount - 1 ) ) ) { pObject[object].pTriangleIndice[n * 3 + 1] = vertex; } 
				if( ( pObject[object].pTriangleIndice[n * 3 + 2] == ( pObject[object].VertexCount - 1 ) ) ) { pObject[object].pTriangleIndice[n * 3 + 2] = vertex; } 
			}
		}

		if( pObject[object].HasLines )
		{
		//Delete Now Invalid Lines
 			for( n = 0; n < pObject[object].LineCount; n++ )
			{
				if( ( pObject[object].pLineIndice[n * 2] == vertex ) || 
					( pObject[object].pLineIndice[n * 2 + 1] == vertex ) ) { DeleteLine( object, n ); }
			}

		//Update Vertex Indexes in Lines, that use the Moved Vertex
 			for( n = 0; n < pObject[object].LineCount; n++ )
			{
				if( ( pObject[object].pLineIndice[n * 2] == ( pObject[object].VertexCount - 1 ) ) ) { pObject[object].pLineIndice[n * 2]		 = vertex; } 
				if( ( pObject[object].pLineIndice[n * 2 + 1] == ( pObject[object].VertexCount - 1 ) ) ) { pObject[object].pLineIndice[n * 2 + 1] = vertex; } 
			}
		}

		pObject[object].VertexCount--;
		pObject[object].pVertex[vertex * 3]		= pObject[object].pVertex[pObject[object].VertexCount * 3];
		pObject[object].pVertex[vertex * 3 + 1] = pObject[object].pVertex[pObject[object].VertexCount * 3 + 1];
		pObject[object].pVertex[vertex * 3 + 2] = pObject[object].pVertex[pObject[object].VertexCount * 3 + 2];

		pObject[object].pVertex = (GLfloat*)realloc( pObject[object].pVertex, sizeof( GLfloat ) * 3 * pObject[object].VertexCount ); 

		if( pObject[object].HasVertexColors ) { pObject[object].pColor = (GLfloat*)realloc( pObject[object].pColor, sizeof( GLfloat ) * 4 * pObject[object].VertexCount ); }
	}
}

void CMesh::AddLine( WORD object, WORD vertex1, WORD vertex2 )
{
	pObject[object].LineCount++;

//Safely Realloc Arrays
	GLushort *pTempLineIndice = (GLushort*)realloc( pObject[object].pLineIndice, sizeof( GLushort ) * 2 * pObject[object].LineCount ); 

//Check for Success
	if( !pTempLineIndice )
	{
	//Update Triangle Count	
		pObject[object].LineCount--;
		
		MessageBox( NULL, "Out of Memory! -> Not Adding Line.", "CMesh::AddLine()", MB_ICONERROR | MB_OK );
		return; 
	}

//Copy Pointers
	pObject[object].pLineIndice = pTempLineIndice; 

	pObject[object].pLineIndice[( pObject[object].LineCount - 1 ) * 2]	   = vertex1;
	pObject[object].pLineIndice[( pObject[object].LineCount - 1 ) * 2 + 1] = vertex2;

	pObject[object].HasLines = TRUE;
}

void CMesh::DeleteLine( WORD object, WORD line )
{
	if( pObject[object].LineCount == 0 ) { pObject[object].HasLines = FALSE; return; }
	
	pObject[object].LineCount--;

	if( pObject[object].LineCount == 0 )
	{ 
		free( pObject[object].pLineIndice );
		pObject[object].pLineIndice = NULL;
		
		pObject[object].HasLines = FALSE; 
	}
	else
	{
		pObject[object].pLineIndice[line * 2] = pObject[object].pLineIndice[pObject[object].LineCount * 2];

		pObject[object].pLineIndice = (GLushort*)realloc( pObject[object].pLineIndice, sizeof( GLushort ) * 2 * pObject[object].LineCount ); 
	}
}

void CMesh::AddTriangles( WORD object, WORD count )
{
	pObject[object].TriangleCount += count;

//Safely Realloc Arrays
	tTriangle	*pTempTriangle = (tTriangle*)realloc(	pObject[object].pTriangle,	sizeof( tTriangle ) *		pObject[object].TriangleCount ); 
	GLushort		*pTempTriangleIndice =		(GLushort*)realloc(	pObject[object].pTriangleIndice,	sizeof( GLushort )	* 3 *	pObject[object].TriangleCount );
	GLboolean	*pTempEdgeFlag = (GLboolean*)realloc(	pObject[object].pEdgeFlag,	sizeof( GLboolean ) * 3 *	pObject[object].TriangleCount );

//Check for Success
	if( !pTempTriangle || !pTempTriangleIndice || !pTempEdgeFlag )
	{
	//Update Triangle Count	
		pObject[object].TriangleCount -= count;
		
		MessageBox( NULL, "Out of Memory! -> Not Adding Triangles.", "CMesh::AddTriangles()", MB_ICONERROR | MB_OK );
		return; 
	}

//Copy Pointers
	pObject[object].pTriangle = pTempTriangle; 
	pObject[object].pTriangleIndice = pTempTriangleIndice;
	pObject[object].pEdgeFlag = pTempEdgeFlag;

//Initialize Data
	for( WORD n = pObject[object].TriangleCount - count; n < pObject[object].TriangleCount; n++)
	{
		pObject[object].pTriangle[n].Visible = TRUE;

		pObject[object].pTriangle[n].Plane[0] = 0;
		pObject[object].pTriangle[n].Plane[1] = 1;
		pObject[object].pTriangle[n].Plane[2] = 0;
		pObject[object].pTriangle[n].Plane[3] = 0;

		pObject[object].pTriangleIndice[n * 3]	   = 0;
		pObject[object].pTriangleIndice[n * 3 + 1] = 0;
		pObject[object].pTriangleIndice[n * 3 + 2] = 0;

		pObject[object].pEdgeFlag[n * 3]	 = FALSE;
		pObject[object].pEdgeFlag[n * 3 + 1] = FALSE;
		pObject[object].pEdgeFlag[n * 3 + 2] = FALSE;
	}

//Update State
	pObject[object].HasTriangles = TRUE;
}


void CMesh::AddTriangle( WORD object, WORD vertex1, WORD vertex2, WORD vertex3 )
{
	pObject[object].TriangleCount++;

//Safely Realloc Arrays
	tTriangle	*pTempTriangle = (tTriangle*)realloc(	pObject[object].pTriangle,	sizeof( tTriangle ) *		pObject[object].TriangleCount ); 
	GLushort		*pTempTriangleIndice =		(GLushort*)realloc(	pObject[object].pTriangleIndice,	sizeof( GLushort )	* 3 *	pObject[object].TriangleCount );
	GLboolean	*pTempEdgeFlag = (GLboolean*)realloc(	pObject[object].pEdgeFlag,	sizeof( GLboolean ) * 3 *	pObject[object].TriangleCount );

//Check for Success
	if( !pTempTriangle || !pTempTriangleIndice || !pTempEdgeFlag )
	{
	//Update Triangle Count	
		pObject[object].TriangleCount--;
		
		MessageBox( NULL, "Out of Memory! -> Not Adding Triangle.", "CMesh::AddTriangle()", MB_ICONERROR | MB_OK );
		return; 
	}

//Copy Pointers
	pObject[object].pTriangle = pTempTriangle; 
	pObject[object].pTriangleIndice = pTempTriangleIndice;
	pObject[object].pEdgeFlag = pTempEdgeFlag;

//Initialize Data
	pObject[object].pTriangle[pObject[object].TriangleCount - 1].Visible = TRUE;

	pObject[object].pTriangle[pObject[object].TriangleCount - 1].Plane[0] = 0;
	pObject[object].pTriangle[pObject[object].TriangleCount - 1].Plane[1] = 1;
	pObject[object].pTriangle[pObject[object].TriangleCount - 1].Plane[2] = 0;
	pObject[object].pTriangle[pObject[object].TriangleCount - 1].Plane[3] = 0;

	pObject[object].pTriangleIndice[( pObject[object].TriangleCount - 1 ) * 3]	   = vertex1;
	pObject[object].pTriangleIndice[( pObject[object].TriangleCount - 1 ) * 3 + 1] = vertex2;
	pObject[object].pTriangleIndice[( pObject[object].TriangleCount - 1 ) * 3 + 2] = vertex3;

	pObject[object].pEdgeFlag[( pObject[object].TriangleCount - 1 ) * 3]	 = FALSE;
	pObject[object].pEdgeFlag[( pObject[object].TriangleCount - 1 ) * 3 + 1] = FALSE;
	pObject[object].pEdgeFlag[( pObject[object].TriangleCount - 1 ) * 3 + 2] = FALSE;

//Update State
	pObject[object].HasTriangles = TRUE;
}

void CMesh::DeleteTriangle( WORD object, WORD triangle )
{
	if( pObject[object].TriangleCount == 0 ) { pObject[object].HasTriangles = FALSE; return; }
	
	pObject[object].TriangleCount--;

	if( pObject[object].TriangleCount == 0 )
	{ 
		free( pObject[object].pTriangleIndice );
		pObject[object].pTriangleIndice = NULL;
		
		pObject[object].HasTriangles = FALSE; 
	}
	else
	{
		pObject[object].pTriangle[triangle] = pObject[object].pTriangle[pObject[object].TriangleCount];

		pObject[object].pTriangle = (tTriangle*)realloc(	pObject[object].pTriangle,	sizeof( tTriangle ) *		pObject[object].TriangleCount ); 

		pObject[object].pTriangleIndice[triangle * 3] = pObject[object].pTriangleIndice[pObject[object].TriangleCount * 3];
		pObject[object].pTriangleIndice =	(GLushort*)realloc(		pObject[object].pTriangleIndice,	sizeof( GLushort )	* 3 *	pObject[object].TriangleCount ); 

		pObject[object].pEdgeFlag[triangle * 3] = pObject[object].pEdgeFlag[pObject[object].TriangleCount * 3];
		pObject[object].pEdgeFlag = (GLboolean*)realloc(	pObject[object].pEdgeFlag,	sizeof( GLboolean ) * 3 *	pObject[object].TriangleCount ); 
	}
}

/*
void CMesh::EvaluateTriangleNeighbour( WORD object, WORD triangle,, bool smooth, double s, double t );
*/

void CMesh::Render( WORD renderStyle )
{
//Save Previous Info
	glPushAttrib( GL_ENABLE_BIT );

//Render Style
	if( ( renderStyle & ENABLE_LIGHTING ) == ENABLE_LIGHTING ) { glEnable( GL_LIGHTING ); }
	else { glDisable( GL_LIGHTING ); }

	if( ( renderStyle & RENDER_FACES ) == RENDER_FACES )
	{
		for( WORD n = 0; n < ObjectCount; n++ )
		{
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();

			glTranslated( pObject[n].Position[0], pObject[n].Position[1], pObject[n].Position[2] );

			glRotated( pObject[n].Rotation[0], 1, 0, 0 ); 
			glRotated( pObject[n].Rotation[1], 0, 1, 0 ); 
			glRotated( pObject[n].Rotation[2], 0, 0, 1 ); 

	/*		if( ( pObject[n].Scale[0] != 1 ) || ( pObject[n].Scale[1] != 1 ) || ( pObject[n].Scale[1] != 1 ) ) 
			{
				glEnable( GL_NORMALIZE );
			
				glScaled( pObject[n].Scale[0], pObject[n].Scale[1], pObject[n].Scale[2] );
			}
			else { glDisable( GL_NORMALIZE ); }*/

			RenderObject( n ); 

			glPopMatrix();
		}
	}

	if( ( renderStyle & RENDER_EDGES ) == RENDER_EDGES )
	{
		glDisable( GL_LIGHTING );

		for( WORD m = 0; m < ObjectCount; m++ )
		{
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();

			glTranslated( pObject[m].Position[0], pObject[m].Position[1], pObject[m].Position[2] );

			glRotated( pObject[m].Rotation[0], 1, 0, 0 ); 
			glRotated( pObject[m].Rotation[1], 0, 1, 0 ); 
			glRotated( pObject[m].Rotation[2], 0, 0, 1 ); 

/*			if( ( pObject[m].Scale[0] != 1 ) || ( pObject[m].Scale[1] != 1 ) || ( pObject[m].Scale[1] != 1 ) ) 
			{
				glEnable( GL_NORMALIZE );
			
				glScaled( pObject[m].Scale[0], pObject[m].Scale[1], pObject[m].Scale[2] );
			}
			else { glDisable( GL_NORMALIZE ); }*/

			if( ( renderStyle & HIGHLIGHT_EDGES ) == HIGHLIGHT_EDGES ) { RenderObjectEdges( m, TRUE ); }
			else { RenderObjectEdges( m, FALSE ); }

			glPopMatrix();
		}
	}

//Render Vertices
	if( ( renderStyle & RENDER_VERTEX ) == RENDER_VERTEX )
	{
		glDisable( GL_LIGHTING );
		for( WORD p = 0; p < ObjectCount; p++ )
		{
			glBegin( GL_POINTS );

			for( WORD r = 0; r < pObject[p].VertexCount; r++ ) { glVertex3fv( &pObject[p].pVertex[r * 3] ); }

			glEnd();
		}
	}

	glPopAttrib();
}


void CMesh::RenderObject( WORD object )
{
//Per-Object Color
	glColor4fv( &pObject[object].Color[0] );

//Transparency
	if( pObject[object].Color[3] != 1 ) { glEnable( GL_BLEND ); glBlendFunc( GL_SRC_ALPHA, GL_DST_ALPHA ); }
	else { glDisable( GL_BLEND ); }

//Initialize Data Arrays
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, pObject[object].pVertex );

	glEnableClientState( GL_NORMAL_ARRAY );
	glNormalPointer( GL_FLOAT, 0, pObject[object].pNormal );

//Initialize Vertex Colors
	if( pObject[object].HasVertexColors )
	{
		glEnableClientState( GL_COLOR_ARRAY );
		glColorPointer( 4, GL_FLOAT, 0, pObject[object].pColor );
	}
	else { glDisableClientState( GL_COLOR_ARRAY ); }


//Render Lines
	glDisableClientState( GL_EDGE_FLAG_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glShadeModel( GL_FLAT ); 

	if( pObject[object].HasLines ) { glDrawElements( GL_LINES, pObject[object].LineCount * 2, GL_UNSIGNED_SHORT, pObject[object].pLineIndice ); }

//Render Triangles
	if( pObject[object].HasTriangles )
	{
		glShadeModel( GL_SMOOTH );

	//Initialize Texturing
		if( pObject[object].HasTexture )
		{
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glColorPointer( 2, GL_FLOAT, 0, pObject[object].pTexCoord );
		}
		else { glDisableClientState( GL_TEXTURE_COORD_ARRAY ); }

		glDrawElements( GL_TRIANGLES, pObject[object].TriangleCount * 3, GL_UNSIGNED_SHORT, pObject[object].pTriangleIndice );
	}

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glDisableClientState( GL_EDGE_FLAG_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}


void CMesh::RenderObjectEdges( WORD object, bool highlight )
{
	if( highlight ) { glColor4f( 1.0f, 1.0f, 1.0f, 1.0f ); }
	else { glColor4fv( &pObject[object].Color[0] ); }

//Render Triangles
/*	if( pObject[object].HasTriangles )
	{
		glPolygonMode( GL_FRONT, GL_LINE );
		glEnable( GL_POLYGON_OFFSET_LINE );
		glPolygonOffset( -1.0f, 0 );

		glShadeModel( GL_FLAT ); 

//Initialize Data Arrays
		glEnableClientState( GL_EDGE_FLAG_ARRAY );
		glEdgeFlagPointer( 0, pObject[object].pEdgeFlag );

		glDisableClientState( GL_COLOR_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );

		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( 3, GL_FLOAT, 0, pObject[object].pVertex );

		glDrawElements( GL_TRIANGLES, pObject[object].TriangleCount * 3, GL_UNSIGNED_SHORT, pObject[object].pTriangleIndice );

		glDisable( GL_POLYGON_OFFSET_LINE );
		glPolygonMode( GL_FRONT, GL_FILL );

		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );

		glDisableClientState( GL_EDGE_FLAG_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
*/
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_EDGE_FLAG_ARRAY );

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

//Render Triangles
	if( pObject[object].HasTriangles )
	{
		glPolygonMode( GL_FRONT, GL_LINE );
		glEnable( GL_POLYGON_OFFSET_LINE );
		glPolygonOffset( -1.0f, 0 );

		glShadeModel( GL_FLAT ); 
		glBegin( GL_TRIANGLES );

		for( WORD n = 0; n < pObject[object].TriangleCount * 3; n += 3 )
		{
			glEdgeFlag( pObject[object].pEdgeFlag[n] );
			glVertex3fv( &pObject[object].pVertex[pObject[object].pTriangleIndice[n] * 3] );
	
			glEdgeFlag( pObject[object].pEdgeFlag[n + 1] );
			glVertex3fv( &pObject[object].pVertex[pObject[object].pTriangleIndice[n + 1] * 3] );

			glEdgeFlag( pObject[object].pEdgeFlag[n + 2] );
			glVertex3fv( &pObject[object].pVertex[pObject[object].pTriangleIndice[n + 2] * 3] );
		}

		glEnd();

		glDisable( GL_POLYGON_OFFSET_LINE );
		glPolygonMode( GL_FRONT, GL_FILL );
	}
}


void CMesh::UpdateNormals() { for( n = 0; n < ObjectCount; n++ ) { UpdateObjectNormals( n ); } }

void CMesh::UpdateConnectivity() { for( n = 0; n < ObjectCount; n++ ) { UpdateObjectConnectivity( n ); } }

void CMesh::UpdateFacePlanes() { for( n = 0; n < ObjectCount; n++ ) { UpdateObjectFacePlanes( n ); } }

void CMesh::UpdateVisibility( Float4 *lightPos ) { for( n = 0; n < ObjectCount; n++ ) { UpdateObjectVisibility( n, lightPos ); } }



void CMesh::UpdateObjectConnectivity( WORD index )
{
	WORD Size = pObject[index].TriangleCount * 3;

	for( n = 0; n < pObject[index].TriangleCount; n ++ )
	{
		pObject[index].pTriangle[n].Neighbour[0] = - 1;
		pObject[index].pTriangle[n].Neighbour[1] = - 1;
		pObject[index].pTriangle[n].Neighbour[2] = - 1;
	}

	for( n = 0; n < Size - 3; n += 3 )
	{
		i = 0;

		for( m = n + 3; m < Size; m += 3 )
		{
			if( i == 3 ) { break; }

		//(n) edge[0] ; (m) edge[0]
			if(	( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m] ) && 	( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m + 1] ) ||
				( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m] ) && 	( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m + 1] ) )
			{
				pObject[index].pTriangle[n / 3].Neighbour[0] = m; pObject[index].pTriangle[m / 3].Neighbour[0] = n; i++; continue;
			}

		//(n) edge[0] ; (m) edge[1]
			if(	( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m + 1] ) && 	( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m + 2] ) ||
				( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m + 1] ) && 	( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m + 2] ) )
			{
				pObject[index].pTriangle[n / 3].Neighbour[0] = m; pObject[index].pTriangle[m / 3].Neighbour[1] = n; i++; continue;
			}

		//(n) edge[0] ; (m) edge[2]
			if(	( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m + 2] ) && 	( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m] ) ||
				( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m + 2] ) && 	( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m] ) )
			{
				pObject[index].pTriangle[n / 3].Neighbour[0] = m; pObject[index].pTriangle[m / 3].Neighbour[2] = n; i++; continue;
			}


			
		//(n) edge[1] ; (m) edge[0]
			if(	( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m] ) && 	( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m + 1] ) ||
				( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m] ) && 	( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m + 1] ) )
			{
				pObject[index].pTriangle[n / 3].Neighbour[1] = m; pObject[index].pTriangle[m / 3].Neighbour[0] = n; i++; continue;
			}

		//(n) edge[1] ; (m) edge[1]
			if(	( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m + 1] ) && 	( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m + 2] ) ||
				( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m + 1] ) && 	( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m + 2] ) )
			{
				pObject[index].pTriangle[n / 3].Neighbour[1] = m; pObject[index].pTriangle[m / 3].Neighbour[1] = n; i++; continue;
			}

		//(n) edge[1] ; (m) edge[2]
			if(	( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m + 2] ) && 	( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m] ) ||
				( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m + 2] ) && 	( pObject[index].pTriangleIndice[n + 1] == pObject[index].pTriangleIndice[m] ) )
			{
				pObject[index].pTriangle[n / 3].Neighbour[1] = m; pObject[index].pTriangle[m / 3].Neighbour[2] = n; i++; continue;
			}

			

		//(n) edge[2] ; (m) edge[0]
			if(	( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m] ) && 	( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m + 1] ) ||
				( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m] ) && 	( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m + 1] ) )
			{
				pObject[index].pTriangle[n / 3].Neighbour[2] = m; pObject[index].pTriangle[m / 3].Neighbour[0] = n; i++; continue;
			}

		//(n) edge[2] ; (m) edge[1]
			if(	( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m + 1] ) && 	( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m + 2] ) ||
				( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m + 1] ) && 	( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m + 2] ) )
			{
				pObject[index].pTriangle[n / 3].Neighbour[2] = m; pObject[index].pTriangle[m / 3].Neighbour[1] = n; i++; continue;
			}

		//(n) edge[2] ; (m) edge[2]
			if(	( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m + 2] ) && 	( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m] ) ||
				( pObject[index].pTriangleIndice[n] == pObject[index].pTriangleIndice[m + 2] ) && 	( pObject[index].pTriangleIndice[n + 2] == pObject[index].pTriangleIndice[m] ) )
			{
				pObject[index].pTriangle[n / 3].Neighbour[2] = m; pObject[index].pTriangle[m / 3].Neighbour[2] = n; i++; continue;
			}
		}
	}
}

void CMesh::UpdateObjectFacePlanes( WORD index )
{
	for( n = 0; n < pObject[index].TriangleCount; n++ )
	{
		Math.DataToVector3( &TempVector3a, &pObject[index].pVertex[pObject[index].pTriangleIndice[n * 3] * 3] );
		Math.DataToVector3( &TempVector3b, &pObject[index].pVertex[pObject[index].pTriangleIndice[n * 3 + 1] * 3] );
		Math.DataToVector3( &TempVector3c, &pObject[index].pVertex[pObject[index].pTriangleIndice[n * 3 + 2] * 3] );

		Math.MakePlane( &TempVector3a, &TempVector3b, &TempVector3c, &pObject[index].pTriangle[n].Plane );
	}
}

void CMesh::UpdateObjectVisibility(WORD index, Float4 *lightPos )
{
	for( n = 0; n < pObject[index].TriangleCount; n++ )
	{
		if( ( lightPos[0][0] * pObject[index].pTriangle[n].Plane[0] + lightPos[0][1] * pObject[index].pTriangle[n].Plane[1] + lightPos[0][2] * pObject[index].pTriangle[n].Plane[2] + pObject[index].pTriangle[n].Plane[3] ) > 0 ) { pObject[index].pTriangle[n].Visible = TRUE; }
		else  { pObject[index].pTriangle[n].Visible = FALSE; }
	}
}

void CMesh::UpdateObjectNormals( WORD index )
{
	for( n = 0; n < pObject[index].VertexCount; n++ )
	{
		Math.ZeroVector3( &TempVector3b );	

		for( m = 0; m < pObject[index].TriangleCount; m++ )
		{
			if( ( pObject[index].pTriangleIndice[m * 3] == n ) || ( pObject[index].pTriangleIndice[m * 3 + 1] == n ) || ( pObject[index].pTriangleIndice[m * 3 + 2] == n ) )
			{
				TempVector3a[0] = pObject[index].pTriangle[m].Plane[0];
				TempVector3a[1] = pObject[index].pTriangle[m].Plane[1];
				TempVector3a[2] = pObject[index].pTriangle[m].Plane[2];

				Math.AddVectors3( &TempVector3b, &TempVector3a, &TempVector3b );
			}
		}

		Math.NormalizeVector3( &TempVector3b, &TempVector3b );
		pObject[index].pNormal[n * 3] = TempVector3a[0];
		pObject[index].pNormal[n * 3 + 1] = TempVector3a[1];
		pObject[index].pNormal[n * 3 + 2] = TempVector3a[2];
	}
}
