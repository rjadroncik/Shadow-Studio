// CreateHandler.cpp: implementation of the CCreateHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CreateHandler.h"

#include "View.h"
#include "System.h"
#include "Options.h"
#include "Shadow.h"
#include "State.h"
#include "Afc.h"
#include "Resource.h"
#include "RenderGL/RenderGL.h"

//Other handlers
#include "ViewHandler.h"

//Common Include Files
#include "Timer.h"
#include "Math.h"

extern CState				State; 
extern COptions				Options; 
extern CScene				*pScene; 
extern CMath				Math; 
extern CSystem				System;

extern ADialogBar			MainBar;
extern AToolBar				MainToolBar;
extern AToolBar				BottomToolBar;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCreateHandler::CCreateHandler()
{

}

CCreateHandler::~CCreateHandler()
{

}

void CCreateHandler::OnLButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
//Ignore Mouse Down While Creating
	if( ( State.Tool.ID == TOOL_CREATE_BOX ) && ( State.Tool.Step == 2  ) ) { return; } 

//Mouse Work
	GetCursorPos( &State.TempPoint1 );
	SetCapture( hwnd );
	State.UpdateMouse( 0, State.TempPoint1 );

	if( State.Tool.Step == 0 )
	{
		if( State.Tool.ID == TOOL_CREATE_PLANE )	{ model = pScene->AddModel( OBJECT_PLANE ); }
		if( State.Tool.ID == TOOL_CREATE_BOX )		{ model = pScene->AddModel( OBJECT_BOX ); }

		State.MasterObjectType = MODEL;
		State.pMasterObject = (void*)&pScene->pModel[model];			
		State.UsingMaster = FALSE;

		Math.ZeroVector3( &pScene->pModel[model].Rotation );
		Math.SetVector3( &pScene->pModel[model].Scale, 1, 1, 1 );

	//Update Position
		if( !State.UpdateMousePos3D( 0, &State.View.CreationPlane ) ) { return; }

		Math.CopyVector3( &pScene->pModel[model].Position, &State.Mouse[0].Pos3D );

		if( State.Tool.ID == TOOL_CREATE_PLANE )
		{
		//Set Up and Generate a New Plane 
			CPlane *pPlane = (CPlane*)pScene->pModel[model].pBaseObject; 

			Math.ZeroVector2( &pPlane->Size );

			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_PLANE, IDC_TASSELLATION_X_SPIN ), ASM_GET_VAL_INT, 0, (LPARAM)&pPlane->Tassellation[0] ); 
			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_PLANE, IDC_TASSELLATION_Y_SPIN ), ASM_GET_VAL_INT, 0, (LPARAM)&pPlane->Tassellation[1] ); 

			pPlane->GenerateMesh();

			if( State.View.ID == VIEW_FRONT )	{ pPlane->pMesh->pObject[0].Rotation[0] = 90; }
			if( State.View.ID == VIEW_LEFT )	{ pPlane->pMesh->pObject[0].Rotation[0] = 90; pPlane->pMesh->pObject[0].Rotation[2] = 90; }
		}

		if( State.Tool.ID == TOOL_CREATE_BOX )
		{
		//Set Up and Generate a New Plane 
			CBox *pBox = (CBox*)pScene->pModel[model].pBaseObject; 

			Math.ZeroVector3( &pBox->Size );

			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_TASSELLATION_X_SPIN ), ASM_GET_VAL_INT, 0, (LPARAM)&pBox->Tassellation[0] ); 
			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_TASSELLATION_Y_SPIN ), ASM_GET_VAL_INT, 0, (LPARAM)&pBox->Tassellation[1] ); 
			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_TASSELLATION_Z_SPIN ), ASM_GET_VAL_INT, 0, (LPARAM)&pBox->Tassellation[2] ); 

			pBox->GenerateMesh();

//			if( State.View.ID == VIEW_FRONT )	{ pBox->pMesh->pObject[0].Rotation[0] = 90; }
//			if( State.View.ID == VIEW_LEFT )	{ pBox->pMesh->pObject[0].Rotation[0] = 90; pBox->pMesh->pObject[0].Rotation[2] = 90; }
		}
		
	//Set Origin
		Math.ZeroVector3( &pScene->pModel[model].Mesh.pObject[0].Position );

	//Set Object Info
		GetDlgItemText( MainBar.GetGroupDialog( 0, IDD_OBJECT_INFO ), IDC_OBJECT_INFO_NAME, &pScene->pModel[model].Name[0], 32 );  
		AssignRandomColor( model );
		
		UpdateCreationDialog( model );

	//Update State Variables
		State.Tool.Step++;

	//Update Selection
		System.Selection.ClearEntries();
		State.TempValue4 = (float)System.Selection.AddEntry( MODEL, model, (void*)&pScene->pModel[model], TRUE );

		System.Selection.GizmoVisible = FALSE;
		System.Selection.BoxesVisible = FALSE;

		System.UpdateAllViews( FALSE );	
	}
}

void CCreateHandler::OnLButtonUp( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( !State.FileReady ) { return; }

//Quit if there are No Models
	if( pScene->ModelCount == 0 ) { State.Tool.Step = 0; return; }

	if( ( State.Tool.ID == TOOL_CREATE_BOX ) && ( State.Tool.Step == 1 ) )
	{ 
		GetCursorPos( &State.TempPoint1 );
		State.UpdateMouse( 0, State.TempPoint1 );

		State.Tool.Step = 2; 
		return; 
	}

	State.hTempCursor = SetCursor( LoadCursor( NULL, IDC_WAIT ) );

	if( ( State.Tool.ID == TOOL_CREATE_PLANE ) && ( State.Tool.Step == 1 ) )
	{
	//Fetch the Base Object
		CPlane * pPlane = (CPlane*)pScene->pModel[pScene->ModelCount - 1].pBaseObject; 

	//Determine if we Keep the Model ( Based on Size )
		if( ( pPlane->Size[0] == 0 ) && ( pPlane->Size[1] == 0 ) ) { pScene->DeleteModel( pScene->ModelCount - 1 ); System.Selection.ClearEntries(); }
		else
		{
			pPlane->UpdateMesh( TRUE );
			pScene->pModel[pScene->ModelCount - 1].CastShadow = TRUE;
		}
	}

	if( ( State.Tool.ID == TOOL_CREATE_BOX ) && ( State.Tool.Step == 2 ) )
	{
	//Fetch the Base Object
		CBox * pBox = (CBox*)pScene->pModel[pScene->ModelCount - 1].pBaseObject; 

	//Determine if we Keep the Model ( Based on Size )
		if( ( pBox->Size[0] == 0 ) && ( pBox->Size[1] == 0 ) && ( pBox->Size[2] == 0 ) ) { pScene->DeleteModel( pScene->ModelCount - 1 ); System.Selection.ClearEntries(); }
		else
		{
			pBox->UpdateMesh( TRUE );
			pScene->pModel[pScene->ModelCount - 1].CastShadow = TRUE;
		}
	}

//Update Views & Variables
	System.UpdateAllViews( FALSE );	
	State.Tool.Step = 0; 

	State.Tool.Active = FALSE;
	ReleaseCapture();

	SetCursor( State.hTempCursor );
}

void CCreateHandler::OnRButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( State.Tool.Active )
	{
		if( ( State.Tool.ID == TOOL_CREATE_PLANE ) && ( State.Tool.Step == 1 ) )
		{
			pScene->DeleteModel( pScene->ModelCount - 1 );
			System.Selection.ClearEntries();
		}

		if( ( State.Tool.ID == TOOL_CREATE_BOX ) && ( State.Tool.Step >= 1 ) )
		{
			pScene->DeleteModel( pScene->ModelCount - 1 );
			System.Selection.ClearEntries();
		}

	//Update Views & Variables
		System.UpdateAllViews( FALSE );	
		State.Tool.Step = 0; 

		State.Tool.Active = FALSE;
		ReleaseCapture();

		return;
	}

//Rollback Tool One Level
	if( State.RollbackTool() ) { return; } 
}


void CCreateHandler::OnMouseMove( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( !State.FileReady ) { return; }

	if( ( State.Tool.Step == 1 ) || ( State.Tool.Step == 2 ) )
	{
	//Update Cursor
		State.UpdateMousePos3D( 1, &State.View.CreationPlane );

		if( State.Tool.ID == TOOL_CREATE_PLANE )
		{
			CPlane * pPlane = (CPlane*)pScene->pModel[pScene->ModelCount - 1].pBaseObject; 

			if( State.View.Type == VT_PERSPECTIVE )	{ Math.SetVector2( &pPlane->Size, State.Mouse[1].Pos3D[0] - State.Mouse[0].Pos3D[0], State.Mouse[1].Pos3D[2] - State.Mouse[0].Pos3D[2] ); }
			if( State.View.ID == VIEW_TOP )			{ Math.SetVector2( &pPlane->Size, State.Mouse[1].Pos3D[0] - State.Mouse[0].Pos3D[0], State.Mouse[1].Pos3D[2] - State.Mouse[0].Pos3D[2] ); }
			if( State.View.ID == VIEW_FRONT )		{ Math.SetVector2( &pPlane->Size, State.Mouse[1].Pos3D[0] - State.Mouse[0].Pos3D[0], State.Mouse[1].Pos3D[1] - State.Mouse[0].Pos3D[1] ); }
			if( State.View.ID == VIEW_LEFT )		{ Math.SetVector2( &pPlane->Size, State.Mouse[1].Pos3D[2] - State.Mouse[0].Pos3D[2], State.Mouse[1].Pos3D[1] - State.Mouse[0].Pos3D[1] ); }

			if( IsDlgButtonChecked( MainBar.GetGroupDialog( 0, IDD_CREATE_PLANE ), IDC_CREATION_CENTER ) ) { Math.ScaleVector2( &pPlane->Size, 2.0f, &pPlane->Size ); }				
			else { Math.AverageVectors3( &State.Mouse[0].Pos3D, &State.Mouse[1].Pos3D, &pScene->pModel[pScene->ModelCount - 1].Position ); }

			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_PLANE, IDC_DIMENSION_X_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pPlane->Size[0] ); 
			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_PLANE, IDC_DIMENSION_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pPlane->Size[1] ); 

			pPlane->UpdateMesh( FALSE );
		}

		if( ( State.Tool.ID == TOOL_CREATE_BOX ) && ( State.Tool.Step == 1 ) )
		{
			CBox * pBox = (CBox*)pScene->pModel[pScene->ModelCount - 1].pBaseObject; 

			if( State.View.Type == VT_PERSPECTIVE )	{ Math.SetVector3( &pBox->Size, State.Mouse[1].Pos3D[0] - State.Mouse[0].Pos3D[0], 0, State.Mouse[1].Pos3D[2] - State.Mouse[0].Pos3D[2] ); }
			if( State.View.ID == VIEW_TOP )			{ Math.SetVector3( &pBox->Size, State.Mouse[1].Pos3D[0] - State.Mouse[0].Pos3D[0], 0, State.Mouse[1].Pos3D[2] - State.Mouse[0].Pos3D[2] ); }
			if( State.View.ID == VIEW_FRONT )		{ Math.SetVector3( &pBox->Size, State.Mouse[1].Pos3D[0] - State.Mouse[0].Pos3D[0], State.Mouse[1].Pos3D[1] - State.Mouse[0].Pos3D[1], 0 ); }
			if( State.View.ID == VIEW_LEFT )		{ Math.SetVector3( &pBox->Size, 0, State.Mouse[1].Pos3D[1] - State.Mouse[0].Pos3D[1], State.Mouse[1].Pos3D[2] - State.Mouse[0].Pos3D[2] ); }

			if( IsDlgButtonChecked( MainBar.GetGroupDialog( 0, IDD_CREATE_BOX ), IDC_CREATION_CENTER ) ) { Math.ScaleVector3( &pBox->Size, 2.0f, &pBox->Size ); }				
			else { Math.AverageVectors3( &State.Mouse[0].Pos3D, &State.Mouse[1].Pos3D, &pScene->pModel[pScene->ModelCount - 1].Position ); }

			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_DIMENSION_X_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pBox->Size[0] ); 
			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_DIMENSION_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pBox->Size[1] ); 
			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_DIMENSION_Z_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pBox->Size[2] ); 

			if( State.View.Type == VT_PERSPECTIVE ) { pScene->pModel[model].Mesh.pObject[pBox->Object].Position[1] = pBox->Size[1] / 2; }

			pBox->UpdateMesh( FALSE );
		}

		if( ( State.Tool.ID == TOOL_CREATE_BOX ) && ( State.Tool.Step == 2 ) )
		{
			CBox * pBox = (CBox*)pScene->pModel[pScene->ModelCount - 1].pBaseObject; 

			if( State.View.Type == VT_PERSPECTIVE )	{ pBox->Size[1] = (float)( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ); }
			if( State.View.ID == VIEW_TOP )			{ pBox->Size[1] = (float)( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ); }
			if( State.View.ID == VIEW_FRONT )		{ pBox->Size[2] = (float)( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ); }
			if( State.View.ID == VIEW_LEFT )		{ pBox->Size[0] = (float)( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ); }

			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_DIMENSION_X_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pBox->Size[0] ); 
			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_DIMENSION_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pBox->Size[1] ); 
			SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_DIMENSION_Z_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pBox->Size[2] ); 

			if( State.View.Type == VT_PERSPECTIVE ) { pScene->pModel[model].Position[1] = pBox->Size[1] / 2; }
			if( State.View.ID == VIEW_TOP )			{ pScene->pModel[model].Position[1] = pBox->Size[1] / 2; }

			pBox->UpdateMesh( FALSE );
		}

		SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_X_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[pScene->ModelCount - 1].Position[0] ); 
		SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[pScene->ModelCount - 1].Position[1] ); 
		SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_Z_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[pScene->ModelCount - 1].Position[2] ); 
	
		pScene->pModel[pScene->ModelCount - 1].UpdateExtents();

		System.Selection.SetEntry( (UINT)State.TempValue4, MODEL, pScene->ModelCount - 1, (void*)&pScene->pModel[pScene->ModelCount - 1], TRUE, &pScene->pModel[pScene->ModelCount - 1].Position );
		System.Selection.UpdateCenter();

		System.Selection.GizmoVisible = TRUE;
		System.Selection.BoxesVisible = TRUE;

		System.UpdateAllViews( TRUE );	
	}
}

void CCreateHandler::AssignRandomColor( WORD model )
{
	System.CurrentColorIndex = 200000 / rand();
	if( System.CurrentColorIndex > (UINT)( System.BasePaletteSize.x * System.BasePaletteSize.y - 1 ) ) { System.CurrentColorIndex = (UINT)( System.BasePaletteSize.x * System.BasePaletteSize.y - 1 ); }

	tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( MainBar.GetGroupControl( 0, IDD_OBJECT_INFO, IDC_OBJECT_INFO_COLOR ), GWL_USERDATA );
	if( pInfo )
	{
		pInfo->Color = System.BasePalette[(UINT)( System.CurrentColorIndex % 16 )][(UINT)( System.CurrentColorIndex / 16 )]; 
		RedrawWindow( MainBar.GetGroupControl( 0, IDD_OBJECT_INFO, IDC_OBJECT_INFO_COLOR ), NULL, NULL, RDW_INVALIDATE );

		pScene->pModel[model].SetColor( System.BasePalette[(UINT)( System.CurrentColorIndex % 16 )][(UINT)( System.CurrentColorIndex / 16 )] );
	}
}

void CCreateHandler::UpdateCreationDialog( WORD model )
{
//Update Creation Dialog
	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_X_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Position[0] ); 
	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Position[1] ); 
	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_Z_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Position[2] ); 

	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ROT_X_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Rotation[0] ); 
	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ROT_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Rotation[1] ); 
	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ROT_Z_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Rotation[2] ); 

	State.TempValue1 = pScene->pModel[model].Scale[0] * 100;
	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_SCALE_X_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TempValue1 ); 
	State.TempValue1 = pScene->pModel[model].Scale[1] * 100;
	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_SCALE_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TempValue1 ); 
	State.TempValue1 = pScene->pModel[model].Scale[2] * 100;
	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_SCALE_Z_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TempValue1 ); 

	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ORIGIN_X_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Mesh.pObject[0].Position[0] ); 
	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ORIGIN_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Mesh.pObject[0].Position[1] ); 
	SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ORIGIN_Z_SPIN ), ASM_SET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Mesh.pObject[0].Position[2] ); 
}
