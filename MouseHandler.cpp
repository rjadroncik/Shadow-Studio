// MouseHandler.cpp: implementation of the CMouseHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MouseHandler.h"

#include "View.h"
#include "ViewUI.h"
#include "System.h"
#include "Shadow.h"
#include "Options.h"
#include "State.h"
#include "Afc.h"
#include "Resource.h"
#include "RenderGL/RenderGL.h"
#include "winuser.h"

//Other handlers
#include "ViewHandler.h"
#include "CreateHandler.h"
#include "EditHandler.h"
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

//Other handlers
extern CViewHandler			ViewHandler;
extern CCreateHandler		CreateHandler;
extern CEditHandler			EditHandler;
extern CSelectHandler		SelectHandler;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMouseHandler::CMouseHandler()
{

}

CMouseHandler::~CMouseHandler()
{

}


void CMouseHandler::OnLButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( !State.FileReady || !State.Tool.Enabled ) { return; }

//Activate Current Window
	UINT viewIndex = GetWindowLong( hwnd, GWL_USERDATA ); 
	if( viewIndex != System.ViewActive ) { System.ActivateView( viewIndex ); }

//Update State
	State.Tool.Active = TRUE;
	State.View = System.View[viewIndex];

//Call Other Handlers
	if( State.Tool.Type == TOOL_SELECT )	{ SelectHandler.OnLButtonDown( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_VIEW )		{ ViewHandler.OnLButtonDown( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_CREATE )	{ CreateHandler.OnLButtonDown( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_EDIT )		{ EditHandler.OnLButtonDown( hwnd, wParam, lParam ); }

	SetFocus( System.hWnd );
}

void CMouseHandler::OnLButtonUp( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( !State.FileReady || !State.Tool.Enabled ) { return; }
	UINT viewIndex = GetWindowLong( hwnd, GWL_USERDATA ); 

//Update State
	State.View = System.View[viewIndex];

//Mouse Work
	GetCursorPos( &State.TempPoint1 );
	State.UpdateMouse( 1, State.TempPoint1 );

//Call Other Handlers
	if( State.Tool.Type == TOOL_SELECT )	{ SelectHandler.OnLButtonUp( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_VIEW )		{ ViewHandler.OnLButtonUp( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_CREATE )	{ CreateHandler.OnLButtonUp( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_EDIT )		{ EditHandler.OnLButtonUp( hwnd, wParam, lParam ); }
	
	SetFocus( System.hWnd );
}

void CMouseHandler::OnRButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( !State.FileReady || !State.Tool.Enabled ) { return; }

	UINT viewIndex = GetWindowLong( hwnd, GWL_USERDATA ); 
	if( viewIndex != System.ViewActive )
	{ 
		System.ActivateView( viewIndex ); 
		
	//Mouse Work
		GetCursorPos( &State.TempPoint1 );
		SetCapture( hwnd );
		State.UpdateMouse( 1, State.TempPoint1 );

	//Update View Info
		State.View = System.View[viewIndex];
		
		return; 
	}

//Mouse Work
	GetCursorPos( &State.TempPoint1 );
	SetCapture( hwnd );
	State.UpdateMouse( 0, State.TempPoint1 );

//Update View Info
	State.View = System.View[viewIndex];

//Call Other Handlers
	if( State.Tool.Type == TOOL_SELECT )	{ SelectHandler.OnRButtonDown( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_CREATE )	{ CreateHandler.OnRButtonDown( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_EDIT )		{ EditHandler.OnRButtonDown( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_VIEW )		{ ViewHandler.OnRButtonDown( hwnd, wParam, lParam ); }

	SetFocus( System.hWnd );
}

void CMouseHandler::OnRButtonUp( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( !State.FileReady || !State.Tool.Enabled ) { return; }

//Call Other Handlers
	if( State.Tool.Type == TOOL_SELECT )	{ SelectHandler.OnRButtonUp( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_VIEW )		{ ViewHandler.OnRButtonUp( hwnd, wParam, lParam ); }

	ReleaseCapture();

	SetFocus( System.hWnd );
}

void CMouseHandler::OnMButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( !State.FileReady || !State.Tool.Enabled ) { return; }

//Activate Current Window
	UINT viewIndex = GetWindowLong( hwnd, GWL_USERDATA ); 
	if( viewIndex != System.ViewActive ) { System.ActivateView( viewIndex ); }

//Update State
	State.View = System.View[viewIndex];

//Mouse Work
	GetCursorPos( &State.TempPoint1 );
	SetCapture( hwnd );
	State.UpdateMouse( 0, State.TempPoint1 );

//Call Other Handlers
	if( State.Tool.Type == TOOL_VIEW ) { ViewHandler.OnMButtonDown( hwnd, wParam, lParam ); }

	SetFocus( System.hWnd );
}

void CMouseHandler::OnMButtonUp( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( !State.FileReady || !State.Tool.Enabled ) { return; }
	UINT viewIndex = GetWindowLong( hwnd, GWL_USERDATA ); 

//Update State
	State.View = System.View[viewIndex];

//Call Other Handlers
	if( State.Tool.Type == TOOL_VIEW ) { ViewHandler.OnMButtonUp( hwnd, wParam, lParam ); }

	ReleaseCapture();

	SetFocus( System.hWnd );
}

void CMouseHandler::OnMouseMove( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( !State.FileReady || !State.Tool.Enabled ) { return; }
	UINT viewIndex = GetWindowLong( hwnd, GWL_USERDATA ); 

//Set Cursor If View Not Active
	if( viewIndex != System.ViewActive ) { SetCursor( System.hCursorNone ); return; }

	GetCursorPos( &State.TempPoint2 );
	State.UpdateMouse( 1, State.TempPoint2 );

//Discard Useless Messages
	if( ( State.Mouse[1].Pos2D.x == State.Mouse[0].Pos2D.x ) && ( State.Mouse[1].Pos2D.y == State.Mouse[0].Pos2D.y ) ) { return; }

//Auto Panning
	if( ( wParam & MK_MBUTTON ) == MK_MBUTTON ) { AutoPan(); return; }

// Set Apropriate Cursor for Selecting
	if( ( State.Tool.Type == TOOL_SELECT ) && !State.Tool.Active  )
	{
		System.Selection.UpdateRegion( State.Mouse[1].Pos2D );

		if( System.Selection.ParseModels( SM_CURSOR_TEST_ALL ) ) { SetCursor( State.Tool.hCursor ); }
		else { SetCursor( System.hCursorNone ); }
	}

// Set Apropriate Cursor for Editing
	if( ( State.Tool.Type == TOOL_EDIT ) && !State.Tool.Active ) 
	{	
		System.Selection.UpdateRegion( State.Mouse[1].Pos2D );

		if( System.Selection.GizmoVisible )
		{
			if( System.Selection.ParseGizmos() ) { System.UpdateAllViews( FALSE ); }

		//If We Are Over a Gizmo Part, Set Cursor  
			if( System.Selection.GizmoHighlighted ) { SetCursor( State.Tool.hCursor ); } 
		}
		
		if( !System.Selection.GizmoHighlighted )
		{
		//If We Are Over a Model, Set Cursor  
			if( System.Selection.ParseModels( SM_CURSOR_TEST_ALL ) ) { SetCursor( State.Tool.hCursor ); }
			else { SetCursor( System.hCursorNone ); }
		}
	}

	if( ( State.Tool.Type != TOOL_EDIT ) && ( State.Tool.Type != TOOL_SELECT ) ) { SetCursor( State.Tool.hCursor ); }

//Call Handlers
	if( State.Tool.Type == TOOL_SELECT )	{ SelectHandler.OnMouseMove( hwnd, wParam, lParam ); }

//Skip if Tool Inactive
	if( !State.Tool.Active ) { return; }

//Call Active Only Handlers
	if( State.Tool.Type == TOOL_EDIT )		{ EditHandler.OnMouseMove( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_VIEW )		{ ViewHandler.OnMouseMove( hwnd, wParam, lParam ); }
	if( State.Tool.Type == TOOL_CREATE )	{ CreateHandler.OnMouseMove( hwnd, wParam, lParam ); }
}

void CMouseHandler::AutoPan()
{
	SetCursor( System.hCursorHand );

	System.View[System.ViewActive].RePosition( State.View.Position[0], State.View.Position[1], State.View.Position[2], FALSE );
	
	if( State.View.ID == VIEW_PERSPECTIVE )
	{
		System.View[System.ViewActive].Move( (float)( -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) * State.View.Distance / State.View.Rect.bottom ), (float)( -( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * State.View.Distance / State.View.Rect.bottom ), 0 ); 
	}
	if( State.View.ID == VIEW_TOP )
	{
		System.View[System.ViewActive].Move( (float)( -( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) * State.View.Size / State.View.Rect.bottom * 2 ), (float)( -( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * State.View.Size / State.View.Rect.bottom * 2 ), 0 ); 
	}
	if( State.View.ID == VIEW_FRONT )
	{
		System.View[System.ViewActive].Move( (float)( ( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) * State.View.Size / State.View.Rect.bottom * 2 ), (float)( ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * State.View.Size / State.View.Rect.bottom * 2 ), 0 ); 
	}
	if( State.View.ID == VIEW_LEFT )
	{
		System.View[System.ViewActive].Move( (float)( ( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) * State.View.Size / State.View.Rect.bottom * 2 ), (float)( ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * State.View.Size / State.View.Rect.bottom * 2 ), 0 ); 
	}

	System.Update = TRUE;
	System.View[System.ViewActive].NeedUpdate = TRUE;
}

