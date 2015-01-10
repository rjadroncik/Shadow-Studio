// Light.h: interface for the CLight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT_H__E75860E0_1242_11D8_A845_0002440D03A4__INCLUDED_)
#define AFX_LIGHT_H__E75860E0_1242_11D8_A845_0002440D03A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderGL.h"

class RENDERGL_API CLight  
{
public:
	CLight();
	virtual ~CLight();


private:

//Transformation Information
	Float4	Position;
	double		Radius;

//Common Variables
	UINT		Type;
	Float4	Color;

//Directional Specific
	Float3	Orientation;
	Float3	Rotation;
	Float3	Focus;
};

#endif // !defined(AFX_LIGHT_H__E75860E0_1242_11D8_A845_0002440D03A4__INCLUDED_)
