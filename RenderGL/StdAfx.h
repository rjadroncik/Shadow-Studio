// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FA376F09_13A8_11D8_A845_0002440D03A4__INCLUDED_)
#define AFX_STDAFX_H__FA376F09_13A8_11D8_A845_0002440D03A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_FONT_SIZE		16

// Insert your headers here
#include <windows.h>
#include <winuser.h>
#include <mmsystem.h>
#include <OpenGL\gl.h>
#include <OpenGL\glu.h>
#include <OpenGL\glaux.h>
#include <OpenGL\glext.h>
#include <OpenGL\wglext.h>
#include <math.h>
#include <stdio.h> 

//#include <stdlib.h>
//#include <stdarg.h>
//#include <direct.h>
//#include <commctrl.h>
//#include <shlwapi.h>
//#include <shlobj.h>
//#include <commdlg.h>

#include <SCFMathematics.h>
using namespace SCFMathematics;

#include "Light.h"
#include "Plane.h"
#include "Grid.h"
#include "Box.h"
#include "Sphere.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FA376F09_13A8_11D8_A845_0002440D03A4__INCLUDED_)
