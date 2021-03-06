/*
 * Util.h
 *
 *  Created on: 28.12.2012
 *      Author: blacksmith
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <sys/time.h>
#include <stdio.h>
#include <stdarg.h>
#include <IMAGE.h>

struct ngxTexture {
	GLint id;					//GL texture id
	GLuint format;				//Data format, eg GL_RGBA
	GLuint type;				//Data format, eg GL_RGBA
	unsigned int bpp;			//Image color depth in bits per pixel
	unsigned int width;			//Image width
	unsigned int height;		//Image height
	unsigned char * data;		//Image data
};

class Util {
	static Util *_self;
protected:
	Util();
	virtual ~Util();
	const char* logfile;
public:
	static double getTime() {
		struct timeval now;
		gettimeofday(&now, NULL);
		return now.tv_sec * 1000 + now.tv_usec / 1000;
	}

	static Util* Instance() {
		if (!_self)
			_self = new Util();
		return _self;
	}

	static void log(const char * text, ...) {
		Util::Instance();
		FILE * file;
		if (!Util::Instance()->logfile)
			Util::Instance()->logfile = "logfile.txt";
		va_list arg_list;
		//Initialise varible argument list
		va_start(arg_list, text);

		//Open the file for append
		if ((file = fopen(Util::Instance()->logfile, "a+")) == NULL)
			return;

		//Write the text
		vfprintf(file, text, arg_list);
		putc('\n', file);
		va_end(arg_list);
		//Close the file
		fclose(file);
//		Util::Instance()->_log(text, arg_list);
		return;
	}
	static bool LoadTexture(const char *name, ngxTexture &tex);
	static bool SaveTexture(const char *name, ngxTexture &tex);
};

#endif /* UTIL_H_ */
