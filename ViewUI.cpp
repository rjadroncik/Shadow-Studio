// ViewUI.cpp: implementation of the CViewUI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ViewUI.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CViewUI::CViewUI()
{
	MoveGizmo.Create( GT_MOVE, 50, 12 );

	ScaleGizmo.Create( GT_SCALE, 50, 12 );
}

CViewUI::~CViewUI()
{

}
