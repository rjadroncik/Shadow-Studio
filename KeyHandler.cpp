// KeyHandler.cpp: implementation of the CKeyHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KeyHandler.h"

#include "View.h"
#include "System.h"
#include "Shadow.h"
#include "Options.h"
#include "State.h"
#include "Afc.h"
#include "Resource.h"
#include "RenderGL/RenderGL.h"


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

CKeyHandler::CKeyHandler()
{

}

CKeyHandler::~CKeyHandler()
{

}

void CKeyHandler::OnKeyDown( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	switch( (int)wParam )
	{
	case 'A': { SendMessage( MainToolBar.GetControl( IDC_EDIT_MOVE ), BM_PUSH, 0, 0 );		SetCursor( State.Tool.hCursor ); break; }
	case 'S': { SendMessage( MainToolBar.GetControl( IDC_EDIT_ROTATE ), BM_PUSH, 0, 0 );	SetCursor( State.Tool.hCursor ); break; }
	case 'D': { SendMessage( MainToolBar.GetControl( IDC_EDIT_SCALE ), BM_PUSH, 0, 0 );	SetCursor( State.Tool.hCursor ); break; }
	case 'Z': { SendMessage( BottomToolBar.GetControl( IDC_PAN ), BM_PUSH, 0, 0 );			SetCursor( State.Tool.hCursor ); break; }
	case 'X': { SendMessage( BottomToolBar.GetControl( IDC_ROTATE ), BM_PUSH, 0, 0 );		SetCursor( State.Tool.hCursor ); break; }
	case 'C': { SendMessage( BottomToolBar.GetControl( IDC_ZOOM ), BM_PUSH, 0, 0 );		SetCursor( State.Tool.hCursor ); break; }

	case 'H': { DialogBox( System.hInstance, MAKEINTRESOURCE( IDD_SELECT ), System.hWnd, *SelectProc ); break; }

	case VK_SUBTRACT: { if( Options.GizmoSize > 0.5f ) { Options.GizmoSize -= 0.1f; System.UpdateAllViews( FALSE ); } break; }
	case VK_ADD: { if( Options.GizmoSize < 2.5f ) { Options.GizmoSize += 0.1f; System.UpdateAllViews( FALSE ); } break; }

	case VK_DELETE:
		{
			for( UINT n = 0; n < System.Selection.ActiveInfos; n++ )
			{
				if( System.Selection.pInfo[n].ObjectType == MODEL )
				{
					pScene->DeleteModel( System.Selection.pInfo[n].ObjectIndex );
					
					for( UINT m = 0; m < System.Selection.ActiveInfos; m++ )
					{
						if( m == n ) { continue; }

						if( System.Selection.pInfo[m].ObjectIndex == pScene->ModelCount )
						{
							System.Selection.pInfo[m].ObjectIndex = System.Selection.pInfo[n].ObjectIndex;
						}
					}

				}
			}

			System.Selection.GizmoVisible = FALSE;
			System.Selection.ActiveInfos = 0;

			System.Selection.UpdateInfoDialog();
			System.Selection.UpdateTypeInDialog( FALSE );

			System.UpdateAllViews( TRUE );
			
			break;
		}

	case 'G':
		{
			if( System.View[System.ViewActive].GridVisible ) { State.View.GridVisible = FALSE; System.View[System.ViewActive].GridVisible = FALSE; }
			else { State.View.GridVisible = TRUE; System.View[System.ViewActive].GridVisible = TRUE; }
		
			System.View[System.ViewActive].Update( TRUE );
			break; 
		}

	case VK_F3:
		{
			if( State.Tool.Active ) { return; }

			if( State.View.RenderMode == RM_WIREFRAME ) { State.View.RenderMode = RM_SHADED; System.View[System.ViewActive].RenderMode = RM_SHADED; System.View[System.ViewActive].Update( TRUE ); return; }
			if( State.View.RenderMode == RM_SHADED ) { State.View.RenderMode = RM_COMPOSITE; System.View[System.ViewActive].RenderMode = RM_COMPOSITE; System.View[System.ViewActive].Update( TRUE ); return; }
			if( State.View.RenderMode == RM_COMPOSITE ) { State.View.RenderMode = RM_WIREFRAME; System.View[System.ViewActive].RenderMode = RM_WIREFRAME; System.View[System.ViewActive].Update( TRUE ); return; }
			
			break;
		}

	case VK_F4:
		{
			if( State.Tool.Active ) { return; }

			if( Options.DrawOrigins ) { Options.DrawOrigins = FALSE; }
			else { Options.DrawOrigins = TRUE; }
			
			System.UpdateAllViews( TRUE );
			break;
		}

	case VK_F2:
		{
			if( State.Tool.Active ) { break; }
			if( System.FourViews ) { System.EnlargeView( System.ViewActive ); }
			else { System.FourViews = TRUE; System.ResizeWindowMain(); }
			break;
		}
	}
}
