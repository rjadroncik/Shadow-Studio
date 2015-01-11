// Scene.h: interface for the CScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENE_H__5C858A42_D561_4B63_9AF1_D7B19BFB884E__INCLUDED_)
#define AFX_SCENE_H__5C858A42_D561_4B63_9AF1_D7B19BFB884E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SCFMathematics.h>
using namespace SCFMathematics;

#include "FontGL.h"

#include "../ModifierStack.h"

#include "RenderGL.h"
#include "Model.h"
#include "Light.h"
#include "Gizmo.h"


//Grid Type
#define	GT_ORTHO_STATIC		1
#define	GT_ORTHO_SCALABLE	2

#define	GT_PERSPECTIVE		3
#define	GT_USER				4


class RENDERGL_API CScene  
{
public:

	CScene();
	virtual ~CScene();

	void	Initialize();
	void	Delete();
	void	DeleteModels();
	void	DeleteLights();
	void	DeleteFonts();

//Rendering
	void	Render( UINT viewType, UINT viewId, UINT renderMode );

	void	DrawAxisGizmo( float size, UINT font, bool fromCenter );

//Modelling
	int		AddModel( UINT type );
	bool	DeleteModel( UINT index );

	UINT	AddLight( UINT type );
	bool	DeleteLight( UINT index );

	
	UINT	AddFont( LPSTR name, int size, HDC hdc );
	bool	DeleteFont( UINT index );

	CModel			*pModel;
	UINT			ModelCount;

	CLight			*pLight;
	UINT			LightCount;

	CFontGL			*pFont;
	UINT			FontCount;

//Basic Grids
	CModel			OrthoGrid;
	CModel			PerspectGrid;

//OpenGL Variables
	float			AmbientLight[4];

private:

	double			ModelMatrix[16];
	double			ProjMatrix[16];
	int				Viewport[4];

//Temporary Variables
	Float3		TempVector3a;
	Float3		TempVector3b;
	Float3		TempVector3c;
	Float3		TempVector3d;

	Double3		TempVectord3a;
	Double3		TempVectord3b;
	Double3		TempVectord3c;
};

#endif // !defined(AFX_SCENE_H__5C858A42_D561_4B63_9AF1_D7B19BFB884E__INCLUDED_)
