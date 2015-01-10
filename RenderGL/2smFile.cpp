// 2smFile.cpp: implementation of the C2smFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "2smFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C2smFile::C2smFile()
{
	ID = 0x1020;
	Version = 0x1000;

	ChunkSize = sizeof( t2smChunk );

	BYTESize = sizeof( BYTE );
	WORDSize = sizeof( WORD );
	FloatSize = sizeof( float );
}

C2smFile::~C2smFile()
{

}


bool C2smFile::Load( CScene *pscene, LPSTR fileName )
{

//Open File
	pFile = fopen( fileName, "rb" );

	if( !pFile )
	{
	//Display Warning and Return
		char Buffer[256];

		strcpy( Buffer, fileName );
		strcat( Buffer, "\nCan't Open File for Reading!" );

		MessageBox( NULL, Buffer, "C2smFile::Load()", MB_ICONERROR );
		return FALSE; 
	}

//Check File Format ID & Version
	ReadChunk();

	if( TempChunk.ID != ID )
	{ 
	//Display Warning and Return
		char Buffer[256];

		strcpy( Buffer, fileName );
		strcat( Buffer, "\nNot a Valid *.2sm File!" );

		MessageBox( NULL, Buffer, "C2smFile::Load()", MB_ICONERROR );
		return FALSE; 
	}

	if( TempChunk.Size != Version )
	{ 
	//Display Warning and Return
		char Buffer[256];

		strcpy( Buffer, fileName );
		strcat( Buffer, "\nIncompatible File Version!" );

		MessageBox( NULL, Buffer, "C2smFile::Load()", MB_ICONERROR );
		return FALSE; 
	}

	pscene->DeleteModels();
	pscene->DeleteLights();

	int Model = -1, Object, ObjectCount;

	while( ReadChunk() )
	{
		switch( TempChunk.ID )
		{
		case MODEL_HEADER:
			{
				pscene->AddModel( OBJECT_MESH );

				ObjectCount = ReadWORD();
				Model++;
				Object = -1;
				break;
			}

		case MODEL_TRANSFORMS:
			{
				fread( &pscene->pModel[Model].Position,	sizeof( Float3 ), 1, pFile ); 
				fread( &pscene->pModel[Model].Rotation,	sizeof( Float3 ), 1, pFile ); 
				fread( &pscene->pModel[Model].Scale,	sizeof( Float3 ), 1, pFile ); 
				break;
			}
					
		case MODEL_INFO:
			{
				ReadString( pscene->pModel[Model].Name );

				BYTE r = ReadBYTE();
				BYTE g = ReadBYTE();
				BYTE b = ReadBYTE();
			
				pscene->pModel[Model].Color = RGB( r, g, b );
				break;
			}

		case OBJECT_HEADER:
			{
				pscene->pModel[Model].Mesh.AddObject();
				Object++;
				break;
			}

		case OBJECT_TRANSFORMS:
			{
				fread( &pscene->pModel[Model].Mesh.pObject[Object].Position, sizeof( Float3 ), 1, pFile ); 
				fread( &pscene->pModel[Model].Mesh.pObject[Object].Rotation, sizeof( Float3 ), 1, pFile ); 
				break;
			}
			
		case OBJECT_INFO:
			{
				pscene->pModel[Model].Mesh.pObject[Object].Color[0] = (float) ReadBYTE() / 255;
				pscene->pModel[Model].Mesh.pObject[Object].Color[1] = (float) ReadBYTE() / 255;
				pscene->pModel[Model].Mesh.pObject[Object].Color[2] = (float) ReadBYTE() / 255;
				pscene->pModel[Model].Mesh.pObject[Object].Color[3] = (float) ReadBYTE() / 255;
				break;
			}

		case MATERIAL_INFO:
			{
				fread( &pscene->pModel[Model].Mesh.pObject[Object].Material.Ambient, sizeof( Float4 ), 1, pFile ); 
				fread( &pscene->pModel[Model].Mesh.pObject[Object].Material.Diffuse, sizeof( Float4 ), 1, pFile ); 
				fread( &pscene->pModel[Model].Mesh.pObject[Object].Material.Specular, sizeof( Float4 ), 1, pFile ); 
				fread( &pscene->pModel[Model].Mesh.pObject[Object].Material.Shininess, sizeof( float ), 1, pFile ); 
				fread( &pscene->pModel[Model].Mesh.pObject[Object].Material.Emission, sizeof( Float4 ), 1, pFile ); 
				break;
			}

		case TEXTURE_INFO:
			{
				fseek( pFile, TempChunk.Size, SEEK_CUR );

			//	ReadString( pscene->pModel[Model].Mesh.pObject[Object].TextureFile );
	
				break;
			}

		case VERTEX_DATA:
			{
				ReadBYTE();  

				pscene->pModel[Model].Mesh.AddVertices( Object, ReadWORD() );
				fread( pscene->pModel[Model].Mesh.pObject[Object].pVertex, FloatSize, pscene->pModel[Model].Mesh.pObject[Object].VertexCount * 3, pFile ); 
				break;
			}

		case NORMAL_DATA:
			{
				fread( pscene->pModel[Model].Mesh.pObject[Object].pNormal, FloatSize, pscene->pModel[Model].Mesh.pObject[Object].VertexCount * 3, pFile ); 
				break;
			}
	
		case TEXCOORD_DATA:
			{
				ReadBYTE(); 

				fread( pscene->pModel[Model].Mesh.pObject[Object].pTexCoord, FloatSize, pscene->pModel[Model].Mesh.pObject[Object].VertexCount * 2, pFile ); 
				break;
			}

		case COLOR_DATA:
			{
				ReadBYTE(); 
				
				pscene->pModel[Model].Mesh.pObject[Object].HasVertexColors = TRUE;
				pscene->pModel[Model].Mesh.AddTriangles( Object, 0 );

				fread( pscene->pModel[Model].Mesh.pObject[Object].pColor, FloatSize, pscene->pModel[Model].Mesh.pObject[Object].VertexCount * 4, pFile ); 
				break;
			}

		case TRIANGLE_DATA:
			{
				pscene->pModel[Model].Mesh.AddTriangles( Object, ReadWORD() );

				fread( pscene->pModel[Model].Mesh.pObject[Object].pTriangleIndice, WORDSize, pscene->pModel[Model].Mesh.pObject[Object].TriangleCount * 3, pFile ); 
				break;
			}

		case EDGEFLAG_DATA:
			{
				fread( pscene->pModel[Model].Mesh.pObject[Object].pEdgeFlag, BYTESize, pscene->pModel[Model].Mesh.pObject[Object].TriangleCount * 3, pFile ); 
				break;
			}

		default:
			{
				fseek( pFile, TempChunk.Size, SEEK_CUR );
				break;
			}
		}
	}

	fclose( pFile );


	for( UINT n = 0; n < pscene->ModelCount; n++ )
	{
		pscene->pModel[n].Mesh.UpdateConnectivity();
		pscene->pModel[n].Mesh.UpdateFacePlanes();
//		pscene->pModel[n].Mesh.UpdateNormals();
		pscene->pModel[n].UpdateExtents();
	}
	

	return TRUE;
}

BYTE C2smFile::ReadString( LPSTR data )
{
	fread( &StringSize, BYTESize, 1, pFile );
	fread( data, BYTESize, StringSize, pFile );

	return StringSize;
}

BYTE C2smFile::WriteString( LPSTR data )
{
	StringSize = strlen( data ) + 1;
	fwrite( &StringSize, BYTESize, 1, pFile );
	fwrite( data, BYTESize, StringSize, pFile );

	return StringSize;
}

void C2smFile::WriteChunk( WORD id, WORD size )
{
	TempChunk.ID = id;
	TempChunk.Size = size;

	fwrite( &TempChunk, ChunkSize, 1, pFile );
}

bool C2smFile::ReadChunk()
{
	if( feof( pFile ) ) { return FALSE; } 
	else { fread( &TempChunk, ChunkSize, 1, pFile ); return TRUE; }
}

void C2smFile::WriteWORD( WORD data )	{ TempWORD = data; fwrite( &TempWORD, WORDSize, 1, pFile ); }
void C2smFile::WriteBYTE( BYTE data )	{ TempBYTE = data; fwrite( &TempBYTE, BYTESize, 1, pFile ); }
void C2smFile::WriteFloat( float data ) { TempFloat = data; fwrite( &TempFloat, FloatSize, 1, pFile ); }

bool C2smFile::ReadBool()	{ fread( &TempBool, BYTESize, 1, pFile ); return TempBool; }
WORD C2smFile::ReadWORD()	{ fread( &TempWORD, WORDSize, 1, pFile ); return TempWORD; }
BYTE C2smFile::ReadBYTE()	{ fread( &TempBYTE, BYTESize, 1, pFile ); return TempBYTE; }
float C2smFile::ReadFloat() { fread( &TempFloat, FloatSize, 1, pFile ); return TempFloat; }


bool C2smFile::Save( LPSTR fileName, CScene *pscene )
{

//Open File
	pFile = fopen( fileName, "wb" );
	if( !pFile )
	{
	//Display Warning and Return
		char Buffer[256];

		strcpy( Buffer, fileName );
		strcat( Buffer, "\nCan't Open File for Writing!" );

		MessageBox( NULL, Buffer, "C2smFile::Save()", MB_ICONERROR );
		return FALSE; 
	}

//Write File Format ID & Version
	WriteChunk( ID, Version );

//Write Model Data
	for( WORD n = 0; n < pscene->ModelCount; n++ )
	{
	//Header
		WriteChunk( MODEL_HEADER, sizeof( BYTE ) * ( 4 + strlen( pscene->pModel[n].Name ) ) + WORDSize * 5 +  sizeof( FLOAT ) * 9 );

	//ObjectCount
		WriteWORD( (USHORT)pscene->pModel[n].Mesh.ObjectCount );

	//Transforms
		WriteChunk( MODEL_TRANSFORMS, FloatSize * 9 );
		fwrite( &pscene->pModel[n].Position,	sizeof( Float3 ), 1, pFile );
		fwrite( &pscene->pModel[n].Rotation,	sizeof( Float3 ), 1, pFile );
		fwrite( &pscene->pModel[n].Scale,		sizeof( Float3 ), 1, pFile );

	//Info
		WriteChunk( MODEL_INFO, sizeof( BYTE ) * ( 4 + strlen( pscene->pModel[n].Name ) ) );

	//Name
		WriteString( pscene->pModel[n].Name );

	//Color
		WriteBYTE( GetRValue( pscene->pModel[n].Color) );
		WriteBYTE( GetGValue( pscene->pModel[n].Color) );
		WriteBYTE( GetBValue( pscene->pModel[n].Color) );

	//Write Object Data
		for( WORD m = 0; m < pscene->pModel[n].Mesh.ObjectCount; m++ )
		{
		//Header
			WriteChunk( OBJECT_HEADER, sizeof( BYTE ) * 4 + WORDSize * 4 +  FloatSize * 6 );

		//Transforms
			WriteChunk( OBJECT_TRANSFORMS, FloatSize * 6 );
			fwrite( &pscene->pModel[n].Mesh.pObject[m].Position, sizeof( Float3 ), 1, pFile );
			fwrite( &pscene->pModel[n].Mesh.pObject[m].Rotation, sizeof( Float3 ), 1, pFile );

		//Info
			WriteChunk( OBJECT_INFO, sizeof( BYTE ) * 4 );
	
		//Color
			WriteBYTE( (BYTE)( pscene->pModel[n].Mesh.pObject[m].Color[0] * 255 ) );
			WriteBYTE( (BYTE)( pscene->pModel[n].Mesh.pObject[m].Color[1] * 255 ) );
			WriteBYTE( (BYTE)( pscene->pModel[n].Mesh.pObject[m].Color[2] * 255 ) );
			WriteBYTE( (BYTE)( pscene->pModel[n].Mesh.pObject[m].Color[3] * 255 ) );

		//Material
			WriteChunk( MATERIAL_INFO, FloatSize * 15 );
			fwrite( &pscene->pModel[n].Mesh.pObject[m].Material.Ambient,	sizeof( Float4 ), 1, pFile );
			fwrite( &pscene->pModel[n].Mesh.pObject[m].Material.Diffuse,	sizeof( Float4 ), 1, pFile );
			fwrite( &pscene->pModel[n].Mesh.pObject[m].Material.Specular,	sizeof( Float4 ), 1, pFile );
			fwrite( &pscene->pModel[n].Mesh.pObject[m].Material.Shininess,	FloatSize,	 1, pFile );
			fwrite( &pscene->pModel[n].Mesh.pObject[m].Material.Emission,	sizeof( Float4 ), 1, pFile );

			if( pscene->pModel[n].Mesh.pObject[m].HasTexture )
			{
			//Texture
				WriteChunk( TEXTURE_INFO, BYTESize * ( strlen( pscene->pModel[n].Mesh.pObject[m].TextureFile ) + 1 ) );
				WriteString( pscene->pModel[n].Mesh.pObject[m].TextureFile );
			}


		//Vertex
			WriteChunk( VERTEX_DATA, FloatSize * pscene->pModel[n].Mesh.pObject[m].VertexCount * 3 + WORDSize + sizeof( BYTE ) );

		//Components & Count 
			WriteBYTE( 3 );
			WriteWORD( pscene->pModel[n].Mesh.pObject[m].VertexCount );

			fwrite( pscene->pModel[n].Mesh.pObject[m].pVertex, FloatSize, pscene->pModel[n].Mesh.pObject[m].VertexCount * 3, pFile );
	

		//Normal
			WriteChunk( NORMAL_DATA, FloatSize * pscene->pModel[n].Mesh.pObject[m].VertexCount * 3 );
			fwrite( pscene->pModel[n].Mesh.pObject[m].pNormal, FloatSize, pscene->pModel[n].Mesh.pObject[m].VertexCount * 3, pFile );

		
		//TexCoord
			WriteChunk( TEXCOORD_DATA, FloatSize * pscene->pModel[n].Mesh.pObject[m].VertexCount * 2 + sizeof( BYTE ) );

		//TexCoordComponents
			WriteBYTE( 2 );

			fwrite( pscene->pModel[n].Mesh.pObject[m].pTexCoord, FloatSize, pscene->pModel[n].Mesh.pObject[m].VertexCount * 2, pFile );

			
		//Color
			if( pscene->pModel[n].Mesh.pObject[m].HasVertexColors )
			{
				WriteChunk( COLOR_DATA, FloatSize * pscene->pModel[n].Mesh.pObject[m].VertexCount * 4 + sizeof( BYTE ) );

			//ColorComponents
				WriteBYTE( 4 );

				fwrite( pscene->pModel[n].Mesh.pObject[m].pColor, FloatSize, pscene->pModel[n].Mesh.pObject[m].VertexCount * 4, pFile );
			}

		//Line
			if( pscene->pModel[n].Mesh.pObject[m].HasLines )
			{
				WriteChunk( LINE_DATA, WORDSize * ( 1 + pscene->pModel[n].Mesh.pObject[m].LineCount * 2 ) );

			//Count
				WriteWORD( pscene->pModel[n].Mesh.pObject[m].LineCount );

				fwrite( pscene->pModel[n].Mesh.pObject[m].pLineIndice, WORDSize, pscene->pModel[n].Mesh.pObject[m].LineCount * 2, pFile );
			}

		//Triangle
			if( pscene->pModel[n].Mesh.pObject[m].HasTriangles )
			{
				WriteChunk( TRIANGLE_DATA, WORDSize * ( 1 + pscene->pModel[n].Mesh.pObject[m].TriangleCount * 3 ) );

			//Count
				WriteWORD( pscene->pModel[n].Mesh.pObject[m].TriangleCount );

				fwrite( pscene->pModel[n].Mesh.pObject[m].pTriangleIndice, WORDSize, pscene->pModel[n].Mesh.pObject[m].TriangleCount * 3, pFile );

			//EdgeFlag
				WriteChunk( EDGEFLAG_DATA, BYTESize * pscene->pModel[n].Mesh.pObject[m].TriangleCount * 3 );
				fwrite( pscene->pModel[n].Mesh.pObject[m].pEdgeFlag, sizeof( bool ), pscene->pModel[n].Mesh.pObject[m].TriangleCount * 3, pFile );
			}
		}
	}

	fclose( pFile );

	return TRUE;
}

bool C2smFile::Import( CScene *pscene, LPSTR fileName )
{
	return FALSE;
}


UINT C2smFile::GetVersion() { return Version; }
