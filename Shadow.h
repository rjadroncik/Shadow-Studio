#ifndef SHADOW_STUDIO_H
#define SHADOW_STUDIO_H

#include "Resource.h"
#include "View.h"
#include "System.h"
#include "Afc.h"

//Common Include Files
#include "Timer.h"
#include "Math.h"

//RenderGL.dll Includes
#include "RenderGL/RenderGL.h"
#include "RenderGL/Scene.h"

#include "RenderGL/3sfFile.h"
#include "RenderGL/2smFile.h"

//Scene Objects
#include "RenderGL/Model.h"
#include "RenderGL/Mesh.h"
#include "RenderGL/Light.h"
#include "RenderGL/Gizmo.h"

//Base Objects
#include "RenderGL/Sphere.h"
#include "RenderGL/Plane.h"
#include "RenderGL/Box.h"
#include "RenderGL/Grid.h"
#include "RenderGL/Cone.h"

//Modifiers
#include "RenderGL/Noise.h"

//Tools
#define	TOOL_NONE					0		
#define	TOOL_SELECT					1

#define	TOOL_EDIT					60		//60..99
#define	TOOL_EDIT_MOVE				61		
#define	TOOL_EDIT_USCALE			62
#define TOOL_EDIT_NSCALE			63
#define	TOOL_EDIT_ROTATE			65

#define	TOOL_CREATE					100		//120..149
#define	TOOL_CREATE_PLANE			101	
#define	TOOL_CREATE_BOX				102	
#define	TOOL_CREATE_GRID			103		

#define	TOOL_VIEW					150		//150..169
#define	TOOL_VIEW_PAN				151		
#define TOOL_VIEW_ROTATE_FOCUS		152
#define TOOL_VIEW_ROTATE_SELECTED	153
#define TOOL_VIEW_ROTATE_SUBOBJECT	154
#define TOOL_VIEW_ROTATE_ORIGIN		155
#define	TOOL_VIEW_ZOOM				156
#define	TOOL_VIEW_ZOOM_ALL 			157
#define	TOOL_VIEW_FOV				158


#define	CT_ORIGINAL			1
#define	CT_BASE				2
#define	CT_USER				3


struct tObjectColorDlgData
{
	COLORREF	CurrentColor;
	COLORREF	OriginalColor;
	
	POINT		CurrentColorPos;
	UINT		CurrentColorType;

	UINT		ImageIndex;
};

//System Functions
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	WndProcGL( HWND, UINT, WPARAM, LPARAM );


BOOL	CALLBACK	AboutProc( HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK	ViewOptionsProc( HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK	SelectProc( HWND, UINT, WPARAM, LPARAM );


//ToolBar Functions
BOOL	CALLBACK	MainToolBarProc( HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK	AnimToolBarProc( HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK	BottomToolBarProc( HWND, UINT, WPARAM, LPARAM );

//Main DialogBar Functions 
BOOL	CALLBACK	MainDlgProc( HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK	MainTabProc( HWND, UINT, WPARAM, LPARAM );





/*********** MODIFIERS ***********/

//Modifier Related Functions
BOOL	CALLBACK	ModifierSelectProc( HWND, UINT, WPARAM, LPARAM );



/************ OBJECTS ************/

//Properties Related Functions
BOOL	CALLBACK	ObjectTransformsProc( HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK	ObjectInfoProc( HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK	ObjectColorProc( HWND, UINT, WPARAM, LPARAM );




//Creation Related Functions

BOOL	CALLBACK	CreateProc( HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK	CreateGeometryProc( HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK	CreateBoxProc( HWND, UINT, WPARAM, LPARAM );
BOOL	CALLBACK	CreatePlaneProc( HWND, UINT, WPARAM, LPARAM );

#endif