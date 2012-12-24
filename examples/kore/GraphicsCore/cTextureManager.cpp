/*
 *  cTextureManager.cpp
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "cTextureManager.h"
#include "GraphicsCore.h"

using namespace std;

cTextureManager::cTextureManager()
{
        m_Textures.clear();
}

cTextureManager::~cTextureManager()
{
    //clean up everything
    RemoveAllTextures();
}

uint cTextureManager::LoadTextureFromFile(std::string filename)
{
    //load the image from a file via sdl_img
    SDL_Surface* surface = IMG_Load(filename.c_str());

	if(surface == NULL)
	{
		cout << "Failed to load the image: " << filename << ", Error: " << SDL_GetError() << endl;
		return 0;
	}

    return LoadTextureFromMemory(surface, filename);
}

uint cTextureManager::LoadTextureFromMemory(SDL_Surface* Surface, std::string filename)
{
    GLuint texture;

	glPixelStorei(GL_UNPACK_ALIGNMENT,4);

	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_PixelFormat *fmt = Surface->format;

    //if there is alpha
	if (fmt->Amask)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D,4,Surface->w,Surface->h,GL_BGRA,GL_UNSIGNED_BYTE,Surface->pixels);
	}
	else // no alpha
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D,3,Surface->w,Surface->h,GL_BGR,GL_UNSIGNED_BYTE,Surface->pixels);
	}

	//make our new texture format
	sTexture Tex;

	Tex.Texture = texture;
	Tex.Filename = filename;

	Tex.Width = Surface->w;
	Tex.Height = Surface->h;

	SDL_FreeSurface(Surface);

    //lets find out the next number to use in the map
    uint place;

    if( m_Textures.empty() )
    {
        place = 1;
    }
    else
    {
        place = m_Textures.size() + 1;
    }

    //set the map accordingly and return its id/place
    m_Textures[ place ] = Tex;

    return place;
}

uint cTextureManager::CreateTextureSection(uint TextureId, sSection Section)
{
    //check to see if the texture exists
    if( m_Textures.find( TextureId ) == m_Textures.end() )
    {
        return 0; //return failure
    }

    sTextureSection texSect;

    texSect.Section = Section;
    texSect.TextureId = TextureId;

    //get width and height
    texSect.Height = (Section.vMax - Section.vMin) * m_Textures[TextureId].Height;
    texSect.Width =  (Section.uMax - Section.uMin) * m_Textures[TextureId].Width;

    //get the place to map it to
    uint place;

    //if there was nothing found
    if( m_TextureSections.size() == 0 )
    {
        place = 1;
    }
    else //use the last place + 1
    {
        place = m_TextureSections.size() + 1;
    }

    //set the map accordingly and return its id/place
    m_TextureSections[ place ] = texSect;

    return place;
}

void cTextureManager::RemoveTexture(uint TextureId)
{
    //erase it
    m_Textures.erase(TextureId);
}

GLfloat cTextureManager::GetTextureWidth(uint Texture)
{
    return m_Textures[Texture].Width;
}

GLfloat cTextureManager::GetTextureHeight(uint Texture)
{
    return m_Textures[Texture].Height;
}

void cTextureManager::RemoveTextureSection(uint SectionId)
{
    //erase it
    m_TextureSections.erase(SectionId);
}

void cTextureManager::RemoveAllTextures()
{
    RemoveAllTextureSections();
    m_Textures.clear();
}

void cTextureManager::DrawTextureSection(uint SectionId, GLfloat X, GLfloat Y, GLfloat Scale, GLfloat Rotation, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    //check if the right texture is bound
    if( g_Graphics->CurrentTexture != m_TextureSections[ SectionId ].TextureId )
    {
        //bind texture
        glBindTexture(GL_TEXTURE_2D, m_Textures[ m_TextureSections[SectionId].TextureId].Texture );

        //set graphics varible
        g_Graphics->CurrentTexture = m_TextureSections[ SectionId ].TextureId;
    }

    //scale the points if needed
    GLfloat width = m_TextureSections[ SectionId ].Width;
    GLfloat height = m_TextureSections[ SectionId ].Height;

    glLoadIdentity();
    glTranslatef(g_Camera->GetXposition() + X, g_Camera->GetYposition() + Y, 0.0);
    glScaled(Scale, Scale, 0);
    glRotatef(Rotation,0.0f,0.0f,1.0f);

    //draw the quad
    glBegin(GL_QUADS);
		//Top-left vertex (corner)
		glColor4f(red, green, blue, alpha);
		glTexCoord2f(m_TextureSections[ SectionId ].Section.uMin , m_TextureSections[ SectionId ].Section.vMax);
		glVertex2f(0, 0);

		//Bottom-left vertex (corner)
		glTexCoord2f(m_TextureSections[ SectionId ].Section.uMin , m_TextureSections[ SectionId ].Section.vMin);
		glVertex2f(0, height);

		//Bottom-right vertex (corner)
		glTexCoord2f(m_TextureSections[ SectionId ].Section.uMax , m_TextureSections[ SectionId ].Section.vMin);
		glVertex2f(width, height);

		//Top-right vertex (corner)
		glTexCoord2f(m_TextureSections[ SectionId ].Section.uMax , m_TextureSections[ SectionId ].Section.vMax);
		glVertex2f(width, 0);
	glEnd();
}

void cTextureManager::DrawTexture(uint TextureId, GLfloat X, GLfloat Y, GLfloat Scale, GLfloat Rotation, GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    //check if the right texture is bound
    if( g_Graphics->CurrentTexture != TextureId )
    {
        //bind texture
        glBindTexture(GL_TEXTURE_2D, m_Textures[TextureId].Texture );

        //set graphics varible
        g_Graphics->CurrentTexture = TextureId;
    }

    //scale the points if needed
    GLfloat width = m_Textures[ TextureId ].Width * Scale;
    GLfloat height = m_Textures[ TextureId ].Height * Scale;

    glLoadIdentity();
    glTranslatef(g_Camera->GetXposition() + X, g_Camera->GetYposition() + Y, 0.0);
    glScaled(Scale, Scale, 0);
    glRotatef(Rotation,0.0f,0.0f,1.0f);

    //draw the quad
    glBegin(GL_QUADS);
		//Top-left vertex (corner)
		glColor4f(red, green, blue, alpha);
		glTexCoord2f(0, 1);
		glVertex2f(0, 0);

		//Bottom-left vertex (corner)
		glTexCoord2f(0, 0);
		glVertex2f(0, height);

		//Bottom-right vertex (corner)
		glTexCoord2f(1, 0);
		glVertex2f(width, height);

		//Top-right vertex (corner)
		glTexCoord2f(1, 1);
		glVertex2f(width, Y);
	glEnd();
}

void cTextureManager::ReloadTextures()
{
    //loop through all
    std::map<uint, sTexture>::iterator itr = m_Textures.begin();

    for (itr; itr != m_Textures.end(); itr++)
    {
        //load the image from a file via sdl_img
        SDL_Surface* Surface = IMG_Load(itr->second.Filename.c_str());

        if(Surface == NULL)
        {
            cout << "Failed to load the image: " << itr->second.Filename << ", Error: " << SDL_GetError() << endl;
            return;
        }

        GLuint texture;

        glPixelStorei(GL_UNPACK_ALIGNMENT,4);

        glGenTextures(1,&texture);
        glBindTexture(GL_TEXTURE_2D,texture);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        SDL_PixelFormat *fmt = Surface->format;

        //if there is alpha
        if (fmt->Amask)
        {
            gluBuild2DMipmaps(GL_TEXTURE_2D,4,Surface->w,Surface->h,GL_BGRA,GL_UNSIGNED_BYTE,Surface->pixels);
        }
        else // no alpha
        {
            gluBuild2DMipmaps(GL_TEXTURE_2D,3,Surface->w,Surface->h,GL_BGR,GL_UNSIGNED_BYTE,Surface->pixels);
        }

        SDL_FreeSurface(Surface);

        //set the new texture
        itr->second.Texture = texture;
    }
}

void cTextureManager::RemoveAllTextureSections()
{
    m_TextureSections.clear();
}
