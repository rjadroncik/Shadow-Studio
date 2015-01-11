// Font.h: interface for the CFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONT_H__39A4F864_6C03_4C91_8A97_8696537424AD__INCLUDED_)
#define AFX_FONT_H__39A4F864_6C03_4C91_8A97_8696537424AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderGL.h"

class RENDERGL_API CFontGL
{

public:
				CFontGL();
	virtual		~CFontGL();

	void		Begin();
	void		End();

	bool		Create( LPSTR fontName, int height, HDC hDC, UINT count );
	void		Destroy();

	void		PositionText( int x, int y );

	void		DrawText( int x, int y, const char *string, ... );

	void		SetViewport( UINT width, UINT height );

private:
	UINT		ListBase;
	UINT		Height;

	char		FontName[32];

	bool		InitDone;

	UINT		CharCount;
	SIZE		Viewport;
	
//Handles	
	HFONT		hFont;		
	HFONT		hFontOld;
	HDC			hDC;
};

#endif // !defined(AFX_FONT_H__39A4F864_6C03_4C91_8A97_8696537424AD__INCLUDED_)
