// State.h: interface for the CState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATE_H__6F03462F_50EC_4ABC_8721_B0F9846EA408__INCLUDED_)
#define AFX_STATE_H__6F03462F_50EC_4ABC_8721_B0F9846EA408__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "View.h"

struct tToolInfo
{
	UINT		Type;
	UINT		ID;

	bool		Enabled;
	bool		Active;
	UINT		Step;

	HWND		hButton;
	HCURSOR		hCursor;

	void		*pData;
};

struct tMouseInfo
{
	POINT		Pos2D;
	Float3	Pos3D;

	Float3	Vector;

	Float3	StartPoint3D;
	Float3	EndPoint3D;
};


class CState  
{
public:
	CState();
	virtual ~CState();

	void		UpdateMouse( UINT index, POINT position );
	bool		UpdateMousePos3D( UINT index, Float4* plane );

	bool		RollbackTool();
	void		SetToolInfo( UINT index, HWND hButton );

	void		Reset();

	void		UpdateTransforms( Float3 *pdata );


//File State
	bool		FileReady;
	char		CurrentFile[256];
	UINT		FileNameOffset;

	UINT		FileType;

//Tool State
	tToolInfo	Tool;
	UINT		ToolLevel;
	HWND		hToolButton[3];

//Mouse State
	tMouseInfo	Mouse[2];
	Float3	MouseOffset;

//Select State
	void		*pMasterObject;

	UINT		MasterObjectType;
	UINT		SubObjectType;

	bool		UsingMaster;

//View State
	CView		View;
	CView		TempView[4];

//OpenGL State
	GLdouble	ModelMatrix[16];   
	GLdouble	ProjMatrix[16];   
	GLint		Viewport[4];   

//UI State
	Float3	TransformInfo;

//Temp Variables

	HCURSOR		hTempCursor;

	RECT		TempRect1;
	RECT		TempRect2;

	POINT		TempPoint1;
	POINT		TempPoint2;

	POINT		*pRegionPoint;
	UINT		RegionPointCount;

	Float4	TempVector4a;
	Float4	TempVector4b;
	Float4	TempVector4c;
	Float4	TempVector4d;

	Float3	TempVector3a;
	Float3	TempVector3b;
	Float3	TempVector3c;
	Float3	TempVector3d;
	Float3	TempVector3e;

	Double3	TempVectord3a;
	Double3	TempVectord3b;

	Float2	TempVector2a;
	Float2	TempVector2b;
	Float2	TempVector2c;

	float		TempValue1;
	float		TempValue2;
	float		TempValue3;
	float		TempValue4;

	bool		TempBool1;
	bool		TempBool2;

	char		TempBuffer256a[256];
	char		TempBuffer128a[128];
	char		TempBuffer64a[64];
	char		TempBuffer32a[32];
	char		TempBuffer16a[16];
};

#endif // !defined(AFX_STATE_H__6F03462F_50EC_4ABC_8721_B0F9846EA408__INCLUDED_)
