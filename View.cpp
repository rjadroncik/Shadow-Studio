 // View.cpp: implementation of the CView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "View.h"
#include "ViewUI.h"
#include "System.h"
#include "State.h"
#include "Shadow.h"
#include "Options.h"
#include "Afc.h"

#include "Resource.h"
#include "../System/Math.h"
#include "../System/JPEGlib.h"

extern CState				State; 
extern COptions				Options; 
extern CScene				*pScene; 
extern CMath				Math; 
extern CSystem				System;
extern CViewUI				ViewUI;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CView::CView()
{

}

CView::~CView()
{

}

bool CView::Create( UINT type, UINT id, HGLRC *pRc, LPCTSTR className, HINSTANCE hInstance, HWND parent, UINT index )
{
//Make us a Window 
	hWnd = CreateWindowEx( /*WS_EX_STATICEDGE*/WS_EX_CLIENTEDGE, className, "WindowGL", WS_CHILD, 0, 0, 100, 100, parent, NULL, hInstance, NULL );
	hDC = GetDC( hWnd );
	ID = id;
	Type = type;
	GridVisible = TRUE;
	pRC = pRc; 
	Active = FALSE;
	Aspect = 1; 

	SetWindowLong( hWnd, GWL_USERDATA, index );

//Check if the Window was Created
	if( !hWnd ) { MessageBox( NULL, "CView::Create()", "Error A1", MB_ICONERROR ); return FALSE; }

	if( type == VT_ORTHO )
	{
		Size = 100.0;
		ClipFar = Options.ViewDistance;
		ClipNear = -Options.ViewDistance;

		Focus[0] = 0.0;
		Focus[1] = 0.0;
		Focus[2] = -1.0;

		Position[0] =  0.0;
		Position[1] =  0.0;
		Position[2] =  0.0;

		Math.MakeVector3( &Position, &Focus, &Direction );

		Math.NormalizeVector3( &Direction, &Direction );
		
		UpVector[0] = 0.0;
		UpVector[1] = 1.0;
		UpVector[2] = 0.0;

		RenderMode = RM_WIREFRAME;
	
		Math.CrossVectors3( &Direction, &UpVector, &RightVector ); 

		Math.MakePlane( &Position, &Direction, &CreationPlane );
	}

	if( type == VT_PERSPECTIVE )
	{
	//Rendering Information
		ClipFar = Options.ViewDistance;
		ClipNear = 1.0;
	
		FOV = 45.0f;
		RenderMode = RM_SHADED;

	//Transformation Information
		Focus[0] = 0.0;
		Focus[1] = 0.0;
		Focus[2] = -1.0;

		Position[0] =  0.0;
		Position[1] =  0.0;
		Position[2] =  0.0;

		Math.MakeVector3( &Position, &Focus, &Direction );
	
		Math.NormalizeVector3( &Direction, &Direction );

		Distance = 1.0; 

		UpVector[0] = 0.0;
		UpVector[1] = 1.0;
		UpVector[2] = 0.0;

		Math.CrossVectors3( &Direction, &UpVector, &RightVector ); 

		CreationPlane[0] = 0.0f;
		CreationPlane[1] = 1.0f;
		CreationPlane[2] = 0.0f;
		CreationPlane[3] = 0.0f;
	}

//Make Window Visible 
	ShowWindow( hWnd, SW_SHOW );	
	return TRUE;
}


void CView::PrepareSelection()
{
	wglMakeCurrent( hDC, *pRC );
	glRenderMode( GL_RENDER );

	switch( Type )
	{
	case VT_ORTHO:
		{
		//Set all the "Orthographic Specific" OpenGL Parameters

			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();

			if( Aspect != 0 ) { glOrtho( -Size * Aspect, Size * Aspect, -Size, Size, ClipNear * 0.0001f, ClipFar * 0.0001f ); }

			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();

			glRotated( Rotation[0], 1, 0, 0 ); 
			glRotated( Rotation[1], 0, 1, 0 ); 
			glRotated( Rotation[2], 0, 0, 1 ); 

			glTranslated( Position[0], Position[1], Position[2] ); 

			break;
		}

	case VT_PERSPECTIVE:
		{
		//Set all the "Perspective Specific" OpenGL Parameters

			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
	
			gluPerspective( FOV, Aspect, ClipNear, ClipFar );
	
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();

			gluLookAt(	Position[0], Position[1], Position[2],
						Focus[0],	Focus[1],	Focus[2],
						UpVector[0], UpVector[1], UpVector[2] );

			break;
		}
	}
}

bool CView::Initialize()
{
	wglMakeCurrent( hDC, *pRC );

	glRenderMode( GL_RENDER );
	glClearColor( 0.556f, 0.553f, 0.55f, 1.0f ); 
	glClearDepth( 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	switch( RenderMode )
	{
	case RM_WIREFRAME:
		{
		//Set all the basic OpenGL Parameters
			glDisable( GL_TEXTURE_2D );
			glShadeModel( GL_FLAT ); 
			glEnable( GL_DEPTH_TEST ); 
			glDisable( GL_BLEND );
 			glDepthFunc( GL_LEQUAL );
			glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST ); 
			glDisable( GL_LIGHTING );
			glDisable( GL_FOG );
			glEnable( GL_COLOR_MATERIAL );
			glEnable( GL_CULL_FACE );
			break;
		}

	case RM_SHADED:
		{
		//Set all the basic OpenGL Parameters
			glEnable( GL_TEXTURE_2D );
//			glEnable( GL_TEXTURE_CUBE_MAP );

			
			glShadeModel( GL_SMOOTH ); 
			glEnable( GL_DEPTH_TEST ); 
			glDisable( GL_BLEND );

			glDepthFunc( GL_LEQUAL );
			glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST ); 

//			glFogi( GL_FOG_MODE, GL_LINEAR );
//			glFogf( GL_FOG_DENSITY, 1.35f );
//			glHint( GL_FOG_HINT, GL_DONT_CARE );
//			glFogf( GL_FOG_START, 5.0f );
//			glFogf( GL_FOG_END, 100.0f );
//			glFogfv( GL_FOG_COLOR, farbahmly );
//			glEnable( GL_FOG );

			glEnable( GL_COLOR_MATERIAL );
			glEnable( GL_CULL_FACE );
			glEnable( GL_NORMALIZE );
			glEnable( GL_LIGHTING );

			break;
		}
	}


	switch( Type )
	{
	case VT_ORTHO:
		{
		//Set all the "Orthographic Specific" OpenGL Parameters

			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();

			if( Aspect != 0 ) { glOrtho( -Size * Aspect, Size * Aspect, -Size, Size, ClipNear, ClipFar ); }

			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();

			glRotated( Rotation[0], 1, 0, 0 ); 
			glRotated( Rotation[1], 0, 1, 0 ); 
			glRotated( Rotation[2], 0, 0, 1 ); 

			glTranslated( Position[0], Position[1], Position[2] ); 

			break;
		}

	case VT_PERSPECTIVE:
		{
		//Set all the "Perspective Specific" OpenGL Parameters

			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
	
			gluPerspective( FOV, Aspect, ClipNear, ClipFar );
	
			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();

			gluLookAt(	Position[0], Position[1], Position[2],
						Focus[0],	 Focus[1],	  Focus[2],
						UpVector[0], UpVector[1], UpVector[2] );

			break;
		}
	}

	return TRUE;
}


void CView::Destroy()
{
//Free DC Memory
	if( hDC && !ReleaseDC( hWnd, hDC ) )
	{
		MessageBox( NULL, "CSystem::DestroyWindowGL()", "Error B1", MB_ICONERROR );
		hDC = NULL;
	}

//Free Window Memory
	if( hWnd && !DestroyWindow( hWnd ) ) 
	{
		MessageBox( NULL, "CSystem::DestroyWindowGL()", "Error B2", MB_ICONERROR );
		hWnd = NULL;
	}
}

void CView::DrawGrid()
{
	glPushAttrib( GL_ENABLE_BIT | GL_TRANSFORM_BIT );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	glDisable( GL_LIGHTING );

	if( Type == VT_PERSPECTIVE ) { glEnable( GL_DEPTH_TEST ); pScene->PerspectGrid.Mesh.Render( RENDER_FACES ); }

	if( Type == VT_ORTHO )
	{
		glDisable( GL_DEPTH_TEST );

		State.TempValue1 = 0.0625f; 
		State.TempVector2a[0] = 3.125f;
		State.TempVector2a[1] = 12.5f;
		State.TempBool1 = FALSE;

	//Calculate Size	
		while( !State.TempBool1 )
		{
			if( ( Size >= State.TempVector2a[0] ) && ( Size < State.TempVector2a[1] ) ) { glScalef( State.TempValue1, State.TempValue1, State.TempValue1 ); State.TempBool1 = TRUE; }

			if( !State.TempBool1 ) { State.TempValue1 *= 4.0f; State.TempVector2a[0] *= 4.0f; State.TempVector2a[1] *= 4.0f; }
		}

	//Calculate Position	
		State.TempValue2 = (float)(int)( Position[0] / ( 250.0f * State.TempValue1 ) );

		if( ( (int)State.TempValue2 % 2 ) == 0 ) { glTranslatef( - State.TempValue2 * 250.0f, 0, 0 ); }
		else
		{					
			if( State.TempValue2 > 0 ) { glTranslatef( - State.TempValue2 * 250.0f - 250, 0, 0 ); }
			if( State.TempValue2 < 0 ) { glTranslatef( - State.TempValue2 * 250.0f + 250, 0, 0 ); }
		}

		State.TempValue2 = (float)(int)( Position[1] / ( 250 * State.TempValue1 ) );

		if( ( (int)State.TempValue2 % 2 ) == 0 ) { glTranslatef( 0, - State.TempValue2 * 250, 0 ); }
		else
		{
			if( State.TempValue2 > 0 ) { glTranslatef( 0, - State.TempValue2 * 250.0f - 250, 0 ); }
			if( State.TempValue2 < 0 ) { glTranslatef( 0, - State.TempValue2 * 250.0f + 250, 0 ); }
		}
		
		State.TempValue2 = (float)(int)( Position[2] / ( 250 * State.TempValue1 ) );

		if( ( (int)State.TempValue2 % 2 ) == 0 ) { glTranslatef( 0, 0, - State.TempValue2 * 250 ); }
		else
		{
			if( State.TempValue2 > 0 ) { glTranslatef( 0, 0, - State.TempValue2 * 250.0f - 250 ); } 
			if( State.TempValue2 < 0 ) { glTranslatef( 0, 0, - State.TempValue2 * 250.0f + 250 ); } 
		}

	//Render Appropriate Grid
		if( ID == VIEW_FRONT ) { glRotatef( 90, 1, 0, 0 ); 
		
			if( pScene->ModelCount == 45 )
			{
			
				State.FileReady = TRUE;


			}

			pScene->OrthoGrid.Mesh.Render( RENDER_FACES ); 
		}
		
		if( ID == VIEW_TOP ) { pScene->OrthoGrid.Mesh.Render( RENDER_FACES ); }

		if( ID == VIEW_LEFT ) { glRotatef( 90, 0, 0, 1 ); pScene->OrthoGrid.Mesh.Render( RENDER_FACES ); }
	}

	glPopMatrix();
	glPopAttrib();
}

void CView::DrawGizmos()
{
	if( System.Selection.InfoCount > 0 )
	{
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
//		glLoadIdentity();
	
	//Calculate Size of the Gizmo
		State.TempBool1 = TRUE;

	//Position The Gizmo
		glTranslated( System.Selection.GizmoPosition[0], System.Selection.GizmoPosition[1], System.Selection.GizmoPosition[2] );

	//Render Move Gizmo
		if( ( State.Tool.ID == TOOL_EDIT_MOVE ) && ( System.Selection.GizmoVisible ) )
		{ 
			ViewUI.MoveGizmo.Render( GizmoSize );
			State.TempBool1 = FALSE;  
		}

	//Render Scale Gizmo
		if( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) && ( System.Selection.GizmoVisible ) )
		{ 
			ViewUI.ScaleGizmo.Render( GizmoSize );
			State.TempBool1 = FALSE;  
		}

	//Draw Selection GizmoPosition
		if( ( System.Selection.GizmoVisible ) && State.TempBool1 )
		{ 
			pScene->DrawAxisGizmo( GizmoSize, 0, FALSE );	
		}

		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();
	}
}

void CView::UpdateGizmos()
{
	if(	Type == VT_PERSPECTIVE )
	{
		Math.MakeVector3( &System.Selection.GizmoPosition, &Position, &State.TempVector3d );
		State.TempValue1 = Math.MeasureVector3( &State.TempVector3d );

		if( System.FourViews ) { GizmoSize = (float)( 0.12 * State.TempValue1 * ( FOV / 45.0f ) ); }
		else { GizmoSize = (float)( 0.06 * State.TempValue1 ); } 
	}
	else
	{
		if( System.FourViews ) { GizmoSize = (float)( 0.3 * Size ); }
		else { GizmoSize = (float)( 0.15 * Size ); }
	}

	GizmoSize *= Options.GizmoSize;

	if( State.Tool.ID == TOOL_EDIT_MOVE ) { ViewUI.MoveGizmo.Resize( GizmoSize ); }
	if( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) ) { ViewUI.ScaleGizmo.Resize( GizmoSize ); }
}

void CView::Update( bool force )
{
	if( !force && !NeedUpdate ) { return; }
	NeedUpdate = FALSE;
		
	UINT	index = GetWindowLong( hWnd, GWL_USERDATA );

//Prepare OpenGL for the Given View
	Initialize(); 

//Render World
	if( !State.FileReady ) { return; }

	if( GridVisible ) { DrawGrid(); }

	if( Options.UseFlashLight )
	{
		glEnable( GL_LIGHT0 );

		glLighti( GL_LIGHT0, GL_SPOT_CUTOFF, 45 ); 

	//Using W = 0 Creates a Directional Light Positioned at Infinity in the Given Direction, Oriended in the Given Direction, 
	//Direction is Threated Regardless Whether you Give the Inverse Direction or Not, But this Affects the Light Position 	
		State.TempVector4a[0] = -(float)Direction[0];
		State.TempVector4a[1] = -(float)Direction[1];
		State.TempVector4a[2] = -(float)Direction[2];
		State.TempVector4a[3] = 0;

		glLightfv( GL_LIGHT0, GL_POSITION, State.TempVector4a );
	}

//Render Models
	pScene->Render( Type, ID, RenderMode );

//Render Selection Boxes
	if( ( System.Selection.ActiveInfos > 0 ) && ( System.Selection.BoxesVisible == TRUE ) && ( RenderMode != RM_WIREFRAME ) )
	{
		for( UINT g = 0; g < System.Selection.ActiveInfos; g++ )
		{ 
			if( ( System.Selection.pInfo[g].ObjectType == MODEL ) && ( System.Selection.pInfo[g].DisplayBox ) )
			{
				glMatrixMode( GL_MODELVIEW );
				glPushMatrix();

				CModel *pModel = (CModel*)System.Selection.pInfo[g].pObject;

				glTranslatef( pModel->Position[0], pModel->Position[1], pModel->Position[2] );
//				glScalef( pModel->Scale[0], pModel->Scale[1], pModel->Scale[2] ); 
				glTranslatef( pModel->BoxPosition[0], pModel->BoxPosition[1], pModel->BoxPosition[2] );
			
				glRotatef( pModel->Rotation[0], 1, 0, 0 ); 
				glRotatef( pModel->Rotation[1], 0, 1, 0 ); 
				glRotatef( pModel->Rotation[2], 0, 0, 1 ); 
			
			
				System.Selection.DrawBox( g );			

				glPopMatrix();
			}
		}		
	}

	if( Options.DrawOrigins && ( System.Selection.ActiveInfos > 0 ) )
	{
	//Draw Origins
		for( UINT l = 0; l < System.Selection.ActiveInfos; l++ )
		{
			if( System.Selection.pInfo[l].ObjectType == MODEL )
			{
				CModel *pModel = (CModel*)System.Selection.pInfo[l].pObject;

				glMatrixMode( GL_MODELVIEW );
				glPushMatrix();
				glPushAttrib( GL_ENABLE_BIT );

				glDisable( GL_LIGHTING );
				glDisable( GL_DEPTH_TEST );

				glTranslatef( pModel->Position[0], pModel->Position[1], pModel->Position[2] );
			
				if(	Type == VT_PERSPECTIVE )
				{
					if( System.FourViews ) { OriginSize = 0.04f * Math.MeasureDistance3( &pModel->Position, &Position ); }
					else { OriginSize = 0.02f * Math.MeasureDistance3( &pModel->Position, &Position ); }
				}
				else
				{
					if( System.FourViews ) { OriginSize = (float)( 0.1 * Size ); }
					else { OriginSize = (float)( 0.05 * Size ); }
				}

				glColor3ub( (BYTE)200, (BYTE)35, (BYTE)35 );

				glBegin( GL_POINTS ); glVertex3f( 0.0f, 0.0f, 0.0f ); glEnd();

				glBegin( GL_LINES );

				glVertex3f( OriginSize, 0.0f, 0.0f );
				glVertex3f( 0.3f * OriginSize, 0.0f, 0.0f );

				glVertex3f( 0.0f, OriginSize, 0.0f );
				glVertex3f( 0.0f, 0.3f * OriginSize, 0.0f );

				glVertex3f( 0.0f, 0.0f, OriginSize );
				glVertex3f( 0.0f, 0.0f, 0.3f * OriginSize );
			
				glEnd();

				glPopAttrib();
				glPopMatrix();
			}		
		}
	}

//Draw Gizmos
	UpdateGizmos();
	DrawGizmos();	

//Draw UI ( View Name, Border, Axis Tripod )
	DrawUI();
	DrawHelperTripod();
	
	SwapBuffers( hDC );	
}	

void CView::DrawRegion()
{
	if( System.Selection.RegionVisible )
	{
		UINT		n, m, Count, Pattern = 0; 
		bool		Draw = TRUE;

		COLORREF	Temp;
		float		xIterator, yIterator; 

		float		x = (float)System.Selection.pRegionPoint[0][0];
		float		y = (float)( Rect.bottom - System.Selection.pRegionPoint[0][1] - 5 );

		for( n = 0; n < System.Selection.ActivePoints - 1; n++ )
		{ 
		//"Vertical" Lines 
			if( abs( System.Selection.pRegionPoint[n + 1][1] - System.Selection.pRegionPoint[n][1] ) >= abs( System.Selection.pRegionPoint[n + 1][0] - System.Selection.pRegionPoint[n][0] ) )
			{
				if( ( System.Selection.pRegionPoint[n + 1][1] - System.Selection.pRegionPoint[n][1] ) == 0 ) { xIterator = 0; }
				else 
				{
					xIterator = (float)abs( System.Selection.pRegionPoint[n + 1][0] - System.Selection.pRegionPoint[n][0] ) / (float)abs( System.Selection.pRegionPoint[n + 1][1] - System.Selection.pRegionPoint[n][1] );
				}

				yIterator = 1;

				Count = abs( System.Selection.pRegionPoint[n + 1][1] - System.Selection.pRegionPoint[n][1] );
			}

		//"Horizontal" Lines 
			if( abs( System.Selection.pRegionPoint[n + 1][0] - System.Selection.pRegionPoint[n][0] ) >= abs( System.Selection.pRegionPoint[n + 1][1] - System.Selection.pRegionPoint[n][1] ) )
			{
				if( ( System.Selection.pRegionPoint[n + 1][0] - System.Selection.pRegionPoint[n][0] ) == 0 ) { yIterator = 0; }
				else
				{
					yIterator = (float)abs( System.Selection.pRegionPoint[n + 1][1] - System.Selection.pRegionPoint[n][1] ) / (float)abs( System.Selection.pRegionPoint[n + 1][0] - System.Selection.pRegionPoint[n][0] );
				}

				xIterator = 1;

				Count = abs( System.Selection.pRegionPoint[n + 1][0] - System.Selection.pRegionPoint[n][0] );
			}

		//Invert Iterators if Neccessary
			if( System.Selection.pRegionPoint[n + 1][0] <= System.Selection.pRegionPoint[n][0] ) { xIterator = - xIterator; }
			if( System.Selection.pRegionPoint[n + 1][1] >= System.Selection.pRegionPoint[n][1] ) { yIterator = - yIterator; }

			for( m = 0; m < Count; m++ )
			{
			//Create Simple Line Stippling
				Pattern++;
				if( Pattern > 4 ) { if( Draw ) { Draw = FALSE; Pattern = 1; } else { Draw = TRUE; Pattern = 0; } }

				if( Draw )
				{
			//Invert Colors
					Temp = GetPixel( hDC, (int)x, (int)y );

					SetPixelV( hDC, (int)x, (int)y, RGB( 255 - GetRValue( Temp ), 255 - GetGValue( Temp ), 255 - GetBValue( Temp ) ) );
				}

			//Update Positions
				x += xIterator;	y += yIterator; 
			}
		}
	}
}


void CView::DrawUI()
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D( 0, Rect.right, 0, Rect.bottom );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glPushAttrib( GL_ENABLE_BIT );

	glDisable( GL_CULL_FACE );
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_FOG );
	glDisable( GL_TEXTURE_2D );

//Draw Selection Region
/*	if( System.Selection.RegionVisible )
	{
		glLineStipple( 1, 0xf1f1 );

		glEnable( GL_LINE_STIPPLE );
//		glEnable( GL_BLEND );
//		glBlendFunc( GL_SRC_ALPHA, GL_DST_ALPHA );

		glColor4f( 1.0f, 1.0f, 1.0f, 0.7f );
		glBegin( GL_LINE_LOOP );
		
		for( UINT n = 0; n < System.Selection.ActivePoints; n++ ) { glVertex2iv( &System.Selection.pRegionPoint[n][0] ); }

		glEnd();

	//	glDisable( GL_BLEND );
		glDisable( GL_LINE_STIPPLE );
	}*/

	glColor3d( 1, 1, 1 );
	System.FontGL.DrawText( 5, 1, Name ); 

//Draw Border Around Active Window
	if( Active && System.FourViews )
	{
		glColor3d( 1.0, 1.0, 0.0 );

		glBegin( GL_LINE_LOOP );
		glVertex2d( 0,					0 );
		glVertex2d( Rect.right - 5,	0 );
		glVertex2d( Rect.right - 5,	Rect.bottom - 5 );
		glVertex2d( 0,							Rect.bottom - 5 );
		glEnd();

		glBegin( GL_LINE_LOOP );
		glVertex2d( 1,						1 );
		glVertex2d( Rect.right - 6,	1 );
		glVertex2d( Rect.right - 6,	Rect.bottom - 6 );
		glVertex2d( 1,							Rect.bottom - 6 );
		glEnd();
	}

	glPopAttrib();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
}

void CView::DrawHelperTripod()
{
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	glOrtho( 0, Rect.right, 0, Rect.bottom, -100, 100 );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glPushAttrib( GL_ENABLE_BIT );

	glDisable( GL_CULL_FACE );
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_FOG );
	glDisable( GL_TEXTURE_2D );


	if(	Type == VT_PERSPECTIVE )
	{
		glTranslated( 25, 25, 0 );

		glRotated( Rotation[0], 1, 0, 0 );
		glRotated( - Rotation[1] + 180, 0, 1, 0 );
		glRotated( Rotation[2], 0, 0, 1 );

		pScene->DrawAxisGizmo( 25, 0, TRUE );  
	}
	else
	{
		glTranslated( 20, 20, 0 );

		glRotated( Rotation[0], 1, 0, 0 );
		glRotated( Rotation[1], 0, 1, 0 );
		glRotated( Rotation[2], 0, 0, 1 );

		pScene->DrawAxisGizmo( 25, 0, TRUE );  
	}

	
	glPopAttrib();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
}				


void CView::Orbit( float x, float y, float z, Float3 *origin )
{
//	Math.SubstractVectors3( &Position, origin, &State.TempVector3d );
//	Math.SubstractVectors3( &Focus, origin, &State.TempVector3e );

//	Math.RotateVector3( &State.TempVector3d, x, y, z, &State.TempVector3d );
//	Math.RotateVector3( &State.TempVector3e, x, y, z, &State.TempVector3e );
//	Math.RotateVector3( &Position,  x, y, z, &Position );
//	Math.RotateVector3( &Focus,  x, y, z, &Focus );

/*	Math.AddVectors3( &Direction, Position, &State.TempVector3a );
	Math.AddVectors3( &UpVector, Position, &State.TempVector3b );
	Math.AddVectors3( &RightVector, Position, &State.TempVector3c );*/

/*	Math.RotateVector3( &State.TempVector3d, 	
	Math.RotateVector3( &State.TempVector3d, */	

//	RePosition( 

//	Math.RotateVector3( &Direction, x, y, z, &Direction );
//	Math.RotateVector3( &UpVector, x, y, z, &UpVector );
//	Math.RotateVector3( &RightVector, x, y, z, &RightVector );


//	Math.RotateVector3( origin, x, y, z, &State.TempVector3a );

//	Math.AddVectors3( &State.TempVector3d, &State.TempVector3a, &Position );
//	Math.AddVectors3( &State.TempVector3e, &State.TempVector3a, &Focus );

	Math.MakeVector3( origin, &Focus, &State.TempVector3a ); 
	Math.RotateVector3( &State.TempVector3a, x, y, 0.0, &State.TempVector3b );
	Math.AddVectors3( origin, &State.TempVector3b, &State.TempVector3c );

	RePosition( State.TempVector3c[0], State.TempVector3c[1], State.TempVector3c[2], TRUE );

	State.TempValue1 = Distance;

	Dolly( State.TempValue1, FALSE, TRUE );
//	AbsRotate( x, y, 0 );

	Math.RotateVector3( &Direction, x, y, z, &Direction  );
	Math.RotateVector3( &UpVector, x, y, z, &UpVector ); 
	Math.RotateVector3( &RightVector, x, y, z, &RightVector ); 

	Math.ScaleVector3( &Direction, Distance, &Focus );
	Math.AddVectors3( &Position, &Focus, &Focus  ); 

	Dolly( -State.TempValue1, FALSE, TRUE );



/*	Math.MakeVector3( origin, &Focus, &State.TempVector3a ); 
	Math.RotateVector3( &State.TempVector3a, 0.0, y, 0.0, &State.TempVector3b );
	Math.AddVectors3( origin, &State.TempVector3b, &State.TempVector3c );

	RePosition( State.TempVector3c[0], State.TempVector3c[1], State.TempVector3c[2], TRUE );

	State.TempValue1 = Distance;

	Zoom( State.TempValue1 );
	AbsRotate( 0.0, y, 0.0 );

	Zoom( -State.TempValue1 );*/

}

//Not yet Finished
void CView::Rotate( float x, float y, float z )
{

//Reset View
	Direction[0] = 0;
	Direction[1] = 1;
	Direction[2] = 0;

	UpVector[0] = 0;
	UpVector[1] = 0;
	UpVector[2] = 1;

	Math.CrossVectors3( &Direction, &UpVector, &RightVector ); 


//Rotate Reseted View by New Values 
	Math.RotateVector3( &Direction, x, y, z, &Direction  );
	Math.RotateVector3( &UpVector, x, y, z, &UpVector ); 
	Math.RotateVector3( &RightVector, x, y, z, &RightVector ); 

//Rotate View by Old Values -> This Creates Relative Rotation 
	Math.RotateVector3( &Direction, &Rotation, &Direction );
	Math.RotateVector3( &UpVector, &Rotation, &UpVector ); 
	Math.RotateVector3( &RightVector, &Rotation, &RightVector ); 

	Math.ScaleVector3( &Direction, Distance, &Focus );
	Math.AddVectors3( &Position, &Focus, &Focus  ); 

//Calculate and Store New Rotation
//	Math.DirectionToRotation3( &Direction, &Rotation );
//	Rotation = Math.RotateVector3( Rotation, x, y, z );

/*	tVector3 temp;

	temp[0] = 0;
	temp[1] = 1;
	temp[2] = 0;

	Math.DirectionToRotation3( &Direction, &Rotation );
	Math.RotateVector3( &temp, &Rotation, &temp );*/

//	Rotation[0] += 20;

//	Rotation.x += x;
//	Rotation.y += y;
//	Rotation.z += z;

}


void CView::AbsRotate( float x, float y, float z )
{
//Not yet Finished
	Rotation[0] += x;
	Rotation[1] += y;
	Rotation[2] += z;

//Reset View
	Direction[0] = 0;
	Direction[1] = 0;
	Direction[2] = 1;

	UpVector[0] = 0;
	UpVector[1] = 1;
	UpVector[2] = 0;

	Math.CrossVectors3( &Direction, &UpVector, &RightVector ); 

//Rotate View
	Math.RotateVector3( &Direction, Rotation[0], Rotation[1], Rotation[2], &Direction  );
	Math.RotateVector3( &UpVector, Rotation[0], Rotation[1], Rotation[2], &UpVector ); 
	Math.RotateVector3( &RightVector, Rotation[0], Rotation[1], Rotation[2], &RightVector ); 

	Math.ScaleVector3( &Direction, Distance, &Focus );
	Math.AddVectors3( &Position, &Focus, &Focus  ); 
}


void CView::RePosition( float x, float y, float z, bool useFocus )
{
	if( useFocus )
	{
		Math.SubstractVectors3( &Position, &Focus, &State.TempVector3a );
		
		Focus[0] = x;
		Focus[1] = y;
		Focus[2] = z;

		Math.AddVectors3( &Focus, &State.TempVector3a, &Position );
	}
	else
	{
		Math.SubstractVectors3( &Focus, &Position, &State.TempVector3a );
		
		Position[0] = x;
		Position[1] = y;
		Position[2] = z;

		Math.AddVectors3( &Position, &State.TempVector3a, &Focus );
	}
}

void CView::Dolly( float distance, bool moveFocus, bool movePosition )
{
	if( Type != VT_PERSPECTIVE ) { return; }

	Math.MakeVector3( &Direction, distance, &State.TempVector3a );

	if( moveFocus ) { Math.AddVectors3( &Focus, &State.TempVector3a, &Focus ); }
	if( movePosition ) { Math.AddVectors3( &Position, &State.TempVector3a, &Position ); }

	Math.MakeVector3( &Position, &Focus, &State.TempVector3a );
	Distance = Math.MeasureVector3( &State.TempVector3a );

/*	Math.ScaleVector3( &Direction, distance, &State.TempVector3a );
	Math.AddVectors3( &Position, &State.TempVector3a, &Position );
	*/
}

void CView::Move( float x, float y, float z )
{
	Math.ScaleVector3( &RightVector, x, &State.TempVector3a );
	Math.AddVectors3( &Position, &State.TempVector3a, &Position ); 
	Math.AddVectors3( &Focus, &State.TempVector3a, &Focus ); 

	Math.ScaleVector3( &UpVector, y, &State.TempVector3a );
	Math.AddVectors3( &Position, &State.TempVector3a, &Position ); 
	Math.AddVectors3( &Focus, &State.TempVector3a, &Focus ); 

	Math.ScaleVector3( &Direction, z, &State.TempVector3a );
	Math.AddVectors3( &Position, &State.TempVector3a, &Position ); 
	Math.AddVectors3( &Focus, &State.TempVector3a, &Focus ); 
}

void CView::ResizeWindow( UINT width, UINT height )
{
	RECT	rt;

//Set Window Size 
	SetWindowPos( hWnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOMOVE );

	
//Update System Variables
	Rect.right = width;
	Rect.bottom = height;
	NeedUpdate = TRUE;
	System.Update = TRUE;


//Update Origin
	GetWindowRect( hWnd, &rt );

	Origin[0] = rt.left + ( rt.right - rt.left ) / 2;
	Origin[1] = rt.top + ( rt.bottom - rt.top ) / 2;

	GetClientRect( hWnd, &rt );

	System.FontGL.SetViewport( width, height );
	glViewport( 0, 0, width, height );

//	UpdateGizmos();
}

void CView::RePositionWindow( UINT pos_x, UINT pos_y )
{
	RECT	rt;

//Set Window Size 
	SetWindowPos( hWnd, NULL, pos_x, pos_y, 0, 0, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOSIZE );

	
//Update System Variables
	Rect.left = pos_x;
	Rect.top = pos_y;
	NeedUpdate = TRUE;
	System.Update = TRUE;


//Update Origin
	GetWindowRect( hWnd, &rt );

	Origin[0] = rt.left + ( rt.right - rt.left ) / 2;
	Origin[1] = rt.top + ( rt.bottom - rt.top ) / 2;
}

void CView::ZoomExtents( bool selection )
{
	if( pScene->ModelCount == 0 ) { return; }

	UINT n;

	if( selection )
	{
		if( System.Selection.ActiveInfos == 0 ) { return; }

		CModel *pModel = (CModel*)System.Selection.pInfo[0].pObject;

		Math.MakeVector3( &System.Selection.GizmoPosition, &pModel->Position, &State.TempVector3a );
		State.TempValue1 = Math.MeasureVector3( &State.TempVector3a ); 

/*		if( ( ( pModel->BoxSize[0] * pModel->Scale[0] ) >= ( pModel->BoxSize[1] * pModel->Scale[1] ) ) && ( ( pModel->BoxSize[0] * pModel->Scale[0] ) >= ( pModel->BoxSize[2] * pModel->Scale[2] ) ) ) { State.TempValue4 = pModel->BoxSize[0] * pModel->Scale[0] * 1.414213f; }
		if( ( ( pModel->BoxSize[1] * pModel->Scale[1] ) >= ( pModel->BoxSize[0] * pModel->Scale[0] ) ) && ( ( pModel->BoxSize[1] * pModel->Scale[1] ) >= ( pModel->BoxSize[2] * pModel->Scale[2] ) ) ) { State.TempValue4 = pModel->BoxSize[1] * pModel->Scale[1] * 1.414213f; }
		if( ( ( pModel->BoxSize[2] * pModel->Scale[2] ) >= ( pModel->BoxSize[0] * pModel->Scale[0] ) ) && ( ( pModel->BoxSize[2] * pModel->Scale[2] ) >= ( pModel->BoxSize[1] * pModel->Scale[1] ) ) ) { State.TempValue4 = pModel->BoxSize[2] * pModel->Scale[2] * 1.414213f; }
*/
		if( ( pModel->BoxSize[0] >= pModel->BoxSize[1] ) && ( pModel->BoxSize[0] >= pModel->BoxSize[2] ) ) { State.TempValue4 = pModel->BoxSize[0] * 1.414213f; }
		if( ( pModel->BoxSize[1] >= pModel->BoxSize[0] ) && ( pModel->BoxSize[1] >= pModel->BoxSize[2] ) ) { State.TempValue4 = pModel->BoxSize[1] * 1.414213f; }
		if( ( pModel->BoxSize[2] >= pModel->BoxSize[0] ) && ( pModel->BoxSize[2] >= pModel->BoxSize[1] ) ) { State.TempValue4 = pModel->BoxSize[2] * 1.414213f; }

		State.TempValue1 += State.TempValue4;

		for( UINT n = 1; n < System.Selection.ActiveInfos; n++ )
		{
			CModel *pModel = (CModel*)System.Selection.pInfo[n].pObject;

			Math.MakeVector3( &System.Selection.GizmoPosition, &pModel->Position, &State.TempVector3b );
			State.TempValue2 = Math.MeasureVector3( &State.TempVector3b ); 

/*			if( ( ( pModel->BoxSize[0] * pModel->Scale[0] ) >= ( pModel->BoxSize[1] * pModel->Scale[1] ) ) && ( ( pModel->BoxSize[0] * pModel->Scale[0] ) >= ( pModel->BoxSize[2] * pModel->Scale[2] ) ) ) { State.TempValue4 = pModel->BoxSize[0] * pModel->Scale[0] * 1.414213f; }
			if( ( ( pModel->BoxSize[1] * pModel->Scale[1] ) >= ( pModel->BoxSize[0] * pModel->Scale[0] ) ) && ( ( pModel->BoxSize[1] * pModel->Scale[1] ) >= ( pModel->BoxSize[2] * pModel->Scale[2] ) ) ) { State.TempValue4 = pModel->BoxSize[1] * pModel->Scale[1] * 1.414213f; }
			if( ( ( pModel->BoxSize[2] * pModel->Scale[2] ) >= ( pModel->BoxSize[0] * pModel->Scale[0] ) ) && ( ( pModel->BoxSize[2] * pModel->Scale[2] ) >= ( pModel->BoxSize[1] * pModel->Scale[1] ) ) ) { State.TempValue4 = pModel->BoxSize[2] * pModel->Scale[2] * 1.414213f; }
*/
			if( ( pModel->BoxSize[0] >= pModel->BoxSize[1] ) && ( pModel->BoxSize[0] >= pModel->BoxSize[2] ) ) { State.TempValue4 = pModel->BoxSize[0] * 1.414213f; }
			if( ( pModel->BoxSize[1] >= pModel->BoxSize[0] ) && ( pModel->BoxSize[1] >= pModel->BoxSize[2] ) ) { State.TempValue4 = pModel->BoxSize[1] * 1.414213f; }
			if( ( pModel->BoxSize[2] >= pModel->BoxSize[0] ) && ( pModel->BoxSize[2] >= pModel->BoxSize[1] ) ) { State.TempValue4 = pModel->BoxSize[2] * 1.414213f; }


			State.TempValue2 += State.TempValue4;

			if( State.TempValue2 > State.TempValue1 ) { State.TempValue1 = State.TempValue2; }
		}

		Math.CopyVector3( &State.TempVector3d, &System.Selection.GizmoPosition );
	}
	else
	{
	//Compute Center of All Models
		Math.ZeroVector3( &State.TempVector3d );

		for( n = 0; n < pScene->ModelCount; n++ )
		{
			Math.AddVectors3( &State.TempVector3d, &pScene->pModel[n].Position, &State.TempVector3d );
			Math.AddVectors3( &State.TempVector3d, &pScene->pModel[n].BoxPosition, &State.TempVector3d );
		}
		Math.DivideVector3( &State.TempVector3d, (float)pScene->ModelCount, &State.TempVector3d );


		Math.MakeVector3( &State.TempVector3d, &pScene->pModel[0].Position, &State.TempVector3a );
		State.TempValue1 = Math.MeasureVector3( &State.TempVector3a ); 

/*		if( ( ( pScene->pModel[0].BoxSize[0] * pScene->pModel[0].Scale[0] ) >= ( pScene->pModel[0].BoxSize[1] * pScene->pModel[0].Scale[1] ) ) && ( ( pScene->pModel[0].BoxSize[0] * pScene->pModel[0].Scale[0] ) >= ( pScene->pModel[0].BoxSize[2] * pScene->pModel[0].Scale[2] ) ) ) { State.TempValue4 = pScene->pModel[0].BoxSize[0] * pScene->pModel[0].Scale[0] * 1.414213f; }
		if( ( ( pScene->pModel[0].BoxSize[1] * pScene->pModel[0].Scale[1] ) >= ( pScene->pModel[0].BoxSize[0] * pScene->pModel[0].Scale[0] ) ) && ( ( pScene->pModel[0].BoxSize[1] * pScene->pModel[0].Scale[1] ) >= ( pScene->pModel[0].BoxSize[2] * pScene->pModel[0].Scale[2] ) ) ) { State.TempValue4 = pScene->pModel[0].BoxSize[1] * pScene->pModel[0].Scale[1] * 1.414213f; }
		if( ( ( pScene->pModel[0].BoxSize[2] * pScene->pModel[0].Scale[2] ) >= ( pScene->pModel[0].BoxSize[0] * pScene->pModel[0].Scale[0] ) ) && ( ( pScene->pModel[0].BoxSize[2] * pScene->pModel[0].Scale[2] ) >= ( pScene->pModel[0].BoxSize[1] * pScene->pModel[0].Scale[1] ) ) ) { State.TempValue4 = pScene->pModel[0].BoxSize[2] * pScene->pModel[0].Scale[2] * 1.414213f; }
*/
		if( ( pScene->pModel[0].BoxSize[0] >= pScene->pModel[0].BoxSize[1] ) && ( pScene->pModel[0].BoxSize[0] >= pScene->pModel[0].BoxSize[2] ) ) { State.TempValue4 = pScene->pModel[0].BoxSize[0] * 1.414213f; }
		if( ( pScene->pModel[0].BoxSize[1] >= pScene->pModel[0].BoxSize[0] ) && ( pScene->pModel[0].BoxSize[1] >= pScene->pModel[0].BoxSize[2] ) ) { State.TempValue4 = pScene->pModel[0].BoxSize[1] * 1.414213f; }
		if( ( pScene->pModel[0].BoxSize[2] >= pScene->pModel[0].BoxSize[0] ) && ( pScene->pModel[0].BoxSize[2] >= pScene->pModel[0].BoxSize[1] ) ) { State.TempValue4 = pScene->pModel[0].BoxSize[2] * 1.414213f; }

		State.TempValue1 += State.TempValue4;

		for( n = 1; n < pScene->ModelCount; n++ )
		{
			Math.MakeVector3( &State.TempVector3d, &pScene->pModel[n].Position, &State.TempVector3b );
			State.TempValue2 = Math.MeasureVector3( &State.TempVector3b ); 

/*			if( ( ( pScene->pModel[n].BoxSize[0] * pScene->pModel[n].Scale[0] ) >= ( pScene->pModel[n].BoxSize[1] * pScene->pModel[n].Scale[1] ) ) && ( ( pScene->pModel[n].BoxSize[0] * pScene->pModel[n].Scale[0] ) >= ( pScene->pModel[n].BoxSize[2] * pScene->pModel[n].Scale[2] ) ) ) { State.TempValue4 = pScene->pModel[n].BoxSize[0] * pScene->pModel[n].Scale[0] * 1.414213f; }
			if( ( ( pScene->pModel[n].BoxSize[1] * pScene->pModel[n].Scale[1] ) >= ( pScene->pModel[n].BoxSize[0] * pScene->pModel[n].Scale[0] ) ) && ( ( pScene->pModel[n].BoxSize[1] * pScene->pModel[n].Scale[1] ) >= ( pScene->pModel[n].BoxSize[2] * pScene->pModel[n].Scale[2] ) ) ) { State.TempValue4 = pScene->pModel[n].BoxSize[1] * pScene->pModel[n].Scale[1] * 1.414213f; }
			if( ( ( pScene->pModel[n].BoxSize[2] * pScene->pModel[n].Scale[2] ) >= ( pScene->pModel[n].BoxSize[0] * pScene->pModel[n].Scale[0] ) ) && ( ( pScene->pModel[n].BoxSize[2] * pScene->pModel[n].Scale[2] ) >= ( pScene->pModel[n].BoxSize[1] * pScene->pModel[n].Scale[1] ) ) ) { State.TempValue4 = pScene->pModel[n].BoxSize[2] * pScene->pModel[n].Scale[2] * 1.414213f; }
*/
			if( ( pScene->pModel[n].BoxSize[0] >= pScene->pModel[n].BoxSize[1] ) && ( pScene->pModel[n].BoxSize[0] >= pScene->pModel[n].BoxSize[2] ) ) { State.TempValue4 = pScene->pModel[n].BoxSize[0] * 1.414213f; }
			if( ( pScene->pModel[n].BoxSize[1] >= pScene->pModel[n].BoxSize[0] ) && ( pScene->pModel[n].BoxSize[1] >= pScene->pModel[n].BoxSize[2] ) ) { State.TempValue4 = pScene->pModel[n].BoxSize[1] * 1.414213f; }
			if( ( pScene->pModel[n].BoxSize[2] >= pScene->pModel[n].BoxSize[0] ) && ( pScene->pModel[n].BoxSize[2] >= pScene->pModel[n].BoxSize[1] ) ) { State.TempValue4 = pScene->pModel[n].BoxSize[2] * 1.414213f; }

			State.TempValue2 += State.TempValue4;

			if( State.TempValue2 > State.TempValue1 ) { State.TempValue1 = State.TempValue2; }
		}
	}

	if( Type == VT_PERSPECTIVE )
	{
		RePosition( State.TempVector3d[0], State.TempVector3d[1], State.TempVector3d[2], TRUE );
	
	//Restrict Zooming 
		State.TempValue4 = (float)( 110.0f * State.TempValue1 / FOV );
		if( State.TempValue4 <= 10.0f ) { State.TempValue4 = 10.000001f; }

		Dolly( Distance - State.TempValue4, FALSE, TRUE );
	}
	else
	{
		if( ID == VIEW_LEFT ) { State.TempVector3d[0] = 0.0f; }
		if( ID == VIEW_TOP ) { State.TempVector3d[1] = 0.0f; }
		if( ID == VIEW_FRONT ) { State.TempVector3d[2] = 0.0f; }

		RePosition( -State.TempVector3d[0], -State.TempVector3d[1], -State.TempVector3d[2], FALSE );
	
	//Restrict Zooming 
		Size = State.TempValue1;
		if( Size < 3.125 ) { Size = 3.125; } 
	}

	Update( TRUE );
}

