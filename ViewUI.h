// ViewUI.h: interface for the CViewUI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWUI_H__5EACAB71_0CB2_4B03_A7D0_DC19BC016DF8__INCLUDED_)
#define AFX_VIEWUI_H__5EACAB71_0CB2_4B03_A7D0_DC19BC016DF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderGL/Gizmo.h"

class CViewUI  
{
public:
	CViewUI();
	virtual ~CViewUI();


//Basic Gizmos
	CGizmo			MoveGizmo;
	CGizmo			RotateGizmo;

	CGizmo			ScaleGizmo;
};

#endif // !defined(AFX_VIEWUI_H__5EACAB71_0CB2_4B03_A7D0_DC19BC016DF8__INCLUDED_)
