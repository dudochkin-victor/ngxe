/*
 * Sound.h
 *
 *  Created on: 27.12.2012
 *      Author: blacksmith
 */

#ifndef SOUND_H_
#define SOUND_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <AL/alut.h>

class Sound {
public:
	Sound();
	virtual ~Sound();
	virtual void init();
};

#endif /* SOUND_H_ */
