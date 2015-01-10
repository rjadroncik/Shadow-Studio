// InitHandler.cpp: implementation of the CInitHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InitHandler.h"

#include "View.h"
#include "ViewUI.h"
#include "System.h"
#include "Shadow.h"
#include "Options.h"
#include "State.h"
#include "Afc.h"
#include "Resource.h"
#include "RenderGL/RenderGL.h"


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
extern ACustomUI			CustomUI;


extern ADialogBar			MainBar;
extern AToolBar				MainToolBar;
extern AToolBar				BottomToolBar;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInitHandler::CInitHandler()
{

}

CInitHandler::~CInitHandler()
{

}

void CInitHandler::InitDialogs()
{
//Initialize Selection Dialog Options
	Options.SelectDialog.CaseSensitive = BST_UNCHECKED;
	Options.SelectDialog.DisplaySubTree = BST_UNCHECKED;

	Options.SelectDialog.SortType = IDC_SORT_ABC;

	Options.SelectDialog.ListBones = BST_CHECKED;
	Options.SelectDialog.ListCameras = BST_CHECKED;
	Options.SelectDialog.ListGeometry = BST_CHECKED;
	Options.SelectDialog.ListGroups = BST_CHECKED;
	Options.SelectDialog.ListHelpers = BST_CHECKED;
	Options.SelectDialog.ListLights = BST_CHECKED;
	Options.SelectDialog.ListShapes = BST_CHECKED;
	Options.SelectDialog.ListWarps = BST_CHECKED;
}

void CInitHandler::InitTools()
{
//Select an Initial Tool
	SendMessage( GetDlgItem( MainToolBar.GetDialog(), IDC_SELECT ), BM_PUSH, 0, 0 ); 

//Select Gizmo Initial Axis Restriction
	ViewUI.MoveGizmo.ActiveObject = 5;
	ViewUI.MoveGizmo.SelectedObject = 5;

	ViewUI.ScaleGizmo.ActiveObject = 6;
	ViewUI.ScaleGizmo.SelectedObject = 6;
}

void CInitHandler::InitBottomToolBar()
{
	BottomToolBar.Create( System.hWnd, System.hInstance, TBS_HSCROLL | TBS_ALIGN_RIGHT );
	BottomToolBar.AddDialog( IDD_BOTTOMTOOLBAR, BottomToolBarProc );

	UINT x = CustomUI.AddImages16( "Data//System//Navigation.bmp", "Data//System//Navigation_a.bmp" );

//Pan
	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_PAN, x + 14 );
	SendMessage( BottomToolBar.GetControl( IDC_PAN ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

//Rotate
	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_ROTATE, x + 16 );
	SendMessage( BottomToolBar.GetControl( IDC_ROTATE ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	SendMessage( BottomToolBar.GetControl( IDC_ROTATE ), BM_ADD_SUBBUTTON, IDC_ROTATE_ORIGIN, x + 34 ); 
//	SendMessage( BottomToolBar.GetControl( IDC_ROTATE ), BM_ADD_SUBBUTTON, IDC_ROTATE_SUBOBJECT, x + 20 ); 
//	SendMessage( BottomToolBar.GetControl( IDC_ROTATE ), BM_ADD_SUBBUTTON, IDC_ROTATE_SELECTED, x + 18 ); 
	SendMessage( BottomToolBar.GetControl( IDC_ROTATE ), BM_ADD_SUBBUTTON, IDC_ROTATE_FOCUS, x + 16 ); 
	SendMessage( BottomToolBar.GetControl( IDC_ROTATE ), BM_SET_DROPDOWN_STYLE, DMS_UPWARD, 0 ); 
	SendMessage( BottomToolBar.GetControl( IDC_ROTATE ), BM_SELECT_SUBBUTTON, 1, 0 ); 
	

//Zoom
	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_ZOOM, x );
	SendMessage( BottomToolBar.GetControl( IDC_ZOOM ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

//Zoom All
	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_ZOOM_ALL, x + 2 );
	SendMessage( BottomToolBar.GetControl( IDC_ZOOM_ALL ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

//Field of View
	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_FOV, x + 24 );
	SendMessage( BottomToolBar.GetControl( IDC_FOV ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

//Zoom Extents
	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_CENTER, x + 28 );
	SendMessage( BottomToolBar.GetControl( IDC_CENTER ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	SendMessage( BottomToolBar.GetControl( IDC_CENTER ), BM_ADD_SUBBUTTON, IDC_CENTER_EVERYTHING, x + 4 ); 
	SendMessage( BottomToolBar.GetControl( IDC_CENTER ), BM_ADD_SUBBUTTON, IDC_CENTER_SELECTED, x + 6 ); 
	SendMessage( BottomToolBar.GetControl( IDC_CENTER ), BM_SET_DROPDOWN_STYLE, DMS_UPWARD, 0 ); 


//Zoom Extents All
	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_CENTER_ALL, x + 8 );
	SendMessage( BottomToolBar.GetControl( IDC_CENTER_ALL ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	SendMessage( BottomToolBar.GetControl( IDC_CENTER_ALL ), BM_ADD_SUBBUTTON, IDC_CENTER_ALL_EVERYTHING, x + 8 ); 
	SendMessage( BottomToolBar.GetControl( IDC_CENTER_ALL ), BM_ADD_SUBBUTTON, IDC_CENTER_ALL_SELECTED, x + 10 ); 
	SendMessage( BottomToolBar.GetControl( IDC_CENTER_ALL ), BM_SET_DROPDOWN_STYLE, DMS_UPWARD, 0 ); 

//Min / Max Toggle
	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_FOURVIEW, x + 22 );
	SendMessage( BottomToolBar.GetControl( IDC_FOURVIEW ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	x = CustomUI.AddImages16( "Data//System//VCRControls.bmp", "Data//System//VCRControls_a.bmp" );

	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_START, x );
	SendMessage( BottomToolBar.GetControl( IDC_START ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_PREVIOUS, x + 4 );
	SendMessage( BottomToolBar.GetControl( IDC_PREVIOUS ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

//Play Button
	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_PLAY, x + 6 );
	SendMessage( BottomToolBar.GetControl( IDC_PLAY ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	SendMessage( BottomToolBar.GetControl( IDC_PLAY ), BM_ADD_SUBIMAGE, (WPARAM)x + 6, 0 );
	SendMessage( BottomToolBar.GetControl( IDC_PLAY ), BM_ADD_SUBIMAGE, (WPARAM)x + 8, 0 );


	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_NEXT, x + 12 );
	SendMessage( BottomToolBar.GetControl( IDC_NEXT ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_END, x + 14 );
	SendMessage( BottomToolBar.GetControl( IDC_END ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_OPTIONS, x + 18 );
	SendMessage( BottomToolBar.GetControl( IDC_OPTIONS ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );


	x = CustomUI.AddImages16( "Data//System//KeyModeToggle.bmp", "Data//System//KeyModeToggle_a.bmp" );

	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_KEYMODE, x );
	SendMessage( BottomToolBar.GetControl( IDC_KEYMODE ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );


	System.SelectGroup( "AnimControls" );
	System.AddButton( BottomToolBar.GetDialog(), IDC_KEYMODE_AUTO );
	System.AddButton( BottomToolBar.GetDialog(), IDC_KEYMODE_SET );

	x = CustomUI.AddImages24( "Data//System//AnimButton.bmp", "Data//System//AnimButton_a.bmp" );

	CustomUI.SetButtonImage24( BottomToolBar.GetDialog(), IDC_SETKEY, x );
	SendMessage( BottomToolBar.GetControl( IDC_SETKEY ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );


/*
	SendDlgItemMessage( hWnd, IDC_TASSELLATION_X_SPIN, UDM_SETRANGE, 0, MAKELONG( 1, 100 ) );
	SendDlgItemMessage( hWnd, IDC_TASSELLATION_Y_SPIN, UDM_SETRANGE, 0, MAKELONG( 1, 100 ) );

	SendDlgItemMessage( hWnd, IDC_DIMENSION_X_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_DIMENSION_X ), 0 );
	SendDlgItemMessage( hWnd, IDC_DIMENSION_Y_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_DIMENSION_Y ), 0 );

	SendDlgItemMessage( hWnd, IDC_TASSELLATION_X_SPIN, UDM_SETACCEL, 1, 0 );
	SendDlgItemMessage( hWnd, IDC_TASSELLATION_Y_SPIN, UDM_SETACCEL, 1, 0 );
	
	SendDlgItemMessage( hWnd, IDC_DIMENSION_X_SPIN, UDM_SETACCEL, 1, 0 );
	SendDlgItemMessage( hWnd, IDC_DIMENSION_Y_SPIN, UDM_SETACCEL, 1, 0 );*/

	SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_X_SPIN ), UDM_SETBUDDY, (WPARAM)GetDlgItem( BottomToolBar.GetDialog(), IDC_TRANSFORM_X ), 0 );
	SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_Y_SPIN ), UDM_SETBUDDY, (WPARAM)GetDlgItem( BottomToolBar.GetDialog(), IDC_TRANSFORM_Y ), 0 );
	SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_Z_SPIN ), UDM_SETBUDDY, (WPARAM)GetDlgItem( BottomToolBar.GetDialog(), IDC_TRANSFORM_Z ), 0 );

	SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_X_SPIN ), ASM_SET_VAL_FLOAT, 0, NULL );
	SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_Y_SPIN ), ASM_SET_VAL_FLOAT, 0, NULL );
	SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_Z_SPIN ), ASM_SET_VAL_FLOAT, 0, NULL );

	System.SelectGroup( "Transforms_Local" );
	System.AddButton( BottomToolBar.GetDialog(), IDC_TRANSFORM_LOCAL );

	x = CustomUI.AddImages16( "Data//System//Transforms.bmp", "Data//System//Transforms_a.bmp" );

	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_TRANSFORM_LOCAL, x );

	SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_LOCAL ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );
	SendMessage( BottomToolBar.GetControl( IDC_TRANSFORM_LOCAL ), BM_SET_PUSHED_IMAGE, x + 1, 0 );

	System.SelectGroup( "Selection_Lock" );
	System.AddButton( BottomToolBar.GetDialog(), IDC_SELECTION_LOCK );

	CustomUI.SetButtonImage16( BottomToolBar.GetDialog(), IDC_SELECTION_LOCK, x + 2 );
	SendMessage( BottomToolBar.GetControl( IDC_SELECTION_LOCK ), BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );
}

void CInitHandler::InitMainToolBar()
{
	MainToolBar.Create( System.hWnd, System.hInstance, TBS_HSCROLL | TBS_MARGIN_TOP );
	MainToolBar.AddDialog( IDD_MAINTOOLBAR, MainToolBarProc );

	UINT x = CustomUI.AddImages24( "Data//System//Maintoolbar.bmp", "Data//System//Maintoolbar_a.bmp" );

	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_UNDO, x );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_UNDO, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_REDO, x + 2 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_REDO, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );


	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_BIND, x + 4 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_BIND, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_UNBIND, x + 6 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_UNBIND, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_WARPBIND, x + 8 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_WARPBIND, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );


	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_SELECT, x + 10 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_SELECT, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	System.SelectGroup( "Selection_Mode" );
	System.AddButton( MainToolBar.GetDialog(), IDC_SELECT_MODE );

	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_SELECT_MODE, x + 12 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_SELECT_MODE, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );
//	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_SELECT_MODE, BM_PUSH, 0, 0 );

	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_SELECT_MODE, BM_ADD_SUBBUTTON, IDC_SELECT_RECT, x + 12 ); 
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_SELECT_MODE, BM_ADD_SUBBUTTON, IDC_SELECT_CIRCLE, x + 14 ); 
//	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_SELECT_MODE, BM_ADD_SUBBUTTON, IDC_SELECT_POLYGON, x + 16 ); 


	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_SELECT_DIALOG, x + 18 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_SELECT_DIALOG, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );


	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_EDIT_MOVE, x + 20 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_EDIT_MOVE, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_EDIT_ROTATE, x + 22 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_EDIT_ROTATE, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );
	
	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_EDIT_SCALE, x + 24 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_EDIT_SCALE, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_EDIT_SCALE, BM_ADD_SUBBUTTON, IDC_EDIT_USCALE, x + 24 ); 
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_EDIT_SCALE, BM_ADD_SUBBUTTON, IDC_EDIT_NSCALE, x + 26 ); 
//	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_SCALE, BM_ADD_SUBBUTTON, IDC_SCALE_SQUASH, x + 28 ); 

	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_EDIT_CENTER, x + 30 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_EDIT_CENTER, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );

	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_EDIT_CENTER, BM_ADD_SUBBUTTON, IDC_EDIT_CENTER_ORIGIN, x + 30 ); 
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_EDIT_CENTER, BM_ADD_SUBBUTTON, IDC_EDIT_CENTER_SELECTION, x + 32 ); 
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_EDIT_CENTER, BM_ADD_SUBBUTTON, IDC_EDIT_CENTER_SYSTEM, x + 34 ); 

	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_EDIT_CENTER, BM_SELECT_SUBBUTTON, 1, 0 ); 

	
	x = CustomUI.AddImages24( "Data//System//SnapsControls.bmp", "Data//System//SnapsControls_a.bmp" );

	CustomUI.SetButtonImage24( MainToolBar.GetDialog(), IDC_SELECT_CROSSING, x + 6 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_SELECT_CROSSING, BM_SET_TOOLTIP, (WPARAM)System.hToolTip, 0 );
	SendDlgItemMessage( MainToolBar.GetDialog(), IDC_SELECT_CROSSING, BM_SET_PUSHED_IMAGE, x + 4, 0 ); 

	System.SelectGroup( "Select_Crossing" );
	System.AddButton( MainToolBar.GetDialog(), IDC_SELECT_CROSSING );
}

void CInitHandler::InitSystem( HINSTANCE hInstance )
{
//Reset State 
	State.Reset();
	System.InitPalette();
	System.SelectGroup( "Global" );

//Create all Windows
	System.RegisterWndClassMain( "ShadowStudio", NULL, WndProc, hInstance );
	System.RegisterWndClassGL( "ViewportGL", WndProcGL );

	LoadString( hInstance, IDS_APP_TITLE, State.TempBuffer64a, 64 );
	System.CreateWindowMain( WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW, State.TempBuffer64a ); 

	System.ViewCount = 4;
	System.View[0].Create( VT_ORTHO, VIEW_TOP, &System.hRC, System.ClassNameGL, System.hInstance, System.hWnd, 0 );
	System.View[0].AbsRotate( 90, 0, 0 );
	strcpy( System.View[0].Name, "Top" );
	Math.MakePlane( &System.View[0].Position, &System.View[0].Direction, &System.View[0].CreationPlane );

	System.View[1].Create( VT_ORTHO, VIEW_FRONT, &System.hRC, System.ClassNameGL, System.hInstance, System.hWnd, 1 );
	strcpy( System.View[1].Name, "Front" );
	
	System.View[2].Create( VT_ORTHO, VIEW_LEFT, &System.hRC, System.ClassNameGL, System.hInstance, System.hWnd, 2 );
	System.View[2].AbsRotate( 0, 90, 0 );
	strcpy( System.View[2].Name, "Left" );
	Math.MakePlane( &System.View[2].Position, &System.View[2].Direction, &System.View[2].CreationPlane );
	
	System.View[3].Create( VT_PERSPECTIVE, VIEW_PERSPECTIVE, &System.hRC, System.ClassNameGL, System.hInstance, System.hWnd, 3 );
	System.View[3].AbsRotate( 30, 180, 0 );
	System.View[3].Dolly( -300, FALSE, TRUE );
	strcpy( System.View[3].Name, "Perspective" );

	System.View[3].Active = TRUE;
	System.View[3].Update( TRUE );

//Create Rendering Context	
	System.SetPixelFormatGL( Options.ColorBits, Options.DepthBits, Options.StencilBits );
	System.CreateRenderingContext();

//Initialize Fonts
//	System.FontTGA.Create( "Data/Font/Font.tga", 12 );

	System.hDC = GetDC( System.hWnd );
	System.FontGL.Create( "Arial", 14, System.hDC, 256 );
	ReleaseDC( System.hWnd, System.hDC );

//Set Main Window Position
	GetWindowRect( GetDesktopWindow(), &System.ScreenRect );
	System.ScreenRect.bottom -= System.TaskBarHeight - 2;

	System.Position.x = 0;
	System.Position.y = 0;
	System.Size.x = System.ScreenRect.right;
	System.Size.y = System.ScreenRect.bottom;

	System.WindowState = Options.WindowState;

	System.Position.x = Options.WindowRect.left;
	System.Position.y = Options.WindowRect.top;

	System.Size.x = Options.WindowRect.right - Options.WindowRect.left;
	System.Size.y = Options.WindowRect.bottom - Options.WindowRect.top;

	System.hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDR_MAINACCEL ) );

//Initialize Controls
	System.MainToolBarHeight = 34;
	System.BottomToolBarHeight = 52;
	System.MainBarWidth = 232;
	System.hCursorHand = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_DRAG ) );
	System.hCursorNone = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_NONE ) );
	System.hCursorCross = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_SELECT ) );
	

//Initialize Selection

	System.OpenGLVersion = glGetString( GL_VERSION ); 

	CustomUI.hInstance = System.hInstance;

	Options.ColorBits = System.ColorBits;
	Options.DepthBits = System.DepthBits;
	Options.StencilBits = System.StencilBits;
}



void CInitHandler::InitMainBar()
{
//Initialize Variables + Create The Dialog Bar
	System.MainBarWidth = 232;
	MainBar.Create( System.hWnd, System.hInstance, IDC_MAINSCROLLBAR );

//Add All The Empty Dialogs + Add a Tab Dialog 
	MainBar.AddDialog( IDD_MAIN, MainDlgProc ); 
	MainBar.AddDialog( IDD_MAIN, MainDlgProc ); 
	MainBar.AddDialog( IDD_MAIN, MainDlgProc ); 
	MainBar.AddDialog( IDD_MAIN, MainDlgProc ); 
	MainBar.AddDialog( IDD_MAIN, MainDlgProc ); 
	MainBar.SetTabBar( IDD_MAINTAB, MainTabProc ); 

	System.SelectGroup( "MainTabBar" );
	System.AddButton( MainBar.GetTabBar(), IDC_TAB_CREATE );
	System.AddButton( MainBar.GetTabBar(), IDC_TAB_MODIFY );
	System.AddButton( MainBar.GetTabBar(), IDC_TAB_MOTION );
	System.AddButton( MainBar.GetTabBar(), IDC_TAB_DISPLAY );
	System.AddButton( MainBar.GetTabBar(), IDC_TAB_UTILITY );

//Update Position
	if( System.WindowState == SWS_NORMAL ) { MainBar.SetBarPosition( System.Size.x - System.MainBarWidth - 7, System.MainToolBarHeight + 1, System.MainBarWidth - 2, System.Dimension.y - 2 + System.TrackBarHeight  ); }
	if( System.WindowState == SWS_MAXIMIZED ) { MainBar.SetBarPosition( System.Size.x - System.MainBarWidth - 3, System.MainToolBarHeight + 1, System.MainBarWidth - 2, System.Dimension.y - 2 + System.TrackBarHeight ); }

//Fill the Empty Dialogs
	MainBar.SelectGroup( 0, IDD_CREATE_GROUPS, CreateProc );
	MainBar.SelectGroup( 1, IDD_MODIFIER_SELECT, ModifierSelectProc );

//	MainBar.ReArrangeGroups();

//Should go Last
	MainBar.Initialize();
}

void CInitHandler::InitTrackBar()
{
	RECT rt; GetClientRect( System.hWnd, &rt );

	System.TrackBarHeight = 48;
	System.TrackBarID = 9871;

	System.hTrackBar = CreateWindowEx( NULL, "ATrackBar", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 10, 10, System.hWnd, (HMENU)System.TrackBarID, System.hInstance, NULL );

//	SetWindowPos( System.hTrackBar, NULL, 0, System.Size.y - System.TrackBarHeight - System.ToolBarHeight, System.Size.x - System.MainBarWidth, System.TrackBarHeight, SWP_SHOWWINDOW );
}
