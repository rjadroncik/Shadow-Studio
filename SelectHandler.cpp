// SelectHandler.cpp: implementation of the CSelectHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectHandler.h"

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

CSelectHandler::CSelectHandler()
{

}

CSelectHandler::~CSelectHandler()
{

}

void CSelectHandler::UpdateRegion( bool draw )
{
	if( draw ) { System.View[System.ViewActive].DrawRegion(); }

	if( System.Selection.RegionType == SRT_RECTANGLE )
	{
	//Prepare Rectangle 
		State.TempRect1.left = State.Mouse[0].Pos2D.x;
		State.TempRect1.top  = State.Mouse[0].Pos2D.y;

		State.TempRect1.right  = State.Mouse[1].Pos2D.x;
		State.TempRect1.bottom = State.Mouse[1].Pos2D.y;

		if( draw ) { System.Selection.UpdateRegion( State.TempRect1, FALSE ); }
		else { System.Selection.UpdateRegion( State.TempRect1, TRUE ); }
	}

	if( System.Selection.RegionType == SRT_CIRCLE )
	{
	//Prepare Buffer 
		if( ( ( State.Tool.Type == TOOL_SELECT ) && ( State.Tool.Step == 0 ) ) || ( ( State.Tool.Type == TOOL_EDIT ) && ( State.Tool.Step == 3 ) ) )
		{
			State.RegionPointCount = 33;
			State.pRegionPoint = (POINT*)malloc( State.RegionPointCount * sizeof( POINT ) );
		}

	//Prepare Variables
		State.TempValue1 = -6.283185307179f / (float)( State.RegionPointCount - 1 );
		State.TempValue2 = 0; 
		State.TempValue3 = (float)sqrt( ( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) * ( State.Mouse[1].Pos2D.x - State.Mouse[0].Pos2D.x ) + ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) * ( State.Mouse[1].Pos2D.y - State.Mouse[0].Pos2D.y ) );

	//Create Circle
		for( UINT n = 0; n < State.RegionPointCount; n++ )
		{
			State.pRegionPoint[n].x = (LONG)( State.Mouse[0].Pos2D.x + (float)cos( State.TempValue2 ) * State.TempValue3 );
			State.pRegionPoint[n].y = (LONG)( State.Mouse[0].Pos2D.y + (float)sin( State.TempValue2 ) * State.TempValue3 );

			State.TempValue2 += State.TempValue1;
		}

		if( draw ) { System.Selection.UpdateRegion( State.pRegionPoint, State.RegionPointCount, FALSE ); }
		else { System.Selection.UpdateRegion( State.pRegionPoint, State.RegionPointCount, TRUE ); }
	}

	System.Selection.RegionVisible = TRUE;

	if( draw ) { System.View[System.ViewActive].DrawRegion(); }

//	SwapBuffers( System.View[System.ViewActive].hDC );
}



void CSelectHandler::OnLButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
//Mouse Work
	GetCursorPos( &State.TempPoint1 );
	SetCapture( hwnd );
	State.UpdateMouse( 0, State.TempPoint1 );

	SetCursor( State.Tool.hCursor );
}

void CSelectHandler::OnLButtonUp( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( State.Tool.Type == TOOL_SELECT )
	{	
		if( State.Tool.Step == 1 )
		{
			UpdateRegion( FALSE );

			if( ( wParam & MK_CONTROL ) == MK_CONTROL ) { System.Selection.Update( SM_ADD ); }
			else { System.Selection.Update( SM_SELECT_MULTY ); }
		}

		if( State.Tool.Step == 0 )
		{
			System.Selection.UpdateRegion( State.Mouse[0].Pos2D );
	
			if( ( wParam & MK_CONTROL ) == MK_CONTROL ) { System.Selection.Update( SM_MODIFY ); }
			else { System.Selection.Update( SM_SELECT_SINGLE ); }
	
			if( System.Selection.ActiveInfos == 0 ) { SetCursor( System.hCursorNone ); }
		}
	
		System.Selection.RegionVisible = FALSE;
		System.UpdateAllViews( FALSE );	

		State.Tool.Step = 0;
	}

	State.Tool.Active = FALSE;

	ReleaseCapture();
}

void CSelectHandler::OnMouseMove( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( ( State.Tool.Type == TOOL_SELECT ) && State.Tool.Active )
	{
		UpdateRegion( TRUE );

		SetCursor( State.Tool.hCursor );

		State.Tool.Step = 1;
	}
}

void CSelectHandler::OnRButtonDown( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( State.Tool.Type == TOOL_SELECT )
	{ 
		if( State.Tool.Step == 0 ) { State.Tool.Step = 3; }

		SetCursor( State.Tool.hCursor );
	}
}

void CSelectHandler::OnRButtonUp( HWND hwnd, WPARAM wParam, LPARAM lParam )
{
	if( State.Tool.Type == TOOL_SELECT )
	{ 
		if( State.Tool.Step == 3 )
		{
			System.Selection.UpdateRegion( State.Mouse[0].Pos2D );
	
			if( ( wParam & MK_CONTROL ) == MK_CONTROL ) { System.Selection.Update( SM_MODIFY ); }
			else { System.Selection.Update( SM_SELECT_SINGLE ); }

			State.Tool.Step = 0;
		}
	 
		System.Selection.RegionVisible = FALSE;
		System.UpdateAllViews( FALSE );
	}
}
