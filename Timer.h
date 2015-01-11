// Timer.h: interface for the Timer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMER_H__7634B68F_5A18_11D7_9F5F_BC7947C4551B__INCLUDED_)
#define AFX_TIMER_H__7634B68F_5A18_11D7_9F5F_BC7947C4551B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTimer
{

public:

					CTimer();									
	virtual			~CTimer();

	bool			Create();

	void			Reset();					
	void			Reset2();

	float			GetTime();				

	float			GetTimeOffset();				
	float			GetTimeOffset2();				


private:

	__int64			Frequency;							
	float			Period;
	
	bool			TimerRunning;					

	__int64			TimerStart;	
	__int64			TimerStart2;	
	
	__int64			Temp;							

};


#endif // !defined(AFX_TIMER_H__7634B68F_5A18_11D7_9F5F_BC7947C4551B__INCLUDED_)
