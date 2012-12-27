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

Sound::Sound() :
		pargc(NULL), argv(NULL) {
	// TODO Auto-generated constructor stub
}

Sound::Sound(int* pargc, char** argv) :
		pargc(NULL), argv(NULL) {
	// TODO Auto-generated constructor stub
	this->pargc = pargc;
	this->argv = argv;
}

Sound::~Sound() {
	// TODO Auto-generated destructor stub
}

void Sound::init() {
	if (this->pargc && this->argv) {
		if (!alutInit(this->pargc, this->argv)) {
			ALenum error = alutGetError();
			fprintf(stderr, "%s\n", alutGetErrorString(error));
			exit(EXIT_FAILURE);
		}
	} else {
		// Not initialized
	}
}
