/*
 * FontManager.h
 *
 *  Created on: 28.12.2012
 *      Author: blacksmith
 */

#ifndef FONTMANAGER_H_
#define FONTMANAGER_H_
#include <map>
#include <FreeType.h>
class FontManager {
public:
	FontManager();
	virtual ~FontManager();
	virtual bool addFont(const char * fname, unsigned int h);
	virtual void print(const char * font, float x, float y, const char *fmt, ...);
private:
	std::map<std::string, freetype::font_data*> sources;
};

#endif /* FONTMANAGER_H_ */
