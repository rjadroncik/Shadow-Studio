#include "stdafx.h"
#include "afc.h"
#include "System.h"
#include "Resource.h"
#include <malloc.h>

#include "Windows.h"
#include "Winuser.h"

LRESULT CALLBACK ADialogBarProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK ATrackBarProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK AScrollBarProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK AControlGroupProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK ASpinnerProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK AButtonProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK AEditProc( HWND, UINT, WPARAM, LPARAM );

LRESULT CALLBACK AToolBarProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK AToolTipProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK ADropdownMenuProc( HWND, UINT, WPARAM, LPARAM );

BOOL CALLBACK AControlGroupDlgProc( HWND, UINT, WPARAM, LPARAM );




extern ACustomUI		CustomUI;
extern ADialogBar		MainBar;
extern CSystem			System;


//Global Vars For SubClassing
HWND	hButtonOrigin;
HWND	hSpinnerOrigin;
HWND	hEditOrigin;

WNDPROC	ButtonProcOrigin;
WNDPROC	SpinnerProcOrigin;
WNDPROC	EditProcOrigin;


void InitControlsAFC( HINSTANCE hInst )
{
//Fill Initialization Structure 
	INITCOMMONCONTROLSEX Controls; 
 	Controls.dwICC = ICC_BAR_CLASSES | ICC_HOTKEY_CLASS | ICC_LISTVIEW_CLASSES | ICC_PROGRESS_CLASS | ICC_TAB_CLASSES | ICC_USEREX_CLASSES | ICC_COOL_CLASSES | ICC_UPDOWN_CLASS;
	Controls.dwSize = sizeof(Controls);

//Initialize Common Controls
	InitCommonControlsEx(&Controls);

//Fu..... Class Work
	WNDCLASSEX ScrollClass;
	ScrollClass.cbClsExtra = 0;
	ScrollClass.cbSize = sizeof( WNDCLASSEX );
	ScrollClass.cbWndExtra = 16;
	ScrollClass.hbrBackground = NULL;
	ScrollClass.hCursor = NULL;
	ScrollClass.hIcon = NULL;
	ScrollClass.hIconSm = NULL;
	ScrollClass.hInstance = hInst;
	ScrollClass.lpfnWndProc = (WNDPROC)AScrollBarProc;
	ScrollClass.lpszClassName = "AScrollBar";
	ScrollClass.lpszMenuName = NULL;
	ScrollClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx( &ScrollClass );

//More Fu..... Class Work -> GRRRRRRRRR.........
	WNDCLASSEX GroupClass;
	GroupClass.cbClsExtra = 0;
	GroupClass.cbSize = sizeof( WNDCLASSEX );
	GroupClass.cbWndExtra = 32;
	GroupClass.hbrBackground = NULL;
	GroupClass.hCursor = NULL;
	GroupClass.hIcon = NULL;
	GroupClass.hIconSm = NULL;
	GroupClass.hInstance = hInst;
	GroupClass.lpfnWndProc = (WNDPROC)AControlGroupProc;
	GroupClass.lpszClassName = "AControlGroup";
	GroupClass.lpszMenuName = NULL;
	GroupClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx( &GroupClass );

//I Can't Take This...
	WNDCLASSEX TrackClass;
	TrackClass.cbClsExtra = 0;
	TrackClass.cbSize = sizeof( WNDCLASSEX );
	TrackClass.cbWndExtra = 20;
	TrackClass.hbrBackground = (HBRUSH)( COLOR_BTNFACE + 1 );
	TrackClass.hCursor = NULL;
	TrackClass.hIcon = NULL;
	TrackClass.hIconSm = NULL;
	TrackClass.hInstance = hInst;
	TrackClass.lpfnWndProc = (WNDPROC)ATrackBarProc;
	TrackClass.lpszClassName = "ATrackBar";
	TrackClass.lpszMenuName = NULL;
	TrackClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx( &TrackClass );

//This is BullShit
	WNDCLASSEX ToolClass;
	ToolClass.cbClsExtra = 0;
	ToolClass.cbSize = sizeof( WNDCLASSEX );
	ToolClass.cbWndExtra = 0;
	ToolClass.hbrBackground = (HBRUSH)( COLOR_BTNFACE + 1 );
	ToolClass.hCursor = NULL;
	ToolClass.hIcon = NULL;
	ToolClass.hIconSm = NULL;
	ToolClass.hInstance = hInst;
	ToolClass.lpfnWndProc = (WNDPROC)AToolBarProc;
	ToolClass.lpszClassName = "AToolBar";
	ToolClass.lpszMenuName = NULL;
	ToolClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx( &ToolClass );

//....
	WNDCLASSEX DropMenuClass;
	DropMenuClass.cbClsExtra = 0;
	DropMenuClass.cbSize = sizeof( WNDCLASSEX );
	DropMenuClass.cbWndExtra = 0;
	DropMenuClass.hbrBackground = (HBRUSH)( COLOR_BTNFACE + 1 );
	DropMenuClass.hCursor = NULL;
	DropMenuClass.hIcon = NULL;
	DropMenuClass.hIconSm = NULL;
	DropMenuClass.hInstance = hInst;
	DropMenuClass.lpfnWndProc = (WNDPROC)ADropdownMenuProc;
	DropMenuClass.lpszClassName = "ADropdownMenu";
	DropMenuClass.lpszMenuName = NULL;
	DropMenuClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx( &DropMenuClass );

//It's Not Worth it...
	WNDCLASSEX ToolTipClass;
	ToolTipClass.cbClsExtra = 0;
	ToolTipClass.cbSize = sizeof( WNDCLASSEX );
	ToolTipClass.cbWndExtra = 0;
	ToolTipClass.hbrBackground = (HBRUSH)( COLOR_BTNFACE + 1 );
	ToolTipClass.hCursor = NULL;
	ToolTipClass.hIcon = NULL;
	ToolTipClass.hIconSm = NULL;
	ToolTipClass.hInstance = hInst;
	ToolTipClass.lpfnWndProc = (WNDPROC)AToolTipProc;
	ToolTipClass.lpszClassName = "AToolTip";
	ToolTipClass.lpszMenuName = NULL;
	ToolTipClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx( &ToolTipClass );


//Spinner Control SubClassing
	hSpinnerOrigin = CreateUpDownControl( WS_CHILD | WS_BORDER, 0, 0, 0, 0, System.hWnd, 9711, hInst, NULL, 100, 0, 0 );
	SpinnerProcOrigin = (WNDPROC)SetClassLong( hSpinnerOrigin, GCL_WNDPROC, (LONG)(WNDPROC)ASpinnerProc );

//Button SubClassing
	hButtonOrigin = CreateWindowEx( NULL, "BUTTON", NULL, WS_CHILD, 0, 0, 0, 0, System.hWnd, (HMENU)9712, hInst, NULL );
	ButtonProcOrigin = (WNDPROC)SetClassLong( hButtonOrigin, GCL_WNDPROC, (LONG)(WNDPROC)AButtonProc );

//EditBox SubClassing
	hEditOrigin = CreateWindowEx( NULL, "EDIT", NULL, WS_CHILD, 0, 0, 0, 0, System.hWnd, (HMENU)9713, hInst, NULL );
	EditProcOrigin = (WNDPROC)SetClassLong( hEditOrigin, GCL_WNDPROC, (LONG)(WNDPROC)AEditProc );
}


///////////////////////////////////////////////////////////////////	
////****			    CLASS: (ADialogBar)				   ****////
///////////////////////////////////////////////////////////////////	

//Construction & Destruction
ADialogBar::ADialogBar()
{
//Main Variables
	pDialog = NULL;
	DialogCount = 0;
	DialogCurrent = 0;

	Dragable = TRUE;
	Dragging = FALSE;


//Positioning Variables
	MarginSize = 2;
	TabBarHeight = 12;
	ScrollBarSize.cx = 6;
	TabBarHeight = 20;
	ScrollPosition = 0;
	CreatedCount = 0;
}

ADialogBar::~ADialogBar() { }


//Other Member Functions

void ADialogBar::SelectGroup( UINT dialog, UINT id, DLGPROC dlgProc )
{
	RECT tmp;

	pDialog[dialog].OrderedCount++;
	pDialog[dialog].pGroupOrdered = (UINT*)realloc( pDialog[dialog].pGroupOrdered, sizeof( UINT ) * pDialog[dialog].OrderedCount );

	for( UINT n = 0; n < pDialog[dialog].GroupCount; n++ )
	{
		if( pDialog[dialog].pGroup[n].ID == id ) 
		{ 
//			ShowWindow( GetGroup( dialog, id ), SW_SHOW );  
			GetClientRect( (HWND)SendMessage( GetGroup( dialog, id ), ACGM_GET_DLG, 0, 0 ), &tmp );

			SetWindowPos( GetGroup( dialog, id ), NULL, 0, pDialog[dialog].Height, (UINT)( DialogRect.right ), tmp.bottom, SWP_SHOWWINDOW );

		//Update Group Information
			pDialog[dialog].pGroup[n].Position = pDialog[dialog].Height;

			pDialog[dialog].Height += tmp.bottom;
			pDialog[dialog].pGroupOrdered[pDialog[dialog].OrderedCount - 1] = n;
			return;
		}
	}

	tControlGroupInfo Info;
	Info.DialogRes = id;
	Info.State = ACGS_NORMAL;
	Info.DlgProc = dlgProc;

	HWND hwnd = CreateWindowEx( NULL, "AControlGroup", NULL, WS_VISIBLE | WS_CHILD, MarginSize * 2, 0, (UINT)( DialogRect.right * 1.18 ), 100, pDialog[dialog].hWnd, (HMENU)id, hInstance, (LPVOID)&Info );

	CreatedCount++;

	if( CreatedCount > 10 ) { MessageBox( NULL, "CFontTGA::CreateTextureTGA()", "Can't open file !", MB_ICONERROR ); }

	GetClientRect( (HWND)SendMessage( hwnd, ACGM_GET_DLG, 0, 0 ), &tmp );

	pDialog[dialog].GroupCount++;
	pDialog[dialog].pGroup = (tGroupData*)realloc( pDialog[dialog].pGroup, pDialog[dialog].GroupCount * sizeof( tGroupData ) );

//Update Group Information
	pDialog[dialog].pGroup[pDialog[dialog].GroupCount - 1].ID = id;
	pDialog[dialog].pGroup[pDialog[dialog].GroupCount - 1].Position = pDialog[dialog].Height;

	SetWindowPos( hwnd, NULL, 0, pDialog[dialog].Height, (UINT)( DialogRect.right ), tmp.bottom, SWP_SHOWWINDOW );
	ShowWindow( GetGroupDialog( dialog, id ), SW_SHOW );
	pDialog[dialog].Height += tmp.bottom;

	pDialog[dialog].pGroupOrdered[pDialog[dialog].OrderedCount - 1] = pDialog[dialog].GroupCount - 1;
}

void ADialogBar::ClearDialogGroups( UINT dialog )
{
	for( UINT n = 0; n < pDialog[dialog].GroupCount; n++ )
	{
		ShowWindow( GetGroup( dialog, pDialog[dialog].pGroup[n].ID ), SW_HIDE );  
	}

	pDialog[dialog].Height = 0;

	if( pDialog[dialog].pGroupOrdered )
	{
		free( pDialog[dialog].pGroupOrdered );
	
		pDialog[dialog].pGroupOrdered = NULL;
		pDialog[dialog].OrderedCount = 0;
	} 
}

void ADialogBar::ReArrangeGroups()
{
	UINT CurrentPos = 0;

	for( UINT n = 0; n < pDialog[DialogCurrent].OrderedCount; n++ )
	{
		if( IsWindowVisible( GetGroup( DialogCurrent, pDialog[DialogCurrent].pGroup[pDialog[DialogCurrent].pGroupOrdered[n]].ID ) ) )
		{
			SetWindowPos( GetGroup( DialogCurrent, pDialog[DialogCurrent].pGroup[pDialog[DialogCurrent].pGroupOrdered[n]].ID ), NULL, 0, CurrentPos, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );

			if( GetWindowLong( GetGroup( DialogCurrent, pDialog[DialogCurrent].pGroup[pDialog[DialogCurrent].pGroupOrdered[n]].ID ), CtrlGroupState ) == ACGS_CLOSED ) { CurrentPos += GetWindowLong( GetGroup( DialogCurrent, pDialog[DialogCurrent].pGroup[pDialog[DialogCurrent].pGroupOrdered[n]].ID ), CtrlGroupHeightClosed ); }
			if( GetWindowLong( GetGroup( DialogCurrent, pDialog[DialogCurrent].pGroup[pDialog[DialogCurrent].pGroupOrdered[n]].ID ), CtrlGroupState ) == ACGS_NORMAL ) { CurrentPos += GetWindowLong( GetGroup( DialogCurrent, pDialog[DialogCurrent].pGroup[pDialog[DialogCurrent].pGroupOrdered[n]].ID ), CtrlGroupHeightOpen ); }
		}
	}

	pDialog[DialogCurrent].Height =	CurrentPos;

	SetBarPosition( Rect.left, Rect.top, Rect.right, Rect.bottom );
}


void ADialogBar::Create( HWND parent, HINSTANCE hInst, UINT scrollBarId )
{
//Class Work...
	WNDCLASSEX DialogBarClass;
	DialogBarClass.cbClsExtra = 0;
	DialogBarClass.cbSize = sizeof( WNDCLASSEX );
	DialogBarClass.cbWndExtra = 0;
	DialogBarClass.hbrBackground = (HBRUSH)( COLOR_BTNFACE + 1 );
	DialogBarClass.hCursor = NULL;
	DialogBarClass.hIcon = NULL;
	DialogBarClass.hIconSm = NULL;
	DialogBarClass.hInstance = hInst;
	DialogBarClass.lpfnWndProc = (WNDPROC)ADialogBarProc;
	DialogBarClass.lpszClassName = "ADialogBar";
	DialogBarClass.lpszMenuName = NULL;
	DialogBarClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx( &DialogBarClass );

//Create Main Window + The One ScrollBar
	hWnd = CreateWindowEx( NULL, "ADialogBar", NULL, WS_VISIBLE | WS_CHILD, 0, 0, 100, 100, parent, (HMENU)1479, hInstance, this );
	hScrollBar = CreateWindowEx( NULL, "AScrollBar", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 10, 10, hWnd, (HMENU)scrollBarId, hInstance, NULL );

//Update Variables
	ScrollBarID = scrollBarId;
	hInstance = hInst;
	hParent = parent;
}

void ADialogBar::AddDialog( UINT resId, DLGPROC procedure )
{
	pDialog = (tDialogData*)realloc( pDialog, ( DialogCount + 1 ) * sizeof( tDialogData ) );  

	pDialog[DialogCount].hWnd = CreateDialogParam( hInstance, MAKEINTRESOURCE( resId ), hWnd, procedure, (LPARAM)this );

	RECT rect;
	GetWindowRect( pDialog[DialogCount].hWnd, &rect );
	
	pDialog[DialogCount].Height = 0;
	pDialog[DialogCount].pGroup = NULL;
	pDialog[DialogCount].GroupCount = 0;
	pDialog[DialogCount].ID = resId;
	pDialog[DialogCount].Position = 0;
	pDialog[DialogCount].RealPos = 0;
	pDialog[DialogCount].Proc = procedure;
	pDialog[DialogCount].HandlePos = 0;

	pDialog[DialogCount].OrderedCount = 0;
	pDialog[DialogCount].pGroupOrdered = NULL;


	ShowWindow( pDialog[DialogCount].hWnd, SW_SHOW );
	DialogCount++;
}

void ADialogBar::RemoveDialog( UINT index )
{
	DialogCount--;
	
	DestroyWindow( pDialog[index].hWnd );
	pDialog[index].hWnd = NULL;

	pDialog[index] = pDialog[DialogCount];
	pDialog = (tDialogData*)realloc( pDialog, DialogCount * sizeof( tDialogData ) );  
}

HWND ADialogBar::GetDialog( UINT index ) { return pDialog[index].hWnd; }

HWND ADialogBar::GetGroup( UINT dialog, UINT id )
{
	if( dialog == -1 ) { dialog = DialogCurrent; }

	for( UINT n = 0; n < pDialog[dialog].GroupCount; n++ )
	{
		if( pDialog[dialog].pGroup[n].ID == id ) { return GetDlgItem( pDialog[dialog].hWnd, id ); }
	}

	return NULL; 
}

bool ADialogBar::IsGroupDialog( UINT dialog, UINT id )
{
	if( dialog == -1 ) { dialog = DialogCurrent; }

	for( UINT n = 0; n < pDialog[dialog].GroupCount; n++ )
	{
		if( pDialog[dialog].pGroup[n].ID == id ) { return TRUE; }
	}

	return FALSE; //(HWND)GetWindowLong( GetGroup( dialog, id ), CtrlGroupDialog ); 
}


HWND ADialogBar::GetGroupDialog( UINT dialog, UINT id )
{
	if( dialog == -1 ) { dialog = DialogCurrent; }

	return (HWND)GetWindowLong( GetGroup( dialog, id ), CtrlGroupDialog ); 
}

HWND ADialogBar::GetGroupControl( UINT dialog, UINT id, UINT controlId )
{
	if( dialog == -1 ) { dialog = DialogCurrent; }

	return GetDlgItem( GetGroupDialog( dialog, id ), controlId );
}

HWND ADialogBar::GetTabBar() { return hTabBar; }
HWND ADialogBar::GetScrollBar() { return hScrollBar; }
UINT ADialogBar::GetCurrentDialog() { return DialogCurrent; }
UINT ADialogBar::GetDialogCount() { return DialogCount; }


void ADialogBar::SetTabBar( UINT resId, DLGPROC procedure )
{
	hTabBar = CreateDialog( hInstance, MAKEINTRESOURCE( resId ), hParent, procedure );
	TabBarID = resId;

	ShowWindow( hTabBar, SW_SHOW );
}

void ADialogBar::SetBarPosition( UINT x, UINT y, UINT width, UINT height )
{
	if( DialogCount <= 0 ) { return; }

//Adjust DialogBar
	Rect.bottom = height;
	Rect.left = x;
	Rect.right = width;
	Rect.top = y;
	SetWindowPos( hWnd, NULL, Rect.left + MarginSize + 1, Rect.top + TabBarHeight, Rect.right - ( MarginSize * 3 ), Rect.bottom - TabBarHeight - MarginSize - 1, SWP_SHOWWINDOW ); 

//Adjust TabBar
	DialogMargin.left = Rect.left + MarginSize;
	DialogMargin.top = Rect.top + TabBarHeight - 1;
	DialogMargin.right = DialogMargin.left + Rect.right - ( MarginSize * 3 ) + 1;
	DialogMargin.bottom = DialogMargin.top + Rect.bottom - TabBarHeight - MarginSize;
	SetWindowPos( hTabBar, NULL, Rect.left, Rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE ); 

//Adjust ScrollBar
	ScrollBarPos.x = width - ( 3 * MarginSize ) - ScrollBarSize.cx;	
	ScrollBarPos.y = 0;
	SetWindowPos( hScrollBar, NULL, ScrollBarPos.x, ScrollBarPos.y, ScrollBarSize.cx, height - MarginSize - TabBarHeight - 1, SWP_SHOWWINDOW ); 

	int HandleSize;
	if( pDialog[DialogCurrent].Height > (UINT)DialogRect.bottom ) { HandleSize = (int)( (double)DialogRect.bottom / (double)pDialog[DialogCurrent].Height * (double)SendMessage( hScrollBar, ASBM_GET_RANGE, 0, 0 ) ); }
	else { HandleSize = SendMessage( hScrollBar, ASBM_GET_RANGE, 0, 0 ); }

	SendMessage( hScrollBar, ASBM_SETHANDLE_SIZE, HandleSize, 0 );
	SendMessage( hScrollBar, ASBM_SETHANDLE_POS, pDialog[DialogCurrent].HandlePos, 0 );

//Update The Inner Rectangle
	DialogRect.bottom = height - TabBarHeight;
	DialogRect.right = width - ( 5 * MarginSize ) - ScrollBarSize.cx;
	DialogRect.left = MarginSize;
	DialogRect.top = 0;

	if( DialogCount > 0 ) { SetWindowPos( pDialog[DialogCurrent].hWnd, NULL, DialogRect.left, DialogRect.top, DialogRect.right, pDialog[DialogCurrent].Height, SWP_SHOWWINDOW ); }
	SetDialogPosition( DialogCurrent, -pDialog[DialogCurrent].HandlePos );
}

/*
void ADialogBar::MoveGroup( UINT dialog, UINT groupId, UINT x, UINT y )
{
	if( pDialog[dialog].GroupCount > 0 )
	{
		UINT group = 0;
		for( UINT f = 0; f < pDialog[dialog].GroupCount; f++ ) { if( pDialog[dialog].pGroup[f].ID == groupId ) { group = f; } }

		SetWindowPos( pDialog[dialog].pGroup[group].hWnd, NULL, MarginSize + x + 1, pDialog[dialog].pGroup[pDialog[dialog].GroupCount].Position + y, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE );
	}
}
*/

void ADialogBar::ResetCurrentDialog()
{
	if( DialogCount > 2 )
	{
		UINT tmp = DialogCurrent;

		if( ( DialogCurrent + 1 ) < DialogCount ) { SelectDialog( DialogCurrent + 1 ); }
		else { SelectDialog( DialogCurrent - 1 ); }
		
		SelectDialog( tmp );
	}
}

void ADialogBar::Initialize()
{
	for( UINT f = 1; f < DialogCount; f++ )
	{
		ShowWindow( pDialog[f].hWnd, SW_HIDE );
	
		for( UINT n = 0; n < pDialog[f].GroupCount; n++ )
		{
			ShowWindow( GetGroupDialog( f, pDialog[f].pGroup[n].ID ), SW_HIDE );
		}
	}

	DialogCurrent = 0;
	InvalidateRect( hTabBar, NULL, TRUE );

	int HandleSize;
	if( pDialog[0].Height > (UINT)DialogRect.bottom ) { HandleSize = (int)( (double)DialogRect.bottom / (double)pDialog[0].Height * (double)SendMessage( hScrollBar, ASBM_GET_RANGE, 0, 0 ) ); }
	else { HandleSize = SendMessage( hScrollBar, ASBM_GET_RANGE, 0, 0 ); }

	SendMessage( hScrollBar, ASBM_SETHANDLE_SIZE, HandleSize, 0 );
	SendMessage( hScrollBar, ASBM_SETHANDLE_POS, pDialog[0].HandlePos, 0 );

	SetBarPosition( Rect.left, Rect.top, Rect.right, Rect.bottom );

	InvalidateRect( hScrollBar, NULL, TRUE );
	UpdateWindow( hScrollBar );

	for( UINT n = 0; n < pDialog[0].GroupCount; n++ )
	{
		ShowWindow( GetGroupDialog( 0, pDialog[0].pGroup[n].ID ), SW_SHOW );
	}	

//	ShowWindow( pDialog[DialogCurrent].hWnd, SW_HIDE );
}

void ADialogBar::SelectDialog( UINT index )
{
	for( UINT f = 0; f < DialogCount; f++ )
	{
//		if( f != index )
//		{
			ShowWindow( pDialog[f].hWnd, SW_HIDE );
		
			for( UINT n = 0; n < pDialog[f].GroupCount; n++ )
			{
				ShowWindow( GetGroupDialog( f, pDialog[f].pGroup[n].ID ), SW_HIDE );
			}
//		}
	}

	DialogCurrent = index;
	InvalidateRect( hTabBar, NULL, TRUE );

	SetBarPosition( Rect.left, Rect.top, Rect.right, Rect.bottom );

	InvalidateRect( hScrollBar, NULL, TRUE );
	UpdateWindow( hScrollBar );

	for( UINT n = 0; n < pDialog[index].GroupCount; n++ )
	{
		ShowWindow( GetGroupDialog( index, pDialog[index].pGroup[n].ID ), SW_SHOW );
	}
	
	ReArrangeGroups();
}	

void ADialogBar::SetDialogPosition( UINT index, int position )
{
	int	ScrollRange = pDialog[index].Height - DialogRect.bottom + MarginSize + 3;
	if( ScrollRange < 0 ) { ScrollRange = 0; }


	if( pDialog != NULL )
	{	
		UINT NewPos = (int)( (double)position / 100.0f * (double)ScrollRange );

		SetWindowPos( pDialog[index].hWnd, NULL, 4, NewPos, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW ); 

		pDialog[index].RealPos = NewPos;
		pDialog[index].HandlePos = -position;
	}
}

///////////////////////////////////////////////////////////////////	
////****			    CLASS: (ATrackBar)				   ****////
///////////////////////////////////////////////////////////////////	

//Construction & Destruction
ATrackBar::ATrackBar()
{
//Positioning Variables
	MarginSize = 3;

	HandlePosition = 0;
	HandleSize = 50;

	RangeStart = 0;
	RangeEnd = 100;
	Range = 100;
}

ATrackBar::~ATrackBar() { }


//Other Member Functions

void ATrackBar::Create( HWND parent, HINSTANCE hInst )
{
/*//Class Work...
	WNDCLASSEX TrackBarClass;
	TrackBarClass.cbClsExtra = 0;
	TrackBarClass.cbSize = sizeof( WNDCLASSEX );
	TrackBarClass.cbWndExtra = 0;
	TrackBarClass.hbrBackground = (HBRUSH)( COLOR_BTNFACE + 1 );
	TrackBarClass.hCursor = NULL;
	TrackBarClass.hIcon = NULL;
	TrackBarClass.hIconSm = NULL;
	TrackBarClass.hInstance = hInst;
	TrackBarClass.lpfnWndProc = (WNDPROC)ATrackBarProc;
	TrackBarClass.lpszClassName = "ADialogBar";
	TrackBarClass.lpszMenuName = NULL;
	TrackBarClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx( &TrackBarClass );

//Create Main Window + The One ScrollBar
	hWnd = CreateWindowEx( NULL, "ADialogBar", NULL, WS_VISIBLE | WS_CHILD, 0, 0, 100, 100, parent, (HMENU)1479, hInstance, NULL );
	hScrollBar = CreateWindowEx( NULL, "AScrollBar", NULL, WS_CHILD | WS_VISIBLE, 0, 0, 10, 10, hWnd, (HMENU)scrollBarId, hInstance, NULL );

//Update Variables
	ScrollBarID = scrollBarId;
	hInstance = hInst;
	hParent = parent;*/
}

void ATrackBar::SetBarPosition( UINT x, UINT y, UINT width, UINT height )
{
/*	if( DialogCount <= 0 ) { return; }

//Adjust DialogBar
	Rect.bottom = height;
	Rect.left = x;
	Rect.right = width;
	Rect.top = y;
	SetWindowPos( hWnd, NULL, Rect.left + MarginSize + 1, Rect.top + TabBarHeight, Rect.right - ( MarginSize * 3 ), Rect.bottom - TabBarHeight - MarginSize - 1, SWP_SHOWWINDOW ); 

//Adjust TabBar
	DialogMargin.left = Rect.left + MarginSize;
	DialogMargin.top = Rect.top + TabBarHeight - 1;
	DialogMargin.right = DialogMargin.left + Rect.right - ( MarginSize * 3 ) + 1;
	DialogMargin.bottom = DialogMargin.top + Rect.bottom - TabBarHeight - MarginSize;
	SetWindowPos( hTabBar, NULL, Rect.left, Rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE ); 

//Adjust ScrollBar
	ScrollBarPos.x = width - ( 3 * MarginSize ) - ScrollBarSize.cx;	
	ScrollBarPos.y = 0;
	SetWindowPos( hScrollBar, NULL, ScrollBarPos.x, ScrollBarPos.y, ScrollBarSize.cx, height - MarginSize - TabBarHeight - 1, SWP_SHOWWINDOW ); 

//Update The Inner Rectangle
	DialogRect.bottom = height - TabBarHeight;
	DialogRect.right = width - ( 5 * MarginSize ) - ScrollBarSize.cx;
	DialogRect.left = MarginSize;
	DialogRect.top = 0;

	if( DialogCount > 0 ) { SetWindowPos( pDialog[DialogCurrent].hWnd, NULL, DialogRect.left, DialogRect.top, DialogRect.right, pDialog[DialogCurrent].Height, SWP_SHOWWINDOW ); }
	SetDialogPosition( DialogCurrent, -pDialog[DialogCurrent].HandlePos );*/
}

void ATrackBar::SetHandlePos( int position )
{
/*	int	ScrollRange = pDialog[index].Height - DialogRect.bottom + MarginSize + 3;
	if( ScrollRange < 0 ) { ScrollRange = 0; }

	if( pDialog != NULL )
	{	
		UINT NewPos = (int)( (double)position / 100.0f * (double)ScrollRange );

		SetWindowPos( pDialog[index].hWnd, NULL, 4, NewPos, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW ); 

		pDialog[index].HandlePos = -position;
	}*/
}

void ATrackBar::SetRange( int start, int end )
{
/*	int	ScrollRange = pDialog[index].Height - DialogRect.bottom + MarginSize + 3;
	if( ScrollRange < 0 ) { ScrollRange = 0; }

	if( pDialog != NULL )
	{	
		UINT NewPos = (int)( (double)position / 100.0f * (double)ScrollRange );

		SetWindowPos( pDialog[index].hWnd, NULL, 4, NewPos, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW ); 

		pDialog[index].HandlePos = -position;
	}*/
}

HWND ATrackBar::GetHandle() { return hWnd; }


///////////////////////////////////////////////////////////////////	
////****			    CLASS: (ACustomUI)				   ****////
///////////////////////////////////////////////////////////////////	

//Construction & Destruction
ACustomUI::ACustomUI()
{
//Fetch System Colors -> Done only once -> SpEEEED.... 
	Color3DLight = GetSysColor( COLOR_BTNHIGHLIGHT ); 
	Color3DShadow = GetSysColor( COLOR_BTNSHADOW );
	Color3DFace = GetSysColor( COLOR_BTNFACE );
	Color3DDarkShadow = GetSysColor( COLOR_3DDKSHADOW );
	Color3DLightShadow = RGB( GetRValue( Color3DShadow ) + 30, GetGValue( Color3DShadow ) + 30, GetBValue( Color3DShadow ) + 30 );

	hImageList16 = ImageList_Create( 16, 16, ILC_MASK | ILC_COLOR8, 32, 32 ); 
	hImageList24= ImageList_Create( 24, 24, ILC_MASK | ILC_COLOR8, 32, 32 ); 

	Pen3DLight = CreatePen( PS_SOLID, 1, Color3DLight );
	Pen3DShadow = CreatePen( PS_SOLID, 1, Color3DShadow );
	Pen3DFace = CreatePen( PS_SOLID, 1, Color3DFace );
	Pen3DDarkShadow = CreatePen( PS_SOLID, 1, Color3DDarkShadow );
	PenBlack = CreatePen( PS_SOLID, 1, RGB( 0, 0, 0 ) );
	PenWhite = CreatePen( PS_SOLID, 1, RGB( 255, 255, 255 ) );
	PenVoid = CreatePen( PS_NULL, 0, 0 );

	Brush3DLightShadow = CreateSolidBrush( Color3DLightShadow );
	Brush3DDarkShadow = CreateSolidBrush( Color3DDarkShadow );
	Brush3DShadow = CreateSolidBrush( Color3DShadow );
	Brush3DFace = CreateSolidBrush( Color3DFace );
	BrushOrange = CreateSolidBrush( RGB( 240, 210, 20 ) );
	BrushRed = CreateSolidBrush( RGB( 255, 0, 0 ) );
	BrushBlack = CreateSolidBrush( RGB( 0, 0, 0 ) );
	BrushWhite = CreateSolidBrush( RGB( 255, 255, 255 ) );
	BrushToolTip = CreateSolidBrush( RGB( 255, 255, 225 ) );
	
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_HOLLOW;
	BrushVoid = CreateBrushIndirect( &logBrush );

	FontArialSmall = CreateFont( 14, 5, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, "Arial" );
	FontArialMedium = CreateFont( 14, 6, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, "Arial" );

} 

ACustomUI::~ACustomUI()
{
	ImageList_Destroy( hImageList16 );
	ImageList_Destroy( hImageList24 );
}


UINT ACustomUI::AddImages16( LPSTR BitmapPath, LPSTR BitmapPath_a ) { return ImageList_Add( hImageList16, (HBITMAP)LoadImage( NULL, BitmapPath, IMAGE_BITMAP, 0, 16, LR_LOADFROMFILE ), (HBITMAP)LoadImage( NULL, BitmapPath_a, IMAGE_BITMAP, 0, 16, LR_LOADFROMFILE ) ); }

void ACustomUI::RemoveImage16( UINT index ) { ImageList_Remove( hImageList16, index ); }

void ACustomUI::SetButtonImage16( HWND dialog, UINT controlID, UINT imageIndex )
{
	tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( GetDlgItem( dialog, controlID ), GWL_USERDATA );

	pInfo->ImageIndex = imageIndex;
	pInfo->Style |= BS_IMAGE16;

	SetWindowLong( GetDlgItem( dialog, controlID ), GWL_USERDATA, (LONG)pInfo );
}

UINT ACustomUI::AddImages24( LPSTR BitmapPath, LPSTR BitmapPath_a ) { return ImageList_Add( hImageList24, (HBITMAP)LoadImage( NULL, BitmapPath, IMAGE_BITMAP, 0, 24, LR_LOADFROMFILE ), (HBITMAP)LoadImage( NULL, BitmapPath_a, IMAGE_BITMAP, 0, 24, LR_LOADFROMFILE ) ); }

void ACustomUI::RemoveImage24( UINT index ) { ImageList_Remove( hImageList24, index ); }

void ACustomUI::SetButtonImage24( HWND dialog, UINT controlID, UINT imageIndex )
{
	tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( GetDlgItem( dialog, controlID ), GWL_USERDATA );

	pInfo->ImageIndex = imageIndex;
	pInfo->Style |= BS_IMAGE24;

	SetWindowLong( GetDlgItem( dialog, controlID ), GWL_USERDATA, (LONG)pInfo );
}

//Additional Drawing
void ACustomUI::DrawADropdownMenu( HWND hButton, UINT style )
{
	tButtonInfo	*pInfo = (tButtonInfo*)GetWindowLong( hButton, GWL_USERDATA );
	if( !pInfo ) { return; }

	GetWindowRect( hButton, &Rect1 );

	if( ( style & DMS_UPWARD ) == DMS_UPWARD )
	{
		Rect2.left = Rect1.left;
		Rect2.top = Rect1.top - ( Rect1.bottom - Rect1.top ) * pInfo->SubButtonCount;
		Rect2.right = Rect1.right - Rect1.left;
		Rect2.bottom =( Rect1.bottom - Rect1.top ) * pInfo->SubButtonCount;
	}

	if( ( style & DMS_DOWNWARD ) == DMS_DOWNWARD )
	{
		Rect2.left = Rect1.left;
		Rect2.top = Rect1.bottom;
		Rect2.right = Rect1.right - Rect1.left;
		Rect2.bottom = ( Rect1.bottom - Rect1.top ) * pInfo->SubButtonCount;
	}

	hMenuWindow = CreateWindowEx( NULL, "ADropdownMenu", NULL, WS_POPUP, Rect2.left, Rect2.top, Rect2.right, Rect2.bottom, hButton, NULL, hInstance, pInfo );

	ShowWindow( hMenuWindow, SW_SHOWNOACTIVATE ); 
}

void ACustomUI::DrawImage16( HDC *pHdc, int xPos, int yPos, UINT index )
{
	if( pHdc ) { hdc = *pHdc; }

	ImageList_Draw( hImageList16, index, hdc, xPos, yPos, ILD_NORMAL );
}		

void ACustomUI::DrawImage24( HDC *pHdc, int xPos, int yPos, UINT index )
{
	if( pHdc ) { hdc = *pHdc; }

	ImageList_Draw( hImageList24, index, hdc, xPos, yPos, ILD_NORMAL );
}

void ACustomUI::DrawSeparator( HDC *pHdc, int xPos, int yPos, int size, bool horizontal )
{
	if( pHdc ) { hdc = *pHdc; }

	if( horizontal )
	{
		SelectObject( hdc, Pen3DShadow );

		MoveToEx( hdc, xPos, yPos, NULL );
		LineTo( hdc, xPos + size, yPos );

		SelectObject( hdc, Pen3DLight );

		MoveToEx( hdc, xPos, yPos + 1, NULL );
		LineTo( hdc, xPos + size, yPos + 1 );
	}
	else
	{
		SelectObject( hdc, Pen3DShadow );

		MoveToEx( hdc, xPos, yPos, NULL );
		LineTo( hdc, xPos, yPos + size );

		SelectObject( hdc, Pen3DLight );

		MoveToEx( hdc, xPos + 1, yPos, NULL );
		LineTo( hdc, xPos + 1, yPos + size );
	}
}

#define ES_SUNKEN	1
#define ES_NORMAL	2
#define ES_RAISED	3 

void ACustomUI::DrawStaticEdge( HDC *pHdc, int xPos, int yPos, int xSize, int ySize, UINT type )
{
	if( pHdc ) { hdc = *pHdc; }

	if( type == ES_SUNKEN )
	{
		SelectObject( hdc, Pen3DShadow ); 

		MoveToEx( hdc, xPos,			yPos, NULL );
		LineTo( hdc, xPos + xSize,	yPos );

		SelectObject( hdc, Pen3DLight ); 

		LineTo( hdc, xPos + xSize,	yPos + ySize );
		LineTo( hdc, xPos - 1,		yPos + ySize );

		SelectObject( hdc, Pen3DShadow ); 

		MoveToEx( hdc, xPos,	yPos + ySize - 1, NULL );
		LineTo( hdc, xPos,	yPos );
	}

	if( type == ES_RAISED )
	{
		SelectObject( hdc, Pen3DLight ); 

		MoveToEx( hdc, xPos,			yPos, NULL );
		LineTo( hdc, xPos + xSize,	yPos );

		SelectObject( hdc, Pen3DShadow ); 

		LineTo( hdc, xPos + xSize,	yPos + ySize );
		LineTo( hdc, xPos - 1,		yPos + ySize );

		SelectObject( hdc, Pen3DLight ); 

		MoveToEx( hdc, xPos,	yPos + ySize - 1, NULL );
		LineTo( hdc, xPos,	yPos );
	}

	if( type == ES_NORMAL )
	{
		SelectObject( hdc, Pen3DFace ); 

		MoveToEx( hdc, xPos,		yPos, NULL );
		LineTo( hdc, xPos + xSize,	yPos );
		LineTo( hdc, xPos + xSize,	yPos + ySize );
		LineTo( hdc, xPos,			yPos + ySize );
		LineTo( hdc, xPos,			yPos );
	}
}

void ACustomUI::DrawMarker( HDC *pHdc, int xPos, int yPos )
{
	if( pHdc ) { hdc = *pHdc; }

	SelectObject( hdc, Pen3DShadow );
	SelectObject( hdc, Brush3DShadow );

	POINT	triangle[3];

	triangle[0].x = xPos + 3;
	triangle[0].y = yPos;
	triangle[1].x = xPos + 3;
	triangle[1].y = yPos + 3;
	triangle[2].x = xPos;
	triangle[2].y = yPos + 3;

	Polygon( hdc, &triangle[0], 3 );
}

void ACustomUI::DrawColorSwatch( HDC *pHdc, int xPos, int yPos, int xSize, int ySize, COLORREF color, bool selected )
{
	if( pHdc ) { hdc = *pHdc; }

//Selected
	SelectObject( hdc, Pen3DShadow ); 

	MoveToEx( hdc, xPos,			yPos, NULL );
	LineTo( hdc, xPos + xSize,	yPos );

	SelectObject( hdc, Pen3DLight ); 

	LineTo( hdc, xPos + xSize,	yPos + ySize );
	LineTo( hdc, xPos - 1,		yPos + ySize );

	SelectObject( hdc, Pen3DShadow ); 

	MoveToEx( hdc, xPos,	yPos + ySize - 1, NULL );
	LineTo( hdc, xPos,	yPos );


	SelectObject( hdc, PenVoid ); 

	for( int x = xPos + 1; x < xPos + xSize; x++ )
	{
		for( int y = yPos + 1; y < yPos + ySize; y++ )
		{
			SetPixel( hdc, x, y, color );
		}
	}
		
	if( selected )
	{
		SelectObject( hdc, BrushVoid ); 
		SelectObject( hdc, PenBlack ); 
	
		Rectangle( hdc, xPos + 1, yPos + 1, xPos + xSize, yPos + ySize );
		Rectangle( hdc, xPos, yPos, xPos + xSize + 1, yPos + ySize + 1 );
	}
}

void ACustomUI::DrawASpinner( HWND hWnd, UINT state )
{
//Prepare Variables
	GetClientRect( hWnd, &Rect1 );
	GetClientRect( hWnd, &Rect2 );
	Rect1.bottom = Rect1.bottom / 2; 
	Rect2.top = Rect1.bottom; 

	POINT position;
	POINT arrow[3];
	UINT height = 2;
	UINT width = 4;

//Select Colors
	hdc = BeginPaint( hWnd, &PaintStruct );

//Draw Upper Button
	if( ( state == AS_STATE_NORMAL ) || ( state == AS_STATE_DOWN ) ) { SelectObject( hdc, Pen3DLight ); } 
	if( ( state == AS_STATE_BOTH ) || ( state == AS_STATE_UP ) ) { SelectObject( hdc, Pen3DShadow ); } 
	MoveToEx( hdc,	Rect1.right - 2,	Rect1.top, NULL );   
	LineTo( hdc,	Rect1.left,	Rect1.top );  
	LineTo( hdc,	Rect1.left,	Rect1.bottom - 1 ); 

	if( ( state == AS_STATE_NORMAL ) || ( state == AS_STATE_DOWN ) ) { SelectObject( hdc, Pen3DShadow ); } 
	if( ( state == AS_STATE_BOTH ) || ( state == AS_STATE_UP ) ) { SelectObject( hdc, Pen3DLight ); } 
	MoveToEx( hdc,	Rect1.left,	Rect1.bottom - 1, NULL );   
	LineTo( hdc,	Rect1.right - 1,	Rect1.bottom - 1 );  
	LineTo( hdc,	Rect1.right - 1,	Rect1.top - 1);  

	SelectObject( hdc, PenVoid ); 
	SelectObject( hdc, Brush3DFace ); 
	Rectangle( hdc, Rect1.left + 1, Rect1.top + 1, Rect1.right - 1, Rect1.bottom - 1 );

//Lower Button
	if( ( state == AS_STATE_NORMAL ) || ( state == AS_STATE_UP ) ) { SelectObject( hdc, Pen3DLight ); } 
	if( ( state == AS_STATE_BOTH ) || ( state == AS_STATE_DOWN ) ) { SelectObject( hdc, Pen3DShadow ); } 
	MoveToEx( hdc,	Rect2.right - 2,	Rect2.top, NULL );   
	LineTo( hdc,	Rect2.left,	Rect2.top );  
	LineTo( hdc,	Rect2.left,	Rect2.bottom - 1 ); 

	if( ( state == AS_STATE_NORMAL ) || ( state == AS_STATE_UP ) ) { SelectObject( hdc, Pen3DShadow ); } 
	if( ( state == AS_STATE_BOTH ) || ( state == AS_STATE_DOWN ) ) { SelectObject( hdc, Pen3DLight ); } 
	MoveToEx( hdc,	Rect2.left,	Rect2.bottom - 1, NULL );   
	LineTo( hdc,	Rect2.right - 1,	Rect2.bottom - 1 );  
	LineTo( hdc,	Rect2.right - 1,	Rect2.top - 1);  

	SelectObject( hdc, PenVoid ); 
	SelectObject( hdc, Brush3DFace ); 
	Rectangle( hdc, Rect2.left + 1, Rect2.top + 1, Rect2.right - 1, Rect2.bottom - 1 );

	SelectObject( hdc, PenBlack ); 
	SelectObject( hdc, BrushBlack );

//Draw Upper Arrow
	if( state == AS_STATE_UP || state == AS_STATE_BOTH )
	{
		Rect1.left += 1;
		Rect1.top += 1;
	}

	position.x = Rect1.left + ( Rect1.right / 2 );
	position.y = Rect1.top + ( Rect1.bottom / 2 ) - 1;

	arrow[0].x = position.x;
	arrow[0].y = position.y - ( height / 2 );
	arrow[1].x = position.x + ( width / 2 );
	arrow[1].y = position.y + ( height / 2 );
	arrow[2].x = position.x - ( width / 2 );
	arrow[2].y = position.y + ( height / 2 );
	Polygon( hdc, arrow, 3 );

//Draw Lower Arrow
	if( state == AS_STATE_DOWN || state == AS_STATE_BOTH )
	{
		Rect2.left += 1;
		Rect2.top += 1;
		Rect2.bottom += 1;
	}

	position.x = Rect2.left + ( Rect2.right / 2 );
	position.y = Rect2.top + ( ( Rect2.bottom - Rect2.top ) / 2 );

	arrow[0].x = position.x;
	arrow[0].y = position.y + ( height / 2 );
	arrow[1].x = position.x + ( width / 2 );
	arrow[1].y = position.y - ( height / 2 );
	arrow[2].x = position.x - ( width / 2 );
	arrow[2].y = position.y - ( height / 2 );
	Polygon( hdc, arrow, 3 );

	EndPaint( hWnd, &PaintStruct );
}


void ACustomUI::DrawADialogBar( HWND hWnd )
{
    hdc = BeginPaint( hWnd, &PaintStruct ); 

//Draw Outer Rectangle
	SelectObject( hdc, Pen3DLight ); 
	MoveToEx( hdc,	MainBar.Rect.left - 1,					MainBar.Rect.top + MainBar.Rect.bottom, NULL );   
	LineTo( hdc,	MainBar.Rect.left + MainBar.Rect.right,	MainBar.Rect.top + MainBar.Rect.bottom );  
	LineTo( hdc,	MainBar.Rect.left + MainBar.Rect.right,	MainBar.Rect.top - 2 );  

	SelectObject( hdc, Pen3DShadow ); 
	MoveToEx( hdc,	MainBar.Rect.left + MainBar.Rect.right - 1,	MainBar.Rect.top - 1, NULL );   
	LineTo( hdc,	MainBar.Rect.left - 1,						MainBar.Rect.top - 1 );  
	LineTo( hdc,	MainBar.Rect.left - 1,						MainBar.Rect.top + MainBar.Rect.bottom );  

//Draw BackGround
	SelectObject( hdc, Brush3DFace );  
	SelectObject( hdc, PenVoid ); 
	Rectangle( hdc, MainBar.Rect.left, MainBar.Rect.top, MainBar.Rect.left + MainBar.Rect.right + 1, MainBar.Rect.top + MainBar.Rect.bottom + 1 );

//Draw Inner Rectangle
	SelectObject( hdc, Pen3DShadow ); 
	MoveToEx( hdc,	MainBar.DialogMargin.left,	MainBar.DialogMargin.bottom, NULL );   
	LineTo( hdc,	MainBar.DialogMargin.right,	MainBar.DialogMargin.bottom );  
	LineTo( hdc,	MainBar.DialogMargin.right,	MainBar.DialogMargin.top);  

	SelectObject( hdc, Pen3DLight ); 
	MoveToEx( hdc,	MainBar.DialogMargin.right,	MainBar.DialogMargin.top, NULL );   
	LineTo( hdc,	MainBar.DialogMargin.left,	MainBar.DialogMargin.top );  
	LineTo( hdc,	MainBar.DialogMargin.left,	MainBar.DialogMargin.bottom ); 

	EndPaint( hWnd, &PaintStruct ); 
}

void ACustomUI::DrawAScrollBar( HWND hWnd )
{
	DeleteObject( hBitmap1 ); 

	int		Range;	
	int		Size;

	RECT	Handle;
	int		HandlePosition;
	UINT	HandleSize;

	GetClientRect( hWnd, &Rect1 );
	HandleSize = (UINT)( (double)GetWindowLong( hWnd, ScrlBarHandleSize ) / 100.0f * (double)( Rect1.bottom - Rect1.top ) ); 
	HandlePosition = GetWindowLong( hWnd, ScrlBarHandlePos ); 

	Size = ( Rect1.bottom - Rect1.top ) - HandleSize;
	Range = GetWindowLong( hWnd, ScrlBarRange ); 

  	Handle = Rect1;
	Handle.top = (long)( (double)HandlePosition * ( (double)Size / (double)Range ) ); 
	Handle.bottom = Handle.top + HandleSize;

	hdc = BeginPaint( hWnd, &PaintStruct ); 

	hMemoryDC = CreateCompatibleDC( hdc );
	hBitmap1 = CreateCompatibleBitmap( hdc, Rect1.right, Rect1.bottom );
	SelectObject( hMemoryDC, hBitmap1 );

	SelectObject( hMemoryDC, Brush3DLightShadow ); // 
	SelectObject( hMemoryDC, PenVoid ); 
	Rectangle( hMemoryDC, Rect1.left, Rect1.top, Rect1.right + 1, Rect1.bottom + 1 );

	SelectObject( hMemoryDC, Brush3DDarkShadow ); // 
	SelectObject( hMemoryDC, PenVoid ); 
	Rectangle( hMemoryDC, Handle.left + 1, Handle.top + 1, Handle.right, Handle.bottom );
	
	BitBlt( hdc, 0, 0, Rect1.right, Rect1.bottom, hMemoryDC, 0, 0, SRCCOPY );

	DeleteObject( hBitmap1 ); 
	DeleteDC( hMemoryDC ); 
	EndPaint( hWnd, &PaintStruct ); 
}


void ACustomUI::DrawATrackBar( HWND hWnd )
{
	DeleteObject( hBitmap1 ); 

	int		Range;	
	int		Size;

	RECT	Handle;
	int		HandlePosition;
	UINT	HandleSize;

	GetClientRect( hWnd, &Rect1 );
	HandleSize = (UINT)( (double)GetWindowLong( hWnd, TrackBarHandleSize ) / 100.0f * (double)( Rect1.right - Rect1.left ) ); 
	HandlePosition = GetWindowLong( hWnd, TrackBarHandlePos ); 

	Size = ( Rect1.right - Rect1.left ) - HandleSize;
	Range = GetWindowLong( hWnd, TrackBarRangeEnd ) - GetWindowLong( hWnd, TrackBarRangeStart ); 

//Calculate Handle Rect	
 	Handle.top = 1;
	Handle.left = (long)( (double)HandlePosition * ( (double)Size / (double)Range ) ); 
	Handle.bottom = Rect1.bottom - 30;
	Handle.right = Handle.left + HandleSize; 

	hdc = BeginPaint( hWnd, &PaintStruct ); 

	hMemoryDC = CreateCompatibleDC( hdc );
	hBitmap1 = CreateCompatibleBitmap( hdc, Rect1.right, Rect1.bottom );
	SelectObject( hMemoryDC, hBitmap1 );

	SelectObject( hMemoryDC, Pen3DLight );
	MoveToEx( hMemoryDC,	Rect1.left,	Rect1.bottom - 1, NULL );   
	LineTo( hMemoryDC,	Rect1.right - 1,	Rect1.bottom - 1 );  
	LineTo( hMemoryDC,	Rect1.right - 1,	Rect1.top - 1);  

	SelectObject( hMemoryDC, Pen3DShadow );
	MoveToEx( hMemoryDC,	Rect1.right - 2,	Rect1.top, NULL );   
	LineTo( hMemoryDC,	Rect1.left,	Rect1.top );  
	LineTo( hMemoryDC,	Rect1.left,	Rect1.bottom - 1 ); 

	SelectObject( hMemoryDC, Brush3DFace ); // 
	SelectObject( hMemoryDC, PenVoid ); 
	Rectangle( hMemoryDC, Rect1.left + 1, Rect1.top + 1, Rect1.right, Rect1.bottom );

//	Brush2 = CreateSolidBrush( RGB( GetRValue( Color3DShadow ) + 20, GetGValue( Color3DShadow ) + 20, GetBValue( Color3DShadow ) + 20 ) );
	SelectObject( hMemoryDC, Brush3DLightShadow ); // 
	SelectObject( hMemoryDC, Pen3DDarkShadow ); 
	Rectangle( hMemoryDC, Handle.left + 1, Handle.top + 1, Handle.right - 1, Handle.bottom );

//	Font1 = CreateFont( 14, 5, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, "Arial" );
	SelectObject( hMemoryDC, FontArialSmall );

	SetBkColor( hMemoryDC, Color3DLightShadow );
	SetTextColor( hMemoryDC, RGB( 255, 255, 255 ) );

	itoa( HandlePosition, Buffer2, 10 );
	strcpy( Buffer1, Buffer2 );
	strcat( Buffer1, "/" );
	itoa( Range, Buffer2, 10 );
	strcat( Buffer1, Buffer2 );

	Handle.top += 2;
	DrawTextEx( hMemoryDC, Buffer1, -1, &Handle, DT_CENTER | DT_VCENTER | DT_SINGLELINE, NULL );

	SelectObject( hMemoryDC, Pen3DDarkShadow ); 


	for( UINT e = 0; e < 11; e++ )
	{
		MoveToEx( hMemoryDC, ( e * Size / 10 )+ ( HandleSize / 2 ), Handle.bottom + 2, NULL );
		LineTo( hMemoryDC, ( e * Size / 10 )+ ( HandleSize / 2 ), Handle.bottom + 18 );

/*		for( UINT e = 0; e < 11; e++ )
		{*/


	}


/*	UINT			Range;	
	int				RangeStart;	
	int				RangeEnd;
	int				Size;*/


	BitBlt( hdc, 0, 0, Rect1.right, Rect1.bottom, hMemoryDC, 0, 0, SRCCOPY );

	DeleteObject( hBitmap1 ); 
	DeleteDC( hMemoryDC ); 
	EndPaint( hWnd, &PaintStruct ); 

	DeleteObject( hBitmap1 ); 
}


void ACustomUI::DrawAGroupControlButton( HWND hWnd )
{
	hdc = GetDC( hWnd ); 

	GetWindowText( hWnd, Buffer1, 64 );
	GetClientRect( hWnd, &Rect1 );

	SelectObject( hdc, Pen3DDarkShadow );
	SelectObject( hdc, Brush3DShadow );
	Rectangle( hdc, Rect1.left, Rect1.top, Rect1.right, Rect1.bottom ); 

	Rect1.right += Rect1.left;
	Rect1.bottom += Rect1.top; 

	SelectObject( hdc, FontArialMedium );

	SetBkColor( hdc, Color3DShadow );
	SetTextColor( hdc, Color3DLight );
	DrawTextEx( hdc, Buffer1, -1, &Rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, NULL );

	ReleaseDC( hWnd, hdc );
}

void ACustomUI::DrawAGroupControlUI( HWND hWnd )
{
	hdc = BeginPaint( hWnd, &PaintStruct );

	int	Height = GetWindowLong( GetParent( hWnd ), CtrlGroupHeightClosed );
	GetClientRect( hWnd, &Rect1 );

	if( GetWindowLong( GetParent( hWnd ), CtrlGroupState ) == ACGS_CLOSED )
	{
		Rect1.top = - GetWindowLong( GetParent( hWnd ), CtrlGroupHeightClosed ) / 4 + 1;
		Rect1.bottom = GetWindowLong( GetParent( hWnd ), CtrlGroupHeightClosed ) / 4 * 3;
	}

	SelectObject( hdc, Pen3DDarkShadow );

//Top
	MoveToEx( hdc,	Rect1.left,						Rect1.top + ( Height / 2 ), NULL );
	LineTo( hdc,	Rect1.right - Rect1.left - 1,	Rect1.top + ( Height / 2 ) );
//Bottom
	MoveToEx( hdc,	Rect1.left,						Rect1.bottom - 2,			NULL );
	LineTo( hdc,	Rect1.right - Rect1.left,		Rect1.bottom - 2 ); 
//Left
	MoveToEx( hdc,	Rect1.left,						Rect1.top + ( Height / 2 ), NULL );
	LineTo( hdc,	Rect1.left,						Rect1.bottom - 2 );
//Right
	MoveToEx( hdc,	Rect1.right - Rect1.left - 1,	Rect1.top + ( Height / 2 ), NULL );
	LineTo( hdc,	Rect1.right - Rect1.left - 1,	Rect1.bottom - 2 );

	EndPaint( hWnd, &PaintStruct );
}


void ACustomUI::DrawAButton( HWND hWnd, tButtonInfo *pInfo )
{
	hdc = BeginPaint( hWnd, &PaintStruct );
	GetClientRect( hWnd, &Rect1 );

	if( ( pInfo->Style & BS_CLICK ) == BS_CLICK )
	{
		if( pInfo->State == BS_PUSHED )
		{
			SelectObject( hdc, Pen3DLight );
			MoveToEx( hdc,	Rect1.left,	Rect1.bottom - 1, NULL );   
			LineTo( hdc,	Rect1.right - 1,	Rect1.bottom - 1 );  
			LineTo( hdc,	Rect1.right - 1,	Rect1.top - 1);  

			SelectObject( hdc, Pen3DShadow );
			MoveToEx( hdc,	Rect1.right - 2,	Rect1.top, NULL );   
			LineTo( hdc,	Rect1.left,	Rect1.top );  
			LineTo( hdc,	Rect1.left,	Rect1.bottom - 1 ); 
		}
		
		if( pInfo->State == BS_NORMAL )
		{
			if( ( pInfo->Style & BS_TRANSPARENT ) == BS_TRANSPARENT )
			{ 
				SelectObject( hdc, Pen3DFace );
				SelectObject( hdc, Brush3DFace );
				Rectangle( hdc, Rect1.left, Rect1.top, Rect1.right, Rect1.bottom );
			}
			else
			{ 
				SelectObject( hdc, Pen3DShadow );
				MoveToEx( hdc,	Rect1.left,	Rect1.bottom - 1, NULL );   
				LineTo( hdc,	Rect1.right - 1,	Rect1.bottom - 1 );  
				LineTo( hdc,	Rect1.right - 1,	Rect1.top - 1);  

				SelectObject( hdc, Pen3DLight );
				MoveToEx( hdc,	Rect1.right - 2,	Rect1.top, NULL );   
				LineTo( hdc,	Rect1.left,	Rect1.top );  
				LineTo( hdc,	Rect1.left,	Rect1.bottom - 1 ); 
			}
		}

		SelectObject( hdc, FontArialSmall );
		SelectObject( hdc, Brush3DFace ); 
		SelectObject( hdc, PenVoid ); 

		Rectangle( hdc, Rect1.left + 1, Rect1.top + 1, Rect1.right - 1, Rect1.bottom - 1 );

		if( pInfo->State == BS_PUSHED ) { Rect1.left += 1; Rect1.top += 1; Rect1.right += 1; Rect1.bottom += 1; }
		
		if( ( ( pInfo->Style & BS_IMAGE24 ) != BS_IMAGE24 ) && ( ( pInfo->Style & BS_IMAGE16 ) != BS_IMAGE16 ) ) 
		{
			SetBkColor( hdc, Color3DFace );
			SetTextColor( hdc, RGB( 0, 0, 0 ) );

			GetWindowText( hWnd, Buffer1, 64 );
			DrawTextEx( hdc, Buffer1, -1, &Rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, NULL );
		}
	}

	if( ( ( pInfo->Style & BS_PUSH ) == BS_PUSH ) || ( ( pInfo->Style & BS_CHECK ) == BS_CHECK ) )
	{
		if( pInfo->State == BS_PUSHED ) 
		{
			Color1 = RGB( 240, 210, 20 );
			if( ( pInfo->Style & BS_IMAGE16 ) == BS_IMAGE16 || ( pInfo->Style & BS_IMAGE24 ) == BS_IMAGE24 ) { Color1 = Color3DFace; }
//			if( GetDlgCtrlID( hWnd ) == IDC_ANIMATE ) { Color1 = RGB( 255, 0, 0 ); }
		}
		else { Color1 = Color3DFace; }

		SelectObject( hdc, FontArialSmall );
		
		if( pInfo->State == BS_PUSHED ) { SelectObject( hdc, Pen3DLight ); }
		else { SelectObject( hdc, Pen3DShadow ); if( ( pInfo->Style & BS_TRANSPARENT ) == BS_TRANSPARENT ) { SelectObject( hdc, Pen3DFace ); } }

		MoveToEx( hdc,	Rect1.left,	Rect1.bottom - 1, NULL );   
		LineTo( hdc,	Rect1.right - 1,	Rect1.bottom - 1 );  
		LineTo( hdc,	Rect1.right - 1,	Rect1.top - 1);  


		if( pInfo->State == BS_PUSHED ) { SelectObject( hdc, Pen3DShadow ); }
		else { SelectObject( hdc, Pen3DLight ); if( ( pInfo->Style & BS_TRANSPARENT ) == BS_TRANSPARENT ) { SelectObject( hdc, Pen3DFace ); } }

		MoveToEx( hdc,	Rect1.right - 2,	Rect1.top, NULL );   
		LineTo( hdc,	Rect1.left,	Rect1.top );  
		LineTo( hdc,	Rect1.left,	Rect1.bottom - 1 ); 


		if( pInfo->State == BS_PUSHED ) 
		{
			SelectObject( hdc, BrushOrange );
			if( ( pInfo->Style & BS_IMAGE16 ) == BS_IMAGE16 || ( pInfo->Style & BS_IMAGE24 ) == BS_IMAGE24 ) { SelectObject( hdc, Brush3DFace ); }
//			if( GetDlgCtrlID( hWnd ) == IDC_ANIMBAR_ANIMATE ) { SelectObject( hdc, BrushRed ); }
		}
		else { SelectObject( hdc, Brush3DFace ); } 

		SelectObject( hdc, PenVoid ); 
		Rectangle( hdc, Rect1.left + 1, Rect1.top + 1, Rect1.right, Rect1.bottom );

		if( pInfo->State == BS_PUSHED ) { Rect1.left += 1; Rect1.top += 1; Rect1.right += 1; Rect1.bottom += 1; }

		if( ( ( pInfo->Style & BS_IMAGE24 ) != BS_IMAGE24 ) && ( ( pInfo->Style & BS_IMAGE16 ) != BS_IMAGE16 ) ) 
		{
			SetBkColor( hdc, Color1 );
			SetTextColor( hdc, RGB( 0, 0, 0 ) );

			GetWindowText( hWnd, Buffer1, 64 );
			DrawTextEx( hdc, Buffer1, -1, &Rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, NULL );
		}
	}

	if( ( pInfo->Style & BS_TAB ) == BS_TAB )
	{
		if( ( pInfo->State & BS_PUSHED ) == BS_PUSHED ) { Rect1.top++; Color1 = RGB( 240, 210, 20 ); }
		if( ( pInfo->State & BS_NORMAL ) == BS_NORMAL ) { Color1 = Color3DFace; }

		SelectObject( hdc, FontArialSmall );

		SelectObject( hdc, Pen3DShadow );
		MoveToEx( hdc,	Rect1.right - 1,	Rect1.bottom - 1, NULL );  
		LineTo( hdc,	Rect1.right - 1,	Rect1.top - 1 );  

		SelectObject( hdc, Pen3DLight );
		MoveToEx( hdc,	Rect1.right - 2,	Rect1.top, NULL );   
		LineTo( hdc,	Rect1.left,	Rect1.top );  
		LineTo( hdc,	Rect1.left,	Rect1.bottom ); 

		if( pInfo->State == BS_PUSHED ) { SelectObject( hdc, BrushOrange ); }
		else { SelectObject( hdc, Brush3DFace ); }
		SelectObject( hdc, PenVoid ); 
		Rectangle( hdc, Rect1.left + 1, Rect1.top + 1, Rect1.right, Rect1.bottom + 1 );

		Rect1.top++;

		if( ( ( pInfo->Style & BS_IMAGE24 ) != BS_IMAGE24 ) && ( ( pInfo->Style & BS_IMAGE16 ) != BS_IMAGE16 ) ) 
		{
			SetBkColor( hdc, Color1 );
			SetTextColor( hdc, RGB( 0, 0, 0 ) );

			GetWindowText( hWnd, Buffer1, 64 );
			DrawTextEx( hdc, Buffer1, -1, &Rect1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, NULL );
		}

		if( pInfo->State == BS_NORMAL )
		{
			SelectObject( hdc, Pen3DLight );
			MoveToEx( hdc,	Rect1.left + 1,	Rect1.bottom - 1, NULL );   
			LineTo( hdc,	Rect1.right,	Rect1.bottom - 1 );  
		}
	}
	
	GetClientRect( hWnd, &Rect1 );

//Draw an Image into the Button 
	if( ( pInfo->Style & BS_IMAGE16 ) == BS_IMAGE16 )
	{
		SelectObject( hdc, Brush3DFace );
		SelectObject( hdc, Pen3DFace );
		Rectangle( hdc, 1, 1, Rect1.right - 1, Rect1.bottom - 1 );

		if( pInfo->SubButtonCount > 0 )
		{
			if( pInfo->State == BS_PUSHED )
			{ 
				SelectObject( hdc, BrushWhite );
				SelectObject( hdc, PenVoid ); 
				Rectangle( hdc, 1, 1, Rect1.right - 1, Rect1.bottom - 1 );

				ImageList_Draw( hImageList16, pInfo->pSubButton[pInfo->ActiveSubButton].ImageIndex, hdc, ( Rect1.right - Rect1.left ) / 2 - 7, ( Rect1.bottom - Rect1.top ) / 2 - 7, ILD_NORMAL ); 
			}
			if( pInfo->State == BS_NORMAL )
			{ 
				SelectObject( hdc, Brush3DFace );
				SelectObject( hdc, Pen3DFace );
				Rectangle( hdc, 1, 1, Rect1.right - 1, Rect1.bottom - 1 );

				ImageList_Draw( hImageList16, pInfo->pSubButton[pInfo->ActiveSubButton].ImageIndex, hdc, ( Rect1.right - Rect1.left ) / 2 - 8, ( Rect1.bottom - Rect1.top ) / 2 - 8, ILD_NORMAL ); 
			}
		}
		else
		{
			if( pInfo->State == BS_PUSHED )
			{ 
				SelectObject( hdc, BrushWhite );
				SelectObject( hdc, PenVoid ); 
				Rectangle( hdc, 1, 1, Rect1.right - 1, Rect1.bottom - 1 );

				if( pInfo->SubImageCount > 0 ) { ImageList_Draw( hImageList16, pInfo->pSubImageIndex[pInfo->ActiveSubImage], hdc, ( Rect1.right - Rect1.left ) / 2 - 7, ( Rect1.bottom - Rect1.top ) / 2 - 7, ILD_NORMAL ); }
				else
				{
					if( pInfo->HasPushedImage ) { ImageList_Draw( hImageList16, pInfo->PushedIndex, hdc, ( Rect1.right - Rect1.left ) / 2 - 7, ( Rect1.bottom - Rect1.top ) / 2 - 7, ILD_NORMAL ); }
					else { ImageList_Draw( hImageList16, pInfo->ImageIndex, hdc, ( Rect1.right - Rect1.left ) / 2 - 7, ( Rect1.bottom - Rect1.top ) / 2 - 7, ILD_NORMAL ); }
				}
			}
			if( pInfo->State == BS_NORMAL )
			{ 
				SelectObject( hdc, Brush3DFace );
				SelectObject( hdc, Pen3DFace );
				Rectangle( hdc, 1, 1, Rect1.right - 1, Rect1.bottom - 1 );

				if( pInfo->SubImageCount > 0 ) { ImageList_Draw( hImageList16, pInfo->pSubImageIndex[pInfo->ActiveSubImage], hdc, ( Rect1.right - Rect1.left ) / 2 - 8, ( Rect1.bottom - Rect1.top ) / 2 - 8, ILD_NORMAL ); }
				else { ImageList_Draw( hImageList16, pInfo->ImageIndex, hdc, ( Rect1.right - Rect1.left ) / 2 - 8, ( Rect1.bottom - Rect1.top ) / 2 - 8, ILD_NORMAL ); }
			}
		}
	}

	if( ( pInfo->Style & BS_IMAGE24 ) == BS_IMAGE24 )
	{
		if( pInfo->SubButtonCount > 0 )
		{
			if( pInfo->State == BS_PUSHED )
			{
				SelectObject( hdc, BrushWhite );
				SelectObject( hdc, PenVoid ); 
				Rectangle( hdc, 1, 1, Rect1.right - 1, Rect1.bottom - 1 );
				
				ImageList_Draw( hImageList24, pInfo->pSubButton[pInfo->ActiveSubButton].ImageIndex, hdc, ( Rect1.right - Rect1.left ) / 2 - 11, ( Rect1.bottom - Rect1.top ) / 2 - 11, ILD_NORMAL ); 
			}
			if( pInfo->State == BS_NORMAL )
			{ 
				SelectObject( hdc, Brush3DFace );
				SelectObject( hdc, Pen3DFace );
				Rectangle( hdc, 1, 1, Rect1.right - 1, Rect1.bottom - 1 );

				ImageList_Draw( hImageList24, pInfo->pSubButton[pInfo->ActiveSubButton].ImageIndex, hdc, ( Rect1.right - Rect1.left ) / 2 - 12, ( Rect1.bottom - Rect1.top ) / 2 - 12, ILD_NORMAL ); 
			}
		}
		else
		{
			if( pInfo->State == BS_PUSHED )
			{ 
				SelectObject( hdc, BrushWhite );
				SelectObject( hdc, PenVoid ); 
				Rectangle( hdc, 1, 1, Rect1.right - 1, Rect1.bottom - 1 );

				if( pInfo->SubImageCount > 0 ) { ImageList_Draw( hImageList24, pInfo->pSubImageIndex[pInfo->ActiveSubImage], hdc, ( Rect1.right - Rect1.left ) / 2 - 11, ( Rect1.bottom - Rect1.top ) / 2 - 11, ILD_NORMAL ); }
				else
				{
					if( pInfo->HasPushedImage ) { ImageList_Draw( hImageList24, pInfo->PushedIndex, hdc, ( Rect1.right - Rect1.left ) / 2 - 11, ( Rect1.bottom - Rect1.top ) / 2 - 11, ILD_NORMAL ); }
					else { ImageList_Draw( hImageList24, pInfo->ImageIndex, hdc, ( Rect1.right - Rect1.left ) / 2 - 11, ( Rect1.bottom - Rect1.top ) / 2 - 11, ILD_NORMAL ); }
				}
			}
			if( pInfo->State == BS_NORMAL )
			{ 
				SelectObject( hdc, Brush3DFace );
				SelectObject( hdc, Pen3DFace );
				Rectangle( hdc, 1, 1, Rect1.right - 1, Rect1.bottom - 1 );

				if( pInfo->SubImageCount > 0 ) { ImageList_Draw( hImageList24, pInfo->pSubImageIndex[pInfo->ActiveSubImage], hdc, ( Rect1.right - Rect1.left ) / 2 - 12, ( Rect1.bottom - Rect1.top ) / 2 - 12, ILD_NORMAL ); }
				else { ImageList_Draw( hImageList24, pInfo->ImageIndex, hdc, ( Rect1.right - Rect1.left ) / 2 - 12, ( Rect1.bottom - Rect1.top ) / 2 - 12, ILD_NORMAL ); }
			}
		}
	}

	if( ( pInfo->HighLighted ) && ( ( pInfo->State & BS_PUSHED ) != ( BS_PUSHED ) ) )
	{
		SelectObject( hdc, Pen3DShadow );
		MoveToEx( hdc,	Rect1.left,	Rect1.bottom - 1, NULL );   
		LineTo( hdc,	Rect1.right - 1,	Rect1.bottom - 1 );  
		LineTo( hdc,	Rect1.right - 1,	Rect1.top - 1);  

		SelectObject( hdc, Pen3DLight );
		MoveToEx( hdc,	Rect1.right - 2,	Rect1.top, NULL );   
		LineTo( hdc,	Rect1.left,	Rect1.top );  
		LineTo( hdc,	Rect1.left,	Rect1.bottom - 1 ); 
	}

	if( pInfo->SubButtonCount > 0 )
	{
		SelectObject( hdc, Pen3DShadow );
		SelectObject( hdc, Brush3DDarkShadow );

		GetClientRect( hWnd, &Rect1 );

//		if( pInfo->State == BS_PUSHED ) { Rect1.right++; Rect1.bottom++; }

		POINT	triangle[3];

		triangle[0].x = Rect1.right - 2;
		triangle[0].y = Rect1.bottom - 5;
		triangle[1].x = Rect1.right - 2;
		triangle[1].y = Rect1.bottom - 2;
		triangle[2].x = Rect1.right - 5;
		triangle[2].y = Rect1.bottom - 2;

		Polygon( hdc, &triangle[0], 3 );
	}

	EndPaint( hWnd, &PaintStruct );
}

LRESULT CALLBACK ADropdownMenuProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
//Parse the Message
	switch( uMsg )
	{
	case WM_CREATE:
		{
			LPCREATESTRUCT createStruct = (LPCREATESTRUCT) lParam;

			tButtonInfo *pInfo = (tButtonInfo*)createStruct->lpCreateParams;
			if( !pInfo ) { return -1; }

			SetWindowLong( hWnd, GWL_USERDATA, (LONG)pInfo );

			SetCapture( hWnd );
			return FALSE; 
		}

	case WM_PAINT:
		{
			tButtonInfo	*pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			CustomUI.hdc = BeginPaint( hWnd, &CustomUI.PaintStruct );
	
			for( int n = 0; n < (int)pInfo->SubButtonCount; n++ )
			{
				if( n == pInfo->ActiveSubButton )
				{
					SelectObject( CustomUI.hdc, CustomUI.BrushWhite );
					SelectObject( CustomUI.hdc, CustomUI.PenVoid ); 
					Rectangle( CustomUI.hdc, 0, pInfo->Size.cx * n, pInfo->Size.cx - 1, pInfo->Size.cx * n + pInfo->Size.cy - 1 );   
		
					if( ( pInfo->DropdownMenuStyle & DMS_IMAGE24 ) == DMS_IMAGE24 ) { CustomUI.DrawImage24( NULL, 3, pInfo->Size.cy * n + 3, pInfo->pSubButton[n].ImageIndex );	}
					else { CustomUI.DrawImage16( NULL, 3, pInfo->Size.cy * n + 3, pInfo->pSubButton[n].ImageIndex ); }
					CustomUI.DrawStaticEdge( NULL, 0, pInfo->Size.cy * n, pInfo->Size.cx - 1, pInfo->Size.cy - 1, ES_SUNKEN );			
					CustomUI.DrawMarker( NULL, pInfo->Size.cx - 5, pInfo->Size.cy * ( n + 1 ) - 5 );
				}
				else
				{ 
					if( ( pInfo->DropdownMenuStyle & DMS_IMAGE24 ) == DMS_IMAGE24 ) { CustomUI.DrawImage24( NULL, 2, pInfo->Size.cy * n + 2, pInfo->pSubButton[n].ImageIndex ); }
					else { CustomUI.DrawImage16( NULL, 2, pInfo->Size.cy * n + 2, pInfo->pSubButton[n].ImageIndex ); }
					CustomUI.DrawStaticEdge( NULL, 0, pInfo->Size.cy * n, pInfo->Size.cx - 1, pInfo->Size.cy - 1, ES_RAISED );
				}
			}

			EndPaint( hWnd, &CustomUI.PaintStruct );
			return FALSE;;
		}

	case WM_MOUSEMOVE:
		{
			tButtonInfo	*pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }
			
			CustomUI.Point1.x = LOWORD( lParam );
			CustomUI.Point1.y = HIWORD( lParam );

			CustomUI.Rect1.left = 0; 
			CustomUI.Rect1.right = CustomUI.Rect1.left + pInfo->Size.cx; 

			for( int n = 0; n < (int)pInfo->SubButtonCount; n++ )
			{
				CustomUI.Rect1.top = pInfo->Size.cx * n; 
				CustomUI.Rect1.bottom = CustomUI.Rect1.top + pInfo->Size.cy; 

				if( PtInRect( &CustomUI.Rect1, CustomUI.Point1 ) )
				{ 
					if( pInfo->LastSubButton != n )
					{
						SelectObject( CustomUI.hdc, CustomUI.Brush3DFace );
						SelectObject( CustomUI.hdc, CustomUI.Pen3DFace ); 
						Rectangle( CustomUI.hdc, 0, pInfo->Size.cy * pInfo->LastSubButton, pInfo->Size.cx, pInfo->Size.cy * pInfo->LastSubButton + pInfo->Size.cy );   
					
						if( ( pInfo->DropdownMenuStyle & DMS_IMAGE24 ) == DMS_IMAGE24 ) { CustomUI.DrawImage24( NULL, 2, pInfo->Size.cy * pInfo->LastSubButton + 2, pInfo->pSubButton[pInfo->LastSubButton].ImageIndex ); }
						else { CustomUI.DrawImage16( NULL, 2, pInfo->Size.cy * pInfo->LastSubButton + 2, pInfo->pSubButton[pInfo->LastSubButton].ImageIndex ); } 
						CustomUI.DrawStaticEdge( NULL, 0, pInfo->Size.cy * pInfo->LastSubButton, pInfo->Size.cx - 1, pInfo->Size.cy - 1, ES_RAISED );
						
						if( pInfo->LastSubButton == pInfo->ActiveSubButton ) { CustomUI.DrawMarker( NULL, pInfo->Size.cx - 6, pInfo->Size.cy * ( pInfo->LastSubButton + 1 ) - 6 ); }
						
						pInfo->LastSubButton = n;

						SelectObject( CustomUI.hdc, CustomUI.BrushWhite );
						SelectObject( CustomUI.hdc, CustomUI.PenVoid ); 
						Rectangle( CustomUI.hdc, 0, pInfo->Size.cy * n, pInfo->Size.cx - 1, pInfo->Size.cy * n + pInfo->Size.cy - 1 );   
			
						if( ( pInfo->DropdownMenuStyle & DMS_IMAGE24 ) == DMS_IMAGE24 ) { CustomUI.DrawImage24( NULL, 3, pInfo->Size.cy * n + 3, pInfo->pSubButton[n].ImageIndex );	}
						else { CustomUI.DrawImage16( NULL, 3, pInfo->Size.cy * n + 3, pInfo->pSubButton[n].ImageIndex ); }
						CustomUI.DrawStaticEdge( NULL, 0, pInfo->Size.cy * n, pInfo->Size.cx - 1, pInfo->Size.cy - 1, ES_SUNKEN );			

						if( n == pInfo->ActiveSubButton ) { CustomUI.DrawMarker( NULL, pInfo->Size.cx - 5, pInfo->Size.cy * ( n + 1 ) - 5 ); }
					}
				}
			}

			return FALSE;
		}

	case WM_LBUTTONUP:
		{
			tButtonInfo	*pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }
			
			CustomUI.Point1.x = LOWORD( lParam );
			CustomUI.Point1.y = HIWORD( lParam );

			CustomUI.Rect1.left = 2; 
			CustomUI.Rect1.right = CustomUI.Rect1.left + pInfo->Size.cx; 

			bool found = FALSE;

			for( UINT n = 0; n < pInfo->SubButtonCount; n++ )
			{
				CustomUI.Rect1.top = pInfo->Size.cx * n + 2; 
				CustomUI.Rect1.bottom = CustomUI.Rect1.top + pInfo->Size.cy; 

				if( PtInRect( &CustomUI.Rect1, CustomUI.Point1 ) ) { pInfo->ActiveSubButton = n; found = TRUE; }
			}

			if( found ) { SendMessage( pInfo->hParent, BN_PUSHED, pInfo->pSubButton[pInfo->ActiveSubButton].ID, (LONG)pInfo->hWnd ); } 

			SendMessage( pInfo->hWnd, WM_LBUTTONUP, 0, 0 ); 

			DestroyWindow( hWnd );

			return FALSE;
		}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

LRESULT CALLBACK AScrollBarProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	int				Range;	
	int				Size;
	POINT			Mouse;
	RECT			rect;

	RECT			Handle;
	int				HandlePosition;
	UINT			HandleSize;
	int				HandleMouseOffset;

//Parse the Message
	switch( uMsg )
	{
	case ASBM_SET_RANGE: { SetWindowLong( hWnd, ScrlBarRange, (UINT)wParam ); InvalidateRect( hWnd, NULL, FALSE ); UpdateWindow( hWnd ); return FALSE; }
	case ASBM_GET_RANGE: { return GetWindowLong( hWnd, ScrlBarRange ); }

	case ASBM_SETHANDLE_SIZE: { SetWindowLong( hWnd, ScrlBarHandleSize, (UINT)wParam ); InvalidateRect( hWnd, NULL, FALSE ); UpdateWindow( hWnd ); return FALSE; }
	case ASBM_GETHANDLE_SIZE: { return GetWindowLong( hWnd, ScrlBarHandleSize ); }

	case ASBM_SETHANDLE_POS: { SetWindowLong( hWnd, ScrlBarHandlePos, (UINT)wParam ); InvalidateRect( hWnd, NULL, FALSE ); UpdateWindow( hWnd ); return FALSE; }
	case ASBM_GETHANDLE_POS: { return GetWindowLong( hWnd, ScrlBarHandlePos ); }

	case WM_CREATE:
		{
		//Update Initial Information
			SetWindowLong( hWnd, ScrlBarRange,  100 ); 
			SetWindowLong( hWnd, ScrlBarHandleSize,  33  ); 
			SetWindowLong( hWnd, ScrlBarHandlePos,  0   ); 
			SetWindowLong( hWnd, ScrlBarHMOffset, -1  ); 
			return FALSE; 
		}

	case WM_PAINT: { CustomUI.DrawAScrollBar( hWnd ); return FALSE; }

	case WM_LBUTTONDOWN:
		{
			GetClientRect( hWnd, &rect );
			HandleSize = (UINT)( (double)GetWindowLong( hWnd, ScrlBarHandleSize ) / 100.0f * (double)( rect.bottom - rect.top + 1 ) ); 
			HandlePosition = GetWindowLong( hWnd, ScrlBarHandlePos ); 

			Size = ( rect.bottom - rect.top ) - HandleSize;
			Range = GetWindowLong( hWnd, ScrlBarRange ); 

  			Handle = rect;
			Handle.top = (long)( (double)HandlePosition * ( (double)Size / ( (double)Range + 1 ) ) ); 
			Handle.bottom = Handle.top + HandleSize;

			Mouse.x = LOWORD( lParam );
			Mouse.y = HIWORD( lParam );

			if( PtInRect( &Handle, Mouse) ) { SetWindowLong( hWnd, ScrlBarHMOffset, Mouse.y - Handle.top ); SetCapture( hWnd ); }
			else { SetWindowLong( hWnd, ScrlBarHMOffset, -1 ); }
			
			return FALSE;
		}

	case WM_LBUTTONUP:
		{
		//Release Capture + Reset Mouse to Handle Offset -> Stop MOvement
			ReleaseCapture();
			SetWindowLong( hWnd, ScrlBarHMOffset, -1 );

			return FALSE; 
		}
	case WM_MOUSEMOVE:
		{
			System.CancelHighLight();

		//Fetch Mouse Coordinates	
			Mouse.x = LOWORD( lParam );
			Mouse.y = HIWORD( lParam );

		//Set Handle Size Based on the Window Size
			GetClientRect( hWnd, &rect );
			HandleSize = (UINT)( (double)GetWindowLong( hWnd, ScrlBarHandleSize ) / 100.0f * (double)( rect.bottom - rect.top + 1 ) ); 

		//Fetch Other Variables
			HandlePosition = GetWindowLong( hWnd, ScrlBarHandlePos ); 
			Size = ( rect.bottom - rect.top ) - HandleSize;
			Range = GetWindowLong( hWnd, ScrlBarRange ); 
			HandleMouseOffset = GetWindowLong( hWnd, ScrlBarHMOffset );	

		//Compensate For Windows Bug
			if( Mouse.y < HIWORD( lParam ) || Mouse.y > rect.bottom * 4 ) { Mouse.y = 0; }
			if( Mouse.y > rect.bottom ) { Mouse.y = rect.bottom; }

		//Calculate Handle Rect	
  			Handle = rect;
			Handle.top = (long)( (double)HandlePosition * ( (double)Size / ( (double)Range + 1 ) ) ); 
			Handle.bottom = Handle.top + HandleSize;

		//Set Cursor
			if( PtInRect( &Handle, Mouse) ) { SetCursor( LoadCursor( NULL, IDC_SIZENS ) ); }
			else{ SetCursor( LoadCursor( NULL, IDC_ARROW ) ); }

			if( HandleMouseOffset > 0 )
			{
			//Handle Computation 
				if( Size ) { HandlePosition = (int)( (double)( (double)abs( Mouse.y ) - (double)HandleMouseOffset ) / (double)( (double)Size / ( (double)Range + 1 ) ) ); } 
				else { HandlePosition = 0; }

			//Range Check
				if( HandlePosition < 0 ) { HandlePosition = 0; }
				if( HandlePosition > (int)Range ) { HandlePosition = Range; }

			//Position Update
				SetWindowLong( hWnd, ScrlBarHandlePos, HandlePosition ); 

			//Window Redraw
				InvalidateRgn( hWnd, NULL, FALSE );
				UpdateWindow( hWnd );

			//Parent Notification
				PostMessage( GetParent( hWnd ), ASBN_SCROLLING, (WPARAM)hWnd, (LPARAM) HandlePosition ); 
			}
			return FALSE; 
		}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

LRESULT CALLBACK ATrackBarProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	UINT			Range;	
//	int				RangeStart;	
//	int				RangeEnd;
	int				Size;

	POINT			Mouse;
	RECT			rect;

	RECT			Handle;
	int				HandleSize;
	int				HandlePosition;
	int				HandleMouseOffset;

//Parse the Message
	switch( uMsg )
	{
	case ATBM_SET_RANGE: { SetWindowLong( hWnd, TrackBarRangeStart, (UINT)wParam ); SetWindowLong( hWnd, TrackBarRangeEnd, (UINT)lParam ); InvalidateRect( hWnd, NULL, FALSE ); UpdateWindow( hWnd ); return FALSE; }
	case ATBM_GET_RANGE: { return GetWindowLong( hWnd, TrackBarRangeEnd ) - GetWindowLong( hWnd, TrackBarRangeStart ); }

	case ATBM_SETHANDLE_SIZE: { SetWindowLong( hWnd, TrackBarHandleSize, (UINT)wParam ); InvalidateRect( hWnd, NULL, FALSE ); UpdateWindow( hWnd ); return FALSE; }
	case ASBM_GETHANDLE_SIZE: { return GetWindowLong( hWnd, TrackBarHandleSize ); }

	case ATBM_SETHANDLE_POS: { SetWindowLong( hWnd, TrackBarHandlePos, (UINT)wParam ); InvalidateRect( hWnd, NULL, FALSE ); UpdateWindow( hWnd ); return FALSE; }
	case ATBM_GETHANDLE_POS: { return GetWindowLong( hWnd, TrackBarHandlePos ); }

	case WM_CREATE:
		{
		//Update Initial Information
			SetWindowLong( hWnd, TrackBarRangeStart,  0 ); 
			SetWindowLong( hWnd, TrackBarRangeEnd,  100 ); 

			SetWindowLong( hWnd, TrackBarHandleSize,  15); 
			SetWindowLong( hWnd, TrackBarHandlePos,  0   ); 
			SetWindowLong( hWnd, TrackBarHMOffset, -1  ); 
			return FALSE; 
		}

	case WM_PAINT: { CustomUI.DrawATrackBar( hWnd ); return FALSE; }

	case WM_LBUTTONDOWN:
		{
			GetClientRect( hWnd, &rect );
			HandleSize = (UINT)( (double)GetWindowLong( hWnd, TrackBarHandleSize ) / 100.0f * (double)( rect.right - rect.left ) ); 
			HandlePosition = GetWindowLong( hWnd, TrackBarHandlePos ); 

			Size = ( rect.right - rect.left ) - HandleSize;
			Range = GetWindowLong( hWnd, TrackBarRangeEnd ) - GetWindowLong( hWnd, TrackBarRangeStart ); 

 		//Calculate Handle Rect	
 			Handle.top = 1;
			Handle.left = (long)( (double)HandlePosition * ( (double)Size / (double)Range ) ); 
			Handle.bottom = rect.bottom - 22;
			Handle.right = Handle.left + HandleSize; 

			Mouse.x = LOWORD( lParam );
			Mouse.y = HIWORD( lParam );

			if( PtInRect( &Handle, Mouse) ) { SetWindowLong( hWnd, TrackBarHMOffset, Mouse.x - Handle.left ); SetCapture( hWnd ); }
			else { SetWindowLong( hWnd, TrackBarHMOffset, -1 ); }
			
			return FALSE;
		}

	case WM_LBUTTONUP:
		{
		//Release Capture + Reset Mouse to Handle Offset -> Stop MOvement
			ReleaseCapture();
			SetWindowLong( hWnd, TrackBarHMOffset, -1 );

			return FALSE; 
		}
	case WM_MOUSEMOVE:
		{
			System.CancelHighLight();

		//Fetch Mouse Coordinates	
			Mouse.x = LOWORD( lParam );
			Mouse.y = HIWORD( lParam );

		//Set Handle Size Based on the Window Size
			GetClientRect( hWnd, &rect );
			HandleSize = (UINT)( (double)GetWindowLong( hWnd, TrackBarHandleSize ) / 100.0f * (double)( rect.right - rect.left ) ); 
//			HandleSize = (UINT)( (double)GetWindowLong( hWnd, TrackBarHandleSize ) / 100.0f * (double)( rect.bottom - rect.top ) ); 

		//Fetch Other Variables
			HandlePosition = GetWindowLong( hWnd, TrackBarHandlePos ); 
			Size = ( rect.right - rect.left ) - HandleSize;
			Range = GetWindowLong( hWnd, TrackBarRangeEnd ) - GetWindowLong( hWnd, TrackBarRangeStart ); 
			HandleMouseOffset = GetWindowLong( hWnd, TrackBarHMOffset );	

		//Compensate For Windows Bug
			if( Mouse.x < LOWORD( lParam ) || Mouse.x > rect.right * 4 )
			{ Mouse.x = 0; }
			if( Mouse.x > rect.right ) { Mouse.x = rect.right; }

/*			if( Mouse.y < HIWORD( lParam ) || Mouse.y > rect.bottom * 100 )
			{ Mouse.y = 0; }
			if( Mouse.y > rect.bottom ) { Mouse.y = rect.bottom; }*/

		//Calculate Handle Rect	
  			Handle.top = 1;
			Handle.left = (long)( (double)HandlePosition * ( (double)Size / (double)Range ) ); 
			Handle.bottom = rect.bottom - 22;
			Handle.right = Handle.left + HandleSize; 

		//Set Cursor
			if( PtInRect( &Handle, Mouse) ) { SetCursor( System.hCursorHand ); }
			else{ SetCursor( LoadCursor( NULL, IDC_ARROW ) ); }

			if( HandleMouseOffset > 0 )
			{
			//Handle Computation 
				HandlePosition = (int)( (double)( (double)abs( Mouse.x ) - (double)HandleMouseOffset ) / (double)( (double)Size / (double)Range ) ); 

			//Range Check
				if( HandlePosition < 0 ) { HandlePosition = 0; }
				if( HandlePosition > (int)Range ) { HandlePosition = Range; }

			//Position Update
				SetWindowLong( hWnd, TrackBarHandlePos, HandlePosition ); 

			//Window Redraw
				InvalidateRgn( hWnd, NULL, FALSE );
				UpdateWindow( hWnd );

			//Parent Notification
				PostMessage( GetParent( hWnd ), ATBN_SCROLLING, (WPARAM)hWnd, (LPARAM) HandlePosition ); 
			}
			return FALSE; 
		}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


LRESULT CALLBACK ADialogBarProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	ADialogBar	*pDialogBar;
	POINT	CursorPos;

//Parse the Message
	switch( uMsg )
	{
//	case WM_MOUSEMOVE: { CancelHighLight(); SetCursor( LoadCursor( NULL, IDC_ARROW ) ); break; }

	case WM_CREATE:
		{
		//Initialize Window Data
			LPCREATESTRUCT CreateStruct = (LPCREATESTRUCT) lParam;

			SetWindowLong( hWnd, GWL_USERDATA, (LONG) CreateStruct->lpCreateParams );
			return FALSE;
		}

	case DBM_DRAG_START:
		{
			pDialogBar = (ADialogBar*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pDialogBar->Dragable ) { break; }

			SetCursor( System.hCursorHand );
			GetCursorPos( &CursorPos );

		//Save Position For Starting Point
			pDialogBar->DragStartMousePos.x = CursorPos.x;
			pDialogBar->DragStartMousePos.y = CursorPos.y;

		//Back Up Relative Stating Position + Prepare for Dragging
			pDialogBar->DragStartPosition = pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].RealPos;
			pDialogBar->Dragging = TRUE;
			SetCapture( hWnd );

			return TRUE;
		}

	case WM_MOUSEMOVE:
		{ 
			System.CancelHighLight(); 
	
			pDialogBar = (ADialogBar*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pDialogBar->Dragable ) { break; }

			SetCursor( LoadCursor( NULL, IDC_ARROW ) );

			if( pDialogBar->Dragging )
			{
				SetCursor( System.hCursorHand );
				GetCursorPos( &CursorPos );
	
			//Calculate Real Scroll Range
				int	ScrollRange = pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].Height -  pDialogBar->DialogRect.bottom + pDialogBar->MarginSize + 3;
				if( ScrollRange <= 0 ) { ScrollRange = 1; }

			//Calculate Real + Relative(Percentual) Position
				pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].RealPos = pDialogBar->DragStartPosition + CursorPos.y - pDialogBar->DragStartMousePos.y;
				pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].Position = (int)( pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].RealPos * 100.0 / ScrollRange );

			//Range Checking
				if( pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].Position < -100 ) { pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].Position = -100; }
				if( pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].Position > 0 ) { pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].Position = 0; }

			//Update ScrollBar
				SendMessage( pDialogBar->GetScrollBar(), ASBM_SETHANDLE_POS, -pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].Position, 0 );

			//Update ScrollBar Handle Position + Set Dialog Position
				pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].HandlePos = -pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].Position;
				pDialogBar->SetDialogPosition( pDialogBar->GetCurrentDialog(), pDialogBar->pDialog[pDialogBar->GetCurrentDialog()].Position ); 
			}
			return FALSE; 
		}

	case WM_LBUTTONUP:
		{ 
			pDialogBar = (ADialogBar*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pDialogBar->Dragable ) { break; }

		//End Dragging	
			pDialogBar->Dragging = FALSE;
			ReleaseCapture();

			return FALSE; 
		}

	case ASBN_SCROLLING:
		{
			MainBar.SetDialogPosition( MainBar.GetCurrentDialog(), -(int)lParam );
   			return FALSE;
		}
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


LRESULT CALLBACK AControlGroupProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

//Parse the Message
	switch( uMsg )
	{
	case ACGM_GET_DLG: { return GetWindowLong( hWnd, CtrlGroupDialog ); }
	case ACGM_SET_STATE: { SetWindowLong( hWnd, CtrlGroupState, wParam ); return TRUE; }
	case ACGM_GET_STATE: { return GetWindowLong( hWnd, CtrlGroupState ); }

	case WM_CREATE:
		{ 
		//Fetch Information
			LPCREATESTRUCT CreateStruct = (LPCREATESTRUCT) lParam;
			tControlGroupInfo *Info;
			Info = (tControlGroupInfo*)CreateStruct->lpCreateParams;

		//Update Basic Information
			SetWindowLong( hWnd, CtrlGroupPosition, CreateStruct->y ); 
			SetWindowLong( hWnd, CtrlGroupState, ACGS_NORMAL ); 
			SetWindowLong( hWnd, CtrlGroupParent, (LONG)(HWND)CreateStruct->hwndParent );	

		//Create The Dialog
			if( Info->DlgProc == NULL ) { SetWindowLong( hWnd, CtrlGroupDialog, (LONG)(HWND)CreateDialog( CreateStruct->hInstance, MAKEINTRESOURCE( Info->DialogRes ), hWnd, AControlGroupDlgProc ) ); }
			else{ SetWindowLong( hWnd, CtrlGroupDialog, (LONG)(HWND)CreateDialog( CreateStruct->hInstance, MAKEINTRESOURCE( Info->DialogRes ), hWnd, Info->DlgProc ) ); }

		//Update Position Information 
			RECT tmp;
			GetClientRect( (HWND)GetWindowLong( hWnd, CtrlGroupDialog ), &tmp );
			SetWindowLong( hWnd, CtrlGroupHeightOpen, tmp.bottom ); 
			SetWindowLong( hWnd, CtrlGroupHeightClosed, 20 ); 
			SetWindowLong( hWnd, CtrlGroupWidth, tmp.right );
			SetWindowPos( (HWND)GetWindowLong( hWnd, CtrlGroupDialog ), NULL, 0, 0, tmp.right, tmp.bottom, SWP_SHOWWINDOW );

			return FALSE; 
		}

	case ACGM_OPEN:
		{
		//Fetch Information
			RECT tmp;
			GetClientRect( (HWND)GetWindowLong( hWnd, CtrlGroupDialog ), &tmp );

		//Update Position + State
			SetWindowPos( hWnd, NULL, 0, 0, tmp.right, tmp.bottom, SWP_SHOWWINDOW | SWP_NOMOVE );
			SetWindowLong( hWnd, CtrlGroupState, ACGS_NORMAL );

		//Notify Parent
			PostMessage( (HWND)GetWindowLong( hWnd, CtrlGroupParent ), ACGN_OPENED, tmp.bottom - GetWindowLong( hWnd, CtrlGroupHeightClosed ), GetWindowLong( (HWND)GetWindowLong( hWnd, CtrlGroupDialog ), GWL_ID ) );
			return FALSE; 
		}

	case ACGM_CLOSE:
		{
		//Fetch Information
			RECT tmp;
			GetClientRect( (HWND)GetWindowLong( hWnd, CtrlGroupDialog ), &tmp );

		//Update Position + State
			SetWindowPos( hWnd, NULL, 0, 0, tmp.right, GetWindowLong( hWnd, CtrlGroupHeightClosed ), SWP_SHOWWINDOW | SWP_NOMOVE );
			SetWindowLong( hWnd, CtrlGroupState, ACGS_CLOSED );

		//Notify Parent
			PostMessage( (HWND)GetWindowLong( hWnd, CtrlGroupParent ), ACGN_CLOSED, tmp.bottom - GetWindowLong( hWnd, CtrlGroupHeightClosed ), GetWindowLong( (HWND)GetWindowLong( hWnd, CtrlGroupDialog ), GWL_ID ) );
			return FALSE; 
		}

	case WM_DESTROY: { DestroyWindow( (HWND)GetWindowLong( hWnd, CtrlGroupDialog ) ); return FALSE; }
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


BOOL CALLBACK AControlGroupDlgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{

	case WM_INITDIALOG: { return FALSE; }
	case WM_MOUSEMOVE: { System.CancelHighLight(); return FALSE; }

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
					break;
				}//case BN_DBLCLK:
			}
			return FALSE; 
		}

	case BN_PUSHED: { PostMessage( (HWND)GetWindowLong( GetParent( hWnd ), CtrlGroupParent ), uMsg, wParam, lParam ); return TRUE; }
	case BN_RELEASED: { PostMessage( (HWND)GetWindowLong( GetParent( hWnd ), CtrlGroupParent ), uMsg, wParam, lParam ); return TRUE; }

	case WM_PAINT: { CustomUI.DrawAGroupControlUI( hWnd ); return FALSE; }
	}
	return FALSE;
}


LRESULT CALLBACK ASpinnerProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	tSpinnerData *pData = NULL; 
	CHAR buffer[64];

	switch( uMsg )
	{
	case WM_CREATE:
		{
		//Initialize Window Data Structure 
			pData = (tSpinnerData*)malloc( sizeof( tSpinnerData ) );

			pData->Range[0] = -100000.0f;
			pData->Range[1] =  100000.0f;
			pData->State = AS_STATE_NORMAL;
			pData->Value = 0.0f;
			pData->Step = 2.0f;
			pData->hBuddy = NULL;
			pData->Dragging = FALSE;
			pData->Pushed = FALSE;

			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(tSpinnerData*)pData );
			return TRUE;
		}

	case ASM_GET_VAL_INT:
		{
		//Fetch Window Data
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );
	
		//Fetch Buddy Value
			if( pData->hBuddy != NULL )
			{
				GetWindowText( pData->hBuddy, buffer, 128 ); 
				if( strlen( buffer ) ) { pData->Value = (float)atof( buffer ); }
			}

		//Range Checking
			if( pData->Value < pData->Range[0] ) { pData->Value = pData->Range[0]; }
			if( pData->Value > pData->Range[1] ) { pData->Value = pData->Range[1]; }

		//Update Buddy Value 
			sprintf( buffer, "%g", pData->Value );
			if( pData->hBuddy != NULL )
			{ 
				SetWindowText( pData->hBuddy, buffer );
				UpdateWindow( pData->hBuddy );
			}

			*(int*)lParam = (int)pData->Value;
		
			return TRUE;
		}

	case ASM_SET_VAL_INT:
		{
		//Fetch Window Data
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );
		
			if( lParam ) { pData->Value = (float)*(int*)lParam; }
			else { pData->Value = (float)wParam; }

		//Range Checking
			if( pData->Value < pData->Range[0] ) { pData->Value = pData->Range[0]; }
			if( pData->Value > pData->Range[1] ) { pData->Value = pData->Range[1]; }

		//Update Buddy Value 
			sprintf( buffer, "%g", pData->Value );
			if( pData->hBuddy != NULL )
			{ 
				SetWindowText( pData->hBuddy, buffer );
				UpdateWindow( pData->hBuddy );
			}

			return TRUE;
		}

	case ASM_GET_VAL_FLOAT:
		{
		//Fetch Window Data
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

		//Fetch Buddy Value
			if( pData->hBuddy != NULL )
			{
				GetWindowText( pData->hBuddy, buffer, 128 ); 
				if( strlen( buffer ) ) { pData->Value = (float)atof( buffer ); }
			}

		//Range Checking
			if( pData->Value < pData->Range[0] ) { pData->Value = pData->Range[0]; }
			if( pData->Value > pData->Range[1] ) { pData->Value = pData->Range[1]; }

		//Update Buddy Value 
			sprintf( buffer, "%g", pData->Value );
			if( pData->hBuddy != NULL )
			{ 
				SetWindowText( pData->hBuddy, buffer );
				UpdateWindow( pData->hBuddy );
			}
			
			*(float*)lParam = pData->Value;
		
			return TRUE;
		}

	case ASM_SET_VAL_FLOAT:
		{
		//Fetch Window Data
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );
		
			if( lParam ) { pData->Value = *(float*)lParam; }
			else { pData->Value = (float)wParam; }

		//Range Checking
			if( pData->Value < pData->Range[0] ) { pData->Value = pData->Range[0]; }
			if( pData->Value > pData->Range[1] ) { pData->Value = pData->Range[1]; }

		//Update Buddy Value 
			sprintf( buffer, "%g", pData->Value );
			if( pData->hBuddy != NULL )
			{ 
				SetWindowText( pData->hBuddy, buffer );
				UpdateWindow( pData->hBuddy );
			}

			return TRUE;
		}

	case ASM_UPDATE:
		{
		//Fetch Window Data
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

		//Fetch Buddy Value
			if( pData->hBuddy != NULL )
			{
				GetWindowText( pData->hBuddy, buffer, 128 ); 
				if( strlen( buffer ) ) { pData->Value = (float)atof( buffer ); }
			}

		//Range Checking
			if( pData->Value < pData->Range[0] ) { pData->Value = pData->Range[0]; }
			if( pData->Value > pData->Range[1] ) { pData->Value = pData->Range[1]; }

		//Update Buddy Value 
			sprintf( buffer, "%g", pData->Value );
			if( pData->hBuddy != NULL )
			{ 
				SetWindowText( pData->hBuddy, buffer );
				UpdateWindow( pData->hBuddy );
			}

		//Update Window Data
			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(tSpinnerData*)pData );
			return TRUE;
		}

	case WM_LBUTTONDOWN:
		{
		//Fetch Window Data
			RECT rt;
			GetClientRect( hWnd, &rt);
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

		//Fetch Buddy Value
			if( pData->hBuddy != NULL )
			{
				GetWindowText( pData->hBuddy, buffer, 128 ); 
				if( strlen( buffer ) ) { pData->Value = (float)atof( buffer ); }
			}

		//Push a Button
			if( HIWORD( lParam ) > rt.bottom / 2 ) { pData->State = AS_STATE_DOWN; pData->Value -= pData->Step; }
			else { pData->State = AS_STATE_UP; pData->Value += pData->Step; }

		//Range Checking
			if( pData->Value < pData->Range[0] ) { pData->Value = pData->Range[0]; }
			if( pData->Value > pData->Range[1] ) { pData->Value = pData->Range[1]; }

		//Update Buddy Value 
			sprintf( buffer, "%g", pData->Value );
			if( pData->hBuddy != NULL )
			{ 
				SetWindowText( pData->hBuddy, buffer );
				UpdateWindow( pData->hBuddy );
			}

		//Prepare Data For Draging
			pData->MousePosStart.x = LOWORD( lParam );
			pData->MousePosStart.y = HIWORD( lParam );
			ClientToScreen( hWnd, &pData->MousePosStart );

			pData->ValueStart = pData->Value;
			pData->Dragging = TRUE;
			SetCapture( hWnd );

		//Start Timer
			SetTimer( hWnd, 15, 300, NULL );

		//Update Window Data
			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(tSpinnerData*)pData );
			RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE );
			return TRUE;
		}

	case WM_LBUTTONUP:
		{
		//Fetch Window Data
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

		//Reset Data + Kill Timer
			pData->State = AS_STATE_NORMAL;
			pData->Dragging = FALSE;
			pData->Pushed = FALSE;
			KillTimer( hWnd, 15 );
			ReleaseCapture();

			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(tSpinnerData*)pData );
			RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE );

		//Notify Buddy
			if( pData->hBuddy != NULL )
			{
				SendMessage( pData->hBuddy, WM_KEYDOWN, 13, 0 );  
			}

			return TRUE;
		}

	case WM_TIMER:
		{
		//Fetch Window Data
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

			if( !pData->Pushed ) 
			{
				KillTimer( hWnd, 15 );
				SetTimer( hWnd, 15, 50, NULL );
				pData->Pushed = TRUE;
			}

		//Update Value
			if( pData->State == AS_STATE_DOWN ) { pData->Value -= pData->Step; }
			if( pData->State == AS_STATE_UP ) { pData->Value += pData->Step; }

		//Range Checking
			if( pData->Value < pData->Range[0] ) { pData->Value = pData->Range[0]; }
			if( pData->Value > pData->Range[1] ) { pData->Value = pData->Range[1]; }

		//Update Buddy Value 
			sprintf( buffer, "%g", pData->Value );
			if( pData->hBuddy != NULL )
			{ 
				SetWindowText( pData->hBuddy, buffer );
				UpdateWindow( pData->hBuddy );
			}

		//Update Window Data
			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(tSpinnerData*)pData );
			return TRUE;
		}

	case WM_MOUSEMOVE:
		{
			System.CancelHighLight();

		//Fetch Window Data
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

			if( ( wParam & MK_LBUTTON ) == MK_LBUTTON && pData->Dragging == TRUE )
			{
			//Update Value + Kill Timer
				GetCursorPos( &pData->MousePos );
				pData->Value = pData->ValueStart + pData->Step * ( ( pData->MousePosStart.y - pData->MousePos.y ) / 2 );
				pData->State = AS_STATE_BOTH;
				KillTimer( hWnd, 15 );

			//Range Checking
				if( pData->Value < pData->Range[0] ) { pData->Value = pData->Range[0]; }
				if( pData->Value > pData->Range[1] ) { pData->Value = pData->Range[1]; }
				
			//Update Buddy Value 
				sprintf( buffer, "%g", pData->Value );
				if( pData->hBuddy != NULL )
				{ 
					SetWindowText( pData->hBuddy, buffer );
					UpdateWindow( pData->hBuddy );
				}

			//Update Window Data
				SetWindowLong( hWnd, GWL_USERDATA, (LONG)(tSpinnerData*)pData );
				RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE );
			}
			return TRUE;
		}

	case WM_PAINT:
		{
		//Fetch Window Data
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

		//Draw Spinner
			CustomUI.DrawASpinner( hWnd, pData->State );
			return FALSE; 
		}

	case UDM_GETBUDDY:
		{ 
		//Fetch Spinner - Buddy Window Handle
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );
			return (LONG)(HWND)pData->hBuddy; 
		}

	case UDM_SETBUDDY:
		{ 
		//Set Spinner - Buddy Window Handle
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );
			pData->hBuddy = (HWND)wParam; 

			if( pData->hBuddy != NULL )
			{
				GetWindowText( pData->hBuddy, buffer, 128 ); 
				if( strlen( buffer ) ) { pData->Value = (float)atof( buffer ); }
			}

			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(tSpinnerData*)pData );
			return TRUE;
		}

	case UDM_SETRANGE:
		{
		//Set Spinner Range
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );
			
			pData->Range[0] = (float)LOWORD( lParam ); 
			pData->Range[1] = (float)HIWORD( lParam ); 

			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(tSpinnerData*)pData );
			return TRUE;
		}

	case UDM_GETRANGE:
		{
		//Get Spinner Range
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );
			return (LONG)MAKELONG( pData->Range[0], pData->Range[1] ); 
		}

	case UDM_SETACCEL:
		{
		//Set Spinner Step
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );
			pData->Step = (float)wParam; 

			SetWindowLong( hWnd, GWL_USERDATA, (LONG)(tSpinnerData*)pData );
			return TRUE;
		}

	case UDM_GETACCEL:
		{
		//Get Spinner Step
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );
			return (LONG)pData->Step; 
		}
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
//	return CallWindowProc( SpinnerProcOrigin, hWnd, uMsg, wParam, lParam );
}

LRESULT CALLBACK AEditProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{

/*	case WM_CREATE:
		{
		//Initialize Window Data
			LPCREATESTRUCT CreateStruct = (LPCREATESTRUCT) lParam;

			return FALSE;
		}

	case WM_DESTROY:
		{
		//Fetch Window Data
			return FALSE;
		}*/


	case WM_KEYDOWN:
		{
			if( (int) wParam == 13 )
			{ 
				SendMessage( hWnd, EM_SETSEL, 0, -1 );
				
				SendMessage( GetParent( hWnd ), EN_ENTER, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd );
			}
			
			break;
		}
	}

	return CallWindowProc( EditProcOrigin, hWnd, uMsg, wParam, lParam );
}


LRESULT CALLBACK AButtonProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	tButtonInfo	*pInfo;	

	switch( uMsg )
	{

	case BM_SET_DROPDOWN_STYLE:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			pInfo->DropdownMenuStyle = wParam;
			return TRUE;
		}		

	case BM_SET_TOOLTIP:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			pInfo->ShowToolTip = TRUE;
			pInfo->hToolTip = (HWND)wParam;

			return TRUE;
		}		

	case BM_ENABLE:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			pInfo->Enabled = TRUE;
			UpdateWindow( hWnd );

			return TRUE;
		}
	case BM_DISABLE:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			pInfo->Enabled = FALSE;
			UpdateWindow( hWnd );

			return TRUE;
		}

	case BM_SET_PUSHED_IMAGE:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			pInfo->PushedIndex = wParam;
			pInfo->HasPushedImage = TRUE;

			return TRUE;
		}
	case BM_SET_DISABLED_IMAGE:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			pInfo->DisabledIndex = wParam;
			pInfo->HasDisabledImage = TRUE;

			return TRUE;
		}

	case BM_SELECT_SUBIMAGE:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			pInfo->ActiveSubImage = wParam;
			UpdateWindow( hWnd );

			return TRUE;
		}

	case BM_ADD_SUBIMAGE:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }
		
			pInfo->SubImageCount++;
			pInfo->pSubImageIndex = (UINT*)realloc( pInfo->pSubImageIndex, sizeof( UINT ) * pInfo->SubImageCount );

			pInfo->pSubImageIndex[pInfo->SubImageCount - 1] = wParam;
			return TRUE;
		}

	case BM_REMOVE_SUBIMAGE:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }
		
			pInfo->SubImageCount--;
			pInfo->pSubImageIndex[wParam] = pInfo->pSubImageIndex[pInfo->SubImageCount];
			pInfo->pSubImageIndex = (UINT*)realloc( pInfo->pSubImageIndex, sizeof( UINT ) * pInfo->SubImageCount );
			
			return TRUE;
		}

	case BM_SELECT_SUBBUTTON:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			if( ( wParam >= 0 ) && ( wParam < pInfo->SubButtonCount ) )
			{
				pInfo->ActiveSubButton = wParam;
				RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE );
			}
			else { return FALSE; }

			if( lParam )
			{
				SendMessage( GetParent( hWnd ), BN_PUSHED, pInfo->pSubButton[wParam].ID, (LONG)hWnd ); 
			}

			return TRUE;
		}

	case BM_ADD_SUBBUTTON:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }
		
			pInfo->SubButtonCount++;
			pInfo->pSubButton = (tSubButtonInfo*)realloc( pInfo->pSubButton, sizeof( tSubButtonInfo ) * pInfo->SubButtonCount );

			pInfo->pSubButton[pInfo->SubButtonCount - 1].ID = wParam;
			pInfo->pSubButton[pInfo->SubButtonCount - 1].ImageIndex = lParam;

			return TRUE;
		}

	case BM_REMOVE_SUBBUTTON:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }
		
			pInfo->SubButtonCount--;
			pInfo->pSubButton[wParam] = pInfo->pSubButton[pInfo->SubButtonCount];
			pInfo->pSubButton = (tSubButtonInfo*)realloc( pInfo->pSubButton, sizeof( tSubButtonInfo ) * pInfo->SubButtonCount );
			
			return TRUE;
		}

	case BM_IS_PUSHED:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return - 1; }
		
			if( pInfo->State == BS_PUSHED ) { return TRUE; }
			
			return FALSE;
		}

	case BM_PUSH:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			if( pInfo->State == BS_NORMAL )
			{
				pInfo->State = BS_PUSHED;

				if( ( GetWindowLong( hWnd, GWL_STYLE ) & BS_OWNERDRAW ) == BS_OWNERDRAW ) { RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE ); }

				System.PressButton( hWnd );

				if( pInfo->SubButtonCount > 0 )
				{
					SendMessage( GetParent( hWnd ), BN_PUSHED, pInfo->pSubButton[pInfo->ActiveSubButton].ID, (LONG)hWnd );
				}
				else { SendMessage( GetParent( hWnd ), BN_PUSHED, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd ); }
			}

			return TRUE;
		}

	case BM_RELEASE:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			if( pInfo->State == BS_PUSHED )
			{
				pInfo->State = BS_NORMAL;

				if( ( GetWindowLong( hWnd, GWL_STYLE ) & BS_OWNERDRAW ) == BS_OWNERDRAW ) { RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE ); }

				System.ReleaseButton( hWnd );
				SendMessage( GetParent( hWnd ), BN_RELEASED, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd );
			}
			return TRUE;
		}

	case BM_HIGHLIGHT:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			pInfo->HighLighted = TRUE;

			if( ( GetWindowLong( hWnd, GWL_STYLE ) & BS_OWNERDRAW ) == BS_OWNERDRAW ) { RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE ); }

//			SendMessage( GetParent( hWnd ), BN_HIGHLIGHTED, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd );
			return TRUE;
		}

	case BM_UNHIGHLIGHT:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }

			pInfo->HighLighted = FALSE;

			if( ( GetWindowLong( hWnd, GWL_STYLE ) & BS_OWNERDRAW ) == BS_OWNERDRAW ) { RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE ); }

//			SendMessage( GetParent( hWnd ), BN_RELEASED, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd );
			return TRUE;
		}

	case WM_CREATE:
		{
		//Initialize Window Data
			LPCREATESTRUCT CreateStruct = (LPCREATESTRUCT) lParam;

			pInfo = (tButtonInfo*)malloc( sizeof( tButtonInfo ) );
		//Basic Data	
			pInfo->Style = 0x0000;
			pInfo->State = BS_NORMAL;
			pInfo->Group = 0x00000000;
			pInfo->HighLighted = FALSE;
			pInfo->Enabled = TRUE;

			pInfo->Size.cx = CreateStruct->cx;
			pInfo->Size.cy = CreateStruct->cy;
			pInfo->hParent = CreateStruct->hwndParent;
			pInfo->hWnd = hWnd;
			pInfo->ID = (UINT)CreateStruct->hMenu;
	
		//Dropdoen Menu Extension
			pInfo->DropdownMenuVisible = FALSE;
			pInfo->DropdownMenuStyle = DMS_DOWNWARD | DMS_IMAGE24;
			pInfo->ActiveSubButton = 0;
			pInfo->LastSubButton = 0;
			pInfo->pSubButton = NULL;
			pInfo->SubButtonCount = 0;

		//Extended State Images
			pInfo->HasPushedImage = FALSE;
			pInfo->PushedIndex = 0;

			pInfo->HasDisabledImage = FALSE;
			pInfo->DisabledIndex = 0;

		//ToolTip Information
			pInfo->ToolTipVisible = FALSE;
			pInfo->hToolTip = NULL;
			pInfo->ShowToolTip = FALSE;

		//SubImage Extension
			pInfo->ActiveSubImage = 0;
			pInfo->SubImageCount = 0;
			pInfo->pSubImageIndex = NULL;

			if( ( ( CreateStruct->style & BS_BITMAP ) != BS_BITMAP ) && ( ( CreateStruct->style & BS_ICON ) != BS_ICON ) && ( ( CreateStruct->style & BS_FLAT ) != BS_FLAT ) && ( ( CreateStruct->style & BS_OWNERDRAW ) != BS_OWNERDRAW ) ) { return CallWindowProc( ButtonProcOrigin, hWnd, uMsg, wParam, lParam ); }

			if( ( CreateStruct->style & BS_ICON ) == BS_ICON ) { pInfo->Style = BS_TAB; }
			if( ( CreateStruct->style & BS_FLAT ) == BS_FLAT ) { pInfo->Style = BS_PUSH; }
			if( ( ( CreateStruct->style & BS_FLAT ) != BS_FLAT ) && ( ( CreateStruct->style & BS_ICON ) != BS_ICON ) && ( ( CreateStruct->style & BS_MULTILINE ) != BS_MULTILINE ) && ( ( CreateStruct->style & BS_OWNERDRAW ) == BS_OWNERDRAW ) ) { pInfo->Style = BS_CLICK; }
			if( ( CreateStruct->style & BS_BITMAP ) == BS_BITMAP ) { pInfo->Style = BS_CHECK; }

			if( ( CreateStruct->dwExStyle & WS_EX_TRANSPARENT ) == WS_EX_TRANSPARENT ) { pInfo->Style |= BS_TRANSPARENT; }

			SetWindowLong( hWnd, GWL_USERDATA, (LONG)pInfo );
			return FALSE;
		}

	case WM_DESTROY:
		{
		//Fetch Window Data
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );

			if( pInfo )
			{ 
				if( pInfo->pSubButton )
				{
					free( pInfo->pSubButton );
				}
				free( pInfo );
			}
			return FALSE;
		}

	case WM_LBUTTONDOWN:
		{
		//Fetch Window Data
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return CallWindowProc( ButtonProcOrigin, hWnd, uMsg, wParam, lParam ); }

			if( ( ( pInfo->Style & BS_CHECK ) != BS_CHECK ) && ( ( pInfo->Style & BS_TAB ) != BS_TAB ) && ( ( pInfo->Style & BS_PUSH ) != BS_PUSH ) && ( ( pInfo->Style & BS_CLICK ) != BS_CLICK ) ) { return CallWindowProc( ButtonProcOrigin, hWnd, uMsg, wParam, lParam ); }

			pInfo->DropdownMenuVisible = TRUE;

		//Hide ToolTip
			if( pInfo->ShowToolTip )
			{
				KillTimer( hWnd, 20 );

				SendMessage( pInfo->hToolTip, TTM_HIDE, 0, 0 ); 
				pInfo->ToolTipVisible = FALSE;
				pInfo->DropdownMenuVisible = FALSE;
			}

		//Select Action
			if( ( ( pInfo->Style & BS_TAB ) == BS_TAB )		&& ( pInfo->State == BS_NORMAL ) )	{ pInfo->State = BS_PUSHED; }
	
			if( ( ( pInfo->Style & BS_PUSH ) == BS_PUSH )	&& ( pInfo->State == BS_NORMAL ) )	{ pInfo->State = BS_PUSHED; }

			if( ( pInfo->Style & BS_CLICK ) == BS_CLICK ) { pInfo->State = BS_PUSHED; }

			if( ( ( pInfo->Style & BS_CHECK ) == BS_CHECK )	) { if( ( pInfo->State & BS_NORMAL ) == BS_NORMAL )	{ pInfo->State = BS_PUSHED; } else { pInfo->State = BS_NORMAL; } }
		
		//Notify Parent
			if( pInfo->SubButtonCount > 0 )
			{
				if( pInfo->State == BS_PUSHED )
				{ 
					if( ( pInfo->Style & BS_CLICK ) != BS_CLICK ) { System.PressButton( hWnd ); }
					SendMessage( GetParent( hWnd ), BN_PUSHED, pInfo->pSubButton[pInfo->ActiveSubButton].ID, (LONG)hWnd ); 
				}
				else
				{ 
					if( ( pInfo->Style & BS_CLICK ) != BS_CLICK ) { System.ReleaseButton( hWnd ); }
					SendMessage( GetParent( hWnd ), BN_RELEASED, pInfo->pSubButton[pInfo->ActiveSubButton].ID, (LONG)hWnd ); 
				}
			}
			else
			{
				if( pInfo->State == BS_PUSHED )
				{ 
					if( ( pInfo->Style & BS_CLICK ) != BS_CLICK ) { System.PressButton( hWnd ); }
					SendMessage( GetParent( hWnd ), BN_PUSHED, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd ); 
				}
				else
				{ 
					if( ( pInfo->Style & BS_CLICK ) != BS_CLICK ) { System.ReleaseButton( hWnd ); }
					SendMessage( GetParent( hWnd ), BN_RELEASED, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd ); 
				}
			}

			SetCapture( hWnd );

		//Update Window Data
			if( ( GetWindowLong( hWnd, GWL_STYLE ) & BS_OWNERDRAW ) == BS_OWNERDRAW ) { RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE ); }
			return FALSE;
		}

	case WM_LBUTTONUP:
		{
		//Fetch Window Data
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return CallWindowProc( ButtonProcOrigin, hWnd, uMsg, wParam, lParam ); }

			if( ( ( pInfo->Style & BS_CHECK ) != BS_CHECK ) && ( ( pInfo->Style & BS_TAB ) != BS_TAB ) && ( ( pInfo->Style & BS_PUSH ) != BS_PUSH ) && ( ( pInfo->Style & BS_CLICK ) != BS_CLICK ) ) { return CallWindowProc( ButtonProcOrigin, hWnd, uMsg, wParam, lParam ); }

		//Notify Parent
			if( ( pInfo->Style & BS_CLICK ) == BS_CLICK ) { pInfo->State = BS_NORMAL; SendMessage( GetParent( hWnd ), BN_RELEASED, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd ); }
//			if( ( pInfo->Style & BS_TRANSPARENT ) == BS_TRANSPARENT ) { PostMessage( GetParent( hWnd ), BN_HIGHLIGHTED, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd ); }

			ReleaseCapture();

		//Update Window Data
			if( ( GetWindowLong( hWnd, GWL_STYLE ) & BS_OWNERDRAW ) == BS_OWNERDRAW ) { RedrawWindow( hWnd, NULL, NULL, RDW_INVALIDATE ); }
			return FALSE;
		}

	case WM_MOUSEMOVE:
		{
		//Fetch Window Data
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return CallWindowProc( ButtonProcOrigin, hWnd, uMsg, wParam, lParam ); }

			if( ( ( pInfo->Style & BS_CHECK ) != BS_CHECK ) && ( ( pInfo->Style & BS_TAB ) != BS_TAB ) && ( ( pInfo->Style & BS_PUSH ) != BS_PUSH ) && ( ( pInfo->Style & BS_CLICK ) != BS_CLICK ) ) { return CallWindowProc( ButtonProcOrigin, hWnd, uMsg, wParam, lParam ); }

		//Notify Parent
			if( ( pInfo->Style & BS_TRANSPARENT ) == BS_TRANSPARENT ) { PostMessage( GetParent( hWnd ), BN_HIGHLIGHTED, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd ); }

			if( ( ( wParam &  MK_LBUTTON ) == MK_LBUTTON ) && ( pInfo->SubButtonCount > 0 ) && pInfo->DropdownMenuVisible )
			{ 
				CustomUI.DrawADropdownMenu( hWnd, pInfo->DropdownMenuStyle ); 

				if( ( pInfo->Style & BS_CLICK ) == BS_CLICK )
				{
				//	System.ReleaseButton( hWnd );
				//	SendMessage( GetParent( hWnd ), BN_RELEASED, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd ); 
				
					if( ( pInfo->Style & BS_CLICK ) == BS_CLICK ) { pInfo->State = BS_NORMAL; SendMessage( GetParent( hWnd ), BN_RELEASED, GetWindowLong( hWnd, GWL_ID ), (LONG)hWnd ); }
				}
			}
			else
			{
				if( pInfo->ShowToolTip )
				{
					KillTimer( hWnd, 20 );
					SetTimer( hWnd, 20, 1000, NULL );

					GetCursorPos( &pInfo->LastMousePos );
				}
			}

			if( ( wParam &  MK_LBUTTON ) == MK_LBUTTON )
			{ 
				if( !pInfo->DropdownMenuVisible ) { pInfo->DropdownMenuVisible = TRUE; }

				KillTimer( hWnd, 20 ); 
			}

			return FALSE;
		}

	case WM_TIMER:
		{
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }
		
			POINT	MousePos;
			GetCursorPos( &MousePos ); 

			if( ( !pInfo->ToolTipVisible ) && ( wParam == 20 ) && ( pInfo->LastMousePos.x == MousePos.x ) && ( pInfo->LastMousePos.y == MousePos.y ) )
			{
				SendMessage( pInfo->hToolTip, TTM_SHOW, (WPARAM)hWnd, 0 );   
					
				KillTimer( hWnd, 20 );

				SetTimer( hWnd, 30, 50, NULL ); 

				pInfo->ToolTipVisible = TRUE;

//				ReleaseCapture();

				return FALSE;
			}			
			
			if( wParam == 30 ) 
			{
				RECT rt;
				
				GetWindowRect( hWnd, &rt );

				if( !PtInRect( &rt, MousePos ) )
				{
					SendMessage( pInfo->hToolTip, TTM_HIDE, 0, 0 );   

					KillTimer( hWnd, 30 );

					pInfo->ToolTipVisible = FALSE;

					return FALSE;
				}
			}

			return FALSE;
		}

	case WM_PAINT:
		{
		//Fetch Window Data
			pInfo = (tButtonInfo*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pInfo ) { return CallWindowProc( ButtonProcOrigin, hWnd, uMsg, wParam, lParam ); }

			if( ( ( pInfo->Style & BS_CHECK ) != BS_CHECK ) && ( ( pInfo->Style & BS_TAB ) != BS_TAB ) && ( ( pInfo->Style & BS_PUSH ) != BS_PUSH ) && ( ( pInfo->Style & BS_CLICK ) != BS_CLICK ) ) { return CallWindowProc( ButtonProcOrigin, hWnd, uMsg, wParam, lParam ); }

		//Draw Button
			if( ( GetWindowLong( hWnd, GWL_STYLE ) & BS_OWNERDRAW ) == BS_OWNERDRAW && pInfo->Style != 0x0000 )
			{
				CustomUI.DrawAButton( hWnd, pInfo ); 

			//Draw-ID Specific Buttons
				if( GetWindowLong( hWnd, GWL_ID ) == IDC_GROUPBUTTON ) { CustomUI.DrawAGroupControlButton( hWnd ); }

				return FALSE; 
			}
			
			return FALSE; 
		}
	}

	return CallWindowProc( ButtonProcOrigin, hWnd, uMsg, wParam, lParam );
}


LRESULT CALLBACK AToolBarProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	AToolBar	*pToolBar;	
	POINT		CursorPos;
	int			pos;

	switch( uMsg )
	{

	case TBM_DRAG_START:
		{
			pToolBar = (AToolBar*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pToolBar->Dragable ) { break; }

			SetCursor( System.hCursorHand );

			GetCursorPos( &CursorPos );
			pToolBar->DragStartMousePos.x = CursorPos.x;
			pToolBar->DragStartMousePos.y = CursorPos.y;

			pToolBar->DragStartPosition = pToolBar->GetPosition();
			pToolBar->Dragging = TRUE;
			SetCapture( hWnd );

			break;
		}

	case WM_MOUSEMOVE:
		{ 
			System.CancelHighLight(); 

			pToolBar = (AToolBar*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pToolBar || !pToolBar->Dragable ) { break; }

			SetCursor( LoadCursor( NULL, IDC_ARROW ) );
			GetCursorPos( &CursorPos );

			GetWindowRect( pToolBar->GetDialog(), &pToolBar->FullRect );
			GetWindowRect( pToolBar->GetWindow(), &pToolBar->ViewRect );

			if( pToolBar->Dragging )
			{
				SetCursor( System.hCursorHand );
				if( ( pToolBar->ViewRect.right - pToolBar->ViewRect.left ) >= ( pToolBar->FullRect.right - pToolBar->FullRect.left ) ) { break; }

				pos = pToolBar->DragStartPosition + CursorPos.x - pToolBar->DragStartMousePos.x;

				if( pos < ( ( pToolBar->ViewRect.right - pToolBar->ViewRect.left ) - ( pToolBar->FullRect.right - pToolBar->FullRect.left ) ) )
				{ 
					pos = ( pToolBar->ViewRect.right - pToolBar->ViewRect.left ) - ( pToolBar->FullRect.right - pToolBar->FullRect.left );
				}

				if( pos > 0 ) 
				{
					pos = 0;
				}

				pToolBar->SetDialogPosition( pos );
			}

			break; 
		}

	case WM_LBUTTONUP:
		{ 
			pToolBar = (AToolBar*)GetWindowLong( hWnd, GWL_USERDATA );
			if( !pToolBar->Dragable ) { break; }

			pToolBar->Dragging = FALSE;

			ReleaseCapture();

			break; 
		}

	case WM_CREATE:
		{
		//Initialize Window Data
			LPCREATESTRUCT CreateStruct = (LPCREATESTRUCT) lParam;
			SetWindowLong( hWnd, GWL_USERDATA, (LONG) CreateStruct->lpCreateParams );

			pToolBar = (AToolBar*)GetWindowLong( hWnd, GWL_USERDATA );

			return TRUE;
		}

	case WM_PAINT:
		{
			pToolBar = (AToolBar*)GetWindowLong( hWnd, GWL_USERDATA );

			CustomUI.hdc = BeginPaint( hWnd, &CustomUI.PaintStruct );

			GetClientRect( hWnd, &CustomUI.Rect1 );

			if( ( pToolBar->GetStyle() & TBS_MARGIN_TOP ) == TBS_MARGIN_TOP )
			{
				SelectObject( CustomUI.hdc, CustomUI.Pen3DShadow );
				MoveToEx( CustomUI.hdc, 0, 0, NULL );
				LineTo( CustomUI.hdc, CustomUI.Rect1.right, 0 );

				SelectObject( CustomUI.hdc, CustomUI.Pen3DLight );
				MoveToEx( CustomUI.hdc, 0, 1, NULL );
				LineTo( CustomUI.hdc, CustomUI.Rect1.right, 1 );
			}

			EndPaint( hWnd, &CustomUI.PaintStruct );

			break; 
		}
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

///////////////////////////////////////////////////////////////////	
////****			    CLASS: (AToolBar)				   ****////
///////////////////////////////////////////////////////////////////	

//Construction & Destruction
AToolBar::AToolBar()
{
//Main Variables
	hDialog = NULL;
	DialogProc = NULL;
	hWnd = NULL;
	hParent = NULL;
	Dragging = FALSE;

//Positioning Variables
	MarginLeft = 0;
	MarginRight = 0;
	MarginTop = 0;
	MarginBottom = 0;
}

AToolBar::~AToolBar() { }


//Other Member Functions

void AToolBar::Create( HWND parent, HINSTANCE hinst, UINT style )
{
	hInstance = hinst;
	hParent = parent;
	Style = style;
	MarginTop = 2;

	if( ( ( style & TBS_HSCROLL ) == TBS_HSCROLL ) || ( ( style & TBS_HSCROLL ) == TBS_HSCROLL ) ) { Dragable = TRUE; }
	else { Dragable = FALSE; }


	if( ( style & TBS_STATIC_EDGE ) == TBS_STATIC_EDGE ) { hWnd = CreateWindowEx( WS_EX_STATICEDGE, "AToolBar", "", WS_CHILD, 1, 1, 1, 1, parent, NULL, hinst, this ); }
	else { hWnd = CreateWindowEx( NULL, "AToolBar", "", WS_CHILD, 1, 1, 1, 1, parent, NULL, hinst, this ); }


	if( ( style & TBS_MARGIN_TOP ) == TBS_MARGIN_TOP ) { MarginTop = 4; }

	GetWindowRect( hWnd, &ViewRect );
}


void AToolBar::AddDialog( UINT resId, DLGPROC procedure )
{
	hDialog = CreateDialog( hInstance, MAKEINTRESOURCE( resId ), hWnd, procedure );
	
	Position = 0;
	GetWindowRect( hDialog, &FullRect );


	SetWindowPos( hDialog, NULL, MarginLeft, MarginTop, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW );
}


void AToolBar::SetDialogPosition( int position )
{
	if( ( Style & TBS_STATIC_EDGE ) == TBS_STATIC_EDGE ) { SetWindowPos( hDialog, NULL, position, 0, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW ); }
	else { SetWindowPos( hDialog, NULL, position, MarginTop, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW ); }

	Position = position;
}

void AToolBar::SetBarPosition( UINT x, UINT y, UINT width, UINT height )
{
	SetWindowPos( hWnd, NULL, x, y, width, height, SWP_SHOWWINDOW );

	GetWindowRect( hWnd, &ViewRect );
	GetWindowRect( hDialog, &FullRect );

	if( ( Style & TBS_ALIGN_LEFT ) == TBS_ALIGN_LEFT ) { SetDialogPosition( 0 ); }
	if( ( Style & TBS_ALIGN_RIGHT ) == TBS_ALIGN_RIGHT ) { SetDialogPosition( ( ViewRect.right - ViewRect.left ) - ( FullRect.right - FullRect.left ) ); }
}

HWND AToolBar::GetControl( UINT id ) { return GetDlgItem( hDialog, id ); }

HWND AToolBar::GetDialog()		{ return hDialog; }
HWND AToolBar::GetWindow()		{ return hWnd; }
UINT AToolBar::GetStyle()		{ return Style; }
int AToolBar::GetPosition()		{ return Position; }

LRESULT CALLBACK AToolTipProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_CREATE:
		{
			ShowWindow( hWnd, SW_HIDE );
			return TRUE;
		}

	case TTM_SHOW:
		{
			tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( (HWND)wParam, GWL_USERDATA );
			if( !pInfo ) { return FALSE; }


			GetCursorPos( &CustomUI.Point1 );

			CustomUI.hdc = BeginPaint( hWnd, &CustomUI.PaintStruct ); 

			char Text[32];
			
			GetWindowText( (HWND)wParam, Text, 32 );

			SelectObject( CustomUI.hdc, CustomUI.FontArialSmall );
			GetTextExtentPoint32( CustomUI.hdc, Text, strlen( Text ), &CustomUI.Size1 ); 

			CustomUI.Size1.cx += 6; 
			CustomUI.Size1.cy += 2; 
		
		//Not Finished ( Avoid Obscuring )
			
			GetWindowRect( GetParent( hWnd ), &CustomUI.Rect2 );

			if( ( CustomUI.Point1.x + CustomUI.Size1.cx ) > CustomUI.Rect2.right )
			{
				CustomUI.Point1.x = CustomUI.Rect2.right - CustomUI.Size1.cx - 10;
			}

			SetWindowPos( hWnd, HWND_TOP, CustomUI.Point1.x, CustomUI.Point1.y - CustomUI.Size1.cy, CustomUI.Size1.cx, CustomUI.Size1.cy, SWP_NOACTIVATE | SWP_SHOWWINDOW  );

			CustomUI.Rect1.left = 3;
			CustomUI.Rect1.top = 1;
			CustomUI.Rect1.right = CustomUI.Size1.cx;
			CustomUI.Rect1.bottom = CustomUI.Size1.cy;

			SelectObject( CustomUI.hdc, CustomUI.BrushToolTip );
			SelectObject( CustomUI.hdc, CustomUI.PenBlack );

			Rectangle( CustomUI.hdc, 0, 0, CustomUI.Size1.cx, CustomUI.Size1.cy );

			SetBkColor( CustomUI.hdc, RGB( 255, 255, 225 ) );
			SetTextColor( CustomUI.hdc, RGB( 0, 0, 0 ) );

			DrawTextEx( CustomUI.hdc, Text, -1, &CustomUI.Rect1, DT_LEFT, NULL );  

			EndPaint( hWnd, &CustomUI.PaintStruct ); 

			return TRUE;
		}


	case TTM_HIDE:
		{
			ShowWindow( hWnd, SW_HIDE );
			return TRUE;
		}

/*	case WM_PAINT:
		{
		//Fetch Window Data
			pData = (tSpinnerData*)(LONG)GetWindowLong( hWnd, GWL_USERDATA );

		//Draw Spinner
			CustomUI.DrawASpinner( hWnd, pData->State );
			return FALSE; 
		}*/
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}



