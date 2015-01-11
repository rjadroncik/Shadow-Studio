// RenderGL.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "RenderGL.h"

#include <SCFMathematics.h>
using namespace SCFMathematics;

#include "../Math.h"

CMath Math;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				//SCFMathematics::Initialize();
				break;
			}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


