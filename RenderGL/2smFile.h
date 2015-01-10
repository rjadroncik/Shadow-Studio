// 2smFile.h: interface for the C2smFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_2SMFILE_H__A3E7E6D4_F573_4FA0_8BD3_B46F03103A4B__INCLUDED_)
#define AFX_2SMFILE_H__A3E7E6D4_F573_4FA0_8BD3_B46F03103A4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderGL.h"
#include "Scene.h"

#define	FORMAT_2SM				0x1020
#define	VERSION_2SM				0x1000

#define	MODEL_HEADER			0x1001
#define	MODEL_TRANSFORMS		0x1002
#define	MODEL_INFO				0x1003
		
#define	OBJECT_HEADER			0x0101
#define	OBJECT_TRANSFORMS		0x0102
#define	OBJECT_INFO				0x0103

#define	MATERIAL_INFO			0x0110
#define	TEXTURE_INFO			0x0111

#define	VERTEX_DATA				0x0120
#define	NORMAL_DATA				0x0121
#define	TEXCOORD_DATA			0x0122
#define	COLOR_DATA				0x0123

#define	LINE_DATA				0x0130

#define	TRIANGLE_DATA			0x0140
#define	EDGEFLAG_DATA			0x0141


///////////////////////////////////////////////////////////////////////
///////////////////////////	 Shadow Studio Mesh	///////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////	 Format Definition	///////////////////////
///////////////////////////////////////////////////////////////////////

//WORD FORMAT_2SM 
//WORD VERSION_2SM

//WORD MODEL_HEADER
//WORD HeaderSize

//	WORD ObjectCount

//	WORD MODEL_TRANSFORMS 
//	WORD TransformSize

//		3xFLOAT Position
//		3xFLOAT Rotation
//		3xFLOAT Scale

//	WORD MODEL_INFO
//	WORD InfoSize

//		BYTE NameLength
//		CHAR Name[NameLength]

//		3xBYTE Color 


//WORD OBJECT_HEADER
//WORD HeaderSize

//	WORD OBJECT_TRANSFORMS
//	WORD TransformSize

//		3xFLOAT Position
//		3xFLOAT Rotation

//	WORD OBJECT_INFO
//	WORD InfoSize

//		4xBYTE Color 


//	WORD MATERIAL_INFO
//	WORD MaterialInfoSize

//		4xFLOAT	Ambient;
//		4xFLOAT	Diffuse;
//		4xFLOAT	Specular;
//			FLOAT Shininess
//		4xFLOAT	Emission;
	

//	WORD TEXTURE_INFO
//	WORD TextureInfoSize

//		BYTE FileNameLength
//		CHAR FileName[FileNameLength]


//	WORD VERTEX_DATA
//	WORD VertexDataSize 

//		BYTE VertexComponents		( 2, 3, 4 )
//		WORD VertexCount

//		FLOAT Vertex[VertexCount * VertexComponents]


//	WORD NORMAL_DATA
//	WORD NormalDataSize

//		FLOAT Normal[VertexCount * 3]


//	WORD TEXCOORD_DATA
//	WORD TexCoordDataSize

//		BYTE TexCoordComponents		( 1, 2, 3, 4 )

//		FLOAT TexCoord[VertexCount * TexCoordComponents]


//	WORD COLOR_DATA
//	WORD ColorDataSize

//		BYTE ColorComponents		( 3, 4 )

//		FLOAT Color[VertexCount * ColorComponents] 


//	WORD LINE_DATA
//	WORD LineDataSize

//		WORD LineCount		

//		UINT LineIndice[LineCount * 2] 


//	WORD TRIANGLE_DATA
//	WORD TriangleDataSize

//		WORD TriangleCount

//		WORD TriangleIndice[TriangleCount * 3]


//	WORD EDGEFLAG_DATA
//	UINT EdgeFlagDataSize

//		BOOL EdgeFlag[TriangleCount * 3]

struct t2smChunk
{
	WORD ID;
	WORD Size;
};

class RENDERGL_API C2smFile  
{
public:
	C2smFile();
	virtual ~C2smFile();

	bool	Load( CScene *pscene, LPSTR fileName );
	bool	Save( LPSTR fileName, CScene *pscene );

	bool	Import( CScene *pscene, LPSTR fileName );

	bool	ReadBool();
	WORD	ReadWORD();
	BYTE	ReadBYTE();
	FLOAT	ReadFloat();

	void	WriteWORD( WORD data );
	void	WriteBYTE( BYTE data );
	void	WriteFloat( float data );

	bool	ReadChunk();
	void	WriteChunk( WORD id, WORD size );
	
	BYTE	ReadString( LPSTR data );
	BYTE	WriteString( LPSTR data );


	UINT	GetVersion();

private:

	UINT		Version;
	UINT		ID;

	UINT		Result;
	FILE		*pFile;

	BYTE		StringSize;

	t2smChunk	TempChunk;
	BYTE		ChunkSize;

	BYTE		BYTESize;
	BYTE		WORDSize;
	BYTE		FloatSize;

	bool		TempBool;
	BYTE		TempBYTE;
	WORD		TempWORD;
	float		TempFloat;
};

#endif // !defined(AFX_2SMFILE_H__A3E7E6D4_F573_4FA0_8BD3_B46F03103A4B__INCLUDED_)
