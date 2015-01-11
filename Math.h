// Math.h: interface for the CMath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined( MATH_H_INCLUDED )
#define MATH_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

#define DegToRad	0.0174532925199432957692369076f
#define RadToDeg	57.295779513082320876798154814f

#define TRUNC_TOP_LEFT			0x1000
#define TRUNC_TOP_RIGHT			0x2000
#define TRUNC_BOTTOM_RIGHT		0x0100
#define TRUNC_BOTTOM_LEFT		0x0200


const double Accuracy = 0.00005;

class CMath  
{
public:
	inline void ZeroVector2(_OUT Float2* pVector) { SCFMathematics::ZeroVector2(*pVector); }
	inline void ZeroVector3(_OUT Float3* pVector) { SCFMathematics::ZeroVector3(*pVector); }
	inline void ZeroVector4(_OUT Float4* pVector) { SCFMathematics::ZeroVector4(*pVector); }

	inline void CopyVector2(_OUT Float2* pDestination, _IN Float2* pSource) { SCFMathematics::CopyVector2(*pDestination, *pSource); }
	inline void CopyVector3(_OUT Float3* pDestination, _IN Float3* pSource) { SCFMathematics::CopyVector3(*pDestination, *pSource); }
	inline void CopyVector4(_OUT Float4* pDestination, _IN Float4* pSource) { SCFMathematics::CopyVector4(*pDestination, *pSource); }

	inline void AddVectors2(_IN Float2* pVector1, _IN Float2* pVector2, _OUT Float2* pResult) { SCFMathematics::AddVectors2(*pResult, *pVector1, *pVector2); }
	inline void AddVectors3(_IN Float3* pVector1, _IN Float3* pVector2, _OUT Float3* pResult) { SCFMathematics::AddVectors3(*pResult, *pVector1, *pVector2); }
	inline void AddVectors4(_IN Float4* pVector1, _IN Float4* pVector2, _OUT Float4* pResult) { SCFMathematics::AddVectors4(*pResult, *pVector1, *pVector2); }

	inline void ScaleVector2(_IN Float2* pVector, float fScale, _OUT Float2* pResult) { SCFMathematics::ScaleVector2(*pResult, *pVector, fScale); }
	inline void ScaleVector3(_IN Float3* pVector, float fScale, _OUT Float3* pResult) { SCFMathematics::ScaleVector3(*pResult, *pVector, fScale); }
	inline void ScaleVector4(_IN Float4* pVector, float fScale, _OUT Float4* pResult) { SCFMathematics::ScaleVector4(*pResult, *pVector, fScale); }

	inline void DivideVector3(_IN Float3* pVector, float fFactor, _OUT Float3* pResult) { SCFMathematics::DivideVector3(*pResult, *pVector, fFactor); }

	inline void SubtractVectors3(_IN Float3* pVector1, _IN Float3* pVector2, _OUT Float3* pResult) { SCFMathematics::SubtractVectors3(*pResult, *pVector1, *pVector2); }
	
	inline void AverageVectors3(_IN Float3* pPoint1, _IN Float3* pPoint2, _OUT Float3* pResult) { SCFMathematics::AverageVectors3(*pResult, *pPoint1, *pPoint2); }

	inline void SetVector2(_OUT Float2* pVector, _IN float fX, _IN float fY)                             { SCFMathematics::SetVector2(*pVector, fX, fY); }
	inline void SetVector3(_OUT Float3* pVector, _IN float fX, _IN float fY, _IN float fZ)               { SCFMathematics::SetVector3(*pVector, fX, fY, fZ); }
	inline void SetVector4(_OUT Float4* pVector, _IN float fX, _IN float fY, _IN float fZ, _IN float fW) { SCFMathematics::SetVector4(*pVector, fX, fY, fZ, fW); }

	inline void MakeVector2(_IN Float2* pPoint1, _IN Float2* pPoint2, _OUT Float2* pResult)	{ SCFMathematics::MakeVector2(*pResult, *pPoint1, *pPoint2); }
	inline void MakeVector3(_IN Float3* pPoint1, _IN Float3* pPoint2, _OUT Float3* pResult)	{ SCFMathematics::MakeVector3(*pResult, *pPoint1, *pPoint2); }
	
	inline void MakeVector3(_IN Float3* pDirection, _IN float Length, _OUT Float3* pResult) { SCFMathematics::MakeVector3(*pResult, *pDirection, Length); }

	inline float MeasureVector2(_IN Float2* pVector) { return SCFMathematics::MeasureVector2(*pVector); }
	inline float MeasureVector3(_IN Float3* pVector) { return SCFMathematics::MeasureVector3(*pVector); }

	inline float MeasureDistance3(_IN Float3* pPoint1, _IN Float3* pPoint2) { return SCFMathematics::MeasureDistance3(*pPoint1, *pPoint2); }
	inline float MeasureDistance3Square(_IN Float3* pPoint1, _IN Float3* pPoint2) { return SCFMathematics::MeasureDistance3Square(*pPoint1, *pPoint2); }

	inline void MakePlane(_IN Float3* pPoint1, _IN Float3* pPoint2, _IN Float3* pPoint3, _OUT Float4* pResult) { SCFMathematics::MakePlane(*pResult, *pPoint1, *pPoint2, *pPoint3); }
	inline void MakePlane(_IN Float3* pPoint, _IN Float3* pNormal, _OUT Float4* pResult)                       { SCFMathematics::MakePlane(*pResult, *pPoint, *pNormal); }

	inline void NormalizeVector2(_IN Float2* pVector, _OUT Float2* pResult) { SCFMathematics::NormalizeVector2(*pResult, *pVector); }
	inline void NormalizeVector3(_IN Float3* pVector, _OUT Float3* pResult) { SCFMathematics::NormalizeVector3(*pResult, *pVector); }

	inline void CrossVectors3(_IN Float3* pVector1, _IN Float3* pVector2, _OUT Float3* pResult) { SCFMathematics::CrossVectors3(*pResult, *pVector1, *pVector2); }

public:
	inline void RotateVector3(_IN Float3* pVector, _IN float fX, _IN float fY, _IN float fZ, _OUT Float3* pResult) { SCFMathematics::RotateVector3(*pResult, *pVector, fX, fY, fZ); }
	inline void RotateVector3(_IN Float3* pVector, _IN Float3* pRotation, _OUT Float3* pResult) { SCFMathematics::RotateVector3(*pResult, *pVector, *pRotation); }

	inline void MultiplyMatrix4Vector3(_IN Matrix4* pMatrix, _IN Float3* pVector, _OUT Float3* pResult) { SCFMathematics::MultiplyMatrix4Vector3(*pResult, *pMatrix, *pVector); }

public:
	inline void CopyVector3(_OUT Float3* pDestination, _IN Double3* pSource)
	{
		(*pDestination)[0] = (float)(*pSource)[0];
		(*pDestination)[1] = (float)(*pSource)[1];
		(*pDestination)[2] = (float)(*pSource)[2];
	}

	inline void DataToVector3(_OUT Float3* pResult, _IN GLfloat* pData)
	{ 
		(*pResult)[0] = pData[0]; 
		(*pResult)[1] = pData[1];
		(*pResult)[2] = pData[2];
	}
};

#endif // !defined( MATH_H_INCLUDED )
