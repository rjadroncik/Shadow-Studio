// Font.cpp: implementation of the CFont class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "FontGL.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFontGL::CFontGL()
{

}

CFontGL::~CFontGL()
{

}


//////////////////////////////////////////////////////////////////////
// Member Functions
//////////////////////////////////////////////////////////////////////


void CFontGL::Begin()
{
//Save the List Base Information + OpenGL State Variables
	glPushAttrib( GL_LIST_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT );

//Set the List Base
	glListBase( ListBase );		
	
//Set State Variables
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_FOG );
	glDisable( GL_BLEND ); 

	InitDone = TRUE;
}


void CFontGL::End()
{
//Restore the List Base Information + OpenGL State Variables
	glPopAttrib();

	InitDone = FALSE;
}

void CFontGL::SetViewport( UINT width, UINT height )
{
	Viewport.cx = width;
	Viewport.cy = height;
}

bool CFontGL::Create( LPSTR fontName, int height, HDC hDC, UINT count )	
{

//Generate the Lists for each Glyph
	ListBase = glGenLists( count );	

	hFont = CreateFont(	height,	0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH, fontName ); 

//Check if the Font was Created
	if( !hFont ) { MessageBox( NULL, "CFont::CreateFontGL()", "Error A1", MB_ICONERROR ); return FALSE; }

	hFontOld = (HFONT) SelectObject( hDC, hFont );

//Check if the Font was Selected
	if( !hFontOld ) { MessageBox( NULL, "CFont::CreateFontGL()", "Error A2", MB_ICONERROR ); return FALSE; }

//Create the Font Bitmaps >> 2nd Parameter - Starting Character, 3rd - Ending Char.   			
	wglUseFontBitmaps( hDC, 0, count - 1, ListBase );	

//Update our Member Variables
	hDC = hDC;

	CharCount = count;
	Height = height;

	strcpy( FontName, fontName );
	InitDone = FALSE;

	return TRUE;
}


void CFontGL::Destroy()										
{

//Free the display list
	glDeleteLists( ListBase, CharCount );			

//Select Old Font >> Don't leave Memory Leaks
	SelectObject( hDC, hFontOld );					

}


void CFontGL::PositionText( int x, int y )
{

//Save our Transformation and Viewport information
	glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );

//Save Projection and Modelview Matrix 
	glMatrixMode( GL_PROJECTION );			
	glPushMatrix();							
	glLoadIdentity();						

	glMatrixMode( GL_MODELVIEW );			
	glPushMatrix();							
	glLoadIdentity();						

//Calculate Screen Position
	y = Viewport.cy - Height - y;				

//Create a new Viewport
	glViewport( x - 1, y - 1, 0, 0 );		

//Set the Drawing Position
	glRasterPos4f( 0, 0, 0, 1 );					

//Restore the Modelview and Projection Matrix 
	glPopMatrix();		

	glMatrixMode( GL_PROJECTION );	
	glPopMatrix();					

//Restore our Transformation and Viewport information
	glPopAttrib();									
}


void CFontGL::DrawText( int x, int y, const char *string, ... )
{

//Pointer to the Argument List	
	va_list		ArgumentPointer;
	
	char		Text[256];

//Check if a String was given
	if( !string ) { return; }

//Parse the string for arguments given
	va_start( ArgumentPointer, string );

//Add the Arguments into the Full String
	vsprintf( Text, string, ArgumentPointer );

//Reset and Free the Argument List Pointer
	va_end( ArgumentPointer );

	bool DidInit = FALSE;

	if(!InitDone){  Begin(); DidInit = TRUE; }	

	PositionText( x, y );

//Render the Text
	glCallLists( strlen( Text ), GL_UNSIGNED_BYTE, Text );

	if( DidInit ){ End(); }
}
