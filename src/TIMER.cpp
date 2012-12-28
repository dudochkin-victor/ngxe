//////////////////////////////////////////////////////////////////////////////////////////
//	TIMER.cpp
//	functions for timer
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include "TIMER.h"
#include "Util.h"

void TIMER::Reset()
{
	startTime=Util::getTime();
}

double TIMER::GetTime()
{
	if(isPaused)
		return pauseTime-startTime;
	else
		return (Util::getTime())-startTime;
}

void TIMER::Pause()
{
	if(isPaused)
		return;		//only pause if unpaused

	isPaused=true;
	pauseTime=Util::getTime();
}

void TIMER::Unpause()
{
	if(!isPaused)
		return;		//only unpause if paused

	isPaused=false;
	startTime+=(Util::getTime()-pauseTime);	//update start time to reflect pause
}
