//////////////////////////////////////////////////////////////////////////////////////////
//	FPS_COUNTER.cpp
//	functions to calculate frames per second
//	Downloaded from: www.paulsprojects.net
//	Created:	20th July 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	

#include "FPS_COUNTER.h"
#include <sys/time.h>
#include <stdlib.h>

double timeGetTime() {
	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_sec*1000 + now.tv_usec / 1000;
}

void FPS_COUNTER::Update(void) {
	//keep track of time lapse and frame count
	time = timeGetTime() * 0.001f;				//get current time in seconds
	++frames;											//increase frame count

	if (time - lastTime > 1.0f)						//if it has been 1 second
			{
		fps = frames / (time - lastTime);					//update fps number
		lastTime = time;								//set beginning count
		frames = 0L;								//reset frames this second
	}
}
