/*
 *  cFontManager.h
 *  Kore
 *
 *  Created by Sean Chapel on 11/21/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef FONTMAN
#define FONTMAN

#include "cFont.h"

class cFontManager
{
	public:
			cFontManager();
			~cFontManager();

            void SetFontPath(std::string Path);
            void SetLetterSpacing(std::string Font, GLfloat Spacing);

			std::string LoadFont(std::string FontName);
            void ReleaseFont(std::string Font);
            void ReleaseAllFonts();

            void DrawText(std::string Font, std::string Text, GLfloat X, GLfloat Y, GLfloat Scale = 1, GLfloat Rotation = 0, GLfloat red = 1, GLfloat green = 1, GLfloat blue = 1, GLfloat alpha = 1);

	private:
        std::string m_FontPath;
        std::map<std::string, sFont> m_Fonts;
};

#endif
