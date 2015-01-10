// ModifierStack.h: interface for the CModifierStack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODIFIERSTACK_H__5D2A6061_FB51_11D7_A845_9F0B8C94C550__INCLUDED_)
#define AFX_MODIFIERSTACK_H__5D2A6061_FB51_11D7_A845_9F0B8C94C550__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SCFMathematics.h>
using namespace SCFMathematics;

#include "RenderGL/Mesh.h"


//Transform Info Mask
#define	TIM_POSITION		0x0001
#define	TIM_ROTATION		0x0010
#define	TIM_SCALE			0x0100
#define	TIM_DIRECTION		0x1000

struct tTransformInfo
{
	Double3	Position;
	Double3	Rotation;
	Double3	Scale;

	Double3	Direction;

	UINT	Mask;
};


struct tGizmoInfo
{
	tTransformInfo	Transform;

	Double3		Size;
	double			Radius;
	double			Height;

	UINT			Type;
};


struct tModifierInfo
{
	UINT	Type;
	UINT	ID;

	bool	Enabled;
	void	*pModifier;
};


struct tObjectInfo
{
	UINT	Type;
	UINT	ID;

	CMesh	*pMesh;
	void	*pObject;
};


class CModifierStack  
{
public:

	CModifierStack();
	virtual ~CModifierStack();

	UINT	AddModifier( UINT Type );
	bool	RemoveModifier( UINT ID );

	bool	SetModifierData( UINT ID, UINT dataType, LONG *data ); 
	bool	SetModifierSelection( UINT ID, LONG *selectionData ); 

	
private:

	tModifierInfo	*pModifierInfo;
	UINT			ModifierCount;
};

#endif // !defined(AFX_MODIFIERSTACK_H__5D2A6061_FB51_11D7_A845_9F0B8C94C550__INCLUDED_)
