// Timer.cpp: implementation of the Timer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Timer.h"

CTimer::CTimer() {}
CTimer::~CTimer() {}

bool CTimer::Create()								
{
	if ( !QueryPerformanceFrequency( (LARGE_INTEGER *) &Frequency ) ) { return FALSE; }
	else
	{
		QueryPerformanceCounter( (LARGE_INTEGER *) &TimerStart );	
		Period = (float) ( 1.0 / ( (double)Frequency ) );	
	}

	return TRUE;
}

float CTimer::GetTime()				
{
	QueryPerformanceCounter( (LARGE_INTEGER *) &Temp );	
	return ( (float) Temp * Period );
}

float CTimer::GetTimeOffset()				
{
	QueryPerformanceCounter( (LARGE_INTEGER *) &Temp );
	return ( (float) ( Temp - TimerStart ) * Period );
}

void CTimer::Reset() { QueryPerformanceCounter( (LARGE_INTEGER *) &TimerStart ); }

float CTimer::GetTimeOffset2()				
{
	QueryPerformanceCounter( (LARGE_INTEGER *) &Temp );
	return ( (float) ( Temp - TimerStart2 ) * Period );
}

void CTimer::Reset2() { QueryPerformanceCounter( (LARGE_INTEGER *) &TimerStart2 ); }
