#include "texture.h"

CTexture::CTexture()
{
	texID = 0;
}

CTexture::~CTexture()
{
	if(glIsTexture(texID)){
		glDeleteTextures(1, &texID);
	}
}

bool CTexture::LoadTexture(char *filename)
{
	FILE *file = fopen(filename, "rb");
	if(file==NULL)return false;

	AUX_RGBImageRec *image = auxDIBImageLoad(filename);
	if(image==NULL)return false;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);

	delete [] image->data;

	return true;
}

bool CTexture::LoadTextureWithAlphaMask(char *texname, char *maskname)
{
	FILE *file = fopen(texname, "rb");
	if(file==NULL)return false;

	file = fopen(maskname, "rb");
	if(file==NULL)return false;

	AUX_RGBImageRec *tex = auxDIBImageLoad(texname);
	AUX_RGBImageRec *mask = auxDIBImageLoad(texname);

	if(tex->sizeX!=mask->sizeX || tex->sizeY!=mask->sizeY){
		MessageBox(NULL, "Texture doesn't match the mask!", "Error", MB_OK);
		return false;
	}

	UCHAR *data = new UCHAR[4*tex->sizeX*tex->sizeY];
	ZeroMemory(data, 4*tex->sizeX*tex->sizeY);

	for(int i=0;i<tex->sizeX*tex->sizeY;i++){
		data[i*4] = tex->data[i*3];
		data[i*4+1] = tex->data[i*3+1];
		data[i*4+2] = tex->data[i*3+2];
		data[i*4+3] = mask->data[i*3];
	}

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->sizeX, tex->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	delete [] data;
	delete [] tex->data;
	delete [] mask->data;

	return true;
}


void CTexture::BindTexture()
{
	glBindTexture(GL_TEXTURE_2D, texID);
}

GLuint CTexture::GetTexID()
{
	return texID;
}
