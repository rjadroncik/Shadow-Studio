// View.h: interface for the CView class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CView  
{
public:
	CView();
	virtual ~CView();

//View Creation 
	bool		Create( UINT type, UINT id, HGLRC *pRc, LPCTSTR className, HINSTANCE hInstance, HWND parent, UINT index  );
	void		Destroy();

//View Manipulation
	void		ResizeWindow( UINT width, UINT height );
	void		RePositionWindow( UINT pos_x, UINT pos_y );

	void		Update( bool force );
	void		UpdateGizmos();

	bool		Initialize();
	void		PrepareSelection();

	void		Rotate( float x, float y, float z );
	void		AbsRotate( float x, float y, float z );
	void		RePosition( float x, float y, float z, bool useFocus );
	void		Move( float x, float y, float z );
//	void		Zoom( float distance );


	void		Dolly( float distance, bool moveFocus, bool movePosition );

	void		Orbit( float x, float y, float z, Float3 *origin );

	void		ZoomExtents( bool selection );

//Drawing Functions
	void		DrawUI();
	void		DrawGrid();
	void		DrawGizmos();

	void		DrawRegion();

	void		DrawHelperTripod();


//Window Information	
	HWND		hWnd;
	HDC			hDC;
	HGLRC		*pRC;

	UINT		ID;
	RECT		Rect;
	CHAR		Name[32];


//Transformation Information
	Float3	Position;
	Float3	Rotation;
	Float3	Focus;
	float		Distance;

	Float3	Direction;
	Float3	RightVector;
	Float3	UpVector;
	Int2	Origin;

	Float4	CreationPlane;

//Rendering Information
	float		FOV;
	float		Aspect;
	float		Size;

	float		ClipNear;
	float		ClipFar;

	float		GizmoSize;
	float		OriginSize;

	bool		NeedUpdate;
	bool		Active;
	UINT		RenderMode;				//RM_WIREFRAME, RM_SHADED
	UINT		Type;					//VT_ORTHO, VT_PERSPECTIVE 
	bool		GridVisible;
};
