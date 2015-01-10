// Selection.h: interface for the CSelection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTION_H__98C27905_2081_4658_872A_2B928543F90E__INCLUDED_)
#define AFX_SELECTION_H__98C27905_2081_4658_872A_2B928543F90E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderGL/Scene.h"
#include "RenderGL/Mesh.h"

#define SM_SELECT_SINGLE		0
#define SM_SELECT_MULTY			1
#define SM_ADD					2
#define SM_MODIFY				3
#define SM_CURSOR_TEST_ALL		8
#define SM_CURSOR_TEST_SELECTED 9
	
#define SRT_RECTANGLE			11
#define SRT_CIRCLE				12
#define SRT_POLYGON				13
	

struct tSelectionInfo
{
	UINT		ObjectType;
	UINT		ObjectIndex;
	void		*pObject;

	UINT		SubObjectType;
	UINT		SubObjectIndex;

	bool		DisplayBox;

	Float3	Position;
	Float3	Rotation;
	Float3	Scale;
};

class CSelection  
{
public:

	CSelection();
	virtual ~CSelection();

//Updating Selection
	void	UpdateRegion( POINT position );
	void	UpdateRegion( RECT rectangle, bool updateVolume );
	void	UpdateRegion( POINT *ppoint, UINT pointCount, bool updateVolume );
	void	UpdateClippingPlanes();

	void	Update( UINT mode );
	void	UpdateUI();

	bool	ObjectInRegion( tObject *pobject, bool selectIntersecting );
	bool	ObjectAtCursor( tObject *pobject );

	bool	BoxInRegion( Float3 *size, bool selectIntersecting );
	bool	BoxAtCursor( Float3 *size );

	bool	PointInRegion( Float3 *point );

	bool	ModelInRegion( UINT index );
	bool	ModelAtCursor( UINT index );

	bool	ParseModels( UINT mode );
	bool	ParseGizmos();

//Updating External Dependencies
	void	UpdateCenter();
	void	UpdateInfoDialog();
	void	UpdateTypeInDialog( bool offSet );

//Selection manipulation		
	int		AddEntry( UINT objType, UINT objIndex, void *pobject, bool displayBox );	
	void	SetEntry( UINT index, UINT objType, UINT objIndex, void *pobject, bool displayBox, Float3 *position );

	int		AddEntry( UINT objType, UINT objIndex, UINT subObjType, UINT subObjIndex, void *pobject, bool displayBox );	
	void	SetEntry( UINT index, UINT objType, UINT objIndex, UINT subObjType, UINT subObjIndex, void *pobject, bool displayBox, Float3 *position );

	void	RemoveEntry( UINT index );	
	void	ClearEntries();
	int		FindEntry( UINT objType, UINT objIndex, UINT subObjType, UINT subObjIndex, bool displayBox );	
	int		FindEntry( UINT objType, UINT objIndex, bool displayBox );	

//Drawing Functions
	void	DrawBox( UINT index );

//Other Functions
	void	SetScene( CScene *pscene );

	void	ClearBuffer();
	void	ResizeBuffer( UINT size );

//Selection Region
	Int2		*pRegionPoint;
	UINT			PointCount;
	UINT			ActivePoints;
	UINT			RegionType;

//Selected Objects
	UINT			InfoCount;
	UINT			ActiveInfos;
	tSelectionInfo	*pInfo;

//Options
	bool			GizmoVisible;
	bool			RegionVisible;
	bool			BoxesVisible;
	
	bool			UseBackFaces;
	bool			GizmoHighlighted;

	bool			SelectIntersecting;

	Float3		GizmoPosition;
	Float3		GizmoPositionStart;

//	float			GizmoSize;
	
	Float3		*pPoint;
	Float3		*pPointReflected;

private:	

//Bound Scene
	CScene			*pScene;

//Selection Range
	Float4		ClipNear;
	Float4		ClipFar;

//Selection Volume
	Float4		*pPlane;

	bool			Selected;

//Temporary Buffers
	UINT			VertexCount;
	UINT			ActiveVertices;
	Float3		*pVertex;

	UINT			TempInfoCount;
	UINT			ActiveTempInfos;
	tSelectionInfo	*pTempInfo;

	double			ModelMatrix[16];
	CMesh			TempBoxMesh;

	UINT			StartIndex;
};

#endif // !defined(AFX_SELECTION_H__98C27905_2081_4658_872A_2B928543F90E__INCLUDED_)
