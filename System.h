// System.h: interface for the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEM_H__D4D3AAEC_13C6_4561_A531_F3357348819F__INCLUDED_)
#define AFX_SYSTEM_H__D4D3AAEC_13C6_4561_A531_F3357348819F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderGL/FontGL.h"
#include "View.h"
#include "Selection.h"


//Messages
#define	MSG_UPDATE_SELECTION	0x8000		//0x8000 .. 0xB000
#define	MSG_UPDATE_POSITION		0x8001
#define MSG_UPDATE				0x8002

//Studio Options
#define	SO_SELECT_INTERSECTING			0x00010000		//0x0001 0000 .. 0x1000 0000


//Rendering Modes
#define RM_WIREFRAME			1		
#define RM_SHADED				2
#define RM_COMPOSITE			3

//View Types
#define VT_ORTHO				0x1010
#define VT_PERSPECTIVE			0x1011

//View IDs
#define VIEW_LEFT				0x0001
#define VIEW_RIGHT				0x0002
#define VIEW_TOP				0x0004
#define VIEW_BOTTOM				0x0008
#define VIEW_FRONT				0x0010
#define VIEW_BACK				0x0020
#define VIEW_PERSPECTIVE		0x0040

#define VIEW_CAMERA				0x0080
#define VIEW_USER				0x0100

//Selection Data
#define	SD_POSITION			0x00000001 
#define	SD_ROTATION			0x00000010 
#define	SD_SCALE			0x00000100 

#define	SDX_INT				0x00001000 
#define	SDX_FLOAT			0x00010000 
#define	SDX_VECTOR2			0x00100000 
#define	SDX_VECTOR3			0x01000000 

//Studio Window States
#define SWS_NORMAL			1
#define SWS_MINIMIZED		2
#define SWS_MAXIMIZED		3



struct	tGroupEntry
{
	CHAR	Name[64];
	UINT	ID;
};

class CSystem
{
public:
				CSystem();
	virtual		~CSystem();

	bool		CreateWindowMain( DWORD style, DWORD styleEx, LPSTR title );
	void		DestroyWindowMain();
	void		ResizeWindowMain();
	void		UpdateWindowPosition();
	void		UpdateWindowTitle();

//View Manipulation
	void		EnlargeView( UINT index );
	void		ActivateView( UINT index );
	void		UpdateAllViews( bool directly );

	bool		SetPixelFormatGL( UINT colorBits, UINT depthBits, UINT stencilBits );
	bool		RegisterWndClassGL( LPCTSTR classNameGL, WNDPROC Proc );
	bool		RegisterWndClassMain( LPCTSTR className, DWORD style, WNDPROC Proc, HINSTANCE hInst );

	bool		CreateRenderingContext();
	void		DestroyRenderingContext();
	bool		ResetRenderingContext();

	void		BuildModifierList( CHAR *path );
	bool		RegisterModifier( UINT id );

	void		InitPalette();

	void		HighLightButton( HWND hButton );
	void		CancelHighLight();
	void		PressButton( HWND hButton );
	void		ReleaseButton( HWND hButton );

	void		AddButton( HWND dialog, UINT controlID );
	void		SelectGroup( LPSTR groupName );
	void		ClearLastButton();
	void		ClearGroup( LPSTR groupName ); 
	void		ClearLastGroup(); 


//State Variables
	bool		Quit;
	bool		Animate;
	bool		Update;
	
//General Win32 Variables
	MSG			Msg;
	HINSTANCE	hInstance;
	HWND		hWnd;

//View Information
	CView		View[4];
	UINT		ViewMode;
	UINT		ViewActive;
	UINT		ViewCount;
	HGLRC		hRC;
	bool		FourViews;

//Position, Size and Dimension of the Main Window
	POINT		Position;
	POINT		Size;
	POINT		Dimension;
	UINT		WindowState;

//Handles to System Dialogs
//	HWND		hCurrentTools;
//	HWND		hCurrentDialog;
	HWND		hTrackBar;
	UINT		TrackBarID;

//Offsets Used to Calculate View Size
	UINT		MainToolBarHeight;
	UINT		BottomToolBarHeight;
	UINT		MenuHeight;
	UINT		TaskBarHeight;
	UINT		TrackBarHeight;
	UINT		MainBarWidth; 
	UINT		MarginSize;

//Display Information
	BYTE		ColorBits;
	BYTE		DepthBits;
	BYTE		StencilBits;

	CHAR		WorkingDirectory[256];
	RECT		ScreenRect;	
	
//UI Variables
	COLORREF	BasePalette[16][4];
	POINT		BasePaletteSize;
	COLORREF	UserPalette[10];
	UINT		UserPaletteSize;

	UINT		CurrentColorIndex;

	HWND		LastHighLighted;
	HWND		*pPressed;
	int			PressedCount;
	UINT		LastGroup;

	HCURSOR		hCursorHand;
	HCURSOR		hCursorNone;
	HCURSOR		hCursorCross;

	HACCEL		hAccelTable;

	PAINTSTRUCT	ps;

//Font Variables
	CFontGL		FontGL;
	HDC			hDC;

	CSelection	Selection;

	HWND		hToolTip;
			
//Window Class Names
	LPCTSTR		ClassName;
	LPCTSTR		ClassNameGL;


	const unsigned char		*OpenGLVersion;
	const unsigned char		*OpenGLExtensions;

//Main Planes
	Float4	PlaneXY;
	Float4	PlaneYZ;
	Float4	PlaneZX;

private:

//Window Procedures
	WNDPROC		WndProc;
	WNDPROC		WndProcGL;


//Window Styles
	DWORD		WndStyle;
	DWORD		WndStyleEx;
	DWORD		ClassStyle;

	tGroupEntry	*pGroup;
	UINT		GroupCount;

	HWND		hLastButton;

	UINT		CurrentGroupID;
	UINT		HighestGroupID;
};

#endif // !defined(AFX_SYSTEM_H__D4D3AAEC_13C6_4561_A531_F3357348819F__INCLUDED_)
