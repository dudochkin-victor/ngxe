/*
 * Sound.cpp
 *
 *  Created on: 27.12.2012
 *      Author: blacksmith
 */

#include "Sound.h"
/**
 * TODO: Need to create cache buffer manager for playing sound files
 */

Sound::Sound() {
	// TODO Auto-generated constructor stub
}

Sound::~Sound() {
	// TODO Auto-generated destructor stub
}

void Sound::init()
{
	if (!alutInit(0, NULL)) {
		ALenum error = alutGetError();
		fprintf(stderr, "%s\n", alutGetErrorString(error));
		exit (EXIT_FAILURE);
	}
}
