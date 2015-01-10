
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RENDERGL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RENDERGL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RENDERGL_EXPORTS
#define RENDERGL_API __declspec(dllexport)
#else
#define RENDERGL_API __declspec(dllimport)
#endif


#define MODEL					8000

#define OBJECT_MESH				8001
#define OBJECT_BOX				8002
#define OBJECT_PLANE			8003
#define OBJECT_SPHERE			8004
#define OBJECT_GRID				8005

#define MODIFIER_NOISE			9000
#define MODIFIER_EDIT_MESH		9001
#define MODIFIER_FFD_BOX		9002


#define SIZE_ALL			1
#define SIZE_X				2
#define SIZE_Y				3
#define SIZE_Z				4

#define TASSELLATION_ALL	6
#define TASSELLATION_X		7
#define TASSELLATION_Y		8
#define TASSELLATION_Z		9





