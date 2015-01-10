// Shadow.h: interface for the CShadow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADOW_H__0DC90F68_0641_4234_ACB7_30330778B102__INCLUDED_)
#define AFX_SHADOW_H__0DC90F68_0641_4234_ACB7_30330778B102__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShadow  
{
public:
	CShadow();
	virtual ~CShadow();

	void	SetObject( tObject *pobject );
	void	SetLight( Float4 *position );

	void	Project();
	void	Render();

	bool	ResizeBuffer( WORD count );

private:
	
	Float4	BoxPlane[6];

	WORD		*pIndice;
	WORD		IndiceCount;

	tObject		*pObject;
};

#endif // !defined(AFX_SHADOW_H__0DC90F68_0641_4234_ACB7_30330778B102__INCLUDED_)
