// Model.h: interface for the CModel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODEL_H__E75860EA_1242_11D8_A845_0002440D03A4__INCLUDED_)
#define AFX_MODEL_H__E75860EA_1242_11D8_A845_0002440D03A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mesh.h"
#include "RenderGL.h"
#include "../ModifierStack.h"


class RENDERGL_API CModel  
{
public:
	CModel();
	virtual ~CModel();

	bool	SelectBaseObject( UINT type );
	void	Initialize();
	void	Delete();
	void	SetColor( COLORREF color );

	void	Rotate( float x, float y, float z );
	void	AbsRotate( float x, float y, float z );

	void	ReScale( float x, float y, float z );
	
	void	Move( float x, float y, float z );
	void	RePosition( float x, float y, float z );

	void	UpdateExtents(); 


//Informations
	char			Name[32];
	bool			Visible;
	bool			Selected;

	bool			Static;
	bool			CastShadow;

	COLORREF		Color;

	Float3		BoxPosition;
	Float3		BoxSize;
	float			Radius;

//Transformation Information
	Float3		Position;
	Float3		Direction;
	Float3		Scale;
	Float3		Rotation;

//Modifiers
	tModifierInfo	*pModifier;
	UINT			ModifierCount;

//Source + Product Objects 
	CMesh			Mesh;
	void			*pBaseObject;
	UINT			BaseObjectType;
};

#endif // !defined(AFX_MODEL_H__E75860EA_1242_11D8_A845_0002440D03A4__INCLUDED_)
