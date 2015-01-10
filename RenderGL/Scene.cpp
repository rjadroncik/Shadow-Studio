// Scene.cpp: implementation of the CScene class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Scene.h"
#include "../System.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScene::CScene()
{
	pModel = NULL;
	ModelCount = 0;

	pLight = NULL;
	LightCount = 0;

	pFont = NULL;
	FontCount = 0;

//	Initialize();
}

CScene::~CScene()
{

}


void CScene::Initialize()
{
	pModel = NULL;
	ModelCount = 0;

	pLight = NULL;
	LightCount = 0;

	AmbientLight[0] = 0.4f;
	AmbientLight[1] = 0.4f;
	AmbientLight[2] = 0.4f;
	AmbientLight[3] = 1.0f;

	CGrid *pGrid;

	PerspectGrid.Initialize();
	if( PerspectGrid.SelectBaseObject( OBJECT_GRID ) )
	{
		pGrid = (CGrid*)PerspectGrid.pBaseObject;

		pGrid->Size[0] = 500;
		pGrid->Size[1] = 500;

		pGrid->Tassellation[0] = 20;
		pGrid->Tassellation[1] = 20;

		pGrid->GenerateMesh();

		pGrid->pRenderMesh->pObject[0].Color[0] = 0.3f;
		pGrid->pRenderMesh->pObject[0].Color[1] = 0.3f;
		pGrid->pRenderMesh->pObject[0].Color[2] = 0.3f;
	}

	OrthoGrid.Initialize();
	if( OrthoGrid.SelectBaseObject( OBJECT_GRID ) )
	{
		pGrid = (CGrid*)OrthoGrid.pBaseObject;

		pGrid->Size[0] = 1000;
		pGrid->Size[1] = 1000;

		pGrid->Tassellation[0] = 100;
		pGrid->Tassellation[1] = 100;

		pGrid->GenerateMesh();

		pGrid->pRenderMesh->pObject[0].Color[0] = 0.3f;
		pGrid->pRenderMesh->pObject[0].Color[1] = 0.3f;
		pGrid->pRenderMesh->pObject[0].Color[2] = 0.3f;
	}

//	MoveGizmo.Create( GT_MOVE, 50.0f );
}

void CScene::DeleteModels()
{
	if( pModel && (	ModelCount > 0 ) )
	{
		for( UINT n = 0; n < ModelCount; n++ ) { pModel[n].Delete(); }

		free( pModel );
	}

	pModel = NULL;
	ModelCount = 0;
}

void CScene::DeleteLights()
{
	if( pLight && (	LightCount > 0 ) )
	{
//		for( UINT n = 0; n < LightCount; n++ ) { pLight[n].Destroy(); }

		free( pLight );
	}

	pLight = NULL;
	LightCount = 0;
}

void CScene::DeleteFonts()
{
	if( pFont && (	FontCount > 0 ) )
	{
		for( UINT n = 0; n < FontCount; n++ ) { pFont[n].Destroy(); }

		free( pFont );
	}

	pFont = NULL;
	FontCount = 0;
}


void CScene::Delete()
{
	DeleteModels();
	DeleteLights();
	DeleteFonts();
}


int CScene::AddModel( UINT type )
{
	ModelCount++;

//Safely Realloc Object Array
	CModel	*pTmp = (CModel*)realloc( pModel, sizeof( CModel ) * ModelCount );

//Check for Success
	if( !pTmp )
	{ 
	//Update Model Count
		ModelCount++;

		MessageBox( NULL, "Out of Memory! -> Not Creating object.", "CScene::AddModel();", MB_ICONERROR | MB_OK ); 
		return -1; 
	}

//Copy Pointers
	pModel = pTmp;

//Initialize New Object	
	pModel[ModelCount - 1].Initialize();
	pModel[ModelCount - 1].SelectBaseObject( type );

//Return Index of Added Model
	return ModelCount - 1;
}


bool CScene::DeleteModel( UINT index )
{
	if( ( ModelCount == 0 ) || ( index < 0 ) || ( index >= ModelCount ) ) { return FALSE; }
	
	ModelCount--;
	pModel[index].Delete(); 

//Release Model Memory
	if( ModelCount == 0 )
	{ 
		free( pModel ); 

		pModel = NULL; 
	}
	else 
	{ 
		pModel[index] = pModel[ModelCount];

		pModel = (CModel*)realloc( pModel, sizeof( CModel ) * ModelCount ); 
	}

	return TRUE;
}


UINT CScene::AddFont( LPSTR name, int size, HDC hdc )
{
	FontCount++;

//Safely Realloc Object Array
	CFontGL	*pTmp = NULL;
	pTmp = (CFontGL*)realloc( pFont, sizeof( CFontGL ) * FontCount );

//Check for Success
	if( !pTmp ) { MessageBox( NULL, "CScene::AddFont();", "Out of Memory! -> Not Creating object.", MB_ICONERROR | MB_OK ); return -1; }
	pFont = pTmp;

//Initialize New Object	
	pFont[FontCount - 1].Create( name, size, hdc, 256 );
	pFont[FontCount - 1].SetViewport( Viewport[2], Viewport[3] );

//Return Index of Added Model
	return FontCount - 1;
}


void CScene::Render( UINT viewType, UINT viewId, UINT renderMode )
{
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, &AmbientLight[0] );

	for( UINT g = 0; g < ModelCount; g++ )
	{ 
		if( pModel[g].Visible )
		{
			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();

			glTranslated( pModel[g].Position[0], pModel[g].Position[1], pModel[g].Position[2] );

			glRotated( pModel[g].Rotation[0], 1, 0, 0 ); 
			glRotated( pModel[g].Rotation[1], 0, 1, 0 ); 
			glRotated( pModel[g].Rotation[2], 0, 0, 1 ); 

			if( ( pModel[g].Scale[0] != 1 ) || ( pModel[g].Scale[1] != 1 ) || ( pModel[g].Scale[2] != 1 ) ) 
			{
				glEnable( GL_NORMALIZE );
			
				glScaled( pModel[g].Scale[0], pModel[g].Scale[1], pModel[g].Scale[2] );
			}
			else { glDisable( GL_NORMALIZE ); }

			if( ( renderMode == RM_SHADED ) || ( renderMode == RM_COMPOSITE ) ) { pModel[g].Mesh.Render( RENDER_FACES | ENABLE_LIGHTING ); }
			if( renderMode == RM_WIREFRAME )
			{ 
				if( pModel[g].Selected ) { pModel[g].Mesh.Render( RENDER_EDGES | HIGHLIGHT_EDGES ); }
				else { pModel[g].Mesh.Render( RENDER_EDGES ); }
			}

			glPopMatrix();
		}
	}

	if( renderMode == RM_COMPOSITE )
	{
		for( UINT h = 0; h < ModelCount; h++ )
		{ 
			if( pModel[h].Visible )
			{
				glMatrixMode( GL_MODELVIEW );
				glPushMatrix();

				glTranslated( pModel[h].Position[0], pModel[h].Position[1], pModel[h].Position[2] );

				glRotated( pModel[h].Rotation[0], 1, 0, 0 ); 
				glRotated( pModel[h].Rotation[1], 0, 1, 0 ); 
				glRotated( pModel[h].Rotation[2], 0, 0, 1 ); 

				if( ( pModel[h].Scale[0] != 1 ) || ( pModel[h].Scale[1] != 1 ) || ( pModel[h].Scale[1] != 1 ) ) 
				{
					glEnable( GL_NORMALIZE );
				
					glScaled( pModel[h].Scale[0], pModel[h].Scale[1], pModel[h].Scale[2] );
				}
				else { glDisable( GL_NORMALIZE ); }

				if( pModel[h].Selected ) { pModel[h].Mesh.Render( RENDER_EDGES | HIGHLIGHT_EDGES ); }
				else { pModel[h].Mesh.Render( RENDER_EDGES ); }

				glPopMatrix();
			}
		}
	}
}


void CScene::DrawAxisGizmo( float size, UINT font, bool fromCenter )
{
	glPushAttrib( GL_ENABLE_BIT | GL_TRANSFORM_BIT );

	glDisable( GL_CULL_FACE );
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_FOG );
	glDisable( GL_TEXTURE_2D );

	glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );
	glGetDoublev( GL_PROJECTION_MATRIX, ProjMatrix );
	glGetIntegerv( GL_VIEWPORT, Viewport );

	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();

	if( fromCenter )
	{
		glTranslated( - size / 2, - size / 2, - size / 2 );

		if( gluProject(	size / 2, - size / 2, - size / 2,	ModelMatrix, ProjMatrix, Viewport,
						&TempVectord3a[0], &TempVectord3a[1], &TempVectord3a[2] ) )
		{
		}

		if( gluProject(	- size / 2, size / 2, - size / 2,	ModelMatrix, ProjMatrix, Viewport,
						&TempVectord3b[0], &TempVectord3b[1], &TempVectord3b[2] ) )
		{
		}

		if( gluProject(	- size / 2, - size / 2, size / 2,	ModelMatrix, ProjMatrix, Viewport,
						&TempVectord3c[0], &TempVectord3c[1], &TempVectord3c[2] ) )
		{
		}
	}
	else
	{
	//Calculate Corner Vectors
		if( gluProject(	size, 0, 0,	ModelMatrix, ProjMatrix, Viewport,
						&TempVectord3a[0], &TempVectord3a[1], &TempVectord3a[2] ) )
		{
		}

		if( gluProject(	0, size, 0,	ModelMatrix, ProjMatrix, Viewport,
						&TempVectord3b[0], &TempVectord3b[1], &TempVectord3b[2] ) )
		{
		}

		if( gluProject(	0, 0, size,	ModelMatrix, ProjMatrix, Viewport,
						&TempVectord3c[0], &TempVectord3c[1], &TempVectord3c[2] ) )
		{
		}
	}

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	glOrtho( 0, Viewport[2], 0, Viewport[3], -1000, 1000 );

	glMatrixMode( GL_MODELVIEW ); 
	glPushMatrix();
	glLoadIdentity();

	glBegin( GL_LINES ); 

	glColor3f( 1, 0, 0 );

	glVertex2d( TempVectord3a[0] + 1, TempVectord3a[1] + 1 );
	glVertex2d( TempVectord3a[0] + 6, TempVectord3a[1] + 8 );

	glVertex2d( TempVectord3a[0] + 6, TempVectord3a[1] + 1 );
	glVertex2d( TempVectord3a[0] + 1, TempVectord3a[1] + 8 );

	glColor3f( 0, 1, 0 );

	glVertex2d( TempVectord3b[0] + 2, TempVectord3b[1] + 1 );
	glVertex2d( TempVectord3b[0] + 6, TempVectord3b[1] + 8 );

	glVertex2d( TempVectord3b[0] + 3, TempVectord3b[1] + 3 );
	glVertex2d( TempVectord3b[0] + 1, TempVectord3b[1] + 8 );

	glColor3f( 0, 0, 1 );

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


	glBegin( GL_LINES );

	glColor3f( 1, 0, 0 );
	glVertex3f( 0, 0, 0 ); 
	glVertex3f( size, 0, 0 ); 

	glColor3f( 0, 1, 0 );
	glVertex3f( 0, 0, 0 ); 
	glVertex3f( 0, size, 0 ); 

	glColor3f( 0, 0, 1 );
	glVertex3f( 0, 0, 0 ); 
	glVertex3f( 0, 0, size ); 

	glEnd();

	glMatrixMode( GL_MODELVIEW ); 
	glPopMatrix();

	glPopAttrib();
}
