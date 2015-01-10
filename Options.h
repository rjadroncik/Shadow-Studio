// Options.h: interface for the COptions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONS_H__8217DC27_3F9B_46C1_A825_0BCA56864AE3__INCLUDED_)
#define AFX_OPTIONS_H__8217DC27_3F9B_46C1_A825_0BCA56864AE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CENTER_ORIGIN		0
#define CENTER_SELECTION	1
#define CENTER_SYSTEM		2

struct tSelectDialogData
{
	UINT	SortType;

	UINT	ListGeometry;
	UINT	ListShapes;
	UINT	ListLights;
	UINT	ListCameras;
	UINT	ListHelpers;
	UINT	ListWarps;
	UINT	ListGroups;
	UINT	ListBones;

	UINT	DisplaySubTree;	
	UINT	CaseSensitive;	
};


class COptions  
{
public:
	COptions();
	virtual ~COptions();

	void	Load( HINSTANCE hInstance );
	void	Save( HINSTANCE hInstance );

	void	Reset();

//Select Dialog Options
	tSelectDialogData	SelectDialog;

//Viewport Options
	bool	SelectIntersecting;
	bool	UseFlashLight;

	bool	UpdateOnDialogMove;
	float	ViewDistance;

//OpenGL Options
	int		ColorBits;
	int		DepthBits;
	int		StencilBits;

//Window Options
	RECT	WindowRect;
	WORD	WindowState;

//Edit Options
	WORD	MoveCenterType;
	WORD	RotateCenterType;
	WORD	ScaleCenterType;

	WORD	CenterType;

	bool	LockSelection;

//UI Options
	float	GizmoSize;
	bool	DrawOrigins;
};

#endif // !defined(AFX_OPTIONS_H__8217DC27_3F9B_46C1_A825_0BCA56864AE3__INCLUDED_)
