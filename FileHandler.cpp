   // FileHandler.cpp: implementation of the CFileHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileHandler.h"

#include "View.h"
#include "System.h"
#include "Shadow.h"
#include "Options.h"
#include "State.h"
#include "Afc.h"
#include "Resource.h"
#include "RenderGL/RenderGL.h"


//Common Include Files
#include "Timer.h"
#include "Math.h"

#include "RenderGL/3sfFile.h"
#include "RenderGL/2smFile.h"

extern CState				State; 
extern COptions				Options; 
extern CScene				*pScene; 
extern CMath				Math; 
extern CSystem				System;

extern ADialogBar			MainBar;
extern AToolBar				MainToolBar;
extern AToolBar				BottomToolBar;

extern C3sfFile				My3sfFile;
extern C2smFile				My2smFile;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileHandler::CFileHandler()
{

}

CFileHandler::~CFileHandler()
{

}

void CFileHandler::OnFileLoad()
{
//Make Sure We Don't Make Unwanted Changes
	State.Tool.Enabled = FALSE;

//The Filter for Filenames in The Open File Dialog
	char szFilter[] = "Shadow Studio Scene (*.3sf)\0*.3sf\0Shadow Studio Mesh (*.2sm)\0*.2sm\0\0";

	OPENFILENAME FileNameStruct;
	FileNameStruct.lStructSize       = sizeof( OPENFILENAME );
	FileNameStruct.hwndOwner         = System.hWnd;
	FileNameStruct.hInstance         = System.hInstance;
	FileNameStruct.lpstrFilter		 = szFilter;
	FileNameStruct.lpstrCustomFilter = NULL;
	FileNameStruct.nMaxCustFilter    = 0;
	FileNameStruct.nFilterIndex      = State.FileType;
	FileNameStruct.lpstrFile         = State.TempBuffer256a;
	FileNameStruct.nMaxFile          = 255;
	FileNameStruct.lpstrFileTitle    = NULL;
	FileNameStruct.nMaxFileTitle     = 0;
	FileNameStruct.lpstrInitialDir   = NULL;
	FileNameStruct.lpstrTitle        = "Open a File";
	FileNameStruct.nFileOffset       = 0;
	FileNameStruct.nFileExtension    = 0;
	FileNameStruct.lpstrDefExt       = NULL;
	FileNameStruct.lCustData         = NULL;
	FileNameStruct.lpfnHook 		 = NULL;
	FileNameStruct.lpTemplateName    = NULL;
	FileNameStruct.Flags             = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if( GetOpenFileName( &FileNameStruct ) ) 
	{
		State.hTempCursor = SetCursor( LoadCursor( NULL, IDC_WAIT ) );

		State.TempBool2 = FALSE;
		
		if( strstr( State.TempBuffer256a, ".2sm" ) || strstr( State.TempBuffer256a, ".2SM" ) )
		{
			if( My2smFile.Load( pScene, State.TempBuffer256a ) ) { State.TempBool2 = TRUE; }
		}

		if( strstr( State.TempBuffer256a, ".3sf" ) || strstr( State.TempBuffer256a, ".3SF" ) )
		{
			if( My3sfFile.Load( pScene, State.TempBuffer256a ) ) { State.TempBool2 = TRUE; }
		}

		if( State.TempBool2 )
		{
			State.FileType = FileNameStruct.nFilterIndex;
			State.FileNameOffset = FileNameStruct.nFileOffset;
			strcpy( State.CurrentFile, State.TempBuffer256a ); 
		}

		SetCursor( State.hTempCursor );
	}

	State.Tool.Enabled = TRUE;
	System.UpdateWindowTitle();
	System.UpdateAllViews( TRUE );

	System.Selection.SetScene( pScene );
	System.Selection.ClearEntries();
}

void CFileHandler::OnFileSave( bool newFile )
{
//Make Sure We Don't Make Unwanted Changes
	State.Tool.Enabled = FALSE;
	ReleaseCapture();

	if( newFile || !strlen( State.CurrentFile ) )
	{
	//The Filter for Filenames in The Open File Dialog
		char szFilter[] = "Shadow Studio Scene (*.3sf)\0*.3sf\0Shadow Studio Mesh (*.2sm)\0*.2sm\0All files (*.*)\0*.*\0\0";

		OPENFILENAME FileNameStruct;
		FileNameStruct.lStructSize       = sizeof( OPENFILENAME );
		FileNameStruct.hwndOwner         = System.hWnd;
		FileNameStruct.hInstance         = System.hInstance;
		FileNameStruct.lpstrFilter		 = szFilter;
		FileNameStruct.lpstrCustomFilter = NULL;
		FileNameStruct.nMaxCustFilter    = 0;
		FileNameStruct.nFilterIndex      = State.FileType;
		FileNameStruct.lpstrFile         = State.CurrentFile;
		FileNameStruct.nMaxFile          = 255;
		FileNameStruct.lpstrFileTitle    = NULL;
		FileNameStruct.nMaxFileTitle     = 0;
		FileNameStruct.lpstrInitialDir   = NULL;
		FileNameStruct.lpstrTitle        = "Save File";
		FileNameStruct.nFileOffset       = 0;
		FileNameStruct.nFileExtension    = 0;
		FileNameStruct.lpstrDefExt       = NULL;
		FileNameStruct.lCustData         = NULL;
		FileNameStruct.lpfnHook 		 = NULL;
		FileNameStruct.lpTemplateName    = NULL;
		FileNameStruct.Flags             = NULL;

		if( GetSaveFileName( &FileNameStruct ) ) 
		{
			State.hTempCursor = SetCursor( LoadCursor( NULL, IDC_WAIT ) );
		
			if( ( FileNameStruct.nFilterIndex == 1 ) && !strstr( State.CurrentFile, ".3sf" ) && !strstr( State.CurrentFile, ".3SF" ) ) { strcat( State.CurrentFile, ".3sf" ); }
			if( ( FileNameStruct.nFilterIndex == 2 ) && !strstr( State.CurrentFile, ".2sm" ) && !strstr( State.CurrentFile, ".2SM" ) ) { strcat( State.CurrentFile, ".2sm" ); }
			if( ( FileNameStruct.nFilterIndex == 3 ) && !strstr( State.CurrentFile, ".2sm" ) && !strstr( State.CurrentFile, ".2SM" ) && !strstr( State.CurrentFile, ".3sf" ) && !strstr( State.CurrentFile, ".3SF" ) ) { strcat( State.CurrentFile, ".3sf" ); }

			State.FileType = FileNameStruct.nFilterIndex;

			if( strstr( State.CurrentFile, ".2sm" ) || strstr( State.CurrentFile, ".2SM" ) )
			{
				My2smFile.Save( State.CurrentFile, pScene );
			}

			if( strstr( State.CurrentFile, ".3sf" ) || strstr( State.CurrentFile, ".3SF" ) )
			{
				My3sfFile.Save( State.CurrentFile, pScene );
			}

			State.FileNameOffset = FileNameStruct.nFileOffset;
			SetCursor( State.hTempCursor );
		}
	}
	else
	{
		State.hTempCursor = SetCursor( LoadCursor( NULL, IDC_WAIT ) );

		if( strstr( State.CurrentFile, ".2sm" ) || strstr( State.CurrentFile, ".2SM" ) )
		{
			My2smFile.Save( State.CurrentFile, pScene );
		}

		if( strstr( State.CurrentFile, ".3sf" ) || strstr( State.CurrentFile, ".3SF" ) )
		{
			My3sfFile.Save( State.CurrentFile, pScene );
		}

		SetCursor( State.hTempCursor );
	}

	State.Tool.Enabled = TRUE;
	System.UpdateWindowTitle();

	System.Selection.SetScene( pScene );
//	if( !Options.LockSelection ) { System.Selection.ClearEntries(); }
	System.UpdateAllViews( FALSE );
}


void CFileHandler::OnFileNew( bool force )
{
	if( !force )
	{
	//Decide What to Do
		UINT Result = MessageBox( NULL, "Save File Before Creating a new One ?", "Create a New File.", MB_YESNOCANCEL | MB_ICONQUESTION ); 

		if( Result == IDYES ) { OnFileSave( FALSE ); }
		if( Result == IDCANCEL ) { return; }
	}

	State.hTempCursor = SetCursor( LoadCursor( NULL, IDC_WAIT ) );

//Close File
	OnFileClose( FALSE );

//Make New File
	pScene = new CScene;
	pScene->Initialize();

	State.FileReady = TRUE;
	System.Selection.BoxesVisible = FALSE;
	System.Selection.GizmoVisible = FALSE;

	System.Update = TRUE;
	System.UpdateAllViews( FALSE );
	
	System.Selection.SetScene( pScene );

//Create an Initial Font
	pScene->AddFont( "Arial", 12, System.View[0].hDC );

	System.Selection.SetScene( pScene );
	System.Selection.ClearEntries();
	SetCursor( State.hTempCursor );
}

void CFileHandler::OnFileClose( bool reset )
{
	State.hTempCursor = SetCursor( LoadCursor( NULL, IDC_WAIT ) );
	
//Make Sure We Don't Make Unwanted Changes
	State.Tool.Enabled = FALSE;

	if( pScene )
	{
		pScene->Delete();
		delete pScene;
		pScene = NULL;
	}

	if( reset ) { State.Reset(); }

	System.Update = TRUE;
	System.UpdateAllViews( FALSE );

	strcpy( State.CurrentFile, "" );
	State.FileNameOffset = 0;

	State.Tool.Enabled = TRUE;
	System.UpdateWindowTitle();

	System.Selection.SetScene( pScene );
	System.Selection.ClearEntries();
	SetCursor( State.hTempCursor );
}