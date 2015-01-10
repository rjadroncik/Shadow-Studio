 // EditHandler.cpp: implementation of the CEditHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EditHandler.h"

#include "View.h"
#include "ViewUI.h"
#include "System.h"
#include "Options.h"
#include "Shadow.h"
#include "State.h"
#include "Afc.h"
#include "Resource.h"
#include "RenderGL/RenderGL.h"

//Other handlers
#include "ViewHandler.h"
#include "SelectHandler.h"

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

extern CSelectHandler		SelectHandler;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEditHandler::CEditHandler()
{

}

CEditHandler::~CEditHandler()
{

}

void CEditHandler::OnRButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
//Rollback Tool One Level
	if( State.RollbackTool() ) { return; } 

	if( !Options.LockSelection )
	{
		System.Selection.ClearEntries();

		System.Selection.GizmoVisible = FALSE;
		System.Selection.BoxesVisible = FALSE;

		System.UpdateAllViews( FALSE );

	//Update Transform TypeIn
		System.Selection.UpdateTypeInDialog( FALSE );
	}
}

void CEditHandler::OnLButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
//Mouse Work
	GetCursorPos( &State.TempPoint1 );
	SetCapture( hwnd );
	State.UpdateMouse( 0, State.TempPoint1 );


	System.Selection.UpdateRegion( State.Mouse[0].Pos2D );

	if( State.Tool.ID == TOOL_EDIT_MOVE )
	{ 
	//If Gizmo Is Dispalyed, Check For Its Selection 
		if( System.Selection.GizmoVisible )
		{
			System.Selection.ParseGizmos(); 

		//If We Clicked A Gizmo Part, Select It 
			if( ViewUI.MoveGizmo.ActiveObject != -1 ) { ViewUI.MoveGizmo.SelectedObject = ViewUI.MoveGizmo.ActiveObject; } 
		}

	//If We Didn't Select Gizmo Or It's Not Even Visible, Check For Model Selection
		if( ( ViewUI.MoveGizmo.ActiveObject == -1 ) || ( !System.Selection.GizmoVisible ) )
		{ 
			if( !System.Selection.ParseModels( SM_CURSOR_TEST_SELECTED ) ) 
			{
				System.Selection.Update( SM_SELECT_SINGLE ); 
			}

		//Hide Gizmo If We Deselected All Models
			if( System.Selection.ActiveInfos == 0 )
			{ 
				State.Tool.Step = 3; 
				
				SetCursor( System.hCursorCross );
				System.Selection.GizmoVisible = FALSE; 
			}
		}

		if( ( ViewUI.MoveGizmo.SelectedObject >= 0 ) && ( System.Selection.GizmoVisible ) )
		{
		//Update Gizmo
			ViewUI.MoveGizmo.PlaneXY[3] = - ( System.Selection.GizmoPosition[0] * ViewUI.MoveGizmo.PlaneXY[0] + System.Selection.GizmoPosition[1] * ViewUI.MoveGizmo.PlaneXY[1] + System.Selection.GizmoPosition[2] * ViewUI.MoveGizmo.PlaneXY[2] );
			ViewUI.MoveGizmo.PlaneYZ[3] = - ( System.Selection.GizmoPosition[0] * ViewUI.MoveGizmo.PlaneYZ[0] + System.Selection.GizmoPosition[1] * ViewUI.MoveGizmo.PlaneYZ[1] + System.Selection.GizmoPosition[2] * ViewUI.MoveGizmo.PlaneYZ[2] );
			ViewUI.MoveGizmo.PlaneZX[3] = - ( System.Selection.GizmoPosition[0] * ViewUI.MoveGizmo.PlaneZX[0] + System.Selection.GizmoPosition[1] * ViewUI.MoveGizmo.PlaneZX[1] + System.Selection.GizmoPosition[2] * ViewUI.MoveGizmo.PlaneZX[2] );

		//Update State
			State.Tool.Step = 1;
		
			Math.CopyVector3( &System.Selection.GizmoPositionStart, &System.Selection.GizmoPosition );
			Math.ZeroVector3( &State.MouseOffset );

		//Update Mouse
			if( State.View.Type == VT_PERSPECTIVE )
			{
				if( ( ViewUI.MoveGizmo.SelectedObject == 3 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.MoveGizmo.PlaneXY ); }
				if( ( ViewUI.MoveGizmo.SelectedObject == 4 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.MoveGizmo.PlaneYZ ); }
				if( ( ViewUI.MoveGizmo.SelectedObject == 5 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.MoveGizmo.PlaneZX ); }

				if( ( ViewUI.MoveGizmo.SelectedObject == 0 ) || ( ViewUI.MoveGizmo.SelectedObject == 1 ) || ( ViewUI.MoveGizmo.SelectedObject == 2 ) )
				{
					Math.MakePlane( &System.Selection.GizmoPosition, &System.View[System.ViewActive].Direction, &State.TempVector4a );
				}

			}
			else { Math.CopyVector4( &State.TempVector4a, &State.View.CreationPlane ); }
		}
	}

	if( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) )
	{ 
	//If Gizmo Is Dispalyed, Check For Its Selection 
		if( System.Selection.GizmoVisible )
		{
			System.Selection.ParseGizmos(); 

		//If We Clicked A Gizmo Part, Select It 
			if( ViewUI.ScaleGizmo.ActiveObject != -1 ) { ViewUI.ScaleGizmo.SelectedObject = ViewUI.ScaleGizmo.ActiveObject; } 
		}

	//If We Didn't Select Gizmo Or It's Not Even Visible, Check For Model Selection
		if( ( ViewUI.ScaleGizmo.ActiveObject == -1 ) || ( !System.Selection.GizmoVisible ) )
		{ 
			if( !System.Selection.ParseModels( SM_CURSOR_TEST_SELECTED ) ) 
			{
				System.Selection.Update( SM_SELECT_SINGLE ); 
			}

		//Hide Gizmo If We Deselected All Models
			if( System.Selection.ActiveInfos == 0 )
			{ 
				State.Tool.Step = 3; 
				
				SetCursor( System.hCursorCross );
				System.Selection.GizmoVisible = FALSE; 
			}
		}

		if( ( ViewUI.ScaleGizmo.SelectedObject >= 0 ) && ( System.Selection.GizmoVisible ) )
		{
		//Update Gizmo
			ViewUI.ScaleGizmo.PlaneXY[3] = - ( System.Selection.GizmoPosition[0] * ViewUI.ScaleGizmo.PlaneXY[0] + System.Selection.GizmoPosition[1] * ViewUI.ScaleGizmo.PlaneXY[1] + System.Selection.GizmoPosition[2] * ViewUI.ScaleGizmo.PlaneXY[2] );
			ViewUI.ScaleGizmo.PlaneYZ[3] = - ( System.Selection.GizmoPosition[0] * ViewUI.ScaleGizmo.PlaneYZ[0] + System.Selection.GizmoPosition[1] * ViewUI.ScaleGizmo.PlaneYZ[1] + System.Selection.GizmoPosition[2] * ViewUI.ScaleGizmo.PlaneYZ[2] );
			ViewUI.ScaleGizmo.PlaneZX[3] = - ( System.Selection.GizmoPosition[0] * ViewUI.ScaleGizmo.PlaneZX[0] + System.Selection.GizmoPosition[1] * ViewUI.ScaleGizmo.PlaneZX[1] + System.Selection.GizmoPosition[2] * ViewUI.ScaleGizmo.PlaneZX[2] );

		//Update State
			State.Tool.Step = 1;
		
			Math.CopyVector3( &System.Selection.GizmoPositionStart, &System.Selection.GizmoPosition );
			Math.ZeroVector3( &State.MouseOffset );

		//Update Mouse
			if( State.View.Type == VT_PERSPECTIVE )
			{
				if( ( ViewUI.ScaleGizmo.SelectedObject == 3 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.ScaleGizmo.PlaneXY ); }
				if( ( ViewUI.ScaleGizmo.SelectedObject == 4 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.ScaleGizmo.PlaneYZ ); }
				if( ( ViewUI.ScaleGizmo.SelectedObject == 5 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.ScaleGizmo.PlaneZX ); }

				if( ( ViewUI.ScaleGizmo.SelectedObject == 0 ) || ( ViewUI.ScaleGizmo.SelectedObject == 1 ) || ( ViewUI.ScaleGizmo.SelectedObject == 2 ) )
				{
					Math.MakePlane( &System.Selection.GizmoPosition, &System.View[System.ViewActive].Direction, &State.TempVector4a );
				}
			}
			else { Math.CopyVector4( &State.TempVector4a, &State.View.CreationPlane ); }
		}
	}

//Update Mouse
	State.UpdateMousePos3D( 0, &State.TempVector4a );
	State.UpdateMousePos3D( 1, &State.TempVector4a );
}


void CEditHandler::OnLButtonUp( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( ( State.Tool.Step == 1 ) && ( State.Mouse[1].Pos2D.x == State.Mouse[0].Pos2D.x ) && ( State.Mouse[1].Pos2D.y == State.Mouse[0].Pos2D.y ) )
	{
		System.Selection.ParseGizmos(); 

		if( ( State.Tool.ID == TOOL_EDIT_MOVE && ViewUI.MoveGizmo.ActiveObject < 0 ) ||
			( State.Tool.ID == TOOL_EDIT_USCALE && ViewUI.ScaleGizmo.ActiveObject < 0 ) ||
			( State.Tool.ID == TOOL_EDIT_NSCALE && ViewUI.ScaleGizmo.ActiveObject < 0 ) )
		{
			if( ( wParam & MK_CONTROL ) == MK_CONTROL ) { System.Selection.Update( SM_MODIFY ); }
			else { System.Selection.Update( SM_SELECT_SINGLE ); }
		}
	}

	if( State.Tool.Step == 4 )
 	{
		SelectHandler.UpdateRegion( FALSE );

		if( ( wParam & MK_CONTROL ) == MK_CONTROL ) { System.Selection.Update( SM_ADD ); }
		else { System.Selection.Update( SM_SELECT_MULTY ); }

		System.Selection.RegionVisible = FALSE;
		State.Tool.Step = 0;
	}

	System.UpdateAllViews( FALSE );	

	if( ( State.Tool.ID == TOOL_EDIT_MOVE ) && ( ViewUI.MoveGizmo.SelectedObject >= 0 ) && ( State.Tool.Step == 1 ) )
	{
		State.Tool.Step = 0;

		Math.AddVectors3( &System.Selection.GizmoPositionStart, &State.MouseOffset, &System.Selection.GizmoPosition );

		for( UINT n = 0; n < System.Selection.ActiveInfos; n++ )
		{
			if( System.Selection.pInfo[n].ObjectType == MODEL )
			{
				CModel *pModel = (CModel*)System.Selection.pInfo[n].pObject;
				
				Math.CopyVector3( &System.Selection.pInfo[n].Position, &pModel->Position ); 
			}
		}

		Math.CopyVector3( &State.TransformInfo, &System.Selection.GizmoPosition );
	}

	if( ( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) ) && ( ViewUI.MoveGizmo.SelectedObject >= 0 ) && ( State.Tool.Step == 1 ) )
	{
		State.Tool.Step = 0;

		for( UINT n = 0; n < System.Selection.ActiveInfos; n++ )
		{
			if( System.Selection.pInfo[n].ObjectType == MODEL )
			{
				CModel *pModel = (CModel*)System.Selection.pInfo[n].pObject;
				
				Math.CopyVector3( &System.Selection.pInfo[n].Scale, &pModel->Scale ); 
				Math.CopyVector3( &System.Selection.pInfo[n].Position, &pModel->Position ); 
			}
		}
	}

	State.Tool.Active = FALSE;
	ReleaseCapture();

	System.Selection.UpdateCenter();

//Update Transform TypeIn
	System.Selection.UpdateTypeInDialog( FALSE );
}

void CEditHandler::OnMouseMove( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( ( State.Tool.Step == 3 ) || ( State.Tool.Step == 4 )  )
	{
		SelectHandler.UpdateRegion( TRUE );

		State.Tool.Step = 4;
		return; 
	}

	if( ( State.Tool.ID == TOOL_EDIT_MOVE ) && ( State.Tool.Step == 1 ) && ( ViewUI.MoveGizmo.SelectedObject >= 0 ) )
	{ 
		SetCursor( State.Tool.hCursor );

	//Update Mouse
		if( State.View.Type == VT_PERSPECTIVE )
		{
			if( ( ViewUI.MoveGizmo.SelectedObject == 3 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.MoveGizmo.PlaneXY ); }
			if( ( ViewUI.MoveGizmo.SelectedObject == 4 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.MoveGizmo.PlaneYZ ); }
			if( ( ViewUI.MoveGizmo.SelectedObject == 5 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.MoveGizmo.PlaneZX ); }

			if( ( ViewUI.MoveGizmo.SelectedObject == 0 ) || ( ViewUI.MoveGizmo.SelectedObject == 1 ) || ( ViewUI.MoveGizmo.SelectedObject == 2 ) )
			{
				Math.MakePlane( &System.Selection.GizmoPosition, &System.View[System.ViewActive].Direction, &State.TempVector4a );
			}
		}
		else { Math.CopyVector4( &State.TempVector4a, &State.View.CreationPlane ); }

		State.UpdateMousePos3D( 1, &State.TempVector4a );
		Math.MakeVector3( &State.Mouse[0].Pos3D, &State.Mouse[1].Pos3D, &State.MouseOffset );

	//Restrain Movement
		if( ( ViewUI.MoveGizmo.SelectedObject == 0 ) ) { State.MouseOffset[1] = 0.0f; State.MouseOffset[2] = 0.0f; }
		if( ( ViewUI.MoveGizmo.SelectedObject == 1 ) ) { State.MouseOffset[0] = 0.0f; State.MouseOffset[2] = 0.0f; }
		if( ( ViewUI.MoveGizmo.SelectedObject == 2 ) ) { State.MouseOffset[0] = 0.0f; State.MouseOffset[1] = 0.0f; }

/*		if( ( ViewUI.MoveGizmo.SelectedObject == 0 ) ) { State.MouseOffset[0] = State.MouseOffset[1]; State.MouseOffset[1] = 0.0f; State.MouseOffset[2] = 0.0f;}
		if( ( ViewUI.MoveGizmo.SelectedObject == 1 ) ) { State.MouseOffset[1] = State.MouseOffset[1]; State.MouseOffset[0] = 0.0f; State.MouseOffset[2] = 0.0f;}
		if( ( ViewUI.MoveGizmo.SelectedObject == 2 ) ) { State.MouseOffset[2] = State.MouseOffset[1]; State.MouseOffset[0] = 0.0f; State.MouseOffset[1] = 0.0f;}
*/
		if( ( ViewUI.MoveGizmo.SelectedObject == 3 ) ) { State.MouseOffset[2] = 0.0f; }
		if( ( ViewUI.MoveGizmo.SelectedObject == 4 ) ) { State.MouseOffset[0] = 0.0f; }
		if( ( ViewUI.MoveGizmo.SelectedObject == 5 ) ) { State.MouseOffset[1] = 0.0f; }

	//Update Gizmo Position
		Math.AddVectors3( &System.Selection.GizmoPositionStart, &State.MouseOffset, &System.Selection.GizmoPosition );

	//Update Selection Position
		for( UINT n = 0; n < System.Selection.ActiveInfos; n++ )
		{
			if( System.Selection.pInfo[n].ObjectType == MODEL )
			{
				CModel *pModel = (CModel*)System.Selection.pInfo[n].pObject;
				
				Math.AddVectors3( &System.Selection.pInfo[n].Position, &State.MouseOffset, &pModel->Position );
			}
		}

		Math.CopyVector3( &State.TransformInfo,	&State.MouseOffset );

		System.Selection.UpdateTypeInDialog( TRUE );
	}

	if( ( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) ) && ( State.Tool.Step == 1 ) && ( ViewUI.ScaleGizmo.SelectedObject >= 0 ) )
	{ 
		SetCursor( State.Tool.hCursor );

	//Update Mouse
		if( ViewUI.ScaleGizmo.SelectedObject != 6 )
		{
			if( ( State.View.Type == VT_PERSPECTIVE ) )
			{
				if( ( ViewUI.ScaleGizmo.SelectedObject == 3 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.ScaleGizmo.PlaneXY ); }
				if( ( ViewUI.ScaleGizmo.SelectedObject == 4 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.ScaleGizmo.PlaneYZ ); }
				if( ( ViewUI.ScaleGizmo.SelectedObject == 5 ) ) { Math.CopyVector4( &State.TempVector4a, &ViewUI.ScaleGizmo.PlaneZX ); }

				if( ( ViewUI.ScaleGizmo.SelectedObject == 0 ) || ( ViewUI.ScaleGizmo.SelectedObject == 1 ) || ( ViewUI.ScaleGizmo.SelectedObject == 2 ) )
				{
					Math.MakePlane( &System.Selection.GizmoPosition, &System.View[System.ViewActive].Direction, &State.TempVector4a );
				}
			}
			else { Math.CopyVector4( &State.View.CreationPlane, &State.TempVector4a ); }
		}

		State.UpdateMousePos3D( 1, &State.TempVector4a );
		Math.MakeVector3( &State.Mouse[0].Pos3D, &State.Mouse[1].Pos3D, &State.MouseOffset );

		Math.ScaleVector3( &State.MouseOffset, 0.008f, &State.MouseOffset );

	//Restrain Movement
		if( ( ViewUI.ScaleGizmo.SelectedObject == 0 ) ) { State.MouseOffset[1] = 0.0f; State.MouseOffset[2] = 0.0f; }
		if( ( ViewUI.ScaleGizmo.SelectedObject == 1 ) ) { State.MouseOffset[0] = 0.0f; State.MouseOffset[2] = 0.0f; }
		if( ( ViewUI.ScaleGizmo.SelectedObject == 2 ) ) { State.MouseOffset[0] = 0.0f; State.MouseOffset[1] = 0.0f; }

/*		if( ( ViewUI.MoveGizmo.SelectedObject == 0 ) ) { State.MouseOffset[0] = State.MouseOffset[1]; State.MouseOffset[1] = 0.0f; State.MouseOffset[2] = 0.0f;}
		if( ( ViewUI.MoveGizmo.SelectedObject == 1 ) ) { State.MouseOffset[1] = State.MouseOffset[1]; State.MouseOffset[0] = 0.0f; State.MouseOffset[2] = 0.0f;}
		if( ( ViewUI.MoveGizmo.SelectedObject == 2 ) ) { State.MouseOffset[2] = State.MouseOffset[1]; State.MouseOffset[0] = 0.0f; State.MouseOffset[1] = 0.0f;}
*/
	//Update Selection Scale
		for( UINT n = 0; n < System.Selection.ActiveInfos; n++ )
		{
			if( System.Selection.pInfo[n].ObjectType == MODEL )
			{
				CModel *pModel = (CModel*)System.Selection.pInfo[n].pObject;
				
				if( ViewUI.ScaleGizmo.SelectedObject == 6 )
				{ 
					State.TempValue1 = 1 + ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * 0.008f;

					if( State.TempValue1 < 0.001f ) { State.TempValue1 = 0.001f; }

					Math.ScaleVector3( &System.Selection.pInfo[n].Scale, State.TempValue1, &pModel->Scale );
			
					if( ( Options.CenterType == CENTER_SELECTION ) || ( Options.CenterType == CENTER_SYSTEM ) ) //System.Selection.ActiveInfos > 1
					{
						Math.MakeVector3( &System.Selection.GizmoPosition, &System.Selection.pInfo[n].Position, &State.TempVector3a );
						Math.ScaleVector3( &State.TempVector3a, State.TempValue1, &State.TempVector3a );
						Math.AddVectors3( &System.Selection.GizmoPosition, &State.TempVector3a, &pModel->Position );
					}
				}
				else
				{
					if( ( ( State.Tool.ID == TOOL_EDIT_USCALE ) && ( ViewUI.ScaleGizmo.SelectedObject == 3 ) ) ||
						( ( State.Tool.ID == TOOL_EDIT_USCALE ) && ( ViewUI.ScaleGizmo.SelectedObject == 4 ) ) ||
						( ( State.Tool.ID == TOOL_EDIT_USCALE ) && ( ViewUI.ScaleGizmo.SelectedObject == 5 ) ) )
					{
						State.MouseOffset[0] = ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * 0.008f;
						State.MouseOffset[1] = ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * 0.008f;
						State.MouseOffset[2] = ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * 0.008f;
					}

					if( ( ViewUI.ScaleGizmo.SelectedObject == 3 ) ) { State.MouseOffset[2] = 0.0f; }
					if( ( ViewUI.ScaleGizmo.SelectedObject == 4 ) ) { State.MouseOffset[0] = 0.0f; }
					if( ( ViewUI.ScaleGizmo.SelectedObject == 5 ) ) { State.MouseOffset[1] = 0.0f; }

				//Do'nt Allow Negative Scale
					if( State.MouseOffset[0] < -0.999f ) { State.MouseOffset[0] = -0.999f; }
					if( State.MouseOffset[1] < -0.999f ) { State.MouseOffset[1] = -0.999f; }
					if( State.MouseOffset[2] < -0.999f ) { State.MouseOffset[2] = -0.999f; }

					pModel->Scale[0] = System.Selection.pInfo[n].Scale[0] * ( 1 + State.MouseOffset[0] );
					pModel->Scale[1] = System.Selection.pInfo[n].Scale[1] * ( 1 + State.MouseOffset[1] );
					pModel->Scale[2] = System.Selection.pInfo[n].Scale[2] * ( 1 + State.MouseOffset[2] );

					if( ( Options.CenterType == CENTER_SELECTION ) || ( Options.CenterType == CENTER_SYSTEM ) ) //System.Selection.ActiveInfos > 1
					{
						Math.MakeVector3( &System.Selection.GizmoPosition, &System.Selection.pInfo[n].Position, &State.TempVector3a );

						State.TempVector3a[0] *= ( 1 + State.MouseOffset[0] );
						State.TempVector3a[1] *= ( 1 + State.MouseOffset[1] );
						State.TempVector3a[2] *= ( 1 + State.MouseOffset[2] );

						Math.AddVectors3( &System.Selection.GizmoPosition, &State.TempVector3a, &pModel->Position );
					}
				}
				pModel->UpdateExtents();
			}
		}

		if( ViewUI.ScaleGizmo.SelectedObject == 6 )
		{
			State.TransformInfo[0] =  State.TempValue1 * 100;
			State.TransformInfo[1] =  State.TempValue1 * 100;
			State.TransformInfo[2] =  State.TempValue1 * 100;
		} 
		else
		{ 
			Math.ScaleVector3( &State.MouseOffset, 100, &State.TransformInfo ); 

			State.TransformInfo[0] += 100.0f;
			State.TransformInfo[1] += 100.0f;
			State.TransformInfo[2] += 100.0f;
		}


		System.Selection.UpdateTypeInDialog( TRUE );
	}

	System.UpdateAllViews( FALSE );
}


