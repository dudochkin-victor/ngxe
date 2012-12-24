/*
 *  cTextureManager.h
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/18/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef TEXTMAN
#define TEXTMAN

#include "sTexture.h"
#include "../globals.h"
#include "SDL/SDL_image.h"

//for windows
#ifndef GL_BGR
	#define GL_BGR GL_BGR_EXT
#endif
#ifndef GL_BGRA
	#define GL_BGRA GL_BGRA_EXT
#endif

class cTextureManager
{
	public:
			cTextureManager();
			~cTextureManager();

			uint LoadTextureFromFile(std::string filename);
			uint LoadTextureFromMemory(SDL_Surface* Surface, std::string filename = "");

			void ReloadTextures();

			uint CreateTextureSection(uint TextureId, sSection Section);

			void RemoveTexture(uint TextureId);
            void RemoveTextureSection(uint SectionId);

            void DrawTextureSection(uint SectionId, GLfloat X, GLfloat Y, GLfloat Scale = 1, GLfloat Rotatation = 0, GLfloat red = 1, GLfloat green = 1, GLfloat blue = 1, GLfloat alpha = 1);
            void DrawTexture(uint TextureId, GLfloat X, GLfloat Y, GLfloat Scale = 1, GLfloat Rotatation = 0, GLfloat red = 1, GLfloat green = 1, GLfloat blue = 1, GLfloat alpha = 1);

            GLfloat GetTextureWidth(uint Texture);
            GLfloat GetTextureHeight(uint Texture);

			void RemoveAllTextures();
			void RemoveAllTextureSections();

	protected:
            std::map< uint, sTexture > m_Textures;
            std::map< uint, sTextureSection > m_TextureSections;
};

#endif
