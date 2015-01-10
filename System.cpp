// System.cpp: implementation of the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "System.h"
#include "Shadow.h"
#include "Options.h"
#include "State.h"
#include "Afc.h"

#include "Resource.h"
#include "../System/Math.h"
#include "../System/JPEGlib.h"

extern CState				State; 
extern COptions				Options; 
extern CMath				Math; 
extern CScene				*pScene; 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSystem::CSystem()
{
	Quit = FALSE;
	Animate = FALSE;
	Update = FALSE;

	MainToolBarHeight = 0;
	BottomToolBarHeight = 0;

//	hCurrentDialog = NULL;
//	hCurrentTools = NULL;

	ViewCount = 0;
	ViewActive = 3;
	WindowState = SWS_NORMAL;
	FourViews = TRUE;

	MarginSize = 8;

	LastHighLighted = NULL;
	pPressed = NULL;
	PressedCount = 0;

	pGroup = NULL;
	GroupCount = 0;

	CurrentGroupID = 0;
	HighestGroupID = 100;

	PlaneXY[0] = 0.0f;
	PlaneXY[1] = 0.0f;
	PlaneXY[2] = 1.0f;
	PlaneXY[3] = 0.0f;

	PlaneYZ[0] = 1.0f;
	PlaneYZ[1] = 0.0f;
	PlaneYZ[2] = 0.0f;
	PlaneYZ[3] = 0.0f;

	PlaneZX[0] = 0.0f;
	PlaneZX[1] = 1.0f;
	PlaneZX[2] = 0.0f;
	PlaneZX[3] = 0.0f;
}

CSystem::~CSystem()
{

}

//////////////////////////////////////////////////////////////////////
// Member Functions
//////////////////////////////////////////////////////////////////////

bool CSystem::RegisterWndClassGL( LPCTSTR classNameGL, WNDPROC Proc )
{
	WNDCLASSEX wcGL;

//Unusefull Members
	wcGL.cbSize = sizeof(WNDCLASSEX);			
	wcGL.cbClsExtra = 0;							
	wcGL.cbWndExtra = 0;							

//Used Members 
	wcGL.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wcGL.lpfnWndProc = (WNDPROC)Proc;			
	wcGL.hInstance = hInstance;					
	wcGL.lpszClassName = classNameGL;				

	wcGL.hIcon = NULL;
	wcGL.hCursor = NULL;//LoadCursor( hInstance, MAKEINTRESOURCE( IDC_TOOL_NONE ) );
	wcGL.hbrBackground = NULL;					
	wcGL.lpszMenuName = NULL;						
	wcGL.hIconSm = NULL;							

//Check if Class can be Registered
	if( !RegisterClassEx( &wcGL ) )  
	{
		MessageBox( NULL, "CSystem::RegisterWndClassGL()", "Error A1", MB_ICONERROR );
		return FALSE;
	}

//Update our CSystem Member Variables
	ClassNameGL = classNameGL;
	WndProcGL = Proc;

	return TRUE;
}


bool CSystem::RegisterWndClassMain( LPCTSTR	className, DWORD style, WNDPROC Proc, HINSTANCE hInst )
{
	WNDCLASSEX wc;

//Unusefull Members
	wc.cbSize = sizeof(WNDCLASSEX);			
	wc.cbClsExtra = 0;							
	wc.cbWndExtra = 0;							

//Used Members 
	wc.style = CS_PARENTDC | CS_HREDRAW | CS_VREDRAW | style;	
	wc.lpfnWndProc = (WNDPROC)Proc;			
	wc.hInstance = hInst;					
	wc.lpszClassName = className;				
	wc.hIcon = LoadIcon( hInst, MAKEINTRESOURCE( IDI_SHADOW ) );
	wc.hCursor = LoadCursor( hInst, MAKEINTRESOURCE( IDC_TOOL_NONE ) );
	wc.hbrBackground = NULL;					
	wc.lpszMenuName = MAKEINTRESOURCE( IDR_MAIN_MENU );						
	wc.hIconSm = NULL;							

//Check if Class can be Registered
	if( !RegisterClassEx( &wc ) )  
	{
		MessageBox( NULL, "CSystem::RegisterWndClassGL()", "Error A1", MB_ICONERROR );
		return FALSE;
	}

//Update our CSystem Member Variables
	ClassName = className;
	ClassStyle = CS_OWNDC | style;
	WndProc = Proc;
	hInstance = hInst;

	return TRUE;
}

bool CSystem::CreateWindowMain( DWORD style, DWORD styleEx, LPSTR title )
{
//Update our CSystem Member Variables
	WndStyle = style;
	WndStyleEx = styleEx;
	
//Get the height of the Taskbar
	APPBARDATA abd;
	abd.cbSize = sizeof( APPBARDATA ); 
	abd.hWnd = hWnd;

	SHAppBarMessage( ABM_GETTASKBARPOS, &abd );
	TaskBarHeight = abd.rc.bottom - abd.rc.top;

//Make us a Window 
	hWnd = CreateWindowEx( WndStyleEx, ClassName, title, WndStyle, 0, 0, 100, 100, NULL, LoadMenu( hInstance, MAKEINTRESOURCE( IDR_MAIN_MENU ) ), hInstance, NULL );

//Check if Window was Created
	if( !hWnd ) { MessageBox( NULL, "CSystem::CreateWindowMain()", "Window Handle Not Created !", MB_ICONERROR ); return FALSE; }

//Calculate Menu Height
	State.TempRect1.bottom = 100; State.TempRect1.left = 0; State.TempRect1.right = 100; State.TempRect1.top = 0;
	AdjustWindowRect( &State.TempRect1, WndStyle, TRUE );
	MenuHeight = State.TempRect1.bottom - State.TempRect1.top - 100;

//Make Window Visible + Get Keyboard Input
	SetForegroundWindow( hWnd );
	ShowWindow( hWnd, SW_HIDE );	
	SetFocus( hWnd );
	return TRUE;
}


bool CSystem::SetPixelFormatGL( UINT colorBits, UINT depthBits, UINT stencilBits )
{
	GLuint PixelFormat;

//Initialize the pfd structure
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_GENERIC_ACCELERATED | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA,
		(BYTE)colorBits, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (BYTE)depthBits, (BYTE)stencilBits, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
	};

	for( UINT g = 0; g < ViewCount; g ++ )
	{
	//Get our Device Context Handle
		if( !View[g].hDC )
		{
			MessageBox( NULL, "CSystem::SetPixelFormatGL()", "Window Device Context Not Found !", MB_ICONERROR );
			return FALSE;
		}

	//Try to Choose the Apropriate Pixel Format
		PixelFormat = ChoosePixelFormat( View[g].hDC, &pfd ); 
		if ( !PixelFormat )
		{
			MessageBox( NULL, "CSystem::SetPixelFormatGL()", "Pixel Format Not Valid !", MB_ICONERROR );
			return FALSE;
		}

	//Try to Set the Chosen Pixel Format
		if( !SetPixelFormat( View[g].hDC, PixelFormat, &pfd ) ) 
		{
			MessageBox( NULL, "CSystem::SetPixelFormatGL()", "Pixel Format Not Applyable !", MB_ICONERROR );
			return FALSE;
		}
	}

	if( !DescribePixelFormat( View[0].hDC, PixelFormat, sizeof( PIXELFORMATDESCRIPTOR ), &pfd ) )
	{
		MessageBox( NULL, "CSystem::SetPixelFormatGL()", "Couldn't Read Applied PixelFormat !", MB_ICONERROR );
		return FALSE;
	}

	//Update our CSystem Member Variables
		ColorBits = pfd.cColorBits;
		DepthBits = pfd.cDepthBits;
		StencilBits = pfd.cStencilBits;

	return TRUE;
}


bool CSystem::CreateRenderingContext()
{
//Create our Rendering Context
	hRC = wglCreateContext( View[0].hDC );
	if ( !hRC ) 
	{
		MessageBox( NULL, "CSystem::CreateRenderingContext()\nCould Not Create Rendering Context !", "Error A1", MB_ICONERROR );
		return FALSE;
	}

//Activate our RC, ( Make it our Current )
	if ( !wglMakeCurrent( View[0].hDC, hRC ) ) 
	{
		MessageBox( NULL, "CSystem::CreateRenderingContext()\nCould Not Activate Rendering Context !", "Error A2", MB_ICONERROR );
		return FALSE;
	}
	
	int tmp;
	glGetIntegerv( GL_STENCIL_BITS, &tmp );
	StencilBits = (BYTE)tmp;

//	SetPriorityClass( GetCurrentProcess(), HIGH_PRIORITY_CLASS );
//	SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_LOWEST );
	return TRUE;
}


void CSystem::DestroyRenderingContext()
{
//Free RC Memory
	if( hRC )
	{
		if( !wglMakeCurrent( NULL, NULL ) ) //Deactivate RC
		{
			MessageBox( NULL, "CSystem::DestroyWindowGL()", "Error A1", MB_ICONERROR );
		}

		if( !wglDeleteContext( hRC ) ) //Delete RC
		{
			MessageBox( NULL, "CSystem::DestroyWindowGL()", "Error A2", MB_ICONERROR );
		}

		hRC = NULL; 
	}
}


bool CSystem::ResetRenderingContext()
{
	DestroyRenderingContext();
	
	if( !CreateRenderingContext() ) { return FALSE; }

	return TRUE;
}


void CSystem::ActivateView( UINT index )
{
	if( index == ViewActive ) { return; }

	View[ViewActive].Active = FALSE;
	View[ViewActive].NeedUpdate = TRUE;

	ViewActive = index;

	View[index].Active = TRUE;
	View[index].NeedUpdate = TRUE;
	View[index].UpdateGizmos();

	Update = TRUE;
}


void CSystem::UpdateAllViews( bool directly )
{
	if(	FourViews )
	{
		if( directly )
		{
			for( UINT e = 0; e < ViewCount; e++ )
			{ 
				if( e == ViewActive ) { continue; }
				View[e].Update( TRUE ); 
			} 
			View[ViewActive].Update( TRUE );
		}
		else
		{
			View[0].NeedUpdate = TRUE;
			View[1].NeedUpdate = TRUE;
			View[2].NeedUpdate = TRUE;
			View[3].NeedUpdate = TRUE;
		}
	}
	else 
	{ 
		if( directly ) { View[ViewActive].Update( TRUE ); }
		else { View[ViewActive].NeedUpdate = TRUE; }
	}

	if( !directly ) { Update = TRUE; }
}



void CSystem::DestroyWindowMain()
{
//Desroy All Views
	for( UINT n = 0; n < ViewCount; n++ ) { View[n].Destroy(); }

//Unregister our Window Class
	if( !UnregisterClass( ClassNameGL, hInstance ) ) 
	{
		MessageBox( NULL, "CSystem::DestroyWindowGL()", "Error B3", MB_ICONERROR );
		hInstance = NULL;
	}

	DestroyRenderingContext();

//Free Window Memory
	if( hWnd && !DestroyWindow( hWnd ) ) 
	{
		MessageBox( NULL, "CSystem::DestroyWindowMain()", "Error B2", MB_ICONERROR );
		hWnd = NULL;
	}

//Unregister our Window Class
	if( !UnregisterClass( ClassName, hInstance ) ) 
	{
		MessageBox( NULL, "CSystem::DestroyWindowMain()", "Error B3", MB_ICONERROR );
		hInstance = NULL;
	}
}


void CSystem::ResizeWindowMain()
{
//Set Window Size 
	SetWindowPos( hWnd, NULL, Position.x, Position.y, Size.x, Size.y, SWP_NOZORDER | SWP_SHOWWINDOW );

//Update Member Variables	
	Update = TRUE; 

	if( WindowState == SWS_NORMAL )
	{
		Dimension.x = Size.x - ( MainBarWidth + 8 );
		Dimension.y = Size.y - ( MainToolBarHeight + MenuHeight + TrackBarHeight + BottomToolBarHeight );
	}

	if( WindowState == SWS_MAXIMIZED )
	{
		Dimension.x = Size.x - ( MainBarWidth + 4 );
		Dimension.y = Size.y - ( MainToolBarHeight + MenuHeight + TrackBarHeight - 4 + BottomToolBarHeight );
	}

	if( FourViews )
	{
		for( UINT n = 0; n < ViewCount; n++ )
		{ 
			View[n].ResizeWindow( Dimension.x / 2, Dimension.y / 2 );
			View[n].Update( TRUE ); 
		}

		View[0].RePositionWindow( 0, MainToolBarHeight );
		View[1].RePositionWindow( Dimension.x / 2, MainToolBarHeight );
		View[2].RePositionWindow( 0, MainToolBarHeight + Dimension.y / 2 );
		View[3].RePositionWindow( Dimension.x / 2, MainToolBarHeight + Dimension.y / 2 );
	}
	else
	{
		EnlargeView( ViewActive );	
	}

	Math.AdjustDimension( Dimension );
}

void CSystem::EnlargeView( UINT index )
{
	for( UINT f = 0; f < ViewCount; f++ ) { ShowWindow( View[f].hWnd, SW_HIDE ); }

//Set Window Size 
	View[index].RePositionWindow( 0, MainToolBarHeight );
	View[index].ResizeWindow( Dimension.x, Dimension.y );
	FontGL.SetViewport( Dimension.x, Dimension.y );

	FourViews = FALSE;
	Update = TRUE;
}

void CSystem::UpdateWindowTitle()
{
	LoadString( hInstance, IDS_APP_TITLE, State.TempBuffer128a, 128 );

	if( strlen( State.CurrentFile ) )
	{
		strcat( State.TempBuffer128a, " - (" );
		strcat( State.TempBuffer128a, &State.CurrentFile[State.FileNameOffset] );
		strcat( State.TempBuffer128a, ")" );
	}

	SetWindowText( hWnd, State.TempBuffer128a );
}

void CSystem::UpdateWindowPosition()
{
	GetWindowRect( hWnd, &State.TempRect1 );

	State.TempRect1.bottom = ( State.TempRect1.bottom / 2 ) * 2;
	State.TempRect1.left = ( State.TempRect1.left / 2 ) * 2;
	State.TempRect1.right = ( State.TempRect1.right / 2 ) * 2;
	State.TempRect1.top = ( State.TempRect1.top / 2 ) * 2;		


	if( WindowState == SWS_NORMAL )
	{
		Position.x = State.TempRect1.left;
		Position.y = State.TempRect1.top;
		Size.x = State.TempRect1.right - State.TempRect1.left;
		Size.y = State.TempRect1.bottom - State.TempRect1.top;
	}

	if( WindowState == SWS_MAXIMIZED )
	{
		Position.x = State.TempRect1.left;
		Position.y = State.TempRect1.top;
		Size.x = State.TempRect1.right;
		Size.y = State.TempRect1.bottom;
	}

	if( Size.x < 640 ) { Size.x = 640; }
	if( Size.y < 480 ) { Size.y = 480; }
}



void CSystem::BuildModifierList( CHAR *path )
{





}


bool CSystem::RegisterModifier( UINT id )
{
	return TRUE;
}

void CSystem::InitPalette()
{
	int r, g, b;

	UserPaletteSize = 10;
	BasePaletteSize.x = 16;
	BasePaletteSize.y = 4;

	for( int y = 0; y < BasePaletteSize.y; y ++ )
	{
		for( int x = 0; x < BasePaletteSize.x; x ++ )
		{
			if( y == 0 && x == 0 )	{ r = 127; g = 0; b = 0; }
			if( y == 0 && x < 8 )	{ r += 16; }

			if( y == 1 && x == 0 )	{ r = 0; g = 127; b = 0; }
			if( y == 1 && x < 8 )	{ g += 16; }

			if( y == 2 && x == 0 )	{ r = 0; g = 0; b = 127; }
			if( y == 2 && x < 8 )	{ b += 16; }

			if( y == 0 && x == 8 )	{ r = 127 + 16; g = 127 + 16; b = 0; }
			if( y == 0 && x > 8 )	{ r += 16; g += 16; }

			if( y == 1 && x == 8 )	{ r = 0; g = 127 + 16; b = 127 + 16; }
			if( y == 1 && x > 8 )	{ g += 16; b += 16; }

			if( y == 2 && x == 8 )	{ r = 127 + 16; g = 0; b = 127 + 16; }
			if( y == 2 && x > 8 )	{ r += 16; b += 16; }

			if( y == 3 && x == 0 )	{ r = -1; g = -1; b = -1; }
			if( y == 3 && x < 16 )	{ r += 16; g += 16; b += 16; }

			BasePalette[x][y] = RGB( r, g, b );
		}
	}

	for( UINT x = 0; x < UserPaletteSize; x ++ )
	{
		UserPalette[x] = RGB( 200, 200, 200 );
	}

//Seed For Pseudorandom Number Generator
	srand( 218 );

	CurrentColorIndex = 400000 / rand();
	if( CurrentColorIndex > (UINT)( BasePaletteSize.x * BasePaletteSize.y - 1 ) ) { CurrentColorIndex = ( BasePaletteSize.x * BasePaletteSize.y - 1 ); }
}




void CSystem::HighLightButton( HWND hButton )
{
	if( LastHighLighted == hButton ) { return; }

	SendMessage( hButton, BM_HIGHLIGHT, 0, 0 );
	SendMessage( LastHighLighted, BM_UNHIGHLIGHT, 0, 0 );

	LastHighLighted = hButton;
}


void CSystem::CancelHighLight()
{
	if( LastHighLighted == NULL ) { return; }

	SendMessage( LastHighLighted, BM_UNHIGHLIGHT, 0, 0 );
	LastHighLighted = NULL;
}

void CSystem::PressButton( HWND hButton )
{
	if( PressedCount > 10 ) 

	{
	return;
	}

	if( !hButton ) { return; }

	tButtonInfo *pCurrentInfo = (tButtonInfo*)GetWindowLong( hButton, GWL_USERDATA );
	tButtonInfo *pStoredInfo = NULL;

	bool		Found = FALSE;

	if( PressedCount > 0 )
	{
		for( int n = 0; n < PressedCount; n++ )
		{
			pStoredInfo = (tButtonInfo*)GetWindowLong( pPressed[n], GWL_USERDATA );

			if( ( pStoredInfo ) && ( pStoredInfo->Group == pCurrentInfo->Group ) && ( hButton != pPressed[n] ) )
			{
				SendMessage( pPressed[n], BM_RELEASE, 0, 0 ); 
				pPressed[n] = hButton;

				LastGroup = pCurrentInfo->Group;

				Found = TRUE; 
			}
		}
	}

	if( !Found )
	{
		PressedCount++;
		pPressed = (HWND*)realloc( pPressed, PressedCount * sizeof( HWND ) ); 

		pPressed[PressedCount - 1] = hButton;

		hLastButton = hButton;
	}
}

void CSystem::ClearLastButton() { ReleaseButton( hLastButton ); }

void CSystem::ClearLastGroup()
{ 
	tButtonInfo *pStoredInfo = NULL;

	for( int n = 0; n < PressedCount; n++ )
	{
		pStoredInfo = (tButtonInfo*)GetWindowLong( pPressed[n], GWL_USERDATA );

		if( pStoredInfo->Group == LastGroup )
		{
			ReleaseButton( pPressed[n] ); 
		}
	}	
} 

void CSystem::ClearGroup( LPSTR groupName ) 
{
	tButtonInfo *pStoredInfo = NULL;
	bool		Found = FALSE;
	UINT		GroupID;

	for( UINT n = 0; n < GroupCount; n++ )
	{
		if( strstr( pGroup[n].Name, groupName ) ) { Found = TRUE; GroupID = pGroup[n].ID; }
	}

	if( Found ) 
	{
		for( int n = 0; n < PressedCount; n++ )
		{
			pStoredInfo = (tButtonInfo*)GetWindowLong( pPressed[n], GWL_USERDATA );

			if( pStoredInfo->Group == GroupID )
			{
				ReleaseButton( pPressed[n] ); 
			}
		}
	}

}

void CSystem::ReleaseButton( HWND hButton )
{
	tButtonInfo *pCurrentInfo = (tButtonInfo*)GetWindowLong( hButton, GWL_USERDATA );
	tButtonInfo *pStoredInfo = NULL;
	bool		Found = FALSE;

	if( PressedCount > 0 )
	{
		for( int n = 0; n < PressedCount; n++ )
		{
			pStoredInfo = (tButtonInfo*)GetWindowLong( pPressed[n], GWL_USERDATA );

			if( ( pStoredInfo ) && ( pStoredInfo->Group == pCurrentInfo->Group ) && ( hButton != pPressed[n] ) )
			{
				pPressed[n] = pPressed[PressedCount - 1];

				SendMessage( pPressed[n], BM_RELEASE, 0, 0 ); 
				Found = TRUE; 
			}
		}
	
		if( Found )
		{
			PressedCount--;
			pPressed = (HWND*)realloc( pPressed, PressedCount * sizeof( HWND ) ); 
		}
	}
}

void CSystem::SelectGroup( LPSTR groupName )
{
	bool	Found = FALSE;

	for( UINT n = 0; n < GroupCount; n++ )
	{
		if( strstr( pGroup[n].Name, groupName ) ) { CurrentGroupID = pGroup[n].ID; Found = TRUE; }
	}

	if( !Found ) 
	{
	//Icnrease Highest ID
		HighestGroupID++;

	//Resize Array
		GroupCount++;
		pGroup = (tGroupEntry*)realloc( pGroup, GroupCount * sizeof( tGroupEntry ) ); 
	
	//Assign the New Name + ID
		strcpy( pGroup[GroupCount - 1].Name, groupName );
		pGroup[GroupCount - 1].ID = HighestGroupID;

	//Select New ID as Current
		CurrentGroupID = HighestGroupID;
	}
}

void CSystem::AddButton( HWND dialog, UINT controlID )
{
	tButtonInfo *pInfo = (tButtonInfo*)GetWindowLong( GetDlgItem( dialog, controlID ), GWL_USERDATA );

	pInfo->Group = CurrentGroupID;

	SetWindowLong( GetDlgItem( dialog, controlID ), GWL_USERDATA, (LONG)pInfo );
}




