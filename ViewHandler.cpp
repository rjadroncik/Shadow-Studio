// ViewHandler.cpp: implementation of the CViewHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ViewHandler.h"

#include "View.h"
#include "ViewUI.h"
#include "System.h"
#include "Shadow.h"
#include "Options.h"
#include "State.h"
#include "Afc.h"
#include "Resource.h"
#include "RenderGL/RenderGL.h"

//Other handlers
#include "ViewHandler.h"

//Common Include Files
#include "../System/Timer.h"
#include "../System/Math.h"
#include "../System/JPEGlib.h"

extern CState				State; 
extern COptions				Options; 
extern CScene				*pScene; 
extern CMath				Math; 
extern CSystem				System;
extern CViewUI				ViewUI;

extern ADialogBar			MainBar;
extern AToolBar				MainToolBar;
extern AToolBar				BottomToolBar;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CViewHandler::CViewHandler()
{

}

CViewHandler::~CViewHandler()
{

}


void CViewHandler::OnLButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
//Mouse Work
	GetCursorPos( &State.TempPoint1 );
	SetCapture( hwnd );
	State.UpdateMouse( 0, State.TempPoint1 );

	if( State.Tool.ID == TOOL_VIEW_ZOOM_ALL )
	{
		State.TempView[0] = System.View[0];
		State.TempView[1] = System.View[1];
		State.TempView[2] = System.View[2];
		State.TempView[3] = System.View[3];
	}
}

void CViewHandler::OnLButtonUp( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	State.Tool.Active = FALSE;
	ReleaseCapture();
}

void CViewHandler::OnRButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
//Rollback Tool One Level
	if( State.RollbackTool() ) { return; } 
}

void CViewHandler::OnRButtonUp( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
}

void CViewHandler::OnMButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
}

void CViewHandler::OnMButtonUp( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
}

void CViewHandler::OnMouseMove( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
//Panning
	if( ( State.Tool.ID == TOOL_VIEW_PAN ) )
	{
		System.View[System.ViewActive].RePosition( State.View.Position[0], State.View.Position[1], State.View.Position[2], FALSE );
		
		if( State.View.ID == VIEW_PERSPECTIVE )
		{
			System.View[System.ViewActive].Move( (float)( -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) * State.View.Distance / State.View.Rect.bottom ), (float)( -( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * State.View.Distance / State.View.Rect.bottom ), 0 ); 
		}

		if( State.View.ID == VIEW_TOP )
		{
			System.View[System.ViewActive].Move( (float)( -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) * 2 * State.View.Size / State.View.Rect.bottom ), (float)( -( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * 2 * State.View.Size / State.View.Rect.bottom ), 0 ); 
		}

		if( State.View.ID == VIEW_FRONT )
		{
			System.View[System.ViewActive].Move( (float)( ( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) * 2 * State.View.Size / State.View.Rect.bottom ), (float)( ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * 2 * State.View.Size / State.View.Rect.bottom ), 0 ); 
		}

		if( State.View.ID == VIEW_LEFT )
		{
			System.View[System.ViewActive].Move( (float)( ( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) * 2 * State.View.Size / State.View.Rect.bottom ), (float)( ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * 2 * State.View.Size / State.View.Rect.bottom ), 0 ); 
		}

		System.Update = TRUE;
		System.View[System.ViewActive].NeedUpdate = TRUE;
	}

//Zooming All
	if( State.Tool.ID == TOOL_VIEW_ZOOM_ALL )
	{
		for( UINT n = 0; n < System.ViewCount; n++ )
		{
			State.View = State.TempView[n];

			if( State.View.Type == VT_ORTHO )
			{
				System.View[n].Size = State.View.Size - ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * State.View.Size / 250.0f;
		
				if( System.View[n].Size < 3.125 ) { System.View[n].Size = 3.125; } 
			}
			else
			{
				Float3 Tmp;

				Math.ScaleVector3( &State.View.Direction, (float)( ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * State.View.Distance / State.View.Rect.bottom ), &Tmp );
				Math.AddVectors3( &State.View.Position, &Tmp, &System.View[n].Position );
				
				Math.MakeVector3( &System.View[n].Position, &System.View[n].Focus, &Tmp );
				System.View[n].Distance = Math.MeasureVector3( &Tmp );

				if( ( System.View[n].Distance <= 10 ) || ( ( Tmp[0] * State.View.Direction[0] ) < 0 ) || ( ( Tmp[1] * State.View.Direction[1] ) < 0 ) || ( ( Tmp[2] * State.View.Direction[2] ) < 0 ) )
				{
					Math.MakeVector3( &State.View.Direction, - 10.000001f, &Tmp );
					Math.AddVectors3( &State.View.Focus, &Tmp, &System.View[n].Position );

					Math.MakeVector3( &System.View[n].Position, &System.View[n].Focus, &Tmp );
					System.View[n].Distance = Math.MeasureVector3( &Tmp );
				}
			}
		}
		System.UpdateAllViews( TRUE );
	}


	if( State.Tool.ID == TOOL_VIEW_FOV )
	{
		System.View[System.ViewActive].FOV =  State.View.FOV - ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * 0.1f;

		if( System.View[System.ViewActive].FOV > 90 ) { System.View[System.ViewActive].FOV = 90; }
		if( System.View[System.ViewActive].FOV < 1 ) { System.View[System.ViewActive].FOV = 1; }


		System.Update = TRUE;
		System.View[System.ViewActive].NeedUpdate = TRUE;
	}

	if( State.Tool.ID == TOOL_VIEW_ZOOM )
	{
		if( State.View.Type == VT_ORTHO )
		{
			System.View[System.ViewActive].Size = State.View.Size - ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * State.View.Size / 250;
	
			if( System.View[System.ViewActive].Size < 3.125 ) { System.View[System.ViewActive].Size = 3.125; } 
		}
		else
		{
			Float3 Tmp;

			Math.ScaleVector3( &State.View.Direction, (float)( ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * State.View.Distance / State.View.Rect.bottom ), &Tmp );
			Math.AddVectors3( &State.View.Position, &Tmp, &System.View[System.ViewActive].Position );
			
			Math.MakeVector3( &System.View[System.ViewActive].Position, &System.View[System.ViewActive].Focus, &Tmp );
			System.View[System.ViewActive].Distance = Math.MeasureVector3( &Tmp );

			if( ( System.View[System.ViewActive].Distance <= 10 ) || ( ( Tmp[0] * State.View.Direction[0] ) < 0 ) || ( ( Tmp[1] * State.View.Direction[1] ) < 0 ) || ( ( Tmp[2] * State.View.Direction[2] ) < 0 ) )
			{
				Math.MakeVector3( &State.View.Direction, - 10.000001f, &Tmp );
				Math.AddVectors3( &State.View.Focus, &Tmp, &System.View[System.ViewActive].Position );

				Math.MakeVector3( &System.View[System.ViewActive].Position, &System.View[System.ViewActive].Focus, &Tmp );
				System.View[System.ViewActive].Distance = Math.MeasureVector3( &Tmp );
			}
		}

		System.View[System.ViewActive].UpdateGizmos();
		System.Update = TRUE;
		System.View[System.ViewActive].NeedUpdate = TRUE;
	}


/*	if( ( State.Tool.ID == TOOL_VIEW_ROLL ) && ( State.View.Type != VT_ORTHO ) )
	{
		System.View[viewIndex] = State.View;

		//Must be Changed
		System.View[viewIndex].AbsRotate( 0.0f, 0.0f, -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8 ); 


		System.Update = TRUE;
		System.View[viewIndex].NeedUpdate = TRUE;
	}*/

	if( ( State.Tool.ID == TOOL_VIEW_ROTATE_ORIGIN ) && ( State.View.Type != VT_ORTHO ) )
	{
		System.View[System.ViewActive] = State.View;
		
		System.View[System.ViewActive].AbsRotate( (float)( -( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) / 3.8 ), (float)( -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8 ), 0.0 ); 

		System.Update = TRUE;
		System.View[System.ViewActive].NeedUpdate = TRUE;
	}

	if( ( State.Tool.ID == TOOL_VIEW_ROTATE_FOCUS ) && ( State.View.Type != VT_ORTHO ) )
	{
		System.View[System.ViewActive] = State.View;
		
		State.TempValue1 = System.View[System.ViewActive].Distance;
		
		System.View[System.ViewActive].Dolly( State.TempValue1, FALSE, TRUE ); 
		System.View[System.ViewActive].AbsRotate( (float)( -( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) / 3.8 ), (float)( -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8 ), 0.0 ); 
		System.View[System.ViewActive].Dolly( -State.TempValue1, FALSE, TRUE ); 

		System.Update = TRUE;
		System.View[System.ViewActive].NeedUpdate = TRUE;
	}

	if( ( State.Tool.ID == TOOL_VIEW_ROTATE_SELECTED ) && ( State.View.Type != VT_ORTHO ) )
	{
		System.View[System.ViewActive] = State.View;

		System.View[System.ViewActive].Orbit( (float)( ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) / 3.8 ), (float)( -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8 ), 0.0f, &System.Selection.GizmoPosition );
	
/*		Math.RotateVector3( &System.Selection.GizmoPosition, ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) / 3.8, -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8, 0.0, &State.TempVector3b );

		System.View[viewIndex].AbsRotate( ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) / 3.8, -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8, 0.0 ); 

		System.View[viewIndex].RePosition( State.TempVector3b[0], State.TempVector3b[1], State.TempVector3b[2], TRUE );

/*
		Math.SubstractVectors3( &System.View[viewIndex].Focus, &System.Selection.GizmoPosition, &State.TempVector3b );

		System.View[viewIndex].RePosition( State.TempVector3b[0], State.TempVector3b[1], State.TempVector3b[2], TRUE );

	/*	Math.RotateVector3( &System.View[viewIndex].Position, ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) / 3.8, -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8, 0.0, &System.View[viewIndex].Position );
		Math.RotateVector3( &System.View[viewIndex].Focus, ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) / 3.8, -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8, 0.0, &System.View[viewIndex].Focus );

		System.View[viewIndex].Rotation[0] += ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) / 3.8;
		System.View[viewIndex].Rotation[1] -= ( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8;

		System.View[viewIndex].Direction[0] = 0;
		System.View[viewIndex].Direction[1] = 0;
		System.View[viewIndex].Direction[2] = 1;

		System.View[viewIndex].UpVector[0] = 0;
		System.View[viewIndex].UpVector[1] = 1;
		System.View[viewIndex].UpVector[2] = 0;

		System.View[viewIndex].RightVector[0] = 1;
		System.View[viewIndex].RightVector[1] = 0;
		System.View[viewIndex].RightVector[2] = 0;

//		Math.CrossVectors3( &System.View[viewIndex].Direction, &System.View[viewIndex].UpVector, &System.View[viewIndex].RightVector ); 

		Math.RotateVector3( &System.View[viewIndex].Direction, &System.View[viewIndex].Rotation, &System.View[viewIndex].Direction );
		Math.RotateVector3( &System.View[viewIndex].RightVector, &System.View[viewIndex].Rotation, &System.View[viewIndex].RightVector );
//		Math.RotateVector3( &System.View[viewIndex].UpVector, &System.View[viewIndex].Rotation, &System.View[viewIndex].UpVector );

		Math.CrossVectors3( &System.View[viewIndex].Direction, &System.View[viewIndex].RightVector, &System.View[viewIndex].UpVector ); 
*/
/*		State.TempValue1 = System.View[viewIndex].Distance;

		System.View[viewIndex].Zoom( State.TempValue1 ); 
		System.View[viewIndex].AbsRotate( ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) / 3.8, -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8, 0.0 ); 
		System.View[viewIndex].Zoom( -State.TempValue1 ); 

		Math.AddVectors3( &System.View[viewIndex].Focus, &System.Selection.GizmoPosition, &State.TempVector3b );

		System.View[viewIndex].RePosition( State.TempVector3b[0], State.TempVector3b[1], State.TempVector3b[2], TRUE );
*/

/*
		Math.SubstractVectors3( &System.View[viewIndex].Focus, &System.Selection.GizmoPosition, &State.TempVector3b );

//		Math.RotateVector3( &State.TempVector3b, ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) / 3.8, -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8, 0.0, &State.TempVector3b );

		System.View[viewIndex].AbsRotate( ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) / 3.8, -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) / 3.8, 0.0 ); 

		System.View[viewIndex].RePosition( -State.TempVector3b[0], -State.TempVector3b[1], -State.TempVector3b[2], TRUE );

/*
//Not yet Finished

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
*/

		System.Update = TRUE;
		System.View[System.ViewActive].NeedUpdate = TRUE;
		System.UpdateAllViews( FALSE );
	}
}