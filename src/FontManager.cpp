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

bool FontManager::addFont(const char* fname, unsigned int h){
	freetype::font_data *our_font = (freetype::font_data*) malloc(sizeof(freetype::font_data));
	our_font->init(fname, h);
	//TODO remove extensions && make pretty fontname
	char buffer [35];
	sprintf(buffer,"-%d",h);
	std::string key(fname);
	size_t pos = key.find_last_of("/");
	size_t npos = key.find_last_of(".");
	if(pos != std::string::npos)
		key.assign(key.begin() + pos + 1, key.begin() + npos);
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	key.append(buffer);
	Util::log("FONT %s", key.c_str());
	sources.insert( std::pair<std::string, freetype::font_data*>(key, our_font));
	return false;
}

void FontManager::print(const char * font, float x, float y, const char *fmt, ...){
	freetype::font_data *our_font = sources[font];
	freetype::print(*our_font, 320, 240, "Active FreeType Text - %7.2f", 10.25f);
}
