#if !defined( IMG_LOAD_H_INCLUDED )
#define IMG_LOAD_H_INCLUDED 


#define	TGA_RGB		 2			//BGR File 
#define TGA_A		 3			//ALPHA File
#define TGA_RLE		10			//RLE File

#define	JPG_RGB		15			//BGR File 

struct tImageInfo
{
	int		Channels;			//( 3 = BGR : 4 = BGRA )
	WORD	Width;				
	WORD	Height;				

	int		Stride;				//The stride ( Channels * Width )
	int		RowSpan;

	BYTE	DataOffset;			//The Bytes form Header to Data
	BYTE	ImageType;			//The image type ( RLE, RGB, Alpha... )
	BYTE	Bits;				//( 16, 24, 32 )

	unsigned char *pData;	
};


bool LoadTGA( LPCSTR fileName, tImageInfo *pImage );
bool LoadJPG( LPCSTR fileName, tImageInfo *pImage );


#endif
