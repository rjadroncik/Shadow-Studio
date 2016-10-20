// Selection.cpp: implementation of the CSelection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Selection.h"
#include "Shadow.h"
#include "Options.h"
#include "State.h"
#include "System.h"
#include "AFC.h"
#include "ViewUI.h"

#include "Math.h"

#include "RenderGL/Scene.h" 
//#include "RenderGL/Model.h" 

extern	CState			State;
extern	COptions		Options;
extern	CMath			Math;
extern	CSystem			System;
extern	ADialogBar		MainBar;
extern	AToolBar		BottomToolBar;


extern	CViewUI			ViewUI;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelection::CSelection()
{
	InfoCount = 0;
	ActiveInfos = 0;
	pInfo = NULL;

	Selected = FALSE;

	VertexCount = 0;
	ActiveVertices = 0;
	pVertex = NULL;

	GizmoVisible = FALSE;
	BoxesVisible = FALSE;
	RegionVisible = FALSE;

	GizmoHighlighted = FALSE;

	pScene = NULL;

	RegionType = 0;
	PointCount = 0;
	ActivePoints = 0;

	RegionType = SRT_RECTANGLE;

	pRegionPoint = NULL;
	pPoint = NULL;
	pPointReflected = NULL;
	pPlane = NULL;

	TempBoxMesh.AddObject();
	TempBoxMesh.AddVertices( 0, 8 );

	TempBoxMesh.AddTriangle( 0, 3, 1, 0 );
	TempBoxMesh.AddTriangle( 0, 3, 2, 1 );

	TempBoxMesh.AddTriangle( 0, 7, 2, 3 );
	TempBoxMesh.AddTriangle( 0, 7, 6, 2 );

	TempBoxMesh.AddTriangle( 0, 6, 1, 2 );
	TempBoxMesh.AddTriangle( 0, 6, 5, 1 );

	TempBoxMesh.AddTriangle( 0, 4, 3, 0 );
	TempBoxMesh.AddTriangle( 0, 4, 7, 3 );

	TempBoxMesh.AddTriangle( 0, 5, 0, 1 );
	TempBoxMesh.AddTriangle( 0, 5, 4, 0 );

	TempBoxMesh.AddTriangle( 0, 4, 6, 7 );
	TempBoxMesh.AddTriangle( 0, 4, 5, 6 );
}

CSelection::~CSelection()
{

}

//////////////////////////////////////////////////////////////////////
// Member Functions
//////////////////////////////////////////////////////////////////////

void CSelection::SetScene( CScene *pscene )
{
	pScene = pscene;
}

void CSelection::UpdateCenter()
{
	if( !pScene ) { return; }

	GizmoPosition[0] = 0;
	GizmoPosition[1] = 0;
	GizmoPosition[2] = 0;

	CModel *pModel;
	State.TempValue1 = 0;

	for( UINT n = 0; n < ActiveInfos; n++ )
	{
		if( pInfo[n].ObjectType == MODEL )
		{
			pModel = (CModel*)pInfo[n].pObject;

			if( Options.CenterType == CENTER_ORIGIN ) { Math.AddVectors3( &GizmoPosition, &pModel->Position, &GizmoPosition ); }

			if( Options.CenterType == CENTER_SELECTION )
			{
				Math.AddVectors3( &GizmoPosition, &pModel->Position, &GizmoPosition );
				Math.AddVectors3( &GizmoPosition, &pModel->BoxPosition, &GizmoPosition );
			}

			State.TempValue1++;
		}
	}

	Math.DivideVector3( &GizmoPosition, State.TempValue1, &GizmoPosition );
}

void CSelection::UpdateClippingPlanes()
{
//Near Clipping Plane
	Math.ScaleVector3( &State.View.Direction, State.View.ClipNear, &State.TempVector3a );
	Math.AddVectors3( &State.View.Position, &State.TempVector3a, &State.TempVector3a );

	Math.MakePlane( &State.TempVector3a, &State.View.Direction, &ClipNear );

//Far Clipping Plane
	Math.ScaleVector3( &State.View.Direction, -1, &State.TempVector3a );

	Math.ScaleVector3( &State.View.Direction, State.View.ClipFar, &State.TempVector3b );
	Math.AddVectors3( &State.View.Position, &State.TempVector3b, &State.TempVector3b );

	Math.MakePlane( &State.TempVector3b, &State.TempVector3a, &ClipFar );
}

void CSelection::UpdateRegion( POINT position )
{
//Assign Region Type 
//	RegionType = SRT_POINT;

	State.TempRect2.left = position.x - 1;
	State.TempRect2.right = position.x + 1;
	State.TempRect2.top = position.y - 1;
	State.TempRect2.bottom = position.y + 1;

	UpdateRegion( State.TempRect2, TRUE );

	SelectIntersecting = TRUE;

//	UpdateClippingPlanes();
}

void CSelection::UpdateRegion( RECT rectangle, bool updateVolume )
{
	SelectIntersecting = Options.SelectIntersecting;

//Initialize Region
	if( PointCount < 5 )
	{
		PointCount = 5;

		pRegionPoint = (Int2*)realloc( pRegionPoint,PointCount * sizeof( Int2 ) );

		pPoint = (Float3*)realloc( pPoint, PointCount * sizeof( Float3 ) );
		pPointReflected = (Float3*)realloc( pPointReflected, PointCount * sizeof( Float3 ) );

		pPlane = (Float4*)realloc( pPlane, ( PointCount - 1 ) * sizeof( Float4 ) );
	}

	ActivePoints = 5;

// Adjust Rectangle ( Correct Polygon Winding )
	if( ( rectangle.right < rectangle.left )  ) { State.TempValue1 = (float)rectangle.right; rectangle.right = rectangle.left; rectangle.left = (int)State.TempValue1; }
	if( ( rectangle.bottom > rectangle.top ) ) { State.TempValue1 = (float)rectangle.bottom; rectangle.bottom = rectangle.top; rectangle.top = (int)State.TempValue1; }

//Fill Region Point Array
	pRegionPoint[0][0] = rectangle.left;
	pRegionPoint[0][1] = rectangle.top;

	pRegionPoint[1][0] = rectangle.right;
	pRegionPoint[1][1] = rectangle.top;
	
	pRegionPoint[2][0] = rectangle.right;
	pRegionPoint[2][1] = rectangle.bottom;
	
	pRegionPoint[3][0] = rectangle.left;
	pRegionPoint[3][1] = rectangle.bottom;

	pRegionPoint[4][0] = rectangle.left;
	pRegionPoint[4][1] = rectangle.top;

	if( !updateVolume ) { return; }

//Prepare For Un-Projecting
	System.View[System.ViewActive].PrepareSelection();

	glGetDoublev( GL_MODELVIEW_MATRIX, State.ModelMatrix );
	glGetDoublev( GL_PROJECTION_MATRIX, State.ProjMatrix );
	glGetIntegerv( GL_VIEWPORT, State.Viewport );

	UINT n;

//Calculate Region Point Vectors
	for( n = 0; n < ActivePoints; n++ )
	{
		gluUnProject( pRegionPoint[n][0], pRegionPoint[n][1], 1, State.ModelMatrix, State.ProjMatrix, State.Viewport, &State.TempVectord3a[0], &State.TempVectord3a[1], &State.TempVectord3a[2] );

		Math.CopyVector3( &pPoint[n], &State.TempVectord3a );
	}

//Calculate Region Volume Planes
	if( State.View.Type == VT_PERSPECTIVE )
	{
	//Use Classical 3D Position ( 2 Points + Position Form a Triangle -> n - 1 Triangles )
		for( n = 0; n < ActivePoints - 1; n++ )
		{
			Math.MakePlane( &State.View.Position, &pPoint[n], &pPoint[n + 1], &pPlane[n] );
		}
	}
	else
	{
	//Use Classical 3D Position ( 2 Points + 2 Reflected Points Form 2 Triangles -> 2 * ( n - 1 ) Triangles )    
		for( n = 0; n < ActivePoints - 1; n++ )
		{
			Math.MakeVector3( &pPoint[n], &pPoint[n + 1], &State.TempVector3a );
			Math.NormalizeVector3( &State.TempVector3a, &State.TempVector3a );  
	
			Math.CrossVectors3( &State.View.Direction, &State.TempVector3a, &State.TempVector3b );
		
			Math.NormalizeVector3( &State.TempVector3b, &State.TempVector3b );  
		
			Math.MakePlane( &pPoint[n], &State.TempVector3b, &pPlane[n] );
		}

		for( n = 0; n < ActivePoints; n++ )
		{
			Math.ScaleVector3( &State.View.Direction, 0.0002f * State.View.ClipNear, &State.TempVector3e );
			Math.AddVectors3( &State.TempVector3e, &pPoint[n], &pPointReflected[n] );
		}

	}

	UpdateClippingPlanes();
}

void CSelection::UpdateRegion( POINT *ppoint, UINT pointCount, bool updateVolume )
{
	SelectIntersecting = Options.SelectIntersecting;

	if( PointCount < pointCount )
	{
		PointCount = pointCount;

		pRegionPoint = (Int2*)realloc( pRegionPoint,PointCount * sizeof( Int2 ) );

		pPoint = (Float3*)realloc( pPoint, PointCount * sizeof( Float3 ) );
		pPointReflected = (Float3*)realloc( pPointReflected, PointCount * sizeof( Float3 ) );

		pPlane = (Float4*)realloc( pPlane, ( PointCount - 1 ) * sizeof( Float4 ) );
	}

	ActivePoints = pointCount;
	UINT n;

//Fill Region Point Array
	for( n = 0; n < pointCount; n++ ) { pRegionPoint[n][0] = ppoint[n].x; pRegionPoint[n][1] = ppoint[n].y; }

	if( !updateVolume ) { return; }

//Prepare For Un-Projecting
	System.View[System.ViewActive].PrepareSelection();

	glGetDoublev( GL_MODELVIEW_MATRIX, State.ModelMatrix );
	glGetDoublev( GL_PROJECTION_MATRIX, State.ProjMatrix );
	glGetIntegerv( GL_VIEWPORT, State.Viewport );

//Calculate Region Point Vectors
	for( n = 0; n < ActivePoints; n++ )
	{
		gluUnProject( pRegionPoint[n][0], pRegionPoint[n][1], 1, State.ModelMatrix, State.ProjMatrix, State.Viewport, &State.TempVectord3a[0], &State.TempVectord3a[1], &State.TempVectord3a[2] );

		Math.CopyVector3( &pPoint[n], &State.TempVectord3a );
	}

//Calculate Region Volume Planes
	if( State.View.Type == VT_PERSPECTIVE )
	{
	//Use Classical 3D Position ( 2 Points + Position Form a Triangle -> n - 1 Triangles )
		for( n = 0; n < ActivePoints - 1; n++ )
		{
			Math.MakePlane( &State.View.Position, &pPoint[n], &pPoint[n + 1], &pPlane[n] );
		}
	}
	else
	{
	//Use Classical 3D Position ( 2 Points + 2 Reflected Points Form 2 Triangles -> 2 * ( n - 1 ) Triangles )    
		for( n = 0; n < ActivePoints - 1; n++ )
		{
			Math.MakeVector3( &pPoint[n], &pPoint[n + 1], &State.TempVector3a );
			Math.NormalizeVector3( &State.TempVector3a, &State.TempVector3a );  
	
			Math.CrossVectors3( &State.View.Direction, &State.TempVector3a, &State.TempVector3b );
		
			Math.NormalizeVector3( &State.TempVector3b, &State.TempVector3b );  
		
			Math.MakePlane( &pPoint[n], &State.TempVector3b, &pPlane[n] );
		}

		for( n = 0; n < ActivePoints; n++ )
		{
			Math.ScaleVector3( &State.View.Direction, 0.0002f * State.View.ClipNear, &State.TempVector3e );
			Math.AddVectors3( &State.TempVector3e, &pPoint[n], &pPointReflected[n] );
		}
	}

	UpdateClippingPlanes();
}


void CSelection::ClearEntries()
{
	if( !pScene ) { return; }

	for( UINT n = 0; n < pScene->ModelCount; n++ )
	{
		pScene->pModel[n].Selected = FALSE; 
	}

	if( pInfo )
	{
		free( pInfo ); 
	}

	pInfo = NULL;

	ActiveInfos = 0;
	InfoCount = 0;

	GizmoVisible = FALSE;
	BoxesVisible = FALSE; 
	Selected = FALSE;
}

void CSelection::ClearBuffer()
{
	if( pVertex ) { free( pVertex ); }

	VertexCount = 0;
	ActiveVertices = 0;
	pVertex = NULL;
}

void CSelection::ResizeBuffer( UINT size )
{
	VertexCount = size;

	pVertex = (Float3*)realloc( pVertex, sizeof( Float3 ) * VertexCount );
}

int CSelection::AddEntry( UINT objType, UINT objIndex, void *pobject, bool displayBox )
{
	if( !pScene ) { return -1; }

	ActiveInfos++;

	if( ActiveInfos > InfoCount )
	{
		InfoCount++;
		pInfo = (tSelectionInfo*)realloc( pInfo, InfoCount * sizeof( tSelectionInfo ) );
	}

	pInfo[ActiveInfos - 1].ObjectType = objType;
	pInfo[ActiveInfos - 1].ObjectIndex = objIndex;
	pInfo[ActiveInfos - 1].SubObjectType = -1;
	pInfo[ActiveInfos - 1].SubObjectIndex = -1;
	pInfo[ActiveInfos - 1].pObject = pobject;
	pInfo[ActiveInfos - 1].DisplayBox = displayBox;

	pInfo[ActiveInfos - 1].pObject = pobject;
	pInfo[ActiveInfos - 1].DisplayBox = displayBox;

	if( objType == MODEL )
	{
		CModel *pModel = (CModel*)pobject;
		
		pInfo[ActiveInfos - 1].Position[0] = pModel->Position[0];
		pInfo[ActiveInfos - 1].Position[1] = pModel->Position[1];
		pInfo[ActiveInfos - 1].Position[2] = pModel->Position[2];

		pInfo[ActiveInfos - 1].Rotation[0] = pModel->Rotation[0];
		pInfo[ActiveInfos - 1].Rotation[1] = pModel->Rotation[1];
		pInfo[ActiveInfos - 1].Rotation[2] = pModel->Rotation[2];

		pInfo[ActiveInfos - 1].Scale[0] = pModel->Scale[0];
		pInfo[ActiveInfos - 1].Scale[1] = pModel->Scale[1];
		pInfo[ActiveInfos - 1].Scale[2] = pModel->Scale[2];

		pModel->Selected = TRUE; 
	}

	GizmoVisible = TRUE;
	BoxesVisible = TRUE; 

	UpdateInfoDialog();

	return ActiveInfos - 1;
}

void CSelection::SetEntry( UINT index, UINT objType, UINT objIndex, void *pobject, bool displayBox, Float3 *position )
{
	pInfo[index].ObjectIndex = objIndex;
	pInfo[index].ObjectType = objType;
	pInfo[index].SubObjectType = -1;
	pInfo[index].SubObjectIndex = -1;
	pInfo[index].pObject = pobject;
	pInfo[index].DisplayBox = displayBox;

	pInfo[index].Position[0] = position[0][0];
	pInfo[index].Position[1] = position[0][1];
	pInfo[index].Position[2] = position[0][2];

	UpdateInfoDialog();
}

int CSelection::AddEntry( UINT objType, UINT objIndex, UINT subObjType, UINT subObjIndex, void *pobject, bool displayBox )
{
	if( !pScene ) { return -1; }

	ActiveInfos++;

	if( ActiveInfos > InfoCount )
	{
		InfoCount++;
		pInfo = (tSelectionInfo*)realloc( pInfo, InfoCount * sizeof( tSelectionInfo ) );
	}

	pInfo[ActiveInfos - 1].ObjectType = objType;
	pInfo[ActiveInfos - 1].ObjectIndex = objIndex;
	pInfo[ActiveInfos - 1].SubObjectType = subObjType;
	pInfo[ActiveInfos - 1].SubObjectIndex = subObjIndex;
	pInfo[ActiveInfos - 1].pObject = pobject;
	pInfo[ActiveInfos - 1].DisplayBox = displayBox;

	if( objType == MODEL )
	{
		CModel *pModel = (CModel*)pobject;
		
		pInfo[ActiveInfos - 1].Position[0] = pModel->Position[0];
		pInfo[ActiveInfos - 1].Position[1] = pModel->Position[1];
		pInfo[ActiveInfos - 1].Position[2] = pModel->Position[2];

		pInfo[ActiveInfos - 1].Rotation[0] = pModel->Rotation[0];
		pInfo[ActiveInfos - 1].Rotation[1] = pModel->Rotation[1];
		pInfo[ActiveInfos - 1].Rotation[2] = pModel->Rotation[2];

		pInfo[ActiveInfos - 1].Scale[0] = pModel->Scale[0];
		pInfo[ActiveInfos - 1].Scale[1] = pModel->Scale[1];
		pInfo[ActiveInfos - 1].Scale[2] = pModel->Scale[2];

		pModel->Selected = TRUE; 
	}

	GizmoVisible = TRUE;
	BoxesVisible = TRUE; 

	UpdateInfoDialog();

	return ActiveInfos - 1;
}

void CSelection::SetEntry( UINT index, UINT objType, UINT objIndex, UINT subObjType, UINT subObjIndex, void *pobject, bool displayBox, Float3 *position )
{
	pInfo[index].ObjectIndex = objIndex;
	pInfo[index].ObjectType = objType;
	pInfo[index].SubObjectType = subObjType;
	pInfo[index].SubObjectIndex = subObjIndex;
	pInfo[index].pObject = pobject;
	pInfo[index].DisplayBox = displayBox;

	pInfo[index].Position[0] = position[0][0];
	pInfo[index].Position[1] = position[0][1];
	pInfo[index].Position[2] = position[0][2];

	UpdateInfoDialog();
}

void CSelection::RemoveEntry( UINT index )
{
	if( !pScene ) { return; }

	if( ActiveInfos == 0 ) { return; }

	if( pInfo[index].ObjectType == MODEL )
	{
		CModel *pModel = (CModel*)pInfo[index].pObject;

		pModel->Selected = FALSE; 
	}

	pInfo[index] = pInfo[ActiveInfos - 1];

	ActiveInfos--;

	if( ActiveInfos > 0 ) {	UpdateInfoDialog(); }
	if( ActiveInfos == 0 ) { GizmoVisible = FALSE; BoxesVisible = FALSE; }

	
}	

int CSelection::FindEntry( UINT objType, UINT objIndex, UINT subObjType, UINT subObjIndex, bool displayBox )	
{
	for( UINT n = 0; n < ActiveInfos; n++ )
	{
		if( ( pInfo[n].ObjectType == objType ) && ( pInfo[n].ObjectIndex == objIndex ) && ( pInfo[n].SubObjectType == subObjType ) && ( pInfo[n].SubObjectIndex == subObjIndex ) && ( pInfo[n].DisplayBox == displayBox ) ) { return n; }
	}

	return -1;
}

int CSelection::FindEntry( UINT objType, UINT objIndex, bool displayBox )	
{
	for( UINT n = 0; n < ActiveInfos; n++ )
	{
		if( ( pInfo[n].ObjectType == objType ) && ( pInfo[n].ObjectIndex == objIndex ) && ( pInfo[n].DisplayBox == displayBox ) ) { return n; }
	}

	return -1;
}

bool CSelection::ModelInRegion( UINT index )
{
	if( !pScene ) { return FALSE; }

//Range Checking
	if( index >= pScene->ModelCount ) { return FALSE; }

//Clipping
	if( ( ClipNear[0] * ( pScene->pModel[index].Position[0] + pScene->pModel[index].BoxPosition[0] + pScene->pModel[index].BoxSize[0] ) + ClipNear[1] * ( pScene->pModel[index].Position[1] + pScene->pModel[index].BoxPosition[1] + pScene->pModel[index].BoxSize[1] ) + ClipNear[2] * ( pScene->pModel[index].Position[2] + pScene->pModel[index].BoxPosition[2] + pScene->pModel[index].BoxSize[2] ) + ClipNear[3] ) < 0 ) { return FALSE; }
	if( ( ClipFar[0]  * ( pScene->pModel[index].Position[0] + pScene->pModel[index].BoxPosition[0] + pScene->pModel[index].BoxSize[0] ) + ClipFar[1]  * ( pScene->pModel[index].Position[1] + pScene->pModel[index].BoxPosition[1] + pScene->pModel[index].BoxSize[1] ) + ClipFar[2]  * ( pScene->pModel[index].Position[2] + pScene->pModel[index].BoxPosition[2] + pScene->pModel[index].BoxSize[2] ) + ClipFar[3] )  < 0 ) { return FALSE; }

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

//Scale Objects ( Increase Accuracy )
	if( System.View[System.ViewActive].ID == VIEW_TOP )		{ glScalef( 1.0f,		0.0001f,	1.0f ); } 
	if( System.View[System.ViewActive].ID == VIEW_LEFT )	{ glScalef( 0.0001f,	1.0f,		1.0f ); } 
	if( System.View[System.ViewActive].ID == VIEW_FRONT )	{ glScalef( 1.0f,		1.0f,		0.0001f ); } 

//Bounding Box Transformations
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	glTranslatef( pScene->pModel[index].Position[0], pScene->pModel[index].Position[1], pScene->pModel[index].Position[2] );
//	glScalef( pScene->pModel[index].Scale[0], pScene->pModel[index].Scale[1], pScene->pModel[index].Scale[2] ); 
	glTranslatef( pScene->pModel[index].BoxPosition[0], pScene->pModel[index].BoxPosition[1], pScene->pModel[index].BoxPosition[2] );

	glRotatef( pScene->pModel[index].Rotation[0], 1, 0, 0 ); 
	glRotatef( pScene->pModel[index].Rotation[1], 0, 1, 0 ); 
	glRotatef( pScene->pModel[index].Rotation[2], 0, 0, 1 ); 

	glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );

	State.TempBool1 = BoxInRegion( &pScene->pModel[index].BoxSize, TRUE );

	glPopMatrix();

//Model Transformations
	glTranslatef( pScene->pModel[index].Position[0], pScene->pModel[index].Position[1], pScene->pModel[index].Position[2] );

	glRotatef( pScene->pModel[index].Rotation[0], 1, 0, 0 ); 
	glRotatef( pScene->pModel[index].Rotation[1], 0, 1, 0 ); 
	glRotatef( pScene->pModel[index].Rotation[2], 0, 0, 1 ); 

	glScalef( pScene->pModel[index].Scale[0], pScene->pModel[index].Scale[1], pScene->pModel[index].Scale[2] );

	glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );

//Test For Model Extents
	if( State.TempBool1 )
	{
	//Search All Objects of Current Model
		for( UINT m = 0; m < pScene->pModel[index].Mesh.ObjectCount; m++ )
		{
		//Initialize Vertex Buffer 
			if( pScene->pModel[index].Mesh.pObject[m].VertexCount > VertexCount ) { ResizeBuffer( pScene->pModel[index].Mesh.pObject[m].VertexCount ); }

			ActiveVertices = pScene->pModel[index].Mesh.pObject[m].VertexCount;

			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();

		//Object Transformations
			glTranslatef( pScene->pModel[index].Mesh.pObject[m].Position[0], pScene->pModel[index].Mesh.pObject[m].Position[1], pScene->pModel[index].Mesh.pObject[m].Position[2] );

			glRotatef( pScene->pModel[index].Mesh.pObject[m].Rotation[0], 1, 0, 0 ); 
			glRotatef( pScene->pModel[index].Mesh.pObject[m].Rotation[1], 0, 1, 0 ); 
			glRotatef( pScene->pModel[index].Mesh.pObject[m].Rotation[2], 0, 0, 1 ); 

			glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );

		//Object Volume Checking
			if( ObjectInRegion( &pScene->pModel[index].Mesh.pObject[m], SelectIntersecting ) )
			{
				glPopMatrix();
				return TRUE;
			}
			glPopMatrix();
		}
	}

	return FALSE;
}

bool CSelection::ParseModels( UINT mode )
{
	if( ( !pScene ) || ( pScene->ModelCount == 0 ) ) { return FALSE; }

	UINT n, m;

	if( mode == SM_CURSOR_TEST_ALL )
	{
		for( n = 0; n < ActiveInfos; n++ ) { if( ModelAtCursor( pInfo[n].ObjectIndex ) ) { return TRUE; } }
		for( n = 0; n < pScene->ModelCount; n++ )
		{ 
			for( m = 0; m < ActiveInfos; m++ ) if( pInfo[m].ObjectIndex == n ) { continue; }

			if( ModelAtCursor( n ) ) { return TRUE; } 
		}

		return FALSE;
	}

	if( mode == SM_CURSOR_TEST_SELECTED )
	{
		for( n = 0; n < ActiveInfos; n++ ) { if( ModelAtCursor( pInfo[n].ObjectIndex ) ) { return TRUE; } }

		return FALSE;
	}

	StartIndex = 0;
	State.TempBool1 = FALSE;
	State.TempBool2 = FALSE;

	if( mode == SM_SELECT_MULTY ) { ClearEntries(); }

//Handle Single Select -> Selection Cycling
	if( ( mode == SM_SELECT_SINGLE ) )
	{
		if( ( State.Tool.Type == TOOL_EDIT ) && ( ActiveInfos == 1 ) && ModelInRegion( pInfo[0].ObjectIndex ) ) { return TRUE; }

		if( ( ActiveInfos == 1 ) && ModelInRegion( pInfo[0].ObjectIndex ) ) { StartIndex = pInfo[0].ObjectIndex + 1; State.TempBool2 = TRUE; }
		
		if( ActiveInfos > 1 ) { ClearEntries(); }

		if( StartIndex == pScene->ModelCount ) { StartIndex = 0; State.TempBool2 = FALSE; }
	}

//Search All Models
	for( n = StartIndex; n < pScene->ModelCount; n++ )
	{
		if( ModelInRegion( n ) )
		{
			if( mode == SM_SELECT_SINGLE )
			{
				ClearEntries();
				AddEntry( MODEL, n, (void*)&pScene->pModel[n], TRUE );
				State.TempBool1 = TRUE;
				return TRUE;
			}

			if( mode == SM_SELECT_MULTY ) { AddEntry( MODEL, n, (void*)&pScene->pModel[n], TRUE ); }

			if( mode == SM_ADD ) { if( FindEntry( MODEL, n, TRUE ) < 0 ) { AddEntry( MODEL, n, (void*)&pScene->pModel[n], TRUE ); } }

			if( mode == SM_MODIFY )
			{
				State.TempValue1 = (float)FindEntry( MODEL, n, TRUE );

				if( (int)State.TempValue1 >= 0 ) { RemoveEntry( (int)State.TempValue1 ); }
				else { AddEntry( MODEL, n, (void*)&pScene->pModel[n], TRUE ); }

				if( ActiveInfos > 0 ) { return TRUE; }
				else { return FALSE; }
			}
		}
	}

	if( ( mode == SM_SELECT_SINGLE ) && ( State.TempBool1 == FALSE ) )
	{
		for( UINT n = 0; n < StartIndex + 1; n++ )
		{
			if( ModelInRegion( n ) )
			{
				ClearEntries();
				AddEntry( MODEL, n, (void*)&pScene->pModel[n], TRUE );
				State.TempBool1 = TRUE;
				return TRUE;
			}
		}
	}

	if( ( mode == SM_SELECT_SINGLE ) && ( ActiveInfos == 1 ) && ( State.TempBool1 == FALSE ) ) { ClearEntries(); }

	if( ActiveInfos > 0 ) { return TRUE; } 
	return FALSE;
}


bool CSelection::ParseGizmos()
{
	if( VertexCount < 8 ) { ResizeBuffer( 8 ); }
	ActiveVertices = 8;

//	System.View[System.ViewActive].PrepareSelection();

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	UINT n;

	GizmoHighlighted = FALSE;

//Scale Objects ( Increase Accuracy )
	if( System.View[System.ViewActive].ID == VIEW_TOP )		{ glScalef( 1.0f,		0.0001f,	1.0f ); } 
	if( System.View[System.ViewActive].ID == VIEW_LEFT )	{ glScalef( 0.0001f,	1.0f,		1.0f ); } 
	if( System.View[System.ViewActive].ID == VIEW_FRONT )	{ glScalef( 1.0f,		1.0f,		0.0001f ); } 

//Gizmo Transformations
	glTranslated( GizmoPosition[0], GizmoPosition[1], GizmoPosition[2] );
	glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );

	State.TempVector3a[0] = System.View[System.ViewActive].GizmoSize;
	State.TempVector3a[1] = System.View[System.ViewActive].GizmoSize;
	State.TempVector3a[2] = System.View[System.ViewActive].GizmoSize;

//Test For Gizmo Extents
	if( BoxInRegion( &State.TempVector3a, TRUE ) )
	{
		if( State.Tool.ID == TOOL_EDIT_MOVE )
		{
			for( n = 0; n < ViewUI.MoveGizmo.SelectMesh.ObjectCount; n++ )
			{
				if( ObjectInRegion( &ViewUI.MoveGizmo.SelectMesh.pObject[n], TRUE ) )
				{
					glMatrixMode( GL_MODELVIEW );
					glPopMatrix();

					GizmoHighlighted = TRUE;

					if( ViewUI.MoveGizmo.ActiveObject == (int)n ) { return FALSE; }
					else
					{
						ViewUI.MoveGizmo.ActiveObject = n;
						return TRUE;
					}
				}
			}
		}

		if( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) )
		{
			for( n = 0; n < ViewUI.ScaleGizmo.SelectMesh.ObjectCount; n++ )
			{
				if( ObjectInRegion( &ViewUI.ScaleGizmo.SelectMesh.pObject[n], TRUE ) )
				{
					glMatrixMode( GL_MODELVIEW );
					glPopMatrix();

					GizmoHighlighted = TRUE;

					if( ViewUI.ScaleGizmo.ActiveObject == (int)n ) { return FALSE; }
					else
					{
						ViewUI.ScaleGizmo.ActiveObject = n;
						return TRUE;
					}
				}
			}
		}
	}

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	if( ViewUI.MoveGizmo.ActiveObject != -1 ) { ViewUI.MoveGizmo.ActiveObject = -1; return TRUE; }
	if( ViewUI.ScaleGizmo.ActiveObject != -1 ) { ViewUI.ScaleGizmo.ActiveObject = -1; return TRUE; }

	else { return FALSE; }
}

void CSelection::UpdateUI()
{
	if( !pScene ) { return; }

	ParseGizmos();
}

void CSelection::Update( UINT mode )
{
	if( ( mode == SM_CURSOR_TEST_ALL ) || ( mode == SM_CURSOR_TEST_SELECTED ) )
	{
		if( !pScene ) { return; }

		if( ParseModels( mode ) )

		return;
	}

	if( !Options.LockSelection )
	{
		State.hTempCursor = SetCursor( LoadCursor( NULL, IDC_WAIT ) );

		if( !pScene ) { return; }

		if( ParseModels( mode ) )
		{ 
			GizmoVisible = TRUE;
			BoxesVisible = TRUE;
			
			UpdateCenter();
		}
		else{ GizmoVisible = FALSE; BoxesVisible = FALSE; }	

		UpdateInfoDialog(); 
		UpdateTypeInDialog( FALSE ); 

		SetCursor( State.hTempCursor );
	}

}

bool CSelection::PointInRegion( Float3 *point )
{
//PointCount Refers to Region Points
	for( UINT n = 0; n < ActivePoints - 1; n++ )
	{
		if( ( pPlane[n][0] * point[0][0] + pPlane[n][1] * point[0][1] + pPlane[n][2] * point[0][2] + pPlane[n][3] ) < 0 ) { return FALSE; }
	}

	return TRUE;
}
bool CSelection::BoxAtCursor( Float3 *size )
{
	TempBoxMesh.pObject[0].pVertex[0] = - size[0][0];
	TempBoxMesh.pObject[0].pVertex[1] = size[0][1];
	TempBoxMesh.pObject[0].pVertex[2] = - size[0][2];

	TempBoxMesh.pObject[0].pVertex[3] = size[0][0];
	TempBoxMesh.pObject[0].pVertex[4] = size[0][1];
	TempBoxMesh.pObject[0].pVertex[5] = - size[0][2];

	TempBoxMesh.pObject[0].pVertex[6] = size[0][0];
	TempBoxMesh.pObject[0].pVertex[7] = size[0][1];
	TempBoxMesh.pObject[0].pVertex[8] = size[0][2];

	TempBoxMesh.pObject[0].pVertex[9] = - size[0][0];
	TempBoxMesh.pObject[0].pVertex[10] = size[0][1];
	TempBoxMesh.pObject[0].pVertex[11] = size[0][2];


	TempBoxMesh.pObject[0].pVertex[12] = - size[0][0];
	TempBoxMesh.pObject[0].pVertex[13] = - size[0][1];
	TempBoxMesh.pObject[0].pVertex[14] = - size[0][2];

	TempBoxMesh.pObject[0].pVertex[15] = size[0][0];
	TempBoxMesh.pObject[0].pVertex[16] = - size[0][1];
	TempBoxMesh.pObject[0].pVertex[17] = - size[0][2];

	TempBoxMesh.pObject[0].pVertex[18] = size[0][0];
	TempBoxMesh.pObject[0].pVertex[19] = - size[0][1];
	TempBoxMesh.pObject[0].pVertex[20] = size[0][2];

	TempBoxMesh.pObject[0].pVertex[21] = - size[0][0];
	TempBoxMesh.pObject[0].pVertex[22] = - size[0][1];
	TempBoxMesh.pObject[0].pVertex[23] = size[0][2];

//Initialize Vertex Buffer 
	if( VertexCount < 8 ) { ResizeBuffer( 8 ); }
	ActiveVertices = 8;

	if( ObjectAtCursor( &TempBoxMesh.pObject[0] ) ) { return TRUE; }

	return FALSE;
}

bool CSelection::BoxInRegion( Float3 *size, bool selectIntersecting )
{
	TempBoxMesh.pObject[0].pVertex[0] = - size[0][0];
	TempBoxMesh.pObject[0].pVertex[1] = size[0][1];
	TempBoxMesh.pObject[0].pVertex[2] = - size[0][2];

	TempBoxMesh.pObject[0].pVertex[3] = size[0][0];
	TempBoxMesh.pObject[0].pVertex[4] = size[0][1];
	TempBoxMesh.pObject[0].pVertex[5] = - size[0][2];

	TempBoxMesh.pObject[0].pVertex[6] = size[0][0];
	TempBoxMesh.pObject[0].pVertex[7] = size[0][1];
	TempBoxMesh.pObject[0].pVertex[8] = size[0][2];

	TempBoxMesh.pObject[0].pVertex[9] = - size[0][0];
	TempBoxMesh.pObject[0].pVertex[10] = size[0][1];
	TempBoxMesh.pObject[0].pVertex[11] = size[0][2];


	TempBoxMesh.pObject[0].pVertex[12] = - size[0][0];
	TempBoxMesh.pObject[0].pVertex[13] = - size[0][1];
	TempBoxMesh.pObject[0].pVertex[14] = - size[0][2];

	TempBoxMesh.pObject[0].pVertex[15] = size[0][0];
	TempBoxMesh.pObject[0].pVertex[16] = - size[0][1];
	TempBoxMesh.pObject[0].pVertex[17] = - size[0][2];

	TempBoxMesh.pObject[0].pVertex[18] = size[0][0];
	TempBoxMesh.pObject[0].pVertex[19] = - size[0][1];
	TempBoxMesh.pObject[0].pVertex[20] = size[0][2];

	TempBoxMesh.pObject[0].pVertex[21] = - size[0][0];
	TempBoxMesh.pObject[0].pVertex[22] = - size[0][1];
	TempBoxMesh.pObject[0].pVertex[23] = size[0][2];

//Initialize Vertex Buffer 
	if( VertexCount < 8 ) { ResizeBuffer( 8 ); }
	ActiveVertices = 8;

	if( ObjectInRegion( &TempBoxMesh.pObject[0], selectIntersecting ) ) { return TRUE; }

	return FALSE;
}

bool CSelection::ObjectAtCursor( tObject *pobject )
{
	for( UINT n = 0; n < pobject->VertexCount; n++ )
	{
		Math.DataToVector3(&State.TempVector3e, &pobject->pVertex[n * 3]);

		Math.MultiplyMatrix4Vector3((Matrix4*)&ModelMatrix[0], &State.TempVector3e, &pVertex[n]);
	}

	//Calculate Triangle x Cursor Vector
	for( UINT p = 0; p < pobject->TriangleCount; p++ )
	{
		//if (CrossPointVectorTriangleExists(State.Mouse[1].StartPoint3D, State.Mouse[1].EndPoint3D, pVertex[pobject->pTriangleIndice[p * 3]], pVertex[pobject->pTriangleIndice[p * 3 + 1]], pVertex[pobject->pTriangleIndice[p * 3 + 2]])) { return TRUE; }
	}

	return FALSE;
}

bool CSelection::ObjectInRegion( tObject *pobject, bool selectIntersecting )
{
	if( selectIntersecting )
	{
		//Select if Any Vertex In Region
		for( UINT n = 0; n < pobject->VertexCount; n++ )
		{
			Math.DataToVector3( &State.TempVector3e, &pobject->pVertex[n * 3] );

			Math.MultiplyMatrix4Vector3((Matrix4*)&ModelMatrix[0], &State.TempVector3e, &pVertex[n] );

			if( PointInRegion( &pVertex[n] ) ) { return TRUE; }
		}

		//Calculate Triangle / Selection Volume Intersection
		if( State.View.Type == VT_PERSPECTIVE ) 
		{
			for( UINT p = 0; p < pobject->TriangleCount; p++ )
			{
				for( UINT r = 0; r < ActivePoints - 1; r++ )
				{
					if (IntersectionTriangle(pVertex[pobject->pTriangleIndice[p * 3]], pVertex[pobject->pTriangleIndice[p * 3 + 1]], pVertex[pobject->pTriangleIndice[p * 3 + 2]], State.View.Position, pPoint[r], pPoint[r + 1])) { return TRUE; }
				}
			}
		}
		else
		{
			for( UINT p = 0; p < pobject->TriangleCount; p++ )
			{
				for( UINT r = 0; r < ActivePoints - 1; r++ )
				{
					if (IntersectionTriangle(pVertex[pobject->pTriangleIndice[p * 3]], pVertex[pobject->pTriangleIndice[p * 3 + 1]], pVertex[pobject->pTriangleIndice[p * 3 + 2]], pPointReflected[r], pPoint[r], pPoint[r + 1])) { return TRUE; }
					if (IntersectionTriangle(pVertex[pobject->pTriangleIndice[p * 3]], pVertex[pobject->pTriangleIndice[p * 3 + 1]], pVertex[pobject->pTriangleIndice[p * 3 + 2]], pPointReflected[r + 1], pPoint[r], pPoint[r + 1])) { return TRUE; }
				}
			}
		}
	}
	else
	{
		//Select if All Vertices In Region
		for( UINT n = 0; n < pobject->VertexCount; n++ )
		{
			Math.DataToVector3( &State.TempVector3e, &pobject->pVertex[n * 3] );

			Math.MultiplyMatrix4Vector3((Matrix4*)&ModelMatrix[0], &State.TempVector3e, &pVertex[n] );

			if( !PointInRegion( &pVertex[n] ) ) { return FALSE; }
		}

		return TRUE;
	}

	return FALSE;
}

void CSelection::UpdateTypeInDialog( bool offSet )
{
	if( !pScene ) { return; }

	if( ( State.Tool.Type != TOOL_EDIT ) || ( ActiveInfos == 0 ) )
	{
		EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_X ), FALSE );
		EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Y ), FALSE );
		EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Z ), FALSE );

		ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_X_SPIN ), FALSE );
		ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Y_SPIN ), FALSE );
		ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Z_SPIN ), FALSE );

		SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_X_SPIN ), ASM_SET_VAL_FLOAT, 0, NULL );
		SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, NULL );
		SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_Z_SPIN ), ASM_SET_VAL_FLOAT, 0, NULL );

		return;
	}

	if( offSet )
	{
		EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_X ), TRUE );
		EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Y ), TRUE );
		EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Z ), TRUE );

		SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_X_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[0] );
		SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[1] );
		SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_Z_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[2] );
	
		return;
	}
	
	if( SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_LOCAL ), BM_IS_PUSHED, 0, 0 ) )
	{
		EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_X ), TRUE );
		EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Y ), TRUE );
		EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Z ), TRUE );

		ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_X_SPIN ), TRUE );
		ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Y_SPIN ), TRUE );
		ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Z_SPIN ), TRUE );

		if( State.Tool.ID == TOOL_EDIT_MOVE ) { Math.ZeroVector3( &State.TransformInfo ); }

		if( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) ) { Math.SetVector3( &State.TransformInfo, 100, 100, 100 ); }
	}
	else
	{
		if( ( ActiveInfos == 1 ) )
		{
			EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_X ), TRUE );
			EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Y ), TRUE );
			EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Z ), TRUE );

			ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_X_SPIN ), TRUE );
			ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Y_SPIN ), TRUE );
			ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Z_SPIN ), TRUE );

			if( State.Tool.ID == TOOL_EDIT_MOVE ) { Math.CopyVector3( &State.TransformInfo, &System.Selection.GizmoPosition ); }

			if( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) )
			{
				if( System.Selection.pInfo[0].ObjectType == MODEL )
				{
					CModel *pModel = (CModel*)System.Selection.pInfo[0].pObject;

					Math.CopyVector3( &State.TransformInfo, &pModel->Scale );
					Math.ScaleVector3( &State.TransformInfo, 100, &State.TransformInfo ); 
				}
			}
		}

		if( ( ActiveInfos > 1 ) )
		{
/*			if( State.Tool.ID == TOOL_EDIT_MOVE )
			{
				EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_X ), TRUE );
				EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Y ), TRUE );
				EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Z ), TRUE );

				ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_X_SPIN ), TRUE );
				ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Y_SPIN ), TRUE );
				ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Z_SPIN ), TRUE );

				Math.CopyVector3( &State.TransformInfo, &System.Selection.GizmoPosition );
			}*/

			if( ( State.Tool.ID == TOOL_EDIT_MOVE ) || ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) )
			{
				EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_X ), FALSE );
				EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Y ), FALSE );
				EnableWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Z ), FALSE );

				ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_X_SPIN ), FALSE );
				ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Y_SPIN ), FALSE );
				ShowWindow( BottomToolBar.GetControl( IDC_TRANSFORM_Z_SPIN ), FALSE );

				Math.ZeroVector3( &State.TransformInfo );
			}
		}
	}

	SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_X_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[0] );
	SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[1] );
	SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_Z_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[2] );
}

void CSelection::UpdateInfoDialog()
{
	if( !pScene ) { return; }

	if( !MainBar.IsGroupDialog( -1, IDD_OBJECT_INFO ) || !IsWindowVisible( MainBar.GetGroupDialog( -1, IDD_OBJECT_INFO ) ) ) { return; }

	if( ( ActiveInfos == 1 ) )
	{
		CModel *pModel = (CModel*)System.Selection.pInfo[0].pObject; 

		SetDlgItemText( MainBar.GetGroupDialog( -1, IDD_OBJECT_INFO ), IDC_OBJECT_INFO_NAME, pModel->Name );  

		EnableWindow( MainBar.GetGroupControl( -1, IDD_OBJECT_INFO, IDC_OBJECT_INFO_NAME ), TRUE );

		tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( MainBar.GetGroupControl( 0, IDD_OBJECT_INFO, IDC_OBJECT_INFO_COLOR ), GWL_USERDATA );
		if( pInfo )
		{
			pInfo->Color = pModel->Color; 
			RedrawWindow( MainBar.GetGroupControl( 0, IDD_OBJECT_INFO, IDC_OBJECT_INFO_COLOR ), NULL, NULL, RDW_INVALIDATE );
		}
	}

	if( ( ActiveInfos == 0 ) )
	{
		EnableWindow( MainBar.GetGroupControl( -1, IDD_OBJECT_INFO, IDC_OBJECT_INFO_NAME ), FALSE );
	}

	if( ( ActiveInfos > 1 ) )
	{
		EnableWindow( MainBar.GetGroupControl( -1, IDD_OBJECT_INFO, IDC_OBJECT_INFO_NAME ), FALSE );

		CModel *pModel = (CModel*)System.Selection.pInfo[0].pObject; 

		tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( MainBar.GetGroupControl( 0, IDD_OBJECT_INFO, IDC_OBJECT_INFO_COLOR ), GWL_USERDATA );
		if( pInfo )
		{
			pInfo->Color = pModel->Color; 
			RedrawWindow( MainBar.GetGroupControl( 0, IDD_OBJECT_INFO, IDC_OBJECT_INFO_COLOR ), NULL, NULL, RDW_INVALIDATE );
		}
	}
}


bool CSelection::ModelAtCursor( UINT index )
{
	if( !pScene ) { return FALSE; }

//Range Checking
	if( index >= pScene->ModelCount ) { return FALSE; }

//Clipping
	if( ( ClipNear[0] * ( pScene->pModel[index].Position[0] + pScene->pModel[index].BoxPosition[0] + pScene->pModel[index].BoxSize[0] ) + ClipNear[1] * ( pScene->pModel[index].Position[1] + pScene->pModel[index].BoxPosition[1] + pScene->pModel[index].BoxSize[1] ) + ClipNear[2] * ( pScene->pModel[index].Position[2] + pScene->pModel[index].BoxPosition[2] + pScene->pModel[index].BoxSize[2] ) + ClipNear[3] ) < 0 ) { return FALSE; }
	if( ( ClipFar[0]  * ( pScene->pModel[index].Position[0] + pScene->pModel[index].BoxPosition[0] + pScene->pModel[index].BoxSize[0] ) + ClipFar[1]  * ( pScene->pModel[index].Position[1] + pScene->pModel[index].BoxPosition[1] + pScene->pModel[index].BoxSize[1] ) + ClipFar[2]  * ( pScene->pModel[index].Position[2] + pScene->pModel[index].BoxPosition[2] + pScene->pModel[index].BoxSize[2] ) + ClipFar[3]  ) < 0 ) { return FALSE; }

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

//Scale Objects ( Increase Accuracy )
	if( System.View[System.ViewActive].ID == VIEW_TOP )		{ glScalef( 1.0f,		0.0001f,	1.0f ); } 
	if( System.View[System.ViewActive].ID == VIEW_LEFT )	{ glScalef( 0.0001f,	1.0f,		1.0f ); } 
	if( System.View[System.ViewActive].ID == VIEW_FRONT )	{ glScalef( 1.0f,		1.0f,		0.0001f ); } 

//Bounding Box Transformations
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	glTranslatef( pScene->pModel[index].Position[0], pScene->pModel[index].Position[1], pScene->pModel[index].Position[2] );
//	glScalef( pScene->pModel[index].Scale[0], pScene->pModel[index].Scale[1], pScene->pModel[index].Scale[2] ); 
	glTranslatef( pScene->pModel[index].BoxPosition[0], pScene->pModel[index].BoxPosition[1], pScene->pModel[index].BoxPosition[2] );

	glRotatef( pScene->pModel[index].Rotation[0], 1, 0, 0 ); 
	glRotatef( pScene->pModel[index].Rotation[1], 0, 1, 0 ); 
	glRotatef( pScene->pModel[index].Rotation[2], 0, 0, 1 ); 

	glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );

	State.TempBool1 = BoxInRegion( &pScene->pModel[index].BoxSize, TRUE );

	glPopMatrix();

//Model Transformations
	glTranslatef( pScene->pModel[index].Position[0], pScene->pModel[index].Position[1], pScene->pModel[index].Position[2] );

	glRotatef( pScene->pModel[index].Rotation[0], 1, 0, 0 ); 
	glRotatef( pScene->pModel[index].Rotation[1], 0, 1, 0 ); 
	glRotatef( pScene->pModel[index].Rotation[2], 0, 0, 1 ); 

	glScalef( pScene->pModel[index].Scale[0], pScene->pModel[index].Scale[1], pScene->pModel[index].Scale[2] ); 

	glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );

//Test For Model Extents
	if( State.TempBool1 )
	{
	//Search All Objects of Current Model
		for( UINT m = 0; m < pScene->pModel[index].Mesh.ObjectCount; m++ )
		{
		//Initialize Vertex Buffer 
			if( pScene->pModel[index ].Mesh.pObject[m].VertexCount > VertexCount ) { ResizeBuffer( pScene->pModel[index ].Mesh.pObject[m].VertexCount ); }

			ActiveVertices = pScene->pModel[index].Mesh.pObject[m].VertexCount;

			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();

		//Object Transformations
			glTranslatef( pScene->pModel[index ].Mesh.pObject[m].Position[0], pScene->pModel[index ].Mesh.pObject[m].Position[1], pScene->pModel[index ].Mesh.pObject[m].Position[2] );

			glRotatef( pScene->pModel[index ].Mesh.pObject[m].Rotation[0], 1, 0, 0 ); 
			glRotatef( pScene->pModel[index ].Mesh.pObject[m].Rotation[1], 0, 1, 0 ); 
			glRotatef( pScene->pModel[index ].Mesh.pObject[m].Rotation[2], 0, 0, 1 ); 

			glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );

		//Object Volume Checking
			if( ObjectAtCursor( &pScene->pModel[index].Mesh.pObject[m] ) )
			{
				glPopMatrix();
				return TRUE;
			}
			glPopMatrix();
		}
	}

	return FALSE;
}



void CSelection::DrawBox( UINT index )
{
	glPushAttrib( GL_ENABLE_BIT | GL_TRANSFORM_BIT );

	glDisable( GL_CULL_FACE );
	glDisable( GL_LIGHTING );
	glDisable( GL_FOG );
	glDisable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );

	glColor3d( 1, 1, 1 );

	glBegin( GL_LINES );

	CModel * pModel = (CModel*)pInfo[index].pObject;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      TOP                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glVertex3d( pModel->BoxSize[0],
				pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0] - pModel->BoxSize[0] / 4,
				pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				pModel->BoxSize[1] - pModel->BoxSize[1] / 4,
				pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				pModel->BoxSize[1],
				pModel->BoxSize[2] - pModel->BoxSize[2] / 4 );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glVertex3d( -pModel->BoxSize[0],
				pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0] + pModel->BoxSize[0] / 4,
				pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				pModel->BoxSize[1] - pModel->BoxSize[1] / 4,
				pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				pModel->BoxSize[1],
				pModel->BoxSize[2] - pModel->BoxSize[2] / 4 );


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glVertex3d( pModel->BoxSize[0],
				pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0] - pModel->BoxSize[0] / 4,
				pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				pModel->BoxSize[1] - pModel->BoxSize[1] / 4,
				-pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				pModel->BoxSize[1],
				-pModel->BoxSize[2] + pModel->BoxSize[2] / 4 );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glVertex3d( -pModel->BoxSize[0],
				pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0] + pModel->BoxSize[0] / 4,
				pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				pModel->BoxSize[1] - pModel->BoxSize[1] / 4,
				-pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				pModel->BoxSize[1],
				-pModel->BoxSize[2] + pModel->BoxSize[2] / 4 );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                    BOTTOM                                                     //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glVertex3d( pModel->BoxSize[0],
				-pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0] - pModel->BoxSize[0] / 4,
				-pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				-pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				-pModel->BoxSize[1] + pModel->BoxSize[1] / 4,
				pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				-pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				-pModel->BoxSize[1],
				pModel->BoxSize[2] - pModel->BoxSize[2] / 4 );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glVertex3d( -pModel->BoxSize[0],
				-pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0] + pModel->BoxSize[0] / 4,
				-pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				-pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				-pModel->BoxSize[1] + pModel->BoxSize[1] / 4,
				pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				-pModel->BoxSize[1],
				pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				-pModel->BoxSize[1],
				pModel->BoxSize[2] - pModel->BoxSize[2] / 4 );


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glVertex3d( pModel->BoxSize[0],
				-pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0] - pModel->BoxSize[0] / 4,
				-pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				-pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				-pModel->BoxSize[1] + pModel->BoxSize[1] / 4,
				-pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				-pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( pModel->BoxSize[0],
				-pModel->BoxSize[1],
				-pModel->BoxSize[2] + pModel->BoxSize[2] / 4 );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	glVertex3d( -pModel->BoxSize[0],
				-pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0] + pModel->BoxSize[0] / 4,
				-pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				-pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				-pModel->BoxSize[1] + pModel->BoxSize[1] / 4,
				-pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				-pModel->BoxSize[1],
				-pModel->BoxSize[2] );

	glVertex3d( -pModel->BoxSize[0],
				-pModel->BoxSize[1],
				-pModel->BoxSize[2] + pModel->BoxSize[2] / 4 );
	
	
	glEnd();

	glPopAttrib();
}

