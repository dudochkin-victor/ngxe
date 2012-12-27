///////////////////////////////////////////////////////////////////////////////
// TextureManager.cpp : Implementation of the TextureManager class.
//  Loads textures into OpenGL, and can retrieve their texture identifiers
// based on character strings. Provides a central texture repository which
// handles texture reloading when re-initialising OpenGL.

// Author: Quin Pendragon (dragon@graduate.uwa.edu.au)
// Date:   2003-01-05

#include "Interface.h"
#include "TextureManager.h"
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------//
// TEXTURELINK: Struct holding a link in the linked list of textures

typedef struct _TEXTURELINK
{
	GLuint value;
	char  *name;
	_TEXTURELINK *next;

	_TEXTURELINK(const char *name, const GLuint id)
	{
		this->name = new char[strlen(name)+1];
		strcpy(this->name, name);
		this->value = id;
		this->next = this;
	}

	~_TEXTURELINK() { delete[] this->name; }

} TEXTURELINK;

//----------------------------------------------------------------//
// implementation of TextureManager methods

TextureManager::TextureManager()
{
	m_texturelist = NULL;
}

TextureManager::~TextureManager()
{
	Reset();
}

typedef unsigned char BYTE;
//Method:  LoadTexture()
//Purpose: Retrieves an OpenGL texture identifier for the given image
//         filename. Loads the texture if necessary.
GLuint TextureManager::LoadTexture(const char *texname)
{
	//bail if no filename
	if (texname == NULL || strlen(texname) == 0) return 0;
	//return texture ID if we already have one
	GLuint texid = GetTexture(texname);
	if (texid != 0) return ((int)texid < 0 ? 0 : texid);
	//do a quick check to make sure it's a bitmap file
	FILE *file = fopen(texname, "r");
	if (!file) { AddTexture(texname, -1); return 0; }
	BYTE bm[2];
	fread(bm, 1, 2, file); //read two things of size one
	fclose(file);
	if (bm[0] != 'B' || bm[1] != 'M')
	{	AddTexture(texname, -1); return 0; }
	//load the texture from the file
//	AUX_RGBImageRec *teximage = auxDIBImageLoad(texname);
//	if (!teximage) { AddTexture(texname, -1); return 0; }
//	//bind the image to an OpenGL texture name
//	glGenTextures(1, &texid);
//	glBindTexture(GL_TEXTURE_2D, texid);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexImage2D(GL_TEXTURE_2D, 0, 3, teximage->sizeX, teximage->sizeY,
//		0, GL_RGB, GL_UNSIGNED_BYTE, teximage->data);
//	//delete the loaded texture, as it's no longer needed
//	if (teximage->data) free(teximage->data);
//	free(teximage);
//	//add the texture to the list, so we won't reload it from disk
//	AddTexture(texname, texid);
	return texid;
}

//Method:  AddTexture(char *name, GLuint texID)
//Purpose: Adds a texture ID (or any other GLuint value you care to specify)
//         to the TextureManager. If the name is already used, its value
//         will be overwritten, otherwise a new entry will be added.
void TextureManager::AddTexture(const char *name, const GLuint texID)
{
	if (m_texturelist == NULL)
		m_texturelist = new TEXTURELINK(name, texID);
	else
	{
		//see if we already have a texture with this name
		TEXTURELINK *head = (TEXTURELINK *)m_texturelist;
		TEXTURELINK *current = head->next;
		do {
			if (strcmp(name, current->name) == 0)
			{   //yep, so update its value and return
				current->value = texID;
				return;
			}
			//nope. NEXT!
			current = current->next;
		} while (current != head);
	    //nope, so insert this link just after the texture
		TEXTURELINK *lnk = new TEXTURELINK(name, texID);
		lnk->next = head->next;
		head->next = lnk;
	}
}

//Method:  GetTexture(char *name)
//Purpose: Retrieves the texture with the given name from the TextureManager, or
//         0 if no texture with the given name is found.
GLuint TextureManager::GetTexture(const char *name)
{
	if (m_texturelist == NULL) return 0;
	TEXTURELINK *head = (TEXTURELINK *)m_texturelist;
	TEXTURELINK *current = head->next;
	do {
		//is this the link we're looking for?
		if (strcmp(name, current->name) == 0)
			return current->value;
		//nope. NEXT!
		current = current->next;
	} while (current != head);
	return 0;
}

//Method:  Reset()
//Purpose: Clears the list of loaded texture identifiers, so that next time
//         they are required they will be reloaded.
void TextureManager::Reset()
{
	if (m_texturelist == NULL) return;
	TEXTURELINK *head = (TEXTURELINK *)m_texturelist;
	//delete all links in the list
	while (head->next != head)
	{
		TEXTURELINK *cur = head->next;
		head->next = cur->next;
		delete cur;
	}
	delete head;
	m_texturelist = NULL;
}
