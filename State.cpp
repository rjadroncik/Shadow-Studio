// State.cpp: implementation of the CState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "State.h"

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
#include "CreateHandler.h"

//Common Include Files
#include "Timer.h"
#include "Math.h"

extern CState				State; 
extern COptions				Options; 
extern CScene				*pScene; 
extern CMath				Math; 
extern CSystem				System;
extern CViewUI				ViewUI;

extern ADialogBar			MainBar;
extern AToolBar				MainToolBar;
extern AToolBar				BottomToolBar;

//Other handlers
extern CViewHandler			ViewHandler;
extern CCreateHandler		CreateHandler;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CState::CState()
{
	Tool.pData = NULL; 
}

CState::~CState()
{

}

void CState::SetToolInfo( UINT index, HWND hButton )
{
	hToolButton[index] = hButton;
}

bool CState::RollbackTool()
{
	if( ToolLevel == 0 ) { return FALSE; }

	hToolButton[ToolLevel] = NULL;
	ToolLevel--;
	
	if( ( hToolButton[ToolLevel] == NULL ) && ( ToolLevel > 0 ) ) { hToolButton[ToolLevel] = NULL; ToolLevel--; }  

	if( hToolButton[ToolLevel] != NULL ) { SendMessage( hToolButton[ToolLevel], BM_PUSH, 0, 0 ); return TRUE; }

	return FALSE;
}

bool CState::UpdateMousePos3D( UINT index, Float4* plane )
{
	//TODO: UpdateMousePos3D

//Update Crosspoint
	//if( Math.VectorPlaneCrossPoint( &Mouse[index].StartPoint3D, &Mouse[index].EndPoint3D, plane, &Mouse[index].Pos3D ) ) { return TRUE; }

	return FALSE;
}

void CState::UpdateMouse( UINT index, POINT position )
{
//Prepare View + Variables
	System.View[System.ViewActive].PrepareSelection();

	Mouse[index].Pos2D.x = (int)( position.x - System.View[System.ViewActive].Origin[0] + System.View[System.ViewActive].Rect.right / 2 - 2 );
	Mouse[index].Pos2D.y = (int)( System.View[System.ViewActive].Origin[1] + System.View[System.ViewActive].Rect.bottom / 2 - position.y - 2 );

//Unproject at Mouse Position	
	glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );
	glGetDoublev( GL_PROJECTION_MATRIX, ProjMatrix );
	glGetIntegerv( GL_VIEWPORT, Viewport );

	gluUnProject( Mouse[index].Pos2D.x, Mouse[index].Pos2D.y, 0, ModelMatrix, ProjMatrix, Viewport, &TempVectord3a[0], &TempVectord3a[1], &TempVectord3a[2] );

	Math.CopyVector3( &Mouse[index].Vector, &TempVectord3a );

	if( View.Type == VT_PERSPECTIVE ) 
	{
	//Update Cursor Direction		
		Math.MakeVector3( &System.View[System.ViewActive].Position, &Mouse[index].Vector, &Mouse[index].Vector );
		Math.NormalizeVector3( &Mouse[index].Vector, &Mouse[index].Vector );

	//Update Cursor Start Point		
		Math.CopyVector3( &Mouse[index].StartPoint3D, &System.View[System.ViewActive].Position );

	//Update Cursor End Point		
		Math.MakeVector3( &Mouse[index].Vector, System.View[System.ViewActive].ClipFar, &TempVector3d );
		Math.AddVectors3( &Mouse[index].StartPoint3D, &TempVector3d, &Mouse[index].EndPoint3D );
	}

	if( View.Type == VT_ORTHO ) 
	{
	//Update Cursor Start Point		
		Math.CopyVector3( &Mouse[index].StartPoint3D, &Mouse[index].Vector );
/*		Math.ScaleVector3( &View.Direction, 0.0002f * View.ClipNear, &TempVector3e );
		Math.AddVectors3( &TempVector3e, &Mouse[index].Vector, &Mouse[index].StartPoint3D );
*/
	//Update Cursor End Point		
		Math.CopyVector3( &Mouse[index].EndPoint3D, &Mouse[index].Vector );

		if( View.ID == VIEW_LEFT )	{ Mouse[index].EndPoint3D[0] = -Mouse[index].EndPoint3D[0]; }
		if( View.ID == VIEW_TOP )	{ Mouse[index].EndPoint3D[1] = -Mouse[index].EndPoint3D[1]; }
		if( View.ID == VIEW_FRONT )	{ Mouse[index].EndPoint3D[2] = -Mouse[index].EndPoint3D[2]; }

	//Update Cursor Direction		
		Math.CopyVector3( &Mouse[index].Vector, &System.View[System.ViewActive].Direction );
	}
}

void CState::Reset()
{
	FileReady = FALSE;
	strcpy( CurrentFile, "" );
	FileType = 0;

//Init Tool Stack
	ToolLevel = 0;
	
	Tool.Type = TOOL_NONE;
	Tool.ID = TOOL_NONE;
	Tool.Step = 0;

	Tool.Active = FALSE;
	Tool.Enabled = FALSE;


	Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_NONE ) );
	Tool.hButton = NULL;

	if( Tool.pData ) { free( Tool.pData ); }

	Tool.pData = NULL; 

	for( UINT n = 0; n < 3; n++ ) { hToolButton[n] = NULL; }
	
	MasterObjectType = 0;
	SubObjectType = 0;

//	SelectionType = MODEL;
	UsingMaster = FALSE;

	pMasterObject = NULL;

	Math.ZeroVector3( &TransformInfo );

}

void CState::UpdateTransforms( Float3 *pdata )
{
	if( ( State.Tool.ID == TOOL_EDIT_MOVE ) && ( System.Selection.GizmoVisible ) )
	{
		System.Selection.UpdateCenter();

		if( SendDlgItemMessage( BottomToolBar.GetDialog(), IDC_TRANSFORM_LOCAL, BM_IS_PUSHED, 0, 0 ) )
		{
			Math.CopyVector3( &TempVector3d, pdata );
		}
		else
		{
			Math.SubtractVectors3( pdata, &System.Selection.GizmoPosition, &TempVector3d );
		}

		Math.AddVectors3( &System.Selection.GizmoPosition, &TempVector3d, &System.Selection.GizmoPosition );

		for( UINT n = 0; n < System.Selection.ActiveInfos; n++ )
		{
			CModel *pModel = (CModel*)System.Selection.pInfo[n].pObject;
				
			Math.AddVectors3( &pModel->Position, &TempVector3d, &pModel->Position );
			Math.AddVectors3( &System.Selection.pInfo[n].Position, &TempVector3d, &System.Selection.pInfo[n].Position );
		}

		if( SendDlgItemMessage( BottomToolBar.GetDialog(), IDC_TRANSFORM_LOCAL, BM_IS_PUSHED, 0, 0 ) )
		{
			Math.ZeroVector3( &State.TransformInfo );
		}
		else
		{
			Math.CopyVector3( &State.TransformInfo, &System.Selection.GizmoPosition );
		}
	}

	if( ( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) ) && ( System.Selection.GizmoVisible ) )
	{
		if( SendDlgItemMessage( BottomToolBar.GetDialog(), IDC_TRANSFORM_LOCAL, BM_IS_PUSHED, 0, 0 ) )
		{
			Math.ScaleVector3( pdata, 0.01f, &TempVector3d ); 

			for( UINT n = 0; n < System.Selection.ActiveInfos; n++ )
			{
				CModel *pModel = (CModel*)System.Selection.pInfo[n].pObject;
					
				pModel->Scale[0] = System.Selection.pInfo[n].Scale[0] * TempVector3d[0];
				pModel->Scale[1] = System.Selection.pInfo[n].Scale[1] * TempVector3d[1];
				pModel->Scale[2] = System.Selection.pInfo[n].Scale[2] * TempVector3d[2];
	
				System.Selection.pInfo[n].Scale[0] = pModel->Scale[0];
				System.Selection.pInfo[n].Scale[1] = pModel->Scale[1];
				System.Selection.pInfo[n].Scale[2] = pModel->Scale[2];

				if( System.Selection.ActiveInfos > 1 )
				{
					Math.MakeVector3( &System.Selection.GizmoPosition, &System.Selection.pInfo[n].Position, &State.TempVector3a );

					State.TempVector3a[0] *= TempVector3d[0];
					State.TempVector3a[1] *= TempVector3d[1];
					State.TempVector3a[2] *= TempVector3d[2];

					Math.AddVectors3( &System.Selection.GizmoPosition, &State.TempVector3a, &pModel->Position );
					Math.CopyVector3( &System.Selection.pInfo[n].Position, &pModel->Position );
				}

				pModel->UpdateExtents();
			}

			State.TransformInfo[0] = 100.0f;
			State.TransformInfo[1] = 100.0f;
			State.TransformInfo[2] = 100.0f;
		}
		else
		{
			Math.ScaleVector3( pdata, 0.01f, &TempVector3c ); 

			for( UINT n = 0; n < System.Selection.ActiveInfos; n++ )
			{
				CModel *pModel = (CModel*)System.Selection.pInfo[n].pObject;

			//Computate New Relative Scale to Get Final Absolute Scale
				TempVector3d[0] = 1 / pModel->Scale[0] * TempVector3c[0];
				TempVector3d[1] = 1 / pModel->Scale[1] * TempVector3c[1];
				TempVector3d[2] = 1 / pModel->Scale[2] * TempVector3c[2];
					
				pModel->Scale[0] = System.Selection.pInfo[n].Scale[0] * TempVector3d[0];
				pModel->Scale[1] = System.Selection.pInfo[n].Scale[1] * TempVector3d[1];
				pModel->Scale[2] = System.Selection.pInfo[n].Scale[2] * TempVector3d[2];
	
				System.Selection.pInfo[n].Scale[0] = pModel->Scale[0];
				System.Selection.pInfo[n].Scale[1] = pModel->Scale[1];
				System.Selection.pInfo[n].Scale[2] = pModel->Scale[2];

				if( System.Selection.ActiveInfos > 1 )
				{
					Math.MakeVector3( &System.Selection.GizmoPosition, &System.Selection.pInfo[n].Position, &State.TempVector3a );

					State.TempVector3a[0] *= TempVector3d[0];
					State.TempVector3a[1] *= TempVector3d[1];
					State.TempVector3a[2] *= TempVector3d[2];

					Math.AddVectors3( &System.Selection.GizmoPosition, &State.TempVector3a, &pModel->Position );
					Math.CopyVector3( &System.Selection.pInfo[n].Position, &pModel->Position );
				}

				pModel->UpdateExtents();
			}

			if( System.Selection.ActiveInfos == 1 )
			{
				Math.ScaleVector3( &System.Selection.pInfo[0].Scale, 100, &State.TransformInfo ); 
			}
			else
			{
				State.TransformInfo[0] = 100.0f;
				State.TransformInfo[1] = 100.0f;
				State.TransformInfo[2] = 100.0f;
			}
		}

		System.UpdateAllViews( FALSE );
	}

}