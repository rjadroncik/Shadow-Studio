#include "StdAfx.h"
#include "ImgLoad.h"

#include <stdio.h>


/****************************************************************/
//							JPG Images							//
/****************************************************************/

bool LoadJPG( LPCSTR fileName, tImageInfo *pImage )
{
	//TODO: LoadJPG
	//struct jpeg_decompress_struct cinfo;

	//FILE		*pFile;
	//
	//if( ( pFile = fopen( fileName, "rb" ) ) == NULL) 
	//{
	//	MessageBox( NULL, "Unable to Open JPG File!", "Error in: RenderGL.dll ( ImgLoad.cpp ).", MB_ICONERROR );
	//	return FALSE;
	//}
	//
	//jpeg_error_mgr jerr;                // Create an error handler

	//cinfo.err = jpeg_std_error( &jerr ); 
	//
	//jpeg_create_decompress( &cinfo );   // Initialize the decompression object
	//
	//jpeg_stdio_src( &cinfo, pFile );  	// Specify the data source (Our file pointer)	

	//DecodeJPG( &cinfo, pImage );  		// Decode the jpeg file and fill in the image data structure

	//jpeg_destroy_decompress( &cinfo ); 	// This releases all the stored memory for reading and decoding the jpeg

	//fclose( pFile );
	//
	//if( pImage->pData )		// obratenie textury
	//{
	//	unsigned char *pom;
	//	int x,y;

	//	pom = ( unsigned char* ) malloc( pImage->RowSpan * pImage->Height );

	//	for( y = 0; y < pImage->Height; y++ ) for( x = 0; x < pImage->RowSpan; x++ ) pom[x + y * pImage->RowSpan] = pImage->pData[x + ( pImage->Height - 1 - y ) * pImage->RowSpan];
	//	
	//	free( pImage->pData );
	//	pImage->pData = pom;
	//}	

	//pImage->ImageType = JPG_RGB;
	
	return TRUE; 
}



/****************************************************************/
//							TGA Images							//
/****************************************************************/

bool LoadTGA( LPCSTR fileName, tImageInfo *pImage )
{
	FILE		*pFile = NULL;			
	int			i = 0;					

//Open a File Pointer to the Targa File 
	pFile = fopen ( fileName, "rb" );

//Safety Measures...
	if( pFile == NULL ) { MessageBox( NULL, "Unable to Open TGA File !", "Error in: RenderGL.dll ( ImgLoad.cpp ).", MB_ICONERROR ); return FALSE; }
	if( pImage == NULL ) { MessageBox( NULL, "Void Image Structure Sent !", "Error in: RenderGL.dll ( ImgLoad.cpp ).", MB_ICONERROR ); return FALSE; }

//Read the Length from the Header to Pixel pData
	fread( &pImage->DataOffset, sizeof( BYTE ), 1, pFile);
//Jump Over one BYTE
	fseek( pFile, 1, SEEK_CUR ); 
//Read in the imageType (RLE, RGB, etc...)
	fread( &pImage->ImageType, sizeof( BYTE ), 1, pFile );
//Skip past General Information
	fseek( pFile, 9, SEEK_CUR ); 

	WORD temp; 

//Read >> Width, Height and Bits per Pixel
	fread( &temp, sizeof( WORD ), 1, pFile ); pImage->Width = temp;
	fread( &temp, sizeof( WORD ), 1, pFile ); pImage->Height = temp;
	fread( &pImage->Bits,   sizeof( BYTE ), 1, pFile );
	
//Find Dixel pData
	fseek( pFile, pImage->DataOffset + 1, SEEK_CUR ); 

//Check for RLE Compression
	if( pImage->ImageType != TGA_RLE )
	{
	//Check if the Image is 24/32-Bit
		if( pImage->Bits == 32 )
		{
		//Calculate >> Channels, Stride and Allocate Memory for Pixel pData
			pImage->Channels = pImage->Bits / 8;
			pImage->Stride = pImage->Channels * pImage->Width;
	
			pImage->pData = new unsigned char[ pImage->Stride * pImage->Height ];

		//Read the Whole Chunk of pData
			fread( pImage->pData, pImage->Stride * sizeof( BYTE ), pImage->Height, pFile );
		}
	//Else Return FALSE for a Font Unsupported Pixel Format
		else { return FALSE; }
	}
//Else, it must be Run-Length Encoded ( RLE )
	else
	{
	//aaaaabbcccccccc >> a5b2c8

	//Create some variables to hold the rleID, current colors read, channels, & stride.
		BYTE rleID = 0;
		int colorsRead = 0;
		pImage->Channels = pImage->Bits / 8;
		pImage->Stride = pImage->Channels * pImage->Width;

	//Next we want to allocate the memory for the Pixel and create an array,
	//depending on the channel count, to read in for each pixel.
		pImage->pData = new unsigned char[ pImage->Stride * pImage->Height ];
		BYTE *pColor = new BYTE [ pImage->Channels ];

	//Load the Pixel pData
		while( i < pImage->Width * pImage->Height )
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
					fread( pColor, sizeof( BYTE ) * pImage->Channels, 1, pFile );

				//Store the current pixel in our image array
					pImage->pData[colorsRead + 0] = pColor[0];
					pImage->pData[colorsRead + 1] = pColor[1];
					pImage->pData[colorsRead + 2] = pColor[2];

				//If we have a 4 channel 32-bit image, assign one more for the alpha
					if( pImage->Bits == 32 ) pImage->pData[colorsRead + 3] = pColor[3];

				//Increase the current Pixel read, decrease the amount
				//of Pixel left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += pImage->Channels;
				}
			}
		//Else, let's read in a string of the same character
			else
			{
			//Minus the 128 ID + 1 (127) to get the color count that needs to be read
				rleID -= 127;

			//Read in the current color, which is the same for a while
				fread( pColor, sizeof( BYTE ) * pImage->Channels, 1, pFile );

				// Go and read as many Pixel as are the same
				while( rleID )
				{
				//Assign the current pixel to the current index in our pixel array
					pImage->pData[colorsRead + 0] = pColor[0];
					pImage->pData[colorsRead + 1] = pColor[1];
					pImage->pData[colorsRead + 2] = pColor[2];

				//If we have a 4 channel 32-bit image, assign one more for the alpha
					if( pImage->Bits == 32 ) pImage->pData[colorsRead + 3] = pColor[3];

				//Increase the current Pixel read, decrease the amount
				//of Pixel left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += pImage->Channels;
				}
			}
		}
	}

//Close the file pointer that opened the file
	fclose( pFile );

	return TRUE;
}