// Shadow.cpp : Defines the entry point for the application.
//

//Editor Specific Include Files
#include "StdAfx.h"
#include "Shadow.h"
#include "ViewUI.h"
#include "Options.h"
#include "State.h"

//Handlers
#include "InitHandler.h"
#include "MouseHandler.h"
#include "KeyHandler.h"
#include "FileHandler.h"
#include "ViewHandler.h"
#include "CreateHandler.h"
#include "EditHandler.h"
#include "SelectHandler.h"


//Base Classes
	CSystem			System;
	CState			State;
	CMath			Math;
	CViewUI			ViewUI;

//Handler Classes
	CInitHandler		InitHandler;
	CMouseHandler		MouseHandler;
	CKeyHandler			KeyHandler;
	CFileHandler		FileHandler;
	CViewHandler		ViewHandler;
	CCreateHandler		CreateHandler;
	CEditHandler		EditHandler;
	CSelectHandler		SelectHandler;

//File Classes
	CScene			*pScene = NULL;
	
	C3sfFile		My3sfFile;
	C2smFile		My2smFile;


//Important State Variables
	COptions		Options;

//AFC Classes
	ADialogBar		MainBar;
	ACustomUI		CustomUI;

	AToolBar		MainToolBar;
	AToolBar		BottomToolBar;



int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	Options.Load( hInstance );
	InitHandler.InitSystem( hInstance );

	InitControlsAFC( System.hInstance );
	System.hToolTip = CreateWindowEx( NULL, "AToolTip", NULL, WS_POPUP, 0, 0, 100, 100, System.hWnd, NULL, System.hInstance, NULL ); 

	InitHandler.InitTrackBar();
	InitHandler.InitMainToolBar();
	InitHandler.InitBottomToolBar();
	InitHandler.InitMainBar();
	InitHandler.InitDialogs();

//Create a Staring File
	FileHandler.OnFileNew( TRUE );

	_getcwd( System.WorkingDirectory, 256 );
	strncat( System.WorkingDirectory, "/", 1 );

//Timing Variables
	CTimer	SystemTimer;
	SystemTimer.Create();

	float	Lag = 0.0;
	UINT	UpdateCount = 0;

	float	UpdateTime = 0.0;	
	float	RenderTime = 0.0;	

	UINT	FrameRate = 30;
	float	FrameTime;
	FrameTime = 1.0f / FrameRate;

	SystemTimer.Reset();
	SystemTimer.Reset2();
	float start = SystemTimer.GetTime();

	InitHandler.InitTools();
	System.ActivateView( 3 );
	State.View = System.View[3];

//Resize Our Window
	if( Options.WindowState == SWS_MAXIMIZED )
	{
		WINDOWPLACEMENT Placement;
		GetWindowPlacement( System.hWnd, &Placement );

		Placement.rcNormalPosition = System.ScreenRect;
		Placement.showCmd = SW_SHOWMAXIMIZED;

		SetWindowPlacement( System.hWnd, &Placement );
		
		System.MarginSize = 4;
		SendMessage( System.hWnd, WM_SIZE, 0, 0 ); 
	}
	else { System.ResizeWindowMain(); }


	while ( !System.Quit )  
	{
		if ( PeekMessage( &System.Msg, NULL, 0, 0, PM_REMOVE ) )  
		{
			if( System.Msg.message == WM_QUIT ) { System.Quit = TRUE; }
			else  
			{
				if( !TranslateAccelerator( System.hWnd, System.hAccelTable, &System.Msg ) )
				{
					TranslateMessage( &System.Msg );  
					DispatchMessage( &System.Msg );   
				}
			}
		}

		else
		{
			if( ( SystemTimer.GetTimeOffset() >= FrameTime && System.Animate ) || System.Update )
			{
		
			//Add System Timer Lag
				Lag += SystemTimer.GetTimeOffset() - FrameTime;
			
			//Reset Lag Timer	
				SystemTimer.Reset2();

			//Update World
//				Game.UpdateWorld();

			//Check Update Time
				UpdateTime = SystemTimer.GetTimeOffset2();
		
			//Update World >> As Many Times as Necessary to Compensate The Overall Lag
				while( UpdateCount > 0 )
				{
//					Game.UpdateWorld();
					UpdateCount--;
				}

			//Reset Lag Timer
				SystemTimer.Reset2();
	
			//Update/Render Views, which Need to be Updated
				if( System.FourViews )
				{ 
					for( UINT e = 0; e < System.ViewCount; e++ )
					{ 
						if( e == System.ViewActive ) { continue; }
						System.View[e].Update( FALSE ); 
					} 
				}
				System.View[System.ViewActive].Update( FALSE );

			//Check The Render Time	
				RenderTime = SystemTimer.GetTimeOffset2();

			//Count in Lag if There is Some
				if( ( UpdateTime + RenderTime ) > FrameTime )
				{
					Lag += ( UpdateTime + RenderTime ) - FrameTime;
				}

			//Universal:) Lag handler
				while( Lag > FrameTime )
				{
					Lag -= FrameTime;
					UpdateCount ++;
				}

				Lag += RenderTime + UpdateTime * UpdateCount;

			//Reset System Timer
				SystemTimer.Reset();

			//Editor Add-On >> Prevent Unneded Updating
				System.Update = FALSE;
			}
		}
	}
	return (System.Msg.wParam);
}


LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//Parse the Message
	switch( uMsg )
	{
	case WM_KEYDOWN: { KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_MOUSEMOVE: { System.CancelHighLight(); break; }

	case WM_CREATE:
		{
			return FALSE;
		}

	case WM_PAINT:
		{
	//Draw The Main Dialog Frame + Update Views if Necessary
			if( GetUpdateRect( hWnd, &State.TempRect1, FALSE ) )
			{
				CustomUI.DrawADialogBar( hWnd );
				
				if( ( System.ViewCount > 0 ) && Options.UpdateOnDialogMove )
				{
					for( UINT d = 0; d < System.ViewCount; d++ )
					{ 
						if( State.TempRect1.left <= System.View[d].Rect.left + System.View[d].Rect.right  && State.TempRect1.right  >= System.View[d].Rect.left   && 
							State.TempRect1.top  <= System.View[d].Rect.top  + System.View[d].Rect.bottom && State.TempRect1.bottom >= System.View[d].Rect.top )
						{
							System.View[d].Update( TRUE ); 
						}
					}
				}
			}
			break;
		}

	case WM_SIZING:
		{
			LPRECT rect = (LPRECT) lParam;
			System.Position.x = rect->left;
			System.Position.y = rect->top;
			System.Size.x = rect->right - rect->left;
			System.Size.y = rect->bottom - rect->top;

			return TRUE; 
		}
	
	case WM_SIZE:
		{
		//Call System Functions
			System.UpdateWindowPosition();
			System.ResizeWindowMain();  

		//Update View Variables
			for( UINT n = 0; n < System.ViewCount; n++ ) { System.View[n].Aspect = (float)( System.Dimension.x ) / (float)( System.Dimension.y ); }

		//Resize Other Windows/Dialogs 

			MainToolBar.SetBarPosition( 0, 0, System.Size.x - System.MarginSize, System.MainToolBarHeight );

			MainBar.SetBarPosition( System.Size.x - System.MainBarWidth - System.MarginSize + 1,
									System.MainToolBarHeight + 1,
									System.MainBarWidth - 2,
									System.Dimension.y - 2 + System.TrackBarHeight  ); 
			
			SetWindowPos( System.hTrackBar, NULL,	0,
													System.Dimension.y + System.MainToolBarHeight,
													System.Size.x - System.MainBarWidth - System.MarginSize,
													System.TrackBarHeight, SWP_SHOWWINDOW );

			BottomToolBar.SetBarPosition(	0,
											System.Dimension.y + System.MainToolBarHeight + System.TrackBarHeight,
											System.Size.x - System.MarginSize,
											System.BottomToolBarHeight );

		//Update Main Bar
			if( MainBar.GetDialogCount() ) { MainBar.SelectDialog( MainBar.GetCurrentDialog() ); }

			return FALSE;
		}

	case WM_ERASEBKGND: { System.Update = TRUE; System.View[0].NeedUpdate = TRUE; System.View[1].NeedUpdate = TRUE; System.View[2].NeedUpdate = TRUE; System.View[3].NeedUpdate = TRUE;  return FALSE; }

	case WM_MOVE:
		{
			System.UpdateWindowPosition();
			System.ResizeWindowMain();  
			return FALSE;
		}

	case WM_SYSCOMMAND:
		{
			switch( wParam )
			{
			case SC_RESTORE: { System.WindowState = SWS_NORMAL; System.MarginSize = 8; break; }	
			case SC_MAXIMIZE: { System.WindowState = SWS_MAXIMIZED; System.MarginSize = 4; break; }
			}
			break;
		}

	case WM_NOTIFY: 
		{
			switch ( ( (LPNMHDR) lParam )->code )
			{ 
				case TTN_NEEDTEXT: 
					{ 
						LPTOOLTIPTEXT ToolTipText; 
 						ToolTipText = (LPTOOLTIPTEXT) lParam; 
						ToolTipText->hinst = System.hInstance; 
 
					//Specify the resource identifier of the descriptive text for the given button. 
						switch ( ToolTipText->hdr.idFrom )
						{  
						case IDM_FILE_NEW: { ToolTipText->lpszText = MAKEINTRESOURCE( IDS_FILE_NEW ); return TRUE; }
						case IDM_FILE_LOAD: { ToolTipText->lpszText = MAKEINTRESOURCE( IDS_FILE_LOAD ); return TRUE; }
						case IDM_FILE_SAVE: { ToolTipText->lpszText = MAKEINTRESOURCE( IDS_FILE_SAVE ); return TRUE; }
						case IDM_FILE_SAVE_NEW: { ToolTipText->lpszText = MAKEINTRESOURCE( IDS_FILE_SAVE_NEW ); return TRUE; }
						case IDM_ABOUT: { ToolTipText->lpszText = MAKEINTRESOURCE( IDS_ABOUT ); return TRUE; }
						} 
						break; 
					} 
			//Process other notifications here. 
				default: { break; }
			} 
			return FALSE;
		}

	case WM_CLOSE:
		{
			Options.Save( System.hInstance );

			FileHandler.OnFileClose( TRUE );

			System.DestroyWindowMain();

//			State.Reset();

			PostQuitMessage(0); 
	
			return FALSE; 
		}

	case WM_COMMAND:
		{
			switch( LOWORD(wParam) )
			{

			case IDM_EXIT:  { PostMessage( System.hWnd, WM_CLOSE, NULL, NULL ); return FALSE; }

			case IDM_ABOUT:
				{ 
					DialogBox( System.hInstance, MAKEINTRESOURCE( IDD_ABOUT ), System.hWnd, *AboutProc ); 
					break; 
				}	
		//Options	
			case IDM_OPTIONS_VIEWS:
				{ 
					DialogBox( System.hInstance, MAKEINTRESOURCE( IDD_OPTIONS_VIEWS ), System.hWnd, *ViewOptionsProc ); 
					break; 
				}	
		//File
			case IDM_FILE_LOAD:		{ FileHandler.OnFileLoad();	return FALSE; }
			case IDM_FILE_NEW:		{ FileHandler.OnFileNew( FALSE ); return FALSE; }	
			case IDM_FILE_SAVE:		{ FileHandler.OnFileSave( FALSE ); return FALSE; }
			case IDM_FILE_SAVE_NEW: { FileHandler.OnFileSave( TRUE ); return FALSE; }
			}
			return FALSE; 
		}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

LRESULT CALLBACK WndProcGL( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//Parse the Message
	switch( uMsg )
	{
	case WM_CHAR: { KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 

//	case WM_PAINT:		 { UpdateView( 0 ); return TRUE; } 
//	case WM_ERASEBKGND:	 { System.Update = TRUE; return FALSE; } 
	case WM_MOVE:		 { System.Update = TRUE; return FALSE; }
	case WM_SIZE:		 { return FALSE; }
//	case WM_KEYDOWN:	 { Game.pUser->SetKey( wParam, TRUE );	return FALSE; }
//	case WM_KEYUP:		 { Game.pUser->SetKey( wParam, FALSE );	return FALSE; }
	case WM_LBUTTONDOWN: { MouseHandler.OnLButtonDown( hWnd, wParam, lParam ); return FALSE; }
	case WM_LBUTTONUP:   { MouseHandler.OnLButtonUp( hWnd, wParam, lParam );	return FALSE; }
	case WM_RBUTTONDOWN: { MouseHandler.OnRButtonDown( hWnd, wParam, lParam ); return FALSE; }
	case WM_RBUTTONUP:   { MouseHandler.OnRButtonUp( hWnd, wParam, lParam );	return FALSE; }
	case WM_MBUTTONDOWN: { MouseHandler.OnMButtonDown( hWnd, wParam, lParam ); return FALSE; }
	case WM_MBUTTONUP:   { MouseHandler.OnMButtonUp( hWnd, wParam, lParam );	return FALSE; }
	case WM_MOUSEMOVE: 	 { System.CancelHighLight(); MouseHandler.OnMouseMove( hWnd, wParam, lParam );	return FALSE; }
	
/*	case WM_MOUSEWHEEL:  
		{
			float	Zoom = 10;

			if( (short) HIWORD(wParam) < 0 )
			{
				Zoom = -10;
			}
			else
			{
				Zoom = 10;
			}

			if( State.CurrentView.Type == VT_ORTHO )
			{
				System.View[System.ViewActive].Size = State.CurrentView.Size + Zoom;
		
				if( System.View[System.ViewActive].Size < 1 ) { System.View[System.ViewActive].Size = 1; } 
			}
			else
			{
				System.View[System.ViewActive].RePosition( State.CurrentView.Position[0], State.CurrentView.Position[1], State.CurrentView.Position[2], FALSE );
				
				System.View[System.ViewActive].Move( 0, 0, - Zoom ); 
			}

			System.Update = TRUE;
			System.View[System.ViewActive].NeedUpdate = TRUE;

			return FALSE; 
		}*/
		
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

BOOL CALLBACK SelectProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//Parse the Message
	switch( uMsg )
	{
	
	case WM_SHOWWINDOW:
		{
			if( (bool)!wParam ) { return FALSE; }

			UINT n, m, Result, ItemCount = 0;

		//Clear List
			SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_RESETCONTENT, 0, 0 );

		//Prepare Storage
			if( IsDlgButtonChecked( hWnd, IDC_LIST_GEOMETRY ) == BST_CHECKED ) { ItemCount += pScene->ModelCount; }

			SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_INITSTORAGE, pScene->ModelCount, ItemCount * sizeof( char ) * 32 );


			if( IsDlgButtonChecked( hWnd, IDC_SORT_ABC ) )
			{
			//Fill List
				if( IsDlgButtonChecked( hWnd, IDC_LIST_GEOMETRY ) == BST_CHECKED )
				{
					for( n = 0; n < pScene->ModelCount; n++ )
					{
						Result = SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_ADDSTRING, 0, (LPARAM)pScene->pModel[n].Name );
						
						if( Result != LB_ERR ) { SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_SETITEMDATA, Result, MAKELONG( n, MODEL ) ); }
					}
				}
			}

			if( IsDlgButtonChecked( hWnd, IDC_SORT_COLOR ) )
			{
				UINT *pData = (UINT*)malloc( sizeof( UINT ) * ItemCount );

			//Fill List
				if( IsDlgButtonChecked( hWnd, IDC_LIST_GEOMETRY ) == BST_CHECKED )
				{
					for( n = 0; n < pScene->ModelCount; n++ )
					{
						Result = SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_INSERTSTRING, -1, (LPARAM)pScene->pModel[n].Name );
						
						if( Result != LB_ERR )
						{ 
							SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_SETITEMDATA, Result, MAKELONG( n, MODEL ) ); 
						
							pData[n] = pScene->pModel[n].Color;
						}
					}
				}

			//Sort List
				bool Search = TRUE;
				UINT Minimum = 0;

				while( Search )
				{
					Search = FALSE;
				

				
				}
			}

		//Set Selection
			for( n = 0; n < System.Selection.ActiveInfos; n++ )
			{
				for( m = 0; m < (UINT)SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETCOUNT, 0, 0 ); m++ )
				{
					Result = SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETITEMDATA, m, 0 );

					if( ( LOWORD( Result ) == System.Selection.pInfo[n].ObjectIndex ) && ( HIWORD( Result ) == System.Selection.pInfo[n].ObjectType ) )
					{
						SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_SETSEL, TRUE, m );
					}
				}
			}

			SetFocus( GetDlgItem( hWnd, IDC_SELECTION_SEARCH ) );
			break;
		}

	case WM_INITDIALOG:
		{ 
			RECT rt, DlgRect;
			GetWindowRect( System.hWnd, &rt );
			GetWindowRect( hWnd, &DlgRect );
		
			SetWindowPos( hWnd, NULL,  rt.left + ( ( rt.right -  rt.left ) / 2 ) - ( DlgRect.right - DlgRect.left ) / 2 , rt.top + ( ( rt.bottom -  rt.top ) / 2 ) - ( DlgRect.bottom - DlgRect.top ) / 2, 0, 0, SWP_NOSIZE );   
			System.Update = TRUE; 

			CheckDlgButton( hWnd, Options.SelectDialog.SortType, BST_CHECKED );

			CheckDlgButton( hWnd, IDC_DISPLAY_SUBTREE, Options.SelectDialog.DisplaySubTree );
			CheckDlgButton( hWnd, IDC_CASE_SENSITIVE, Options.SelectDialog.CaseSensitive );

			CheckDlgButton( hWnd, IDC_LIST_GEOMETRY, Options.SelectDialog.ListGeometry );
			CheckDlgButton( hWnd, IDC_LIST_SHAPES, Options.SelectDialog.ListShapes );
			CheckDlgButton( hWnd, IDC_LIST_LIGHTS, Options.SelectDialog.ListLights );
			CheckDlgButton( hWnd, IDC_LIST_CAMERAS, Options.SelectDialog.ListCameras );
			CheckDlgButton( hWnd, IDC_LIST_HELPERS, Options.SelectDialog.ListHelpers );
			CheckDlgButton( hWnd, IDC_LIST_WARPS, Options.SelectDialog.ListWarps );
			CheckDlgButton( hWnd, IDC_LIST_GROUPS, Options.SelectDialog.ListGroups );
			CheckDlgButton( hWnd, IDC_LIST_BONES, Options.SelectDialog.ListBones );

			break; 
		}

	case WM_COMMAND:
		{
			switch( HIWORD( wParam ) )
			{
			case EN_CHANGE:
				{
					switch( LOWORD( wParam ) )
					{
					case IDC_SELECTION_SEARCH:
						{
							UINT n, Result;

						//Clear Selection
							for( n = 0; n < (UINT)SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETCOUNT, 0, 0 ); n++ )
							{
								SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_SETSEL, FALSE, n );
							}

						//Get Search String
							GetWindowText( (HWND)lParam, State.TempBuffer32a, 32 );
						

							if( IsDlgButtonChecked( hWnd, IDC_CASE_SENSITIVE ) == BST_CHECKED )
							{
								for( n = 0; n < (UINT)SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETCOUNT, 0, 0 ); n++ )
								{
									Result = SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETTEXT, n, (LPARAM)State.TempBuffer64a );

									if( ( Result != LB_ERR ) && ( strncmp( State.TempBuffer64a, State.TempBuffer32a, strlen( State.TempBuffer32a ) ) == 0 ) )
									{
										SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_SETSEL, TRUE, n );
									}
								}
							}
							else
							{
								for( n = 0; n < (UINT)SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETCOUNT, 0, 0 ); n++ )
								{
									Result = SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETTEXT, n, (LPARAM)State.TempBuffer64a );

									if( ( Result != LB_ERR ) && ( strnicmp( State.TempBuffer64a, State.TempBuffer32a, strlen( State.TempBuffer32a ) ) == 0 ) )
									{
										SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_SETSEL, TRUE, n );
									}
								}
							
							}
								
							break;
						}
					}

					break;
				}

			case BN_CLICKED:
				{
					switch( LOWORD( wParam ) )
					{
					case IDC_LIST_GEOMETRY: { SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break;}
					case IDC_LIST_SHAPES:	{ SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break;}
					case IDC_LIST_LIGHTS:	{ SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break;}
					case IDC_LIST_CAMERAS:	{ SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break;}
					case IDC_LIST_HELPERS:	{ SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break;}
					case IDC_LIST_WARPS:	{ SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break;}
					case IDC_LIST_GROUPS:	{ SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break;}
					case IDC_LIST_BONES:	{ SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break;}

					case IDC_SORT_ABC:		{ SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break; }
					case IDC_SORT_TYPE:		{ SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break; }
					case IDC_SORT_COLOR:	{ SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break; }
					case IDC_SORT_SIZE:		{ SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 ); break; }
					}
		
					break;
				}
			}

			break;
		}

	case BN_RELEASED:
		{
			switch( wParam )
			{
			case IDC_SELECT_ALL:
				{
					for( UINT n = 0; n < (UINT)SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETCOUNT, 0, 0 ); n++ )
					{
						SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_SETSEL, TRUE, n );
					}
				
					break;
				}

			case IDC_SELECT_NONE:
				{
					for( UINT n = 0; n < (UINT)SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETCOUNT, 0, 0 ); n++ )
					{
						SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_SETSEL, FALSE, n );
					}
				
					break;
				}

			case IDC_SELECT_INVERT:
				{
					for( UINT n = 0; n < (UINT)SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETCOUNT, 0, 0 ); n++ )
					{
						if( SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETSEL, n, 0 ) ){ SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_SETSEL, FALSE, n ); }
						else { SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_SETSEL, TRUE, n ); }
					}
				
					break;
				}

			case IDC_LIST_INVERT:
				{
					if( IsDlgButtonChecked( hWnd, IDC_LIST_GEOMETRY ) == BST_CHECKED ) { CheckDlgButton( hWnd, IDC_LIST_GEOMETRY, BST_UNCHECKED ); }
					else { CheckDlgButton( hWnd, IDC_LIST_GEOMETRY, BST_CHECKED ); }
				
					if( IsDlgButtonChecked( hWnd, IDC_LIST_SHAPES ) == BST_CHECKED ) { CheckDlgButton( hWnd, IDC_LIST_SHAPES, BST_UNCHECKED ); }
					else { CheckDlgButton( hWnd, IDC_LIST_SHAPES, BST_CHECKED ); }
				
					if( IsDlgButtonChecked( hWnd, IDC_LIST_LIGHTS ) == BST_CHECKED ) { CheckDlgButton( hWnd, IDC_LIST_LIGHTS, BST_UNCHECKED ); }
					else { CheckDlgButton( hWnd, IDC_LIST_LIGHTS, BST_CHECKED ); }
				
					if( IsDlgButtonChecked( hWnd, IDC_LIST_CAMERAS ) == BST_CHECKED ) { CheckDlgButton( hWnd, IDC_LIST_CAMERAS, BST_UNCHECKED ); }
					else { CheckDlgButton( hWnd, IDC_LIST_CAMERAS, BST_CHECKED ); }
					
					if( IsDlgButtonChecked( hWnd, IDC_LIST_HELPERS ) == BST_CHECKED ) { CheckDlgButton( hWnd, IDC_LIST_HELPERS, BST_UNCHECKED ); }
					else { CheckDlgButton( hWnd, IDC_LIST_HELPERS, BST_CHECKED ); }
					
					if( IsDlgButtonChecked( hWnd, IDC_LIST_WARPS ) == BST_CHECKED ) { CheckDlgButton( hWnd, IDC_LIST_WARPS, BST_UNCHECKED ); }
					else { CheckDlgButton( hWnd, IDC_LIST_WARPS, BST_CHECKED ); }
					
					if( IsDlgButtonChecked( hWnd, IDC_LIST_GROUPS ) == BST_CHECKED ) { CheckDlgButton( hWnd, IDC_LIST_GROUPS, BST_UNCHECKED ); }
					else { CheckDlgButton( hWnd, IDC_LIST_GROUPS, BST_CHECKED ); }
					
					if( IsDlgButtonChecked( hWnd, IDC_LIST_BONES ) == BST_CHECKED ) { CheckDlgButton( hWnd, IDC_LIST_BONES, BST_UNCHECKED ); }
					else { CheckDlgButton( hWnd, IDC_LIST_BONES, BST_CHECKED ); }
	
					SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 );
					break;
				}

			case IDC_LIST_ALL:
				{
					CheckDlgButton( hWnd, IDC_LIST_GEOMETRY, BST_CHECKED );
					CheckDlgButton( hWnd, IDC_LIST_SHAPES, BST_CHECKED );
					CheckDlgButton( hWnd, IDC_LIST_LIGHTS, BST_CHECKED );
					CheckDlgButton( hWnd, IDC_LIST_CAMERAS, BST_CHECKED );
					CheckDlgButton( hWnd, IDC_LIST_HELPERS, BST_CHECKED );
					CheckDlgButton( hWnd, IDC_LIST_WARPS, BST_CHECKED );
					CheckDlgButton( hWnd, IDC_LIST_GROUPS, BST_CHECKED );
					CheckDlgButton( hWnd, IDC_LIST_BONES, BST_CHECKED );
	
					SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 );
					break;
				}

			case IDC_LIST_NONE:
				{
					CheckDlgButton( hWnd, IDC_LIST_GEOMETRY, BST_UNCHECKED );
					CheckDlgButton( hWnd, IDC_LIST_SHAPES, BST_UNCHECKED );
					CheckDlgButton( hWnd, IDC_LIST_LIGHTS, BST_UNCHECKED );
					CheckDlgButton( hWnd, IDC_LIST_CAMERAS, BST_UNCHECKED );
					CheckDlgButton( hWnd, IDC_LIST_HELPERS, BST_UNCHECKED );
					CheckDlgButton( hWnd, IDC_LIST_WARPS, BST_UNCHECKED );
					CheckDlgButton( hWnd, IDC_LIST_GROUPS, BST_UNCHECKED );
					CheckDlgButton( hWnd, IDC_LIST_BONES, BST_UNCHECKED );
	
					SendMessage( hWnd, WM_SHOWWINDOW, TRUE, 0 );
					break;
				}

			case IDOK:
				{
					if( IsDlgButtonChecked( hWnd, IDC_SORT_ABC ) == BST_CHECKED )	{ Options.SelectDialog.SortType = IDC_SORT_ABC; }
					if( IsDlgButtonChecked( hWnd, IDC_SORT_TYPE ) == BST_CHECKED )	{ Options.SelectDialog.SortType = IDC_SORT_TYPE; }
					if( IsDlgButtonChecked( hWnd, IDC_SORT_COLOR ) == BST_CHECKED ) { Options.SelectDialog.SortType = IDC_SORT_COLOR; }
					if( IsDlgButtonChecked( hWnd, IDC_SORT_SIZE ) == BST_CHECKED )	{ Options.SelectDialog.SortType = IDC_SORT_SIZE; }

					Options.SelectDialog.DisplaySubTree = IsDlgButtonChecked( hWnd, IDC_DISPLAY_SUBTREE );
					Options.SelectDialog.CaseSensitive = IsDlgButtonChecked( hWnd, IDC_CASE_SENSITIVE );

					Options.SelectDialog.ListGeometry = IsDlgButtonChecked( hWnd, IDC_LIST_GEOMETRY );
					Options.SelectDialog.ListShapes = IsDlgButtonChecked( hWnd, IDC_LIST_SHAPES );
					Options.SelectDialog.ListLights = IsDlgButtonChecked( hWnd, IDC_LIST_LIGHTS );
					Options.SelectDialog.ListCameras = IsDlgButtonChecked( hWnd, IDC_LIST_CAMERAS );
					Options.SelectDialog.ListHelpers = IsDlgButtonChecked( hWnd, IDC_LIST_HELPERS );
					Options.SelectDialog.ListWarps = IsDlgButtonChecked( hWnd, IDC_LIST_WARPS );
					Options.SelectDialog.ListGroups = IsDlgButtonChecked( hWnd, IDC_LIST_GROUPS );
					Options.SelectDialog.ListBones = IsDlgButtonChecked( hWnd, IDC_LIST_BONES );
					
					UINT n, Result;

					System.Selection.ClearEntries();

					for( n = 0; n < (UINT)SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETCOUNT, 0, 0 ); n++ )
					{
						Result = SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETITEMDATA, n, 0 );

						if( SendDlgItemMessage( hWnd, IDC_SELECTION_LIST, LB_GETSEL, n, 0 ) )
						{
							if( HIWORD( Result ) == MODEL )
							{
								System.Selection.AddEntry( MODEL, LOWORD( Result ), &pScene->pModel[LOWORD( Result )], TRUE );
							}
						}
					}

					EndDialog( hWnd, TRUE );
					System.Selection.UpdateCenter();
					System.Update = TRUE;
					System.UpdateAllViews( FALSE );
					break;
				}

			case IDCANCEL:
				{
					SendMessage( hWnd, WM_SYSCOMMAND, SC_CLOSE, 0 );
					break;
				}
			}
			break;
		}

	case WM_SYSCOMMAND:
		{
			switch( wParam )
			{
			case SC_CLOSE:
				{
					EndDialog( hWnd, TRUE );
					System.Update = TRUE;
					return FALSE; 
				}	
			}
			break;
		}
	}
	return FALSE;
}



BOOL CALLBACK AboutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//Parse the Message
	switch( uMsg )
	{
	case WM_INITDIALOG:
		{ 
			LoadString( System.hInstance, IDS_APP_VERSION, State.TempBuffer16a, 16 );

			strcpy( State.TempBuffer32a, "Version " );
			strcat( State.TempBuffer32a, State.TempBuffer16a );

			SetWindowText( hWnd, State.TempBuffer32a );

			RECT rt, DlgRect;
			GetWindowRect( System.hWnd, &rt );
			GetWindowRect( hWnd, &DlgRect );
		
			SetWindowPos( hWnd, NULL,  rt.left + ( ( rt.right -  rt.left ) / 2 ) - ( DlgRect.right - DlgRect.left ) / 2 , rt.top + ( ( rt.bottom -  rt.top ) / 2 ) - ( DlgRect.bottom - DlgRect.top ) / 2, 0, 0, SWP_NOSIZE );   
			System.Update = TRUE; 
			break; 
		}

	case BN_RELEASED:
		{
			switch( wParam )
			{
			case IDOK:
				{
					EndDialog( hWnd, TRUE );
					System.Update = TRUE;
					break;
				}
			}
			break;
		}

	case WM_SYSCOMMAND:
		{
			switch( wParam )
			{
			case SC_CLOSE:
				{
					EndDialog( hWnd, TRUE );
					System.Update = TRUE;
					return FALSE; 
				}	
			}
			break;
		}
	}
	return FALSE;
}



BOOL CALLBACK MainDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

//Parse the Message
	switch( uMsg )
	{
	case WM_KEYDOWN: { KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_INITDIALOG:
		{
			SetWindowLong( hWnd, GWL_USERDATA, (LONG) lParam );
			break;
		}

	case ACGN_CLOSED:
		{
			MainBar.ReArrangeGroups();
			MainBar.SelectDialog( MainBar.GetCurrentDialog() );
			break;
		}
	
	case ACGN_OPENED:
		{
			MainBar.ReArrangeGroups();
			MainBar.SelectDialog( MainBar.GetCurrentDialog() );
			break;
		}
	}
	return FALSE;
}


BOOL CALLBACK MainTabProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//Parse the Message
	switch( uMsg )
	{
	case WM_KEYDOWN: { KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_PAINT:
		{
			HDC hdc = BeginPaint( hWnd, &System.ps ); 
			HPEN pen = CreatePen( PS_SOLID, 1, GetSysColor( COLOR_BTNHIGHLIGHT ) );

			SelectObject( hdc, pen );
			MoveToEx( hdc,	MainBar.DialogRect.left + MainBar.DialogRect.right,	MainBar.TabBarHeight -1 , NULL );   
			LineTo( hdc,	MainBar.DialogRect.left - 1,						MainBar.TabBarHeight -1 );  

			EndPaint( hWnd, &System.ps ); 
			DeleteObject( pen );
		
			return FALSE;
		}

	case WM_INITDIALOG:
		{
			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(HWND)NULL ); 
			SendMessage( GetDlgItem( hWnd, IDC_TAB_CREATE ), BM_PUSH, 0, 0 );
			break;
		}

	case BS_PUSHED:
		{
			switch( wParam )
			{
			case IDC_TAB_CREATE:
				{
					MainBar.SelectDialog( 0 ); 
					break; 
				}
			case IDC_TAB_MODIFY:
				{ 
					MainBar.SelectDialog( 1 ); 
					break; 
				}
			case IDC_TAB_MOTION:	
				{ 
					MainBar.SelectDialog( 2 ); 
					break; 
				}

			case IDC_TAB_DISPLAY:	
				{ 
					MainBar.SelectDialog( 3 ); 
					break; 
				}

			case IDC_TAB_UTILITY:	
				{ 
					MainBar.SelectDialog( 4 ); 
					break; 
				}
			}
			break;
		}
	}
	return FALSE;
}


BOOL CALLBACK ObjectTransformsProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN: { KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_MOUSEMOVE:		{ SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ); SetCursor( System.hCursorHand ); break; }
	case WM_LBUTTONDOWN:	{ SendMessage( MainBar.hWnd, DBM_DRAG_START, (WPARAM)hWnd, lParam ); break; }
	case BN_HIGHLIGHTED:	{ System.HighLightButton( (HWND)lParam ); break; }

	case WM_INITDIALOG:
		{
			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_POS_X, "0" );
			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_POS_Y, "0" );
			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_POS_Z, "0" );

			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_ROT_X, "0" );
			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_ROT_Y, "0" );
			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_ROT_Z, "0" );

			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_SCALE_X, "100" );
			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_SCALE_Y, "100" );
			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_SCALE_Z, "100" );

			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_ORIGIN_X, "0" );
			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_ORIGIN_Y, "0" );
			SetDlgItemText( hWnd, IDC_OBJECT_TRANSFORMS_ORIGIN_Z, "0" );
			
			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_POS_X_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_POS_X ), 0 );
			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_POS_Y_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_POS_Y ), 0 );
			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_POS_Z_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_POS_Z ), 0 );

			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_ROT_X_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_ROT_X ), 0 );
			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_ROT_Y_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_ROT_Y ), 0 );
			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_ROT_Z_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_ROT_Z ), 0 );
			
			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_SCALE_X_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_SCALE_X ), 0 );
			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_SCALE_Y_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_SCALE_Y ), 0 );
			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_SCALE_Z_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_SCALE_Z ), 0 );

			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_ORIGIN_X_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_ORIGIN_X ), 0 );
			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_ORIGIN_Y_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_ORIGIN_Y ), 0 );
			SendDlgItemMessage( hWnd, IDC_OBJECT_TRANSFORMS_ORIGIN_Z_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_OBJECT_TRANSFORMS_ORIGIN_Z ), 0 );
			break;
		}

	case WM_COMMAND:
		{
			switch( HIWORD( wParam ) )
			{
				
			case BN_DBLCLK:
				{
					switch( LOWORD( wParam ) )
					{
					case IDC_GROUPBUTTON:
						{
							if( SendMessage( GetParent( hWnd ), ACGM_GET_STATE, 0, 0 ) == ACGS_NORMAL ) { SendMessage( GetParent( hWnd ), ACGM_CLOSE, 0, 0 ); }
							else { SendMessage( GetParent( hWnd ), ACGM_OPEN, 0, 0 ); }
							break; 
						}
					}
				}//case BN_DBLCLK:
			}

			PostMessage( (HWND)GetWindowLong( GetParent( hWnd ), CtrlGroupParent ), uMsg, wParam, lParam );
			return TRUE;
		}

	case WM_PAINT: { CustomUI.DrawAGroupControlUI( hWnd ); return FALSE; }
	}
	return FALSE;
}

BOOL CALLBACK ObjectColorProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	tObjectColorDlgData	*pData = NULL;

	switch( uMsg )
	{
	case WM_INITDIALOG:
		{
			RECT rt, DlgRect;
			GetWindowRect( System.hWnd, &rt );
			GetWindowRect( hWnd, &DlgRect );
		
			SetWindowPos( hWnd, NULL,  rt.left + ( ( rt.right -  rt.left ) / 2 ) - ( DlgRect.right - DlgRect.left ) / 2 , rt.top + ( ( rt.bottom -  rt.top ) / 2 ) - ( DlgRect.bottom - DlgRect.top ) / 2, 0, 0, SWP_NOSIZE );   
			System.Update = TRUE; 

			pData = (tObjectColorDlgData*)malloc( sizeof( tObjectColorDlgData ) );
			pData->CurrentColor = (COLORREF)lParam;
			pData->OriginalColor = (COLORREF)lParam;
			pData->CurrentColorType = CT_ORIGINAL;

			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(tObjectColorDlgData*)pData );

			pData->ImageIndex = CustomUI.AddImages16( "Data//System//SelectByColor.bmp", "Data//System//SelectByColor_a.bmp" );

			CustomUI.SetButtonImage16( hWnd, IDC_SELECT_BY_COLOR, pData->ImageIndex );
			break;
		}

	case WM_SYSCOMMAND:
		{
			if( wParam == SC_CLOSE )
			{
				pData = (tObjectColorDlgData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

				CustomUI.RemoveImage16( pData->ImageIndex );

				EndDialog( hWnd, pData->OriginalColor );
				return TRUE; 
			}
			
			break;
		}

	case WM_CLOSE:
		{
			pData = (tObjectColorDlgData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

			CustomUI.RemoveImage16( pData->ImageIndex );

			EndDialog( hWnd, pData->CurrentColor );

			if( pData ) { free( pData ); }
			return TRUE; 
		}

	case BN_RELEASED:
		{
			switch( wParam )
			{
			case IDOK: { PostMessage( hWnd, WM_CLOSE, 0, 0 ); break; }
			}
			break;
		}

	case WM_LBUTTONDOWN:
		{
			pData = (tObjectColorDlgData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

			for( int y = 0; y < System.BasePaletteSize.y; y ++ )
			{
				for( int x = 0; x < System.BasePaletteSize.x; x ++ )
				{
					if( LOWORD( lParam ) > x * 25 + 7  &&  LOWORD( lParam ) < x * 25 + 23 + 7 &&
						HIWORD( lParam ) > y * 25 + 26 &&  HIWORD( lParam ) < y * 25 + 23 + 26 )
					{
						pData->CurrentColor = System.BasePalette[x][y];
						pData->CurrentColorType = CT_BASE;
						pData->CurrentColorPos.x = x;
						pData->CurrentColorPos.y = y;
					}
				}
			}
	
			for( int x = 0; x < (int)System.UserPaletteSize; x ++ )
			{
				if( LOWORD( lParam ) > x * 25 + 7  &&  LOWORD( lParam ) < x * 25 + 23 + 7 &&
					HIWORD( lParam ) > 157 &&  HIWORD( lParam ) < 157 + 22 )
				{
					pData->CurrentColor = System.UserPalette[x];
					pData->CurrentColorType = CT_USER;
					pData->CurrentColorPos.x = x;
				}
			}

			if( LOWORD( lParam ) > 263 &&  LOWORD( lParam ) < 263 + 23 &&
				HIWORD( lParam ) > 157 &&  HIWORD( lParam ) < 157 + 23 )
			{
				pData->CurrentColor = pData->OriginalColor;
				pData->CurrentColorType = CT_ORIGINAL;
			}

			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(tObjectColorDlgData*)pData );
			RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE );
			break;
		}

	case WM_PAINT:
		{
			pData = (tObjectColorDlgData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

			HDC hdc = GetDC( hWnd ); 

		//Draw The "Base Palette:"
			for( int y = 0; y < System.BasePaletteSize.y; y ++ )
			{
				for( int x = 0; x < System.BasePaletteSize.x; x ++ )
				{
					if( pData->CurrentColorType == CT_BASE && x == pData->CurrentColorPos.x
						&& y == pData->CurrentColorPos.y )
					{
						CustomUI.DrawColorSwatch( &hdc, x * 25 + 7, y * 25 + 26, 23, 23, System.BasePalette[x][y], TRUE );
					}
					else { CustomUI.DrawColorSwatch( &hdc, x * 25 + 7, y * 25 + 26, 23, 23, System.BasePalette[x][y], FALSE ); }
				}
			}

		//Draw The "User Palette:"
			for( int x = 0; x < (int)System.UserPaletteSize; x ++ )
			{
				if( pData->CurrentColorType == CT_USER && x == pData->CurrentColorPos.x )
				{
					CustomUI.DrawColorSwatch( &hdc, x * 25 + 7, 157, 23, 23, System.UserPalette[x], TRUE );
				}
				else { CustomUI.DrawColorSwatch( &hdc, x * 25 + 7, 157, 23, 23, System.UserPalette[x], FALSE ); }
			}

		//Draw The "Original:" Color Swatch

			if( pData->CurrentColorType == CT_ORIGINAL )
			{
				CustomUI.DrawColorSwatch( &hdc, 263, 157, 23, 23, pData->OriginalColor, TRUE );
			}
			else { CustomUI.DrawColorSwatch( &hdc, 263, 157, 23, 23, pData->OriginalColor, FALSE ); }

		//Draw The "Current:" Color Swatch
			CustomUI.DrawColorSwatch( &hdc, 308, 145, 35, 35, pData->CurrentColor, FALSE );

			ReleaseDC( hWnd, hdc );
			return FALSE; 
		}

/*	case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT draw = (LPDRAWITEMSTRUCT)lParam;
			
			if( draw->CtlID == IDC_OBJECT_INFO_COLOR )
			{
				
				HBRUSH brush = CreateSolidBrush( RGB( 255, 0, 0 ) );
				SelectObject( draw->hDC, brush );

				Rectangle( draw->hDC , 0, 0, draw->rcItem.right, draw->rcItem.bottom );
				
				return FALSE;
			}
			else { CustomUI.DrawAGroupControlButton( (LPDRAWITEMSTRUCT)lParam, hWnd ); }

			break; 
		}*/
	}
	return FALSE;
}

BOOL CALLBACK ObjectInfoProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN: { KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_MOUSEMOVE:		{ SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ); SetCursor( System.hCursorHand ); break; }
	case WM_LBUTTONDOWN:	{ SendMessage( MainBar.hWnd, DBM_DRAG_START, (WPARAM)hWnd, lParam ); break; }
	case BN_HIGHLIGHTED:	{ System.HighLightButton( (HWND)lParam ); break; }

	case WM_INITDIALOG:
		{
			tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( GetDlgItem( hWnd, IDC_OBJECT_INFO_COLOR ), GWL_USERDATA );
			if( pInfo )
			{
				pInfo->Color = System.BasePalette[(UINT)( System.CurrentColorIndex % 16 )][(UINT)( System.CurrentColorIndex / 16 )]; 
				RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE );
			}

			SetDlgItemText( hWnd, IDC_OBJECT_INFO_NAME, "Untitled" );
			SendMessage( GetDlgItem( hWnd, IDC_OBJECT_INFO_NAME ), EM_LIMITTEXT, 31, 0 );

			break;
		}

	case WM_COMMAND:
		{
			switch( HIWORD( wParam ) )
			{
			case EN_CHANGE:
				{
					switch( LOWORD(wParam) )
					{
					case IDC_OBJECT_INFO_NAME:
						{
							if( GetFocus() == (HWND) lParam )
							{
								if( ( System.Selection.InfoCount == 1 ) && ( System.Selection.pInfo[0].ObjectType == MODEL ) )
								{
									CModel *pModel = (CModel*)System.Selection.pInfo[0].pObject; 

									GetWindowText( (HWND) lParam, pModel->Name, 32 );  
								}
							}
							break;
						}
					}
				} 
				
			case BN_DBLCLK:
				{
					switch( LOWORD( wParam ) )
					{
					case IDC_GROUPBUTTON:
						{
							if( SendMessage( GetParent( hWnd ), ACGM_GET_STATE, 0, 0 ) == ACGS_NORMAL ) { SendMessage( GetParent( hWnd ), ACGM_CLOSE, 0, 0 ); }
							else { SendMessage( GetParent( hWnd ), ACGM_OPEN, 0, 0 ); }
							break; 
						}
					case IDC_OBJECT_INFO_COLOR:
						{
							if( ( System.Selection.InfoCount > 0 ) && ( System.Selection.pInfo[0].ObjectType == MODEL ) )
							{
								CModel *pModel = (CModel*)System.Selection.pInfo[0].pObject; 

								tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( MainBar.GetGroupControl( 0, IDD_OBJECT_INFO, IDC_OBJECT_INFO_COLOR ), GWL_USERDATA );
								if( pInfo )
								{
									pInfo->Color = pModel->Color; 
									RedrawWindow( MainBar.GetGroupControl( 0, IDD_OBJECT_INFO, IDC_OBJECT_INFO_COLOR ), NULL, NULL, RDW_INVALIDATE );
								}

								COLORREF color = DialogBoxParam( System.hInstance, MAKEINTRESOURCE( IDD_OBJECT_COLOR ), hWnd, ObjectColorProc, pModel->Color );  
								
								for( UINT n = 0; n < System.Selection.InfoCount; n++ )
								{
									pModel = (CModel*)System.Selection.pInfo[n].pObject;
									
									pModel->SetColor( color );
								}

								if( pInfo )
								{
									pInfo->Color = pModel->Color; 
									RedrawWindow( MainBar.GetGroupControl( 0, IDD_OBJECT_INFO, IDC_OBJECT_INFO_COLOR ), NULL, NULL, RDW_INVALIDATE );
	
									System.UpdateAllViews( TRUE );
								}
							}
						
							break; 
						}
					}
				}//case BN_DBLCLK:
			}

			PostMessage( (HWND)GetWindowLong( GetParent( hWnd ), CtrlGroupParent ), uMsg, wParam, lParam );
			return TRUE;
		}

	case WM_PAINT: { CustomUI.DrawAGroupControlUI( hWnd ); return FALSE; }

	case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT draw = (LPDRAWITEMSTRUCT)lParam;
			
			if( draw->CtlID == IDC_OBJECT_INFO_COLOR )
			{
				tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( draw->hwndItem, GWL_USERDATA );
				if( pInfo )
				{
					CustomUI.DrawColorSwatch( &draw->hDC, 1, 1, draw->rcItem.right - 2, draw->rcItem.bottom - 2, pInfo->Color, FALSE );
				}

				return FALSE;
			}
			break; 
		}
	}
	return FALSE;
}


BOOL CALLBACK BottomToolBarProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	tButtonInfo *pInfo = NULL;

//Parse the Message
	switch( uMsg )
	{
//Default Message Processing
//	case WM_KEYDOWN: { KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_MOUSEMOVE:		{ SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ); SetCursor( System.hCursorHand ); break; }
	case WM_LBUTTONDOWN:	{ SendMessage( GetParent( hWnd ), TBM_DRAG_START, (WPARAM)hWnd, lParam ); break; }
	case WM_LBUTTONUP:		{ SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ); break; }
	case BN_HIGHLIGHTED: 	{ System.HighLightButton( (HWND)lParam ); return TRUE; }

	case WM_PAINT:
		{
			CustomUI.hdc = BeginPaint( hWnd, &CustomUI.PaintStruct );

			GetClientRect( hWnd, &CustomUI.Rect1 );

			CustomUI.DrawSeparator( NULL, CustomUI.Rect1.right - 653, 0, CustomUI.Rect1.bottom, FALSE );
			CustomUI.DrawSeparator( NULL, CustomUI.Rect1.right - 344, 0, CustomUI.Rect1.bottom, FALSE );
			CustomUI.DrawSeparator( NULL, CustomUI.Rect1.right - 108, 0, CustomUI.Rect1.bottom, FALSE );

			EndPaint( hWnd, &CustomUI.PaintStruct );
			break;
		}


	case EN_ENTER:
		{
		//Fetch Buddy Value
			GetDlgItemText( BottomToolBar.GetDialog(), IDC_TRANSFORM_X, State.TempBuffer64a, 64 ); 
			if( strlen( State.TempBuffer64a ) ) { State.TempValue1 = (float)atof( State.TempBuffer64a ); }
			State.TempVector3e[0] = State.TempValue1;

			GetDlgItemText( BottomToolBar.GetDialog(), IDC_TRANSFORM_Y, State.TempBuffer64a, 64 ); 
			if( strlen( State.TempBuffer64a ) ) { State.TempValue1 = (float)atof( State.TempBuffer64a ); }
			State.TempVector3e[1] = State.TempValue1;

			GetDlgItemText( BottomToolBar.GetDialog(), IDC_TRANSFORM_Z, State.TempBuffer64a, 64 ); 
			if( strlen( State.TempBuffer64a ) ) { State.TempValue1 = (float)atof( State.TempBuffer64a ); }
			State.TempVector3e[2] = State.TempValue1;

			State.UpdateTransforms( &State.TempVector3e );

			SendDlgItemMessage( BottomToolBar.GetDialog(), IDC_TRANSFORM_X_SPIN, ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[0] );
			SendDlgItemMessage( BottomToolBar.GetDialog(), IDC_TRANSFORM_Y_SPIN, ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[1] );
			SendDlgItemMessage( BottomToolBar.GetDialog(), IDC_TRANSFORM_Z_SPIN, ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[2] );
			SendMessage( (HWND)lParam, EM_SETSEL, 0, -1 );

			System.UpdateAllViews( TRUE );

			break;
		}

	case BN_PUSHED:
		{
			switch( wParam )
			{

			case IDC_SELECTION_LOCK:
				{
					Options.LockSelection = TRUE;
					return FALSE;	
				}

			case IDC_TRANSFORM_LOCAL:
				{ 
					System.Selection.UpdateTypeInDialog( FALSE );

					return FALSE;	
				}

			case IDC_PLAY:
				{ 
					pInfo = (tButtonInfo*)GetWindowLong( (HWND)lParam, GWL_USERDATA );
					if( !pInfo ) { return FALSE; }

					if( pInfo->ActiveSubImage == 0 )
					{ 
						pInfo->ActiveSubImage = 1; 
						break; 
					}

					if( pInfo->ActiveSubImage == 1 )
					{ 
						pInfo->ActiveSubImage = 0;
						break; 
					}

					break; 
				}

			case IDC_PAN:
				{ 
					State.Tool.Type = TOOL_VIEW;
					State.Tool.ID = TOOL_VIEW_PAN;
					State.Tool.hCursor = System.hCursorHand;
					break; 
				}
			case IDC_ROTATE_ORIGIN:
				{ 
					State.Tool.Type = TOOL_VIEW;
					State.Tool.ID = TOOL_VIEW_ROTATE_ORIGIN;
					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_ROTATE ) );
					break; 
				}
			case IDC_ROTATE_FOCUS:
				{ 
					State.Tool.ID = TOOL_VIEW_ROTATE_FOCUS;
					State.Tool.Type = TOOL_VIEW;
					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_ROTATE ) );
					break; 
				}
/*			case IDC_ROTATE_SELECTED:
				{ 
					State.Tool.Type = TOOL_VIEW;
					State.Tool.ID = TOOL_VIEW_ROTATE_SELECTED;
					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_ROTATE ) );
					break; 
				}*/

			case IDC_ZOOM:
				{ 
					State.Tool.Type = TOOL_VIEW;
					State.Tool.ID = TOOL_VIEW_ZOOM;
					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_ZOOM ) );
					break; 
				}

			case IDC_ZOOM_ALL:
				{ 
					State.Tool.Type = TOOL_VIEW;
					State.Tool.ID = TOOL_VIEW_ZOOM_ALL;
					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_ZOOM_ALL ) );
					break; 
				}

			case IDC_FOV:
				{ 
					State.Tool.Type = TOOL_VIEW;
					State.Tool.ID = TOOL_VIEW_FOV;
					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_FOV ) );
					break; 
				}

			case IDC_FOURVIEW:
				{ 
					if( System.FourViews ) { System.EnlargeView( System.ViewActive ); }
					else { System.FourViews = TRUE; System.ResizeWindowMain(); }
					return FALSE; 
				}

			case IDC_CENTER_EVERYTHING:
				{ 
					System.View[System.ViewActive].ZoomExtents( FALSE ); 

					return FALSE; 
				}

			case IDC_CENTER_SELECTED:
				{ 
					System.View[System.ViewActive].ZoomExtents( TRUE ); 
					
					return FALSE; 
				}

			case IDC_CENTER_ALL_EVERYTHING:
				{ 
					for( UINT n = 0; n < System.ViewCount; n++ ) { System.View[n].ZoomExtents( FALSE ); }

					return FALSE; 
				}

			case IDC_CENTER_ALL_SELECTED:
				{ 
					for( UINT n = 0; n < System.ViewCount; n++ ) { System.View[n].ZoomExtents( TRUE ); }
					
					return FALSE; 
				}

			}


			State.ToolLevel = 2;
			State.SetToolInfo( 2, (HWND)lParam );

			State.Tool.Step = 0;
			State.Tool.Active = FALSE;
			State.Tool.Enabled = TRUE;
			State.Tool.hButton = (HWND)lParam;


			System.UpdateAllViews( FALSE );

			break;
		}

	case BN_RELEASED:
		{
			switch( wParam )
			{

			case IDC_SELECTION_LOCK:
				{
					Options.LockSelection = FALSE;
					return FALSE;	
				}

			case IDC_TRANSFORM_LOCAL:
				{ 
/*					if( ( State.Tool.Type != TOOL_SELECT ) && ( State.Tool.Type != TOOL_EDIT ) ) { return FALSE; }


					if( State.Tool.ID == TOOL_EDIT_MOVE )
					{
						Math.CopyVector3( &State.TransformInfo, &System.Selection.GizmoPosition );
					}

					if( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) )
					{
						if( System.Selection.ActiveInfos == 1 )
						{
							Math.CopyVector3( &State.TransformInfo, &System.Selection.pInfo[0].Scale );
							Math.ScaleVector3( &State.TransformInfo, 100, &State.TransformInfo );
						}
						else
						{ 
							State.TransformInfo[0] = 100.0f;
							State.TransformInfo[1] = 100.0f;
							State.TransformInfo[2] = 100.0f;
						}
					}

					SendDlgItemMessage( BottomToolBar.GetDialog(), IDC_TRANSFORM_X_SPIN, ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[0] );
					SendDlgItemMessage( BottomToolBar.GetDialog(), IDC_TRANSFORM_Y_SPIN, ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[1] );
					SendDlgItemMessage( BottomToolBar.GetDialog(), IDC_TRANSFORM_Z_SPIN, ASM_SET_VAL_FLOAT, 0, (LPARAM)&State.TransformInfo[2] );
*/

					System.Selection.UpdateTypeInDialog( FALSE );

					return FALSE;	
				}
			}

			break;
		}
	}

	return FALSE;
}


BOOL CALLBACK MainToolBarProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//Parse the Message
	switch( uMsg )
	{
//Default Message Processing
	case WM_KEYDOWN:		{ KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_MOUSEMOVE:		{ SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ); SetCursor( System.hCursorHand ); break; }
	case WM_LBUTTONDOWN:	{ SendMessage( GetParent( hWnd ), TBM_DRAG_START, (WPARAM)hWnd, lParam ); break; }
	case WM_LBUTTONUP:		{ SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ); break; }
	case BN_HIGHLIGHTED:	{ System.HighLightButton( (HWND)lParam ); break; }

	case WM_PAINT:
		{
			CustomUI.hdc = BeginPaint( hWnd, &CustomUI.PaintStruct );

			GetClientRect( hWnd, &CustomUI.Rect1 );

			CustomUI.DrawSeparator( NULL, 63, 0, CustomUI.Rect1.bottom, FALSE );
			CustomUI.DrawSeparator( NULL, 162, 0, CustomUI.Rect1.bottom, FALSE );
			CustomUI.DrawSeparator( NULL, 373, 0, CustomUI.Rect1.bottom, FALSE );
			CustomUI.DrawSeparator( NULL, 589, 0, CustomUI.Rect1.bottom, FALSE );

			EndPaint( hWnd, &CustomUI.PaintStruct );
			break;
		}

	case BN_PUSHED:
		{
			switch( wParam )
			{
		//Must Be Present !
			case IDC_SELECT_DIALOG: { return TRUE; }

			case IDC_EDIT_CENTER_SELECTION:
				{
					if( State.Tool.ID == TOOL_EDIT_MOVE ) { Options.MoveCenterType = CENTER_SELECTION; }
					if( State.Tool.ID == TOOL_EDIT_ROTATE ) { Options.RotateCenterType = CENTER_SELECTION; }
					if( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) ) { Options.ScaleCenterType = CENTER_SELECTION; }

					Options.CenterType = CENTER_SELECTION;
					System.Selection.UpdateCenter();
					System.UpdateAllViews( TRUE );
					return TRUE;
				}
			case IDC_EDIT_CENTER_ORIGIN:
				{
					if( State.Tool.ID == TOOL_EDIT_MOVE ) { Options.MoveCenterType = CENTER_ORIGIN; }
					if( State.Tool.ID == TOOL_EDIT_ROTATE ) { Options.RotateCenterType = CENTER_ORIGIN; }
					if( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) ) { Options.ScaleCenterType = CENTER_ORIGIN; }

					Options.CenterType = CENTER_ORIGIN;
					System.Selection.UpdateCenter();
					System.UpdateAllViews( TRUE );
					return TRUE;
				}
			case IDC_EDIT_CENTER_SYSTEM:
				{
					if( State.Tool.ID == TOOL_EDIT_MOVE ) { Options.MoveCenterType = CENTER_SYSTEM; }
					if( State.Tool.ID == TOOL_EDIT_ROTATE ) { Options.RotateCenterType = CENTER_SYSTEM; }
					if( ( State.Tool.ID == TOOL_EDIT_USCALE ) || ( State.Tool.ID == TOOL_EDIT_NSCALE ) ) { Options.ScaleCenterType = CENTER_SYSTEM; }

					Options.CenterType = CENTER_SYSTEM;
					System.Selection.UpdateCenter();
					System.UpdateAllViews( TRUE );
					return TRUE;
				}

			case IDC_SELECT_RECT:
				{
					System.Selection.RegionType = SRT_RECTANGLE;
					return TRUE;
				}

			case IDC_SELECT_CIRCLE:
				{
					System.Selection.RegionType = SRT_CIRCLE;
					return TRUE;
				}

			case IDC_SELECT_POLYGON:
				{
					System.Selection.RegionType = SRT_POLYGON;
					return TRUE;
				}

			case IDC_SELECT_CROSSING:
				{ 
					Options.SelectIntersecting = FALSE;
					return TRUE; 
				}

			case IDC_SELECT:
				{ 
					State.Tool.Type = TOOL_SELECT;
					State.Tool.ID = TOOL_NONE;
					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_SELECT ) );

				//Update Transform TypeIn
					System.Selection.UpdateTypeInDialog( FALSE );

					break; 
				}

			case IDC_EDIT_MOVE:
				{ 
					State.Tool.Type = TOOL_EDIT;
					State.Tool.ID = TOOL_EDIT_MOVE;
					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_MOVE ) );

				//Update Transform TypeIn
					System.Selection.UpdateTypeInDialog( FALSE );

					SendMessage( MainToolBar.GetControl( IDC_EDIT_CENTER ), BM_SELECT_SUBBUTTON, Options.MoveCenterType, TRUE ); 
					break; 
				}

			case IDC_EDIT_USCALE:
				{ 
					State.Tool.Type = TOOL_EDIT;
					State.Tool.ID = TOOL_EDIT_USCALE;
					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_SCALE ) );
	
				//Update Transform TypeIn
					System.Selection.UpdateTypeInDialog( FALSE );

					SendMessage( MainToolBar.GetControl( IDC_EDIT_CENTER ), BM_SELECT_SUBBUTTON, Options.ScaleCenterType, TRUE ); 
					break; 
				}

			case IDC_EDIT_NSCALE:
				{ 
					State.Tool.Type = TOOL_EDIT;
					State.Tool.ID = TOOL_EDIT_NSCALE;
					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_SCALE ) );

				//Update Transform TypeIn
					System.Selection.UpdateTypeInDialog( FALSE );

					SendMessage( MainToolBar.GetControl( IDC_EDIT_CENTER ), BM_SELECT_SUBBUTTON, Options.ScaleCenterType, TRUE ); 
					break; 
				}
			}

			State.ToolLevel = 0;
			State.SetToolInfo( 0, (HWND)lParam );

			State.Tool.Step = 0;
			State.Tool.Active = FALSE;
			State.Tool.Enabled = TRUE;
			State.Tool.hButton = (HWND)lParam;

			System.UpdateAllViews( FALSE );
			break;
		}

	case BN_RELEASED:
		{
			switch( wParam )
			{
			case IDC_SELECT_DIALOG:
				{
					DialogBox( System.hInstance, MAKEINTRESOURCE( IDD_SELECT ), System.hWnd, *SelectProc ); 
					return TRUE;
				}

			case IDC_SELECT_CROSSING:
				{ 
					Options.SelectIntersecting = TRUE;
					break; 
				}
			}

			break;
		}
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
/******************************* OBJECTS ********************************/
//////////////////////////////////////////////////////////////////////////


BOOL CALLBACK CreatePlaneProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:		{ KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_MOUSEMOVE:		{ SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ); SetCursor( System.hCursorHand ); break; }
	case WM_LBUTTONDOWN:	{ SendMessage( MainBar.hWnd, DBM_DRAG_START, (WPARAM)hWnd, lParam ); break; }
	case BN_HIGHLIGHTED:	{ System.HighLightButton( (HWND)lParam ); break; }

	case WM_SHOWWINDOW:
		{
			if( !(BOOL)wParam )
			{
				SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_PLANE, IDC_CREATE ), BM_RELEASE, 0, 0 );
			}

			break;
		}

	case BN_PUSHED:
		{
			switch( wParam )
			{
			case IDC_CREATE:
				{
					if( !State.FileReady ) { return FALSE; }

					State.ToolLevel = 1;
					State.SetToolInfo( 1, (HWND)lParam );

					State.Tool.Type = TOOL_CREATE;
					State.Tool.ID = TOOL_CREATE_PLANE;
					State.MasterObjectType = MODEL;
					State.UsingMaster = FALSE;

					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_SELECT ) );
					State.Tool.Step = 0;
					State.Tool.Active = FALSE;
					State.Tool.hButton = (HWND)lParam;

					break;
				}
			}
			break;
		}

	case BN_RELEASED:
		{
			switch( wParam )
			{

			case IDC_CREATE_DATA:
				{
					if( !State.FileReady ) { return FALSE; }

					State.hTempCursor = SetCursor( LoadCursor( NULL, IDC_WAIT ) );

					WORD model = pScene->AddModel( OBJECT_PLANE );
					if( model < 0 ) { return FALSE; }

					State.MasterObjectType = MODEL;
					State.pMasterObject = (void*)&pScene->pModel[model];			
					State.UsingMaster = FALSE;

					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_X_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Position[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_Y_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Position[1] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_Z_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Position[2] ); 

					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ROT_X_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Rotation[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ROT_Y_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Rotation[1] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ROT_Z_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Rotation[2] ); 

					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_SCALE_X_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Scale[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_SCALE_Y_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Scale[1] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_SCALE_Z_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Scale[2] ); 

					pScene->pModel[model].Scale[0] /= 100.0f;
					pScene->pModel[model].Scale[1] /= 100.0f;
					pScene->pModel[model].Scale[2] /= 100.0f;

					CPlane * pPlane = (CPlane*)pScene->pModel[model].pBaseObject; 
	
					SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_PLANE, IDC_DIMENSION_X_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pPlane->Size[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_PLANE, IDC_DIMENSION_Y_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pPlane->Size[1] ); 

					SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_PLANE, IDC_TASSELLATION_X_SPIN ), ASM_GET_VAL_INT, 0, (LPARAM)&pPlane->Tassellation[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_PLANE, IDC_TASSELLATION_Y_SPIN ), ASM_GET_VAL_INT, 0, (LPARAM)&pPlane->Tassellation[1] ); 

//					pPlane->Initialize();
					pPlane->GenerateMesh();

					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ORIGIN_X_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Mesh.pObject[pPlane->Object].Position[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ORIGIN_Y_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Mesh.pObject[pPlane->Object].Position[1] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ORIGIN_Z_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Mesh.pObject[pPlane->Object].Position[2] ); 

					GetDlgItemText( MainBar.GetGroupDialog( 0, IDD_OBJECT_INFO ), IDC_OBJECT_INFO_NAME, &pScene->pModel[model].Name[0], 32 );  

				//Assign Random Color
					System.CurrentColorIndex = 200000 / rand();
					if( System.CurrentColorIndex > (UINT)( System.BasePaletteSize.x * System.BasePaletteSize.y - 1 ) ) { System.CurrentColorIndex = (UINT)( System.BasePaletteSize.x * System.BasePaletteSize.y - 1 ); }

					HWND hwnd = MainBar.GetGroupControl( 0, IDD_OBJECT_INFO , IDC_OBJECT_INFO_COLOR ) ;

					tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( MainBar.GetGroupControl( 0, IDD_OBJECT_INFO, IDC_OBJECT_INFO_COLOR ), GWL_USERDATA );
					if( pInfo )
					{
						pInfo->Color = System.BasePalette[(UINT)( System.CurrentColorIndex % 16 )][(UINT)( System.CurrentColorIndex / 16 )]; 
						RedrawWindow( MainBar.GetGroupDialog( 0, IDD_OBJECT_INFO), NULL, NULL, RDW_INVALIDATE );

						pScene->pModel[model].SetColor( System.BasePalette[(UINT)( System.CurrentColorIndex % 16 )][(UINT)( System.CurrentColorIndex / 16 )] );
					}
					pPlane->UpdateMesh( TRUE );

					pScene->pModel[model].UpdateExtents();

					System.Selection.ClearEntries();
					System.Selection.AddEntry( MODEL, model, (void*)&pScene->pModel[model], TRUE );
					System.Selection.UpdateCenter();

					System.Selection.GizmoVisible = TRUE;
					System.Selection.BoxesVisible = TRUE;

					System.UpdateAllViews( TRUE );
					
					SetCursor( State.hTempCursor );
					return TRUE; 
				}
			}
			break;
		}//case BN_RELEASED

	case WM_INITDIALOG:
		{
			SetDlgItemText( hWnd, IDC_TASSELLATION_X, "1" );
			SetDlgItemText( hWnd, IDC_TASSELLATION_Y, "1" );

			SetDlgItemText( hWnd, IDC_DIMENSION_X, "100" );
			SetDlgItemText( hWnd, IDC_DIMENSION_Y, "100" );
			
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_X_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_TASSELLATION_X ), 0 );
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_Y_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_TASSELLATION_Y ), 0 );

			SendDlgItemMessage( hWnd, IDC_TASSELLATION_X_SPIN, UDM_SETRANGE, 0, MAKELONG( 1, 100 ) );
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_Y_SPIN, UDM_SETRANGE, 0, MAKELONG( 1, 100 ) );

			SendDlgItemMessage( hWnd, IDC_DIMENSION_X_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_DIMENSION_X ), 0 );
			SendDlgItemMessage( hWnd, IDC_DIMENSION_Y_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_DIMENSION_Y ), 0 );
	
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_X_SPIN, UDM_SETACCEL, 1, 0 );
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_Y_SPIN, UDM_SETACCEL, 1, 0 );
			
			SendDlgItemMessage( hWnd, IDC_DIMENSION_X_SPIN, UDM_SETACCEL, 1, 0 );
			SendDlgItemMessage( hWnd, IDC_DIMENSION_Y_SPIN, UDM_SETACCEL, 1, 0 );

			break;
		}

	case WM_COMMAND:
		{
			switch( HIWORD( wParam ) )
			{

			case BN_DBLCLK:
				{
					switch( LOWORD( wParam ) )
					{
					case IDC_GROUPBUTTON:
						{
							if( SendMessage( GetParent( hWnd ), ACGM_GET_STATE, 0, 0 ) == ACGS_NORMAL ) { SendMessage( GetParent( hWnd ), ACGM_CLOSE, 0, 0 ); }
							else { SendMessage( GetParent( hWnd ), ACGM_OPEN, 0, 0 ); }
							break; 
						}
					}
				}//case BN_DBLCLK:
			}

			PostMessage( (HWND)GetWindowLong( GetParent( hWnd ), CtrlGroupParent ), uMsg, wParam, lParam );
			return TRUE;
		}

	case WM_PAINT: { CustomUI.DrawAGroupControlUI( hWnd ); return FALSE; }
	}
	return FALSE;
}


BOOL CALLBACK CreateBoxProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:		{ KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_MOUSEMOVE:		{ SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ); SetCursor( System.hCursorHand ); break; }
	case WM_LBUTTONDOWN:	{ SendMessage( MainBar.hWnd, DBM_DRAG_START, (WPARAM)hWnd, lParam ); break; }
	case BN_HIGHLIGHTED:	{ System.HighLightButton( (HWND)lParam ); break; }

	case WM_SHOWWINDOW:
		{
			if( !(BOOL)wParam )
			{
				SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_CREATE ), BM_RELEASE, 0, 0 );
			}

			break;
		}

	case BN_PUSHED:
		{
			switch( wParam )
			{
			case IDC_CREATE:
				{
					if( !State.FileReady ) { return FALSE; }

					State.ToolLevel = 1;
					State.SetToolInfo( 1, (HWND)lParam );

					State.Tool.Type = TOOL_CREATE;
					State.Tool.ID = TOOL_CREATE_BOX;
					State.MasterObjectType = MODEL;
					State.UsingMaster = FALSE;

					State.Tool.hCursor = LoadCursor( System.hInstance, MAKEINTRESOURCE( IDC_TOOL_SELECT ) );
					State.Tool.Step = 0;
					State.Tool.Active = FALSE;
					State.Tool.hButton = (HWND)lParam;

					break;
				}
			}
			break;
		}

	case BN_RELEASED:
		{
			switch( wParam )
			{

			case IDC_CREATE_DATA:
				{
					if( !State.FileReady ) { return FALSE; }

					State.hTempCursor = SetCursor( LoadCursor( NULL, IDC_WAIT ) );

					WORD model = pScene->AddModel( OBJECT_BOX );
					if( model < 0 ) { return FALSE; }

					State.MasterObjectType = MODEL;
					State.pMasterObject = (void*)&pScene->pModel[model];			
					State.UsingMaster = FALSE;

					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_X_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Position[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_Y_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Position[1] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_POS_Z_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Position[2] ); 

					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ROT_X_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Rotation[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ROT_Y_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Rotation[1] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ROT_Z_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Rotation[2] ); 

					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_SCALE_X_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Scale[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_SCALE_Y_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Scale[1] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_SCALE_Z_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Scale[2] ); 

					pScene->pModel[model].Scale[0] /= 100.0f;
					pScene->pModel[model].Scale[1] /= 100.0f;
					pScene->pModel[model].Scale[2] /= 100.0f;

					CBox * pBox = (CBox*)pScene->pModel[model].pBaseObject; 
	
					SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_DIMENSION_X_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pBox->Size[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_DIMENSION_Y_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pBox->Size[1] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_DIMENSION_Z_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pBox->Size[2] ); 

					SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_TASSELLATION_X_SPIN ), ASM_GET_VAL_INT, 0, (LPARAM)&pBox->Tassellation[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_TASSELLATION_Y_SPIN ), ASM_GET_VAL_INT, 0, (LPARAM)&pBox->Tassellation[1] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_CREATE_BOX, IDC_TASSELLATION_Z_SPIN ), ASM_GET_VAL_INT, 0, (LPARAM)&pBox->Tassellation[2] ); 

					pBox->GenerateMesh();

					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ORIGIN_X_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Mesh.pObject[pBox->Object].Position[0] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ORIGIN_Y_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Mesh.pObject[pBox->Object].Position[1] ); 
					SendMessage( MainBar.GetGroupControl( 0, IDD_OBJECT_TRANSFORMS, IDC_OBJECT_ORIGIN_Z_SPIN ), ASM_GET_VAL_FLOAT, 0, (LPARAM)&pScene->pModel[model].Mesh.pObject[pBox->Object].Position[2] ); 

					pScene->pModel[model].Mesh.pObject[pBox->Object].Position[1] += pBox->Size[1] / 2;

					GetDlgItemText( MainBar.GetGroupDialog( 0, IDD_OBJECT_INFO ), IDC_OBJECT_INFO_NAME, &pScene->pModel[model].Name[0], 32 );  

				//Assign Random Color
					System.CurrentColorIndex = 200000 / rand();
					if( System.CurrentColorIndex > (UINT)( System.BasePaletteSize.x * System.BasePaletteSize.y - 1 ) ) { System.CurrentColorIndex = (UINT)( System.BasePaletteSize.x * System.BasePaletteSize.y - 1 ); }

					HWND hwnd = MainBar.GetGroupControl( 0, IDD_OBJECT_INFO , IDC_OBJECT_INFO_COLOR ) ;

					tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( MainBar.GetGroupControl( 0, IDD_OBJECT_INFO, IDC_OBJECT_INFO_COLOR ), GWL_USERDATA );
					if( pInfo )
					{
						pInfo->Color = System.BasePalette[(UINT)( System.CurrentColorIndex % 16 )][(UINT)( System.CurrentColorIndex / 16 )]; 
						RedrawWindow( MainBar.GetGroupDialog( 0, IDD_OBJECT_INFO), NULL, NULL, RDW_INVALIDATE );

						pScene->pModel[model].SetColor( System.BasePalette[(UINT)( System.CurrentColorIndex % 16 )][(UINT)( System.CurrentColorIndex / 16 )] );
					}

					pBox->UpdateMesh( TRUE );

					pScene->pModel[model].UpdateExtents();

					System.Selection.ClearEntries();
					System.Selection.AddEntry( MODEL, model, (void*)&pScene->pModel[model], TRUE );
					System.Selection.UpdateCenter();

					System.Selection.GizmoVisible = TRUE;
					System.Selection.BoxesVisible = TRUE;

					System.UpdateAllViews( TRUE );
					
					SetCursor( State.hTempCursor );
					return TRUE; 
				}
			}
			break;
		}//case BN_RELEASED

	case WM_INITDIALOG:
		{
			SetDlgItemText( hWnd, IDC_TASSELLATION_X, "1" );
			SetDlgItemText( hWnd, IDC_TASSELLATION_Y, "1" );
			SetDlgItemText( hWnd, IDC_TASSELLATION_Z, "1" );

			SetDlgItemText( hWnd, IDC_DIMENSION_X, "100" );
			SetDlgItemText( hWnd, IDC_DIMENSION_Y, "100" );
			SetDlgItemText( hWnd, IDC_DIMENSION_Z, "100" );
			
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_X_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_TASSELLATION_X ), 0 );
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_Y_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_TASSELLATION_Y ), 0 );
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_Z_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_TASSELLATION_Z ), 0 );

			SendDlgItemMessage( hWnd, IDC_TASSELLATION_X_SPIN, UDM_SETRANGE, 0, MAKELONG( 1, 50 ) );
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_Y_SPIN, UDM_SETRANGE, 0, MAKELONG( 1, 50 ) );
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_Z_SPIN, UDM_SETRANGE, 0, MAKELONG( 1, 50 ) );

			SendDlgItemMessage( hWnd, IDC_DIMENSION_X_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_DIMENSION_X ), 0 );
			SendDlgItemMessage( hWnd, IDC_DIMENSION_Y_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_DIMENSION_Y ), 0 );
			SendDlgItemMessage( hWnd, IDC_DIMENSION_Z_SPIN, UDM_SETBUDDY, (WPARAM)GetDlgItem( hWnd, IDC_DIMENSION_Z ), 0 );
	
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_X_SPIN, UDM_SETACCEL, 1, 0 );
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_Y_SPIN, UDM_SETACCEL, 1, 0 );
			SendDlgItemMessage( hWnd, IDC_TASSELLATION_Z_SPIN, UDM_SETACCEL, 1, 0 );
			
			SendDlgItemMessage( hWnd, IDC_DIMENSION_X_SPIN, UDM_SETACCEL, 1, 0 );
			SendDlgItemMessage( hWnd, IDC_DIMENSION_Y_SPIN, UDM_SETACCEL, 1, 0 );
			SendDlgItemMessage( hWnd, IDC_DIMENSION_Z_SPIN, UDM_SETACCEL, 1, 0 );

			break;
		}

	case WM_COMMAND:
		{
			switch( HIWORD( wParam ) )
			{

			case BN_DBLCLK:
				{
					switch( LOWORD( wParam ) )
					{
					case IDC_GROUPBUTTON:
						{
							if( SendMessage( GetParent( hWnd ), ACGM_GET_STATE, 0, 0 ) == ACGS_NORMAL ) { SendMessage( GetParent( hWnd ), ACGM_CLOSE, 0, 0 ); }
							else { SendMessage( GetParent( hWnd ), ACGM_OPEN, 0, 0 ); }
							break; 
						}
					}
				}//case BN_DBLCLK:
			}

			PostMessage( (HWND)GetWindowLong( GetParent( hWnd ), CtrlGroupParent ), uMsg, wParam, lParam );
			return TRUE;
		}

	case WM_PAINT: { CustomUI.DrawAGroupControlUI( hWnd ); return FALSE; }
	}
	return FALSE;
}


BOOL CALLBACK CreateGeometryProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//	tButtonGroupInfo *pButtonInfo;

	switch( uMsg )
	{
	case WM_KEYDOWN: { KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_MOUSEMOVE:		{ SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ); SetCursor( System.hCursorHand ); break; }
	case WM_LBUTTONDOWN:	{ SendMessage( MainBar.hWnd, DBM_DRAG_START, (WPARAM)hWnd, lParam ); break; }
	case BN_HIGHLIGHTED:	{ System.HighLightButton( (HWND)lParam ); break; }

	case WM_INITDIALOG:
		{
			System.SelectGroup( "CreateObjectSelect" );
	
			System.AddButton( hWnd, IDC_CREATE_SELECT_BOX );
			System.AddButton( hWnd, IDC_CREATE_SELECT_SPHERE );
			System.AddButton( hWnd, IDC_CREATE_SELECT_TUBE );
			
			System.AddButton( hWnd, IDC_CREATE_SELECT_CONE );
			System.AddButton( hWnd, IDC_CREATE_SELECT_GEOSPHERE );
			System.AddButton( hWnd, IDC_CREATE_SELECT_PYRAMID );
			
			System.AddButton( hWnd, IDC_CREATE_SELECT_CYLINDER );
			System.AddButton( hWnd, IDC_CREATE_SELECT_PLANE );
			System.AddButton( hWnd, IDC_CREATE_SELECT_TORUS );

			return FALSE;
		}

	case BN_PUSHED:
		{
			MainBar.ClearDialogGroups( 0 );
			MainBar.SelectGroup( 0, IDD_CREATE, CreateProc );
			MainBar.SelectGroup( 0, IDD_CREATE_GEOMETRY, CreateGeometryProc );

			switch( wParam )
			{
			case IDC_CREATE_BOX:
				{
					MainBar.SelectGroup( 0, IDD_OBJECT_INFO, ObjectInfoProc );
					MainBar.SelectGroup( 0, IDD_CREATE_BOX, CreateBoxProc );
					MainBar.SelectGroup( 0, IDD_OBJECT_TRANSFORMS, ObjectTransformsProc );
					break;
				}

			case IDC_CREATE_PLANE:
				{
					MainBar.SelectGroup( 0, IDD_OBJECT_INFO, ObjectInfoProc );
					MainBar.SelectGroup( 0, IDD_CREATE_PLANE, CreatePlaneProc );
					MainBar.SelectGroup( 0, IDD_OBJECT_TRANSFORMS, ObjectTransformsProc );
					break;
				}

			case IDC_CREATE_SPHERE:
				{
					break;
				}

			case IDC_CREATE_GEOSPHERE:
				{
					break;
				}
			}

			MainBar.ReArrangeGroups();
			return FALSE;
		}

	case WM_COMMAND:
		{
			switch( HIWORD( wParam ) )
			{
				
			case BN_DBLCLK:
				{
					switch( LOWORD( wParam ) )
					{
					case IDC_GROUPBUTTON:
						{
							if( SendMessage( GetParent( hWnd ), ACGM_GET_STATE, 0, 0 ) == ACGS_NORMAL ) { SendMessage( GetParent( hWnd ), ACGM_CLOSE, 0, 0 ); }
							else { SendMessage( GetParent( hWnd ), ACGM_OPEN, 0, 0 ); }
							return FALSE; 
						}
					}
				}//case BN_DBLCLK:
			}

			PostMessage( (HWND)GetWindowLong( GetParent( hWnd ), CtrlGroupParent ), uMsg, wParam, lParam );
			return FALSE;
		}

	case WM_PAINT: { CustomUI.DrawAGroupControlUI( hWnd ); return FALSE; }
	}
	return FALSE;
}


BOOL CALLBACK CreateProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//	tButtonGroupInfo *pButtonInfo;

	switch( uMsg )
	{
	case WM_KEYDOWN: { KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_MOUSEMOVE:		{ SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ); SetCursor( System.hCursorHand ); break; }
	case WM_LBUTTONDOWN:	{ SendMessage( MainBar.hWnd, DBM_DRAG_START, (WPARAM)hWnd, lParam ); break; }
	case BN_HIGHLIGHTED:	{ System.HighLightButton( (HWND)lParam ); break; }

	case WM_INITDIALOG:
		{
			System.SelectGroup( "CreateSelect" );
	
			System.AddButton( hWnd, IDC_CREATE_GEOMETRY );
			System.AddButton( hWnd, IDC_CREATE_SHAPES );
			System.AddButton( hWnd, IDC_CREATE_LIGHTS );
			System.AddButton( hWnd, IDC_CREATE_HELPERS );
			return FALSE;
		}

	case BN_PUSHED:
		{
			MainBar.ClearDialogGroups( 0 );
			MainBar.SelectGroup( 0, IDD_CREATE, CreateProc );

			switch( wParam )
			{
			case IDC_CREATE_GEOMETRY:
				{
					MainBar.SelectGroup( 0, IDD_CREATE_GEOMETRY, CreateGeometryProc );
					break;
				}
			}

			MainBar.ReArrangeGroups();
			return FALSE;
		}

	case WM_COMMAND:
		{
			switch( HIWORD( wParam ) )
			{
				
			case BN_DBLCLK:
				{
					switch( LOWORD( wParam ) )
					{
					case IDC_GROUPBUTTON:
						{
							if( SendMessage( GetParent( hWnd ), ACGM_GET_STATE, 0, 0 ) == ACGS_NORMAL ) { SendMessage( GetParent( hWnd ), ACGM_CLOSE, 0, 0 ); }
							else { SendMessage( GetParent( hWnd ), ACGM_OPEN, 0, 0 ); }
							return FALSE; 
						}
					}
				}//case BN_DBLCLK:
			}

			PostMessage( (HWND)GetWindowLong( GetParent( hWnd ), CtrlGroupParent ), uMsg, wParam, lParam );
			return FALSE;
		}

	case WM_PAINT: { CustomUI.DrawAGroupControlUI( hWnd ); return FALSE; }
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
/****************************** MODIFIERS *******************************/
//////////////////////////////////////////////////////////////////////////


BOOL CALLBACK ModifierSelectProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN: { KeyHandler.OnKeyDown( hWnd, wParam, lParam ); break; } 
	case WM_MOUSEMOVE:		{ SendMessage( GetParent( hWnd ), uMsg, wParam, lParam ); SetCursor( System.hCursorHand ); break; }
	case WM_LBUTTONDOWN:	{ SendMessage( MainBar.hWnd, DBM_DRAG_START, (WPARAM)hWnd, lParam ); break; }
	case BN_HIGHLIGHTED:	{ System.HighLightButton( (HWND)lParam ); break; }

	case WM_INITDIALOG:
		{
			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(HWND)NULL ); 
			break;
		}

/*	case BN_PUSHED:
		{
			switch( wParam )
			{
			case IDC_CREATE_SELECT_BOX:
				{
					MainBar.SelectGroup( 0, IDD_CREATE_BOX, CreateBoxProc );
					MainBar.SelectGroup( 0, IDD_OBJECT_TRANSFORMS, ObjectTransformsProc );
					MainBar.ReArrangeGroups();
					MainBar.SelectDialog( MainBar.GetCurrentDialog() );

				//Release Previous Button + Set Current Button
					if( (HWND)GetWindowLong( hWnd, GWL_USERDATA ) != NULL ) { SendMessage( (HWND)GetWindowLong( hWnd, GWL_USERDATA ), BM_RELEASE, 0, 0 ); }
					SetWindowLong( hWnd, GWL_USERDATA, (LONG)(HWND)GetDlgItem( hWnd, wParam ) ); 
					break;
				}

			case IDC_CREATE_SELECT_SPHERE:
				{
				//Release Previous Button + Set Current Button
					if( (HWND)GetWindowLong( hWnd, GWL_USERDATA ) != NULL ) { SendMessage( (HWND)GetWindowLong( hWnd, GWL_USERDATA ), BM_RELEASE, 0, 0 ); }
					SetWindowLong( hWnd, GWL_USERDATA, (LONG)(HWND)GetDlgItem( hWnd, wParam ) ); 
					break;
				}

			case IDC_CREATE_SELECT_GEOSPHERE:
				{
				//Release Previous Button + Set Current Button
					if( (HWND)GetWindowLong( hWnd, GWL_USERDATA ) != NULL ) { SendMessage( (HWND)GetWindowLong( hWnd, GWL_USERDATA ), BM_RELEASE, 0, 0 ); }
					SetWindowLong( hWnd, GWL_USERDATA, (LONG)(HWND)GetDlgItem( hWnd, wParam ) ); 
					break;
				}
			}
			break;
		}//case BN_PUSHED:

	case BN_RELEASED:
		{
			switch( wParam )
			{
			case IDC_CREATE_SELECT_BOX:
				{
					MainBar.HideGroup( 0, 3 );
					MainBar.HideGroup( 0, 2 );
					MainBar.ReArrangeGroups();
					MainBar.SelectDialog( MainBar.GetCurrentDialog() );

				//Reset Previous Button -> as it was the Button, that Sent this Message
					SetWindowLong( hWnd, GWL_USERDATA, (LONG)(HWND)NULL ); 
					break; 
				}
			}
			break;
		}//case BN_RELEASED*/

	case WM_COMMAND:
		{
			switch( HIWORD( wParam ) )
			{
				
			case BN_DBLCLK:
				{
					switch( LOWORD( wParam ) )
					{
					case IDC_GROUPBUTTON:
						{
							if( SendMessage( GetParent( hWnd ), ACGM_GET_STATE, 0, 0 ) == ACGS_NORMAL ) { SendMessage( GetParent( hWnd ), ACGM_CLOSE, 0, 0 ); }
							else { SendMessage( GetParent( hWnd ), ACGM_OPEN, 0, 0 ); }
							break; 
						}
					}
				}//case BN_DBLCLK:
			}

			PostMessage( (HWND)GetWindowLong( GetParent( hWnd ), CtrlGroupParent ), uMsg, wParam, lParam );
			return TRUE;
		}

	case WM_PAINT: { CustomUI.DrawAGroupControlUI( hWnd ); return FALSE; }
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
/************************** DIALOG PROCEDURES ***************************/
//////////////////////////////////////////////////////////////////////////


BOOL CALLBACK ViewOptionsProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//Parse the Message
	switch( uMsg )
	{
	case WM_INITDIALOG:
		{ 
			GetWindowText( hWnd, State.TempBuffer32a, 32 );
			strcat( State.TempBuffer32a, " ( OpenGL " ); 
			strcat( State.TempBuffer32a, (char*)System.OpenGLVersion ); 
			strcat( State.TempBuffer32a, " ) " ); 
			SetWindowText( hWnd, State.TempBuffer32a );


			RECT rt, DlgRect;
			GetWindowRect( System.hWnd, &rt );
			GetWindowRect( hWnd, &DlgRect );
		
			SetWindowPos( hWnd, NULL,  rt.left + ( ( rt.right -  rt.left ) / 2 ) - ( DlgRect.right - DlgRect.left ) / 2 , rt.top + ( ( rt.bottom -  rt.top ) / 2 ) - ( DlgRect.bottom - DlgRect.top ) / 2, 0, 0, SWP_NOSIZE );   

		//OpenGL Information
			SendDlgItemMessage( hWnd, IDC_VIEW_COLOR_BITS, CB_ADDSTRING, 0, (LONG)(LPSTR)"16" );
			SendDlgItemMessage( hWnd, IDC_VIEW_COLOR_BITS, CB_ADDSTRING, 0, (LONG)(LPSTR)"32" );
			if( System.ColorBits == 16 ) { SendDlgItemMessage( hWnd, IDC_VIEW_COLOR_BITS, CB_SETCURSEL, 0, 0 ); }
			else { SendDlgItemMessage( hWnd, IDC_VIEW_COLOR_BITS, CB_SETCURSEL, 1, 0 ); }

			SendDlgItemMessage( hWnd, IDC_VIEW_DEPTH_BITS, CB_ADDSTRING, 0, (LONG)(LPSTR)"16" );
			SendDlgItemMessage( hWnd, IDC_VIEW_DEPTH_BITS, CB_ADDSTRING, 0, (LONG)(LPSTR)"24" );
			SendDlgItemMessage( hWnd, IDC_VIEW_DEPTH_BITS, CB_ADDSTRING, 0, (LONG)(LPSTR)"32" );
			if( System.DepthBits == 16 ) { SendDlgItemMessage( hWnd, IDC_VIEW_DEPTH_BITS, CB_SETCURSEL, 0, 0 ); }
			if( System.DepthBits == 24 ) { SendDlgItemMessage( hWnd, IDC_VIEW_DEPTH_BITS, CB_SETCURSEL, 1, 0 ); }
			if( System.DepthBits == 32 ) { SendDlgItemMessage( hWnd, IDC_VIEW_DEPTH_BITS, CB_SETCURSEL, 2, 0 ); }
			
			SendDlgItemMessage( hWnd, IDC_VIEW_STENCIL_BITS, CB_ADDSTRING, 0, (LONG)(LPSTR)"0" );
			SendDlgItemMessage( hWnd, IDC_VIEW_STENCIL_BITS, CB_ADDSTRING, 0, (LONG)(LPSTR)"8" );
			if( System.StencilBits == 8 ) { SendDlgItemMessage( hWnd, IDC_VIEW_STENCIL_BITS, CB_SETCURSEL, 1, 0 ); }
			else { SendDlgItemMessage( hWnd, IDC_VIEW_STENCIL_BITS, CB_SETCURSEL, 0, 0 ); }

		//General Information
//			if( Options.SelectionGL ) { CheckDlgButton( hWnd, IDC_VIEW_GLSELECTION, BST_CHECKED ); } 

/*			if( IsDlgButtonChecked( hWnd, IDC_VIEW_GLSELECTION ) )
			{ 
				CheckDlgButton( hWnd, IDC_VIEW_WHOLE_SELECTION, BST_UNCHECKED ); 
				EnableWindow( GetDlgItem( hWnd, IDC_VIEW_WHOLE_SELECTION ), FALSE ); 
			}
			else { if( Options.SelectionWhole ) { CheckDlgButton( hWnd, IDC_VIEW_WHOLE_SELECTION, BST_CHECKED ); }  } 
*/
//			if( Options.SelectIntersecting ) { CheckDlgButton( hWnd, IDC_VIEW_SELECT_INTERSECTING, BST_CHECKED ); }
			if( Options.UpdateOnDialogMove ) { CheckDlgButton( hWnd, IDC_VIEW_UPDATE_ON_DIALOG_MOVE, BST_CHECKED );	} 

			sprintf( State.TempBuffer16a, "%g", (float)Options.ViewDistance );
			SetDlgItemText( hWnd, IDC_VIEW_DISTANCE, State.TempBuffer16a );
	

		//Display Information
			System.SelectGroup( "View_Options_Select" );

			System.AddButton( hWnd, IDC_VIEW0 ); 
			System.AddButton( hWnd, IDC_VIEW1 ); 
			System.AddButton( hWnd, IDC_VIEW2 ); 
			System.AddButton( hWnd, IDC_VIEW3 ); 

			SendMessage( GetDlgItem( hWnd, IDC_VIEW0 ), BM_PUSH, 0, 0 );

			SetActiveWindow( hWnd );

			if( System.View[0].RenderMode == RM_SHADED ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_SHADED ), BM_CLICK, 0, 0 ); }
			if( System.View[0].RenderMode == RM_WIREFRAME ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_WIREFRAME ), BM_CLICK, 0, 0 ); }
			if( System.View[0].RenderMode == RM_COMPOSITE ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_COMPOSITE ), BM_CLICK, 0, 0 ); }
	
			System.Update = TRUE;
			break; 
		}

	case BN_PUSHED:
		{
			switch( wParam )
			{
			case IDC_VIEW0:
				{
					if( System.View[0].RenderMode == RM_SHADED ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_SHADED ), BM_CLICK, 0, 0 ); }
					if( System.View[0].RenderMode == RM_WIREFRAME ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_WIREFRAME ), BM_CLICK, 0, 0 ); }
					if( System.View[0].RenderMode == RM_COMPOSITE ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_COMPOSITE ), BM_CLICK, 0, 0 ); }
					break;
				}

			case IDC_VIEW1:
				{
					if( System.View[1].RenderMode == RM_SHADED ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_SHADED ), BM_CLICK, 0, 0 ); }
					if( System.View[1].RenderMode == RM_WIREFRAME ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_WIREFRAME ), BM_CLICK, 0, 0 ); }
					if( System.View[1].RenderMode == RM_COMPOSITE ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_COMPOSITE ), BM_CLICK, 0, 0 ); }
					break;
				}

			case IDC_VIEW2:
				{
					if( System.View[2].RenderMode == RM_SHADED ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_SHADED ), BM_CLICK, 0, 0 ); }
					if( System.View[2].RenderMode == RM_WIREFRAME ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_WIREFRAME ), BM_CLICK, 0, 0 ); }
					if( System.View[2].RenderMode == RM_COMPOSITE ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_COMPOSITE ), BM_CLICK, 0, 0 ); }
					break;
				}

			case IDC_VIEW3:
				{
					if( System.View[3].RenderMode == RM_SHADED ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_SHADED ), BM_CLICK, 0, 0 ); }
					if( System.View[3].RenderMode == RM_WIREFRAME ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_WIREFRAME ), BM_CLICK, 0, 0 ); }
					if( System.View[3].RenderMode == RM_COMPOSITE ) { SendMessage( GetDlgItem( hWnd, IDC_VIEW_COMPOSITE ), BM_CLICK, 0, 0 ); }
					break;
				}
			}
			break;
		}

	case WM_COMMAND:
		{

/*			switch( HIWORD(wParam) )
			{
			case BN_CLICKED:
				{
					switch( LOWORD(wParam) )
					{
					case IDC_VIEW_GLSELECTION:
						{
							if( IsDlgButtonChecked( hWnd, IDC_VIEW_GLSELECTION ) )
							{ 
								CheckDlgButton( hWnd, IDC_VIEW_WHOLE_SELECTION, BST_UNCHECKED ); 
								EnableWindow( GetDlgItem( hWnd, IDC_VIEW_WHOLE_SELECTION ), FALSE ); 
							} 
							else { EnableWindow( GetDlgItem( hWnd, IDC_VIEW_WHOLE_SELECTION ), TRUE ); }

							break;
						}
					}
					break;
				}
				break;
			}*/

			switch( LOWORD(wParam) )
			{
			case IDC_VIEW_SHADED:
				{
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW0 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[0].RenderMode = RM_SHADED; System.View[0].NeedUpdate = TRUE; }
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW1 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[1].RenderMode = RM_SHADED; System.View[1].NeedUpdate = TRUE; }
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW2 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[2].RenderMode = RM_SHADED; System.View[2].NeedUpdate = TRUE; }
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW3 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[3].RenderMode = RM_SHADED; System.View[3].NeedUpdate = TRUE; }
					System.Update = TRUE;
					break;
				}
			case IDC_VIEW_WIREFRAME:
				{
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW0 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[0].RenderMode = RM_WIREFRAME; System.View[0].NeedUpdate = TRUE; }
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW1 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[1].RenderMode = RM_WIREFRAME; System.View[1].NeedUpdate = TRUE; }
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW2 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[2].RenderMode = RM_WIREFRAME; System.View[2].NeedUpdate = TRUE; }
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW3 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[3].RenderMode = RM_WIREFRAME; System.View[3].NeedUpdate = TRUE; }
					System.Update = TRUE;
					break;
				}
			case IDC_VIEW_COMPOSITE:
				{
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW0 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[0].RenderMode = RM_COMPOSITE; System.View[0].NeedUpdate = TRUE; }
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW1 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[1].RenderMode = RM_COMPOSITE; System.View[1].NeedUpdate = TRUE; }
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW2 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[2].RenderMode = RM_COMPOSITE; System.View[2].NeedUpdate = TRUE; }
					if( SendMessage( GetDlgItem( hWnd, IDC_VIEW3 ), BM_IS_PUSHED, 0, 0 ) ) { System.View[3].RenderMode = RM_COMPOSITE; System.View[3].NeedUpdate = TRUE; }
					System.Update = TRUE;
					break;
				}
				break;
			}
		}

	case BN_RELEASED:
		{
			switch( wParam )
			{
			case IDOK:
				{
					SendMessage( hWnd, WM_SYSCOMMAND, SC_CLOSE, 0 );
					break;
				}
			}
			break;
		}

	case WM_SYSCOMMAND:
		{
			switch( wParam )
			{
			case SC_CLOSE:
				{
				//OpenGL Information
					if( SendDlgItemMessage( hWnd, IDC_VIEW_COLOR_BITS, CB_GETCURSEL, 0, 0 ) == 0 ) { Options.ColorBits = 16; } 
					else { Options.ColorBits = 32; }

					if( SendDlgItemMessage( hWnd, IDC_VIEW_DEPTH_BITS, CB_GETCURSEL, 0, 0 ) == 0 ) { Options.DepthBits = 16; } 
					if( SendDlgItemMessage( hWnd, IDC_VIEW_DEPTH_BITS, CB_GETCURSEL, 1, 0 ) == 0 ) { Options.DepthBits = 24; } 
					if( SendDlgItemMessage( hWnd, IDC_VIEW_DEPTH_BITS, CB_GETCURSEL, 2, 0 ) == 0 ) { Options.DepthBits = 32; } 
	
					if( SendDlgItemMessage( hWnd, IDC_VIEW_STENCIL_BITS, CB_GETCURSEL, 0, 0 ) == 0 ) { Options.StencilBits = 0; } 
					else { Options.StencilBits = 8; }


				//General Information
/*					if( IsDlgButtonChecked( hWnd, IDC_VIEW_SELECT_INTERSECTING ) == BST_CHECKED ) { Options.SelectIntersecting = TRUE; }
					else { Options.SelectIntersecting = FALSE; }
*/
					if( IsDlgButtonChecked( hWnd, IDC_VIEW_UPDATE_ON_DIALOG_MOVE ) == BST_CHECKED ) { Options.UpdateOnDialogMove = TRUE; }
					else { Options.UpdateOnDialogMove = FALSE; }

					
					GetDlgItemText( hWnd, IDC_VIEW_DISTANCE, State.TempBuffer16a, 16 );
					float tmpFloat;
					sscanf( State.TempBuffer16a, "%f", &tmpFloat );
					Options.ViewDistance = tmpFloat;
					System.View[3].ClipFar = Options.ViewDistance;
					System.View[3].NeedUpdate = TRUE;
					System.Update = TRUE;

				//Need to Release The Last Pushed Button !!!
					System.ClearLastButton();
					EndDialog( hWnd, TRUE );
					System.Update = TRUE;
					return FALSE; 
				}	
			}
			break;
		}
	}
	return FALSE;
}



