// Options.cpp: implementation of the COptions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Options.h"
#include "System.h"

extern CSystem		System;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptions::COptions()
{

}

COptions::~COptions()
{

}



void COptions::Save( HINSTANCE hInstance )
{
	FILE	*pFile = NULL;

	char	TempBurffer128[128];

	strcpy( TempBurffer128, System.WorkingDirectory );
	strcat( TempBurffer128, "Data//Studio.cfg" );

	pFile = fopen( TempBurffer128, "wt" );

	if( !pFile ) { MessageBox( System.hWnd, "Couldn't Open Configuration pFile ! \n Can't Save Settings.", "COptions::Save()", MB_ICONERROR ); return; } 

	char	TempBurffer64[64];
	char	TempBurffer32[32];
	char	TempBurffer16[16];

//Update Studio Version
	LoadString( hInstance, IDS_APP_NAME, TempBurffer32, 32 );
	LoadString( hInstance, IDS_APP_VERSION, TempBurffer16, 16 );

	strcpy( TempBurffer64, "[" );
	strcat( TempBurffer64, TempBurffer32 );
	strcat( TempBurffer64, " v" );
	strcat( TempBurffer64, TempBurffer16 );
	strcat( TempBurffer64, "]" );

	fprintf( pFile, "%s", TempBurffer64 );
	fprintf( pFile, "%s", "\n\n" );

//Save Viewport Options
	fprintf( pFile, "%s", "[Viewport Options]\n\n" );

	fprintf( pFile, "%s", "Update_OnDialogMove " );	fprintf( pFile, "%i", (int)UpdateOnDialogMove );	fprintf( pFile, "%s", "\n" );
	fprintf( pFile, "%s", "View_Distance " );		fprintf( pFile, "%g", (float)ViewDistance );		fprintf( pFile, "%s", "\n\n\n" );


//Save OpenGL Options
	fprintf( pFile, "%s", "[OpenGL Options]\n\n" ); 

	fprintf( pFile, "%s", "Color_Bits " );		fprintf( pFile, "%i", ColorBits );		fprintf( pFile, "%s", "\n" );
	fprintf( pFile, "%s", "Depth_Bits " );		fprintf( pFile, "%i", DepthBits );		fprintf( pFile, "%s", "\n" );
	fprintf( pFile, "%s", "Stencil_Bits " );	fprintf( pFile, "%i", StencilBits );	fprintf( pFile, "%s", "\n\n\n" );


//Save Window Options
	fprintf( pFile, "%s", "[Window Options]\n\n" ); 

	RECT rt;
	GetWindowRect( System.hWnd, &rt );	

	fprintf( pFile, "%s", "Window_State " );	if( System.WindowState == SWS_NORMAL ) { fprintf( pFile, "%s", "NORMAL" ); } else { fprintf( pFile, "%s", "MAXIMIZED" ); } fprintf( pFile, "%s", "\n" );
	fprintf( pFile, "%s", "Window_Rect " );		fprintf( pFile, "%i ", rt.left );	fprintf( pFile, "%i ", rt.top );
												fprintf( pFile, "%i ", rt.right );	fprintf( pFile, "%i ", rt.bottom );	fprintf( pFile, "%s", "\n\n\n" );
//Save UI Options
	fprintf( pFile, "%s", "[UI Options]\n\n" ); 
	
	fprintf( pFile, "%s", "Gizmo_Size " );		fprintf( pFile, "%g", (float)GizmoSize );	fprintf( pFile, "%s", "\n" );
	fprintf( pFile, "%s", "Draw_Origins " );	fprintf( pFile, "%i", (int)DrawOrigins );	fprintf( pFile, "%s", "\n\n\n" );

	fclose( pFile );
}

void COptions::Reset()
{
	SelectIntersecting = TRUE;
	UpdateOnDialogMove = TRUE;
	ViewDistance = 100000;
	UseFlashLight = TRUE;

	ColorBits = 16;
	DepthBits = 16;
	StencilBits = 8;

	WindowState = SWS_MAXIMIZED;

	MoveCenterType		= CENTER_SELECTION;
	RotateCenterType	= CENTER_SELECTION;
	ScaleCenterType		= CENTER_SELECTION;

	CenterType			= CENTER_SELECTION;

	LockSelection = FALSE;

	GizmoSize = 1.0f;
	DrawOrigins = FALSE;
}

void COptions::Load( HINSTANCE hInstance )
{
	FILE	*pFile = NULL;

	pFile = fopen( "Data//Studio.cfg", "rt" );

	char	Option[64];
	char	TempBurffer64[64];
	char	TempBurffer32[32];
	char	TempBurffer16[16];

	int		IntValue;
	float	FloatValue;

	bool	ResetOptions = FALSE;

//Reseting to Defaults
	Reset();

//Check for Studio Version
	LoadString( hInstance, IDS_APP_NAME, TempBurffer32, 32 );
	LoadString( hInstance, IDS_APP_VERSION, TempBurffer16, 16 );

	strcpy( TempBurffer64, "[" );
	strcat( TempBurffer64, TempBurffer32 );
	strcat( TempBurffer64, " v" );
	strcat( TempBurffer64, TempBurffer16 );
	strcat( TempBurffer64, "]" );

	fread( Option, sizeof( BYTE ), 24, pFile );
	if( !strstr( Option, TempBurffer64 ) ) { ResetOptions = TRUE; }

	if( pFile )
	{
		while ( fscanf( pFile, "%s", Option ) != EOF )
		{
		//Load Viewport Options
			if( strstr( "View_Distance", Option ) )			{ fscanf( pFile, "%f", &FloatValue ); ViewDistance = FloatValue; }
			if( strstr( "Update_OnDialogMove", Option ) )	{ fscanf( pFile, "%i", &IntValue ); if( IntValue == 0) { UpdateOnDialogMove = FALSE; } else { UpdateOnDialogMove = TRUE; } }

		//Load OpenGL Options
			if( strstr( "Color_Bits", Option ) )	{ fscanf( pFile, "%i", &ColorBits ); }
			if( strstr( "Depth_Bits", Option ) )	{ fscanf( pFile, "%i", &DepthBits ); }
			if( strstr( "Stencil_Bits", Option ) )	{ fscanf( pFile, "%i", &StencilBits ); }

		//Load Window Options
			if( strstr( "Window_State", Option ) )
			{
				fscanf( pFile, "%s", Option );  
				if( strstr( "NORMAL", Option ) )
				{ 
					WindowState = SWS_NORMAL; 

					fscanf( pFile, "%s", Option );  
					if( strstr( "Window_Rect", Option ) )
					{
						fscanf( pFile, "%i", &WindowRect.left );
						fscanf( pFile, "%i", &WindowRect.top );
						fscanf( pFile, "%i", &WindowRect.right );
						fscanf( pFile, "%i", &WindowRect.bottom );
					}
				} 
				else { WindowState = SWS_MAXIMIZED; }	
			}

		//Load UI Options
			if( strstr( "Gizmo_Size", Option ) )	{ fscanf( pFile, "%f", &FloatValue ); GizmoSize = FloatValue; }
			if( strstr( "Draw_Origins", Option ) )	{ fscanf( pFile, "%i", &IntValue ); if( IntValue == 0) { DrawOrigins = FALSE; } else { DrawOrigins = TRUE; } }
		}
	}
	else 
	{
	//Write Warning if CFG Not Present
		MessageBox( System.hWnd, "Couldn't Open Configuration pFile ! \n Reseting to Defaults.", "COptions::Load()", MB_ICONERROR ); 
		Save( hInstance );
	} 

	if(  ResetOptions )  
	{
	//Write Warning if CFG Corrupt
		MessageBox( System.hWnd, "Configuration pFile Corrupt ! \n Reseting to Defaults.", "COptions::Load()", MB_ICONERROR ); 
		Save( hInstance );
	}

	if( pFile ) { fclose( pFile ); }
}
