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
#include <string>
#include <AL/alut.h>
#include <map>

struct SoundSource {
	ALuint buffer;
	ALuint source;
	ALint status;
};

class Sound {
public:
	Sound();
	Sound(int* pargc, char** argv);
	virtual ~Sound();
	virtual void play(std::string);
	virtual void init();
private:
	int* pargc;
	char** argv;
	std::map<std::string, SoundSource*> sources;
};

#endif /* SOUND_H_ */
