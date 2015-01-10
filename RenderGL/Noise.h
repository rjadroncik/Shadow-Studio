// Noise.h: interface for the CNoise class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOISE_H__218B2F00_130A_11D8_A845_0002440D03A4__INCLUDED_)
#define AFX_NOISE_H__218B2F00_130A_11D8_A845_0002440D03A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef BASEMOD_EXPORTS
#define BASEMOD_API __declspec(dllexport)
#else
#define BASEMOD_API __declspec(dllimport)
#endif

#include "../ModifierStack.h"
#include "RenderGL.h"
#include "../System.h"


struct tNoiseData
{
//	tSelectionInfo	Selection;

	tGizmoInfo		Gizmo;

	Float3		Magnitude;
};


class RENDERGL_API CNoise  
{
public:

	CNoise();
	virtual ~CNoise();

	UINT	AddModifier();
	bool	SelectModifier( UINT index );
	bool	EvaluateModifier( UINT index );
	bool	DestroyModifier( UINT index );

	bool	SetSelection( UINT index, LONG *selectionData );
	bool	SetData( UINT index, UINT dataType, LONG *data );
	
private:

	tNoiseData	*pNoiseData;

};

#endif // !defined(AFX_NOISE_H__218B2F00_130A_11D8_A845_0002440D03A4__INCLUDED_)
