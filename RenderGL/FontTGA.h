// FontTGA.h: interface for the CFontTGA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTTGA_H__DC321F2C_02E6_4EA3_8965_76369E6C1D2A__INCLUDED_)
#define AFX_FONTTGA_H__DC321F2C_02E6_4EA3_8965_76369E6C1D2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Types.h"

class CFontTGA
{

public:	
			CFontTGA();
	virtual	~CFontTGA();

	bool	Create( LPSTR fileName, double size );
	void	Destroy();

	void	SetViewport( UINT width, UINT height );

	void	DrawText( UINT x, UINT y, char *text, int set );

	void	DrawText( UINT x, UINT y, double number, int set );

	void	Begin();	
	void	End();			

	char		FontFile[96];
	double		Size;


private:
///Don't Use LPSTR
	bool		CreateTextureTGA( TCHAR fileName[96], UINT *texID );

	UINT		ListBase;			
	UINT		TextureID;

	bool		InitDone;

	CHAR		Temp[16];

	SIZE		Viewport;
};


#endif // !defined(AFX_FONTTGA_H__DC321F2C_02E6_4EA3_8965_76369E6C1D2A__INCLUDED_)
