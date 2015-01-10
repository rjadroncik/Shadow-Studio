// Model.cpp: implementation of the CModel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Model.h"
#include "Box.h"
#include "Plane.h"
#include "../../System/Math.h"

extern CMath Math;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModel::CModel()
{

}

CModel::~CModel()
{

}

void CModel::Initialize()
{
	pModifier = NULL;
	ModifierCount = 0;	
	
	BaseObjectType = 0;
	pBaseObject = NULL;

	Direction[0] = 0;
	Direction[1] = 1;
	Direction[2] = 0;

	Rotation[0] = 0;
	Rotation[1] = 0;
	Rotation[2] = 0;

	Scale[0] = 1;
	Scale[1] = 1;
	Scale[2] = 1;

	Position[0] = 0;
	Position[1] = 0;
	Position[2] = 0;

	Visible = TRUE;
	Selected = FALSE;
	Static = FALSE;
	CastShadow = FALSE;
}

void CModel::Delete()
{
	if( pModifier )
	{
		for( UINT m = 0; m < ModifierCount; m++ )
		{
			delete pModifier[m].pModifier; 
		}
	
		free( pModifier ); 
	}

	ModifierCount = 0;

	Mesh.Delete();

	switch( BaseObjectType )
	{
	case OBJECT_MESH:
		{
//			CMesh *pMesh = (CMesh*)pBaseObject;
//			pMesh->Delete();
			break;
		}
	case OBJECT_PLANE:
		{
			CPlane *pPlane = (CPlane*)pBaseObject;
			pPlane->Delete();  
			break;
		}
	case OBJECT_GRID:
		{
			CGrid *pGrid = (CGrid*)pBaseObject;
			pGrid->Delete();  
			break;
		}
	}
}


bool CModel::SelectBaseObject( UINT type )
{
	Mesh.Initialize();
	BaseObjectType = type;

	switch( type )
	{
	case OBJECT_MESH:
		{
			CMesh *pMesh = new CMesh;

			pBaseObject = pMesh;
			pMesh->Initialize();

			return TRUE;
		}

	case OBJECT_BOX:
		{
			CBox *pBox = new CBox;

			pBaseObject = pBox;
			pBox->SetMesh( &Mesh );  
		
			return TRUE;
		}

	case OBJECT_PLANE:
		{
			CPlane *pPlane = new CPlane;

			pBaseObject = pPlane;
			pPlane->SetMesh( &Mesh );  
		
			return TRUE;
		}

	case OBJECT_GRID:
		{
			CGrid *pGrid = new CGrid;

			pBaseObject = pGrid;
			pGrid->SetMesh( &Mesh );  
		
			return TRUE;
		}
	}

	return FALSE;
}

void CModel::SetColor( COLORREF color )
{
	for( UINT n = 0; n < Mesh.ObjectCount; n++ )
	{
		Mesh.pObject[n].HasVertexColors = FALSE;

		Mesh.pObject[n].Color[0] = GetRValue( color ) / 255.0f;
		Mesh.pObject[n].Color[1] = GetGValue( color ) / 255.0f;
		Mesh.pObject[n].Color[2] = GetBValue( color ) / 255.0f;
		Mesh.pObject[n].Color[3] = 1;
	}

	Color = color;
}

void CModel::UpdateExtents()
{
	Float3	LowExtent, HighExtent, CurrentVertex;
	double		ModelMatrix[16];

//	Math.CopyVector3( &LowExtent, &Mesh.pObject[0].Position );
//	Math.CopyVector3( &HighExtent, &Mesh.pObject[0].Position );

	Math.ZeroVector3( &LowExtent );
	Math.ZeroVector3( &HighExtent );

	for( UINT n = 0; n < Mesh.ObjectCount; n++ )
	{
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		glScalef( Scale[0], Scale[1], Scale[2] );

		glTranslated( Mesh.pObject[n].Position[0], Mesh.pObject[n].Position[1], Mesh.pObject[n].Position[2] );

		glRotated( Mesh.pObject[n].Rotation[0], 1, 0, 0 ); 
		glRotated( Mesh.pObject[n].Rotation[1], 0, 1, 0 ); 
		glRotated( Mesh.pObject[n].Rotation[2], 0, 0, 1 ); 

//		glScalef( Mesh.pObject[n].Scale[0], Mesh.pObject[n].Scale[1], Mesh.pObject[n].Scale[2] );

		glGetDoublev( GL_MODELVIEW_MATRIX, ModelMatrix );

		Float3 TempVector;

		for( UINT m = 0; m < Mesh.pObject[n].VertexCount; m++ )
		{
			Math.DataToVector3( &TempVector, &Mesh.pObject[n].pVertex[m * 3] );

			Math.MultiplyMatrix4Vector3( &ModelMatrix[0], &TempVector, &CurrentVertex );
			
			if( CurrentVertex[0] < LowExtent[0] ) { LowExtent[0] = CurrentVertex[0]; }
			if( CurrentVertex[1] < LowExtent[1] ) { LowExtent[1] = CurrentVertex[1]; }
			if( CurrentVertex[2] < LowExtent[2] ) { LowExtent[2] = CurrentVertex[2]; }

			if( CurrentVertex[0] > HighExtent[0] ) { HighExtent[0] = CurrentVertex[0]; }
			if( CurrentVertex[1] > HighExtent[1] ) { HighExtent[1] = CurrentVertex[1]; }
			if( CurrentVertex[2] > HighExtent[2] ) { HighExtent[2] = CurrentVertex[2]; }
		}
	}

	BoxSize[0] = (float)( ( HighExtent[0] - LowExtent[0] ) * 0.505f );
	BoxSize[1] = (float)( ( HighExtent[1] - LowExtent[1] ) * 0.505f );
	BoxSize[2] = (float)( ( HighExtent[2] - LowExtent[2] ) * 0.505f );

	BoxPosition[0] = (float)( LowExtent[0] + BoxSize[0] * 0.99f );
	BoxPosition[1] = (float)( LowExtent[1] + BoxSize[1] * 0.99f );
	BoxPosition[2] = (float)( LowExtent[2] + BoxSize[2] * 0.99f );

	if( ( BoxSize[0] >= BoxSize[1] ) &&  ( BoxSize[0] >= BoxSize[2] ) ) { Radius = BoxSize[0] * 1.414213f; }
	if( ( BoxSize[1] >= BoxSize[0] ) &&  ( BoxSize[1] >= BoxSize[2] ) ) { Radius = BoxSize[1] * 1.414213f; }
	if( ( BoxSize[2] >= BoxSize[0] ) &&  ( BoxSize[2] >= BoxSize[1] ) ) { Radius = BoxSize[2] * 1.414213f; }
}
 
