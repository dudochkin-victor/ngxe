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
	if (!alutExit()) {
		ALenum error = alutGetError();
		fprintf(stderr, "%s\n", alutGetErrorString(error));
		exit(EXIT_FAILURE);
	}
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

void Sound::play(std::string fname) {
	ALenum error;
	SoundSource * sound = sources[fname];
	if ( sound == NULL) {
		sound = (SoundSource*)malloc(sizeof(SoundSource));
		/* Create an AL buffer from the given sound file. */
		sound->buffer = alutCreateBufferFromFile(fname.c_str());
		if (sound->buffer == AL_NONE) {
			error = alutGetError();
			fprintf(stderr, "Error loading file: '%s'\n",
					alutGetErrorString(error));
			alutExit();
			exit(EXIT_FAILURE);
		}

		/* Generate a single source, attach the buffer to it and start playing. */
		alGenSources(1, &sound->source);
		alSourcei(sound->source, AL_BUFFER, sound->buffer);
		sources.insert( std::pair<std::string, SoundSource*>(fname, sound));
	} else
		sound = sources[fname];

	alSourcePlay(sound->source);
	/* Normally nothing should go wrong above, but one never knows... */
	error = alGetError();
	if (error != ALUT_ERROR_NO_ERROR) {
		fprintf(stderr, "%s\n", alGetString(error));
		alutExit();
		exit(EXIT_FAILURE);
	}

	/* Check every 0.1 seconds if the sound is still playing. */
	do {
		alutSleep(0.1f);
		alGetSourcei(sound->source, AL_SOURCE_STATE, &sound->status);
	} while (sound->status == AL_PLAYING);
}
