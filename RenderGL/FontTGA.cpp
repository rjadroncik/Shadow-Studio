// FontTGA.cpp: implementation of the CFontTGA class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "FontTGA.h"
#include "ImgLoad.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFontTGA::CFontTGA()
{

}

CFontTGA::~CFontTGA()
{
	glDeleteLists( ListBase, 256 );					
	glDeleteTextures( 1, &TextureID );
}


//////////////////////////////////////////////////////////////////////
// Member Functions
//////////////////////////////////////////////////////////////////////


bool CFontTGA::Create( LPSTR fileName, double size )
{

//Texture Coordinates
	double	cx, cy;		
	
	glEnable( GL_TEXTURE_2D );

//Load the Texture
	if( !CreateTextureTGA( fileName, &TextureID ) ) { return FALSE; }

//Create 256 Display Lists
	ListBase = glGenLists( 256 );							

//Select Font Texture
	glBindTexture( GL_TEXTURE_2D, TextureID );	

//Fill Dislay Lists >> 1 Textured Quad for Each
	for ( int loop = 0; loop < 256; loop++ )	
	{
	
		cx = double( loop % 16 ) / 16.0f;				//X Position Of Current Character
		cy = double( loop / 16 ) / 16.0f;				//Y Position Of Current Character

		glNewList( ListBase + loop, GL_COMPILE );		
	
		glBegin( GL_QUADS );						
	
			glTexCoord2d( cx, 1.0f - cy - 0.0625f );			//(Bottom Left)
			glVertex2d( 0, size );								//(Bottom Left)
		
			glTexCoord2d( cx + 0.0625f, 1.0f - cy - 0.0625f );	//(Bottom Right)
			glVertex2d( size, size );							//(Bottom Right)
		
			glTexCoord2d( cx + 0.0625f, 1.0f - cy );			//(Top Right)
			glVertex2d( size, 0 );								//(Top Right)
	
			glTexCoord2d( cx, 1.0f - cy );						//(Top Left)
			glVertex2d( 0, 0 );									//(Top Left)
	
		glEnd();									
	
		glTranslated( 12 * ( (double)size / 16 ), 0, 0 );	//10,0,0	//Move To The Right Of The Character

		glEndList();								

	}												

	Size = size;
	strcpy( &FontFile[0], fileName );
	InitDone = FALSE;

	return TRUE;
}

void CFontTGA::Destroy()
{
	glDeleteLists( ListBase, 256 );					
	glDeleteTextures( 1, &TextureID );
}

void CFontTGA::SetViewport( UINT width, UINT height )
{
	Viewport.cx = width;
	Viewport.cy = height;
}

void CFontTGA::Begin()
{

//Save the List Base Information + OpenGL State Variables
	glPushAttrib( GL_LIST_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT );

//Set the List Base
	glListBase( ListBase );		
	
//Set State Variables
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_FOG );

//Set Blending
	glEnable( GL_BLEND ); 
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 

//Save Current View + Setup an Ortho View
	glMatrixMode( GL_PROJECTION );			
	glPushMatrix();								
	glLoadIdentity();							

//	gluOrtho2D( 0, Viewport.cx, Viewport.cy, 0 );
	glOrtho( 0, Viewport.cx, Viewport.cy, 0, -1000, 1000 );

	glMatrixMode( GL_MODELVIEW );				
	glPushMatrix();

	glLoadIdentity();	

//Select Font Texture
	glBindTexture( GL_TEXTURE_2D, TextureID );	

//Set Texture Filtering ( Linear )
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	InitDone = TRUE;

}


void CFontTGA::End()
{

//Restore Previous View
	glMatrixMode( GL_MODELVIEW );			
	glPopMatrix();	

	glMatrixMode( GL_PROJECTION );		
	glPopMatrix();						

//Restore the List Base Information + OpenGL State Variables
	glPopAttrib();

	InitDone = FALSE;
}

void CFontTGA::DrawText( UINT x, UINT y, char *text, int set )
{
	bool DidInit = FALSE;

	if ( set > 1 ) set = 1;

	if(!InitDone){  Begin(); DidInit = TRUE; }	

	glTranslated( (double)x, (double)y, 0 );	

//Choose The Font Set (0 or 1)
	glListBase( ListBase - 32 + ( 128 * set ) );

//Write The Text To The Screen
	glCallLists( strlen( text ), GL_BYTE, text );

	if( DidInit ){ End(); }	
}

void CFontTGA::DrawText( UINT x, UINT y, double number, int set )
{
	gcvt( number, 10, Temp );

	bool DidInit = FALSE;

	if ( set > 1 ) set = 1;

	if(!InitDone){  Begin(); DidInit = TRUE; }	

	glTranslated( (double)x, (double)y, 0 );	

//Choose The Font Set (0 or 1)
	glListBase( ListBase - 32 + ( 128 * set ) );

//Write The Text To The Screen
	glCallLists( strlen( Temp ), GL_BYTE, Temp );

	if(DidInit){ End(); }
}


bool CFontTGA::CreateTextureTGA( TCHAR fileName[96], UINT *texID )
{
	tImageInfo	*pImageInfo = NULL;	
	
	FILE		*pFile = NULL;			
	int			i = 0;					
		

//Open a File Pointer to the Targa File 
	pFile = fopen ( fileName, "rb" );

	if( pFile == NULL ) { MessageBox( NULL, "CFontTGA::CreateTextureTGA()", "Can't open file !", MB_ICONERROR ); return FALSE; }
		
//Allocate Structure for Image Data ( Free it! )
	pImageInfo = (tImageInfo*)malloc( sizeof( tImageInfo ) );

//Read the Length from the Header to Pixel Data
	fread( &pImageInfo->DataOffset, sizeof( BYTE ), 1, pFile);
	
//Jump Over one BYTE
	fseek( pFile, 1, SEEK_CUR ); 

//Read in the imageType (RLE, RGB, etc...)
	fread( &pImageInfo->ImageType, sizeof( BYTE ), 1, pFile );
	
//Skip past General Information
	fseek( pFile, 9, SEEK_CUR ); 

	WORD temp; 

//Read >> Width, Height and Bits per Pixel
	fread( &temp, sizeof( WORD ), 1, pFile ); pImageInfo->Width = temp;
	fread( &temp, sizeof( WORD ), 1, pFile ); pImageInfo->Height = temp;
	fread( &pImageInfo->Bits,   sizeof( BYTE ), 1, pFile );
	
//Find Dixel Data
	fseek( pFile, pImageInfo->DataOffset + 1, SEEK_CUR ); 

//Check for RLE Compression
	if( pImageInfo->ImageType != TGA_RLE )
	{

	//Check if the Image is 24/32-Bit
		if( pImageInfo->Bits == 32 )
		{

		//Calculate >> Channels, Stride and Allocate Memory for Pixel Data
			pImageInfo->Channels = pImageInfo->Bits / 8;
			pImageInfo->Stride = pImageInfo->Channels * pImageInfo->Width;
	
			pImageInfo->Data = new unsigned char[ pImageInfo->Stride * pImageInfo->Height ];

		//Read the Whole Chunk of Data
			fread( pImageInfo->Data, pImageInfo->Stride * sizeof( BYTE ), pImageInfo->Height, pFile );
		
		}
	
	//Else Return FALSE for a Font Unsupported Pixel Format
		else { return FALSE; }

	}

//Else, it must be Run-Length Encoded ( RLE )
	else
	{

	//http://astronomy.swin.edu.au/~pbourke/dataformats/rle/
 
	//aaaaabbcccccccc >> a5b2c8
		
	//Read in an color count (rleID), and if that number is less than 128,
	//it does NOT have any optimization for those colors, so just read the next
	//Pixel normally.  Say, the color count was 28, we read in 28 colors like normal.
	//If the color count is over 128, that means that the next color is optimized and
	//we want to read in the same pixel color for a count of (colorCount - 127).
	//It's 127 because we add 1 to the color count, as you'll notice in the code.

	//Create some variables to hold the rleID, current colors read, channels, & stride.
		BYTE rleID = 0;
		int colorsRead = 0;
		pImageInfo->Channels = pImageInfo->Bits / 8;
		pImageInfo->Stride = pImageInfo->Channels * pImageInfo->Width;

	//Next we want to allocate the memory for the Pixel and create an array,
	//depending on the channel count, to read in for each pixel.
		pImageInfo->Data = new unsigned char[ pImageInfo->Stride * pImageInfo->Height ];
		BYTE *pColor = new BYTE [ pImageInfo->Channels ];

	//Load the Pixel Data
		while( i < pImageInfo->Width * pImageInfo->Height )
		{
	
		//Read the Current Color Count + 1
			fread( &rleID, sizeof( BYTE ), 1, pFile );
			
		//Check if we don't have an encoded string of colors
			if( rleID < 128 )
			{
	
			//Increase the count by 1
				rleID++;

			//Go through and read all the unique colors found
				while( rleID )
				{
				//Read in the current color
					fread( pColor, sizeof( BYTE ) * pImageInfo->Channels, 1, pFile );

				//Store the current pixel in our image array
					pImageInfo->Data[colorsRead + 0] = pColor[0];
					pImageInfo->Data[colorsRead + 1] = pColor[1];
					pImageInfo->Data[colorsRead + 2] = pColor[2];

				//If we have a 4 channel 32-bit image, assign one more for the alpha
					if( pImageInfo->Bits == 32 ) pImageInfo->Data[colorsRead + 3] = pColor[3];

				//Increase the current Pixel read, decrease the amount
				//of Pixel left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += pImageInfo->Channels;

				}
	
			}

		//Else, let's read in a string of the same character
			else
			{
		
			//Minus the 128 ID + 1 (127) to get the color count that needs to be read
				rleID -= 127;

			//Read in the current color, which is the same for a while
				fread( pColor, sizeof( BYTE ) * pImageInfo->Channels, 1, pFile );

				// Go and read as many Pixel as are the same
				while( rleID )
				{
				//Assign the current pixel to the current index in our pixel array
					pImageInfo->Data[colorsRead + 0] = pColor[0];
					pImageInfo->Data[colorsRead + 1] = pColor[1];
					pImageInfo->Data[colorsRead + 2] = pColor[2];

				//If we have a 4 channel 32-bit image, assign one more for the alpha
					if( pImageInfo->Bits == 32 ) pImageInfo->Data[colorsRead + 3] = pColor[3];

				//Increase the current Pixel read, decrease the amount
				//of Pixel left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += pImageInfo->Channels;
			
				}
				
			}
				
		}

	}

//Close the file pointer that opened the file
	fclose( pFile );

	glEnable( GL_TEXTURE_2D );

//Generate a Texture and Store it's ID 
	glGenTextures( 1, texID );

//Bind the Texture 
	glBindTexture( GL_TEXTURE_2D, *texID );

	if( pImageInfo->Channels == 4 )
	{
	//Build a Texture from our Bitmap Data
		glTexImage2D( GL_TEXTURE_2D, 0, 4, pImageInfo->Width, pImageInfo->Height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pImageInfo->Data );
	}

//Free the Bitmap Data ( OpenGL already stored it as a Texture )
	if( pImageInfo ) 										
	{
		if( pImageInfo->Data )	{ free( pImageInfo->Data ); }
		free( pImageInfo );
	}


//Return the TGA data (remember, you must free this data after you are done)
	return TRUE;

}