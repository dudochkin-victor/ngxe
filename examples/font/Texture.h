#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <stdio.h>

class CTexture
{
private:

	GLuint texID;

public:	

	CTexture();

	~CTexture();

	bool LoadTexture(char *filename);
	
	bool LoadTextureWithAlphaMask(char *texture, char *mask);

	void BindTexture();
	
	GLuint GetTexID();
};
