/*
 * FontManager.cpp
 *
 *  Created on: 28.12.2012
 *      Author: blacksmith
 */

#include <stdlib.h>
#include <algorithm>
#include "Util.h"
#include "FontManager.h"

FontManager::FontManager() {
	// TODO Auto-generated constructor stub

}

FontManager::~FontManager() {
	// TODO Auto-generated destructor stub
}

/// A fairly straight forward function that pushes
/// a projection matrix that will make object world
/// coordinates identical to window coordinates.
inline void pushScreenCoordinateMatrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
	glPopAttrib();
}

/// Pops the projection matrix without changing the current
/// MatrixMode.
inline void pop_projection_matrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

bool FontManager::addFont(const char* fname, unsigned int h) {
	freetype::font_data *our_font = (freetype::font_data*) malloc(
			sizeof(freetype::font_data));
	our_font->init(fname, h);
	//TODO remove extensions && make pretty fontname
	char buffer[35];
	sprintf(buffer, "-%d", h);
	std::string key(fname);
	size_t pos = key.find_last_of("/");
	size_t npos = key.find_last_of(".");
	if (pos != std::string::npos)
		key.assign(key.begin() + pos + 1, key.begin() + npos);
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	key.append(buffer);
	Util::log("FONT %s", key.c_str());
	sources.insert(std::pair<std::string, freetype::font_data*>(key, our_font));
	return false;
}

void FontManager::print(const char * font, float x, float y, const char *fmt,
		...) {
	freetype::font_data *our_font = sources[font];
	if (our_font) {
		const freetype::font_data ft_font = *our_font;

		// We want a coordinate system where things coresponding to window pixels.
		pushScreenCoordinateMatrix();

		GLuint font = ft_font.list_base;
		float h = ft_font.h / .63f;		//We make the height about 1.5* that of

		char text[256];								// Holds Our String
		va_list ap;								// Pointer To List Of Arguments

		if (fmt == NULL)								// If There's No Text
			*text = 0;											// Do Nothing

		else {
			va_start(ap, fmt);
			// Parses The String For Variables
			vsprintf(text, fmt, ap);// And Converts Symbols To Actual Numbers
			va_end(ap);
			// Results Are Stored In Text
		}

		Util::log("%s", text);

		//Here is some code to split the text that we have been
		//given into a set of lines.
		//This could be made much neater by using
		//a regular expression library such as the one avliable from
		//boost.org (I've only done it out by hand to avoid complicating
		//this tutorial with unnecessary library dependencies).
		const char *start_line = text;
		std::vector<std::string> lines;

		const char * c = text;
		;

		//for(const char *c=text;*c;c++) {
		for (; *c; c++) {
			if (*c == '\n') {
				std::string line;
				for (const char *n = start_line; n < c; n++)
					line.append(1, *n);
				lines.push_back(line);
				start_line = c + 1;
			}
		}
		if (start_line) {
			std::string line;
			for (const char *n = start_line; n < c; n++)
				line.append(1, *n);
			lines.push_back(line);
		}

		glPushAttrib(
				GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glListBase(font);

		float modelview_matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

		//This is where the text display actually happens.
		//For each line of text we reset the modelview matrix
		//so that the line's text will start in the correct position.
		//Notice that we need to reset the matrix, rather than just translating
		//down by h. This is because when each character is
		//draw it modifies the current matrix so that the next character
		//will be drawn immediatly after it.
		for (unsigned int i = 0; i < lines.size(); i++) {

			glPushMatrix();
			glLoadIdentity();
			glTranslatef(x, y - h * i, 0);
			glMultMatrixf(modelview_matrix);

			//  The commented out raster position stuff can be useful if you need to
			//  know the length of the text that you are creating.
			//  If you decide to use it make sure to also uncomment the glBitmap command
			//  in make_dlist().
			//	glRasterPos2f(0,0);
			glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
			//	float rpos[4];
			//	glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
			//	float len=x-rpos[0];

			glPopMatrix();

		}

		glPopAttrib();

		pop_projection_matrix();
	} else
		Util::log("Font %s not found", font);
}
