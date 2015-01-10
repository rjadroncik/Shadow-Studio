// Timeline.h: interface for the CTimeline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMELINE_H__5D2A6060_FB51_11D7_A845_9F0B8C94C550__INCLUDED_)
#define AFX_TIMELINE_H__5D2A6060_FB51_11D7_A845_9F0B8C94C550__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ModifierStack.h"

struct tKeyData
{
	void		*pData;
	UINT		ModifierID;
};

struct tKeyInfo
{
	BOOL		Active;
	int			Frame;

	tKeyData	*pKeyData;
	UINT		KeyDataCount;
};


class CTimeline  
{
public:
	
	CTimeline();
	virtual ~CTimeline();

	void	Play( bool realtime );
	void	StepForward();
	void	StepBackward();

	bool	AddKey( int Frame );
	void	RemoveKey( int Frame );
	void	MoveKey( int Frame );

	bool	AddKeyData( int Frame, void *dataPointer, UINT dataType );
	bool	SetKeyData( int Frame, UINT dataIndex, void *dataPointer, UINT dataType );
	bool	RemoveKeyData( int Frame, UINT dataIndex );


private:

	tKeyInfo	*pKeyInfo;
	UINT		KeyCount;

};

#endif // !defined(AFX_TIMELINE_H__5D2A6060_FB51_11D7_A845_9F0B8C94C550__INCLUDED_)
