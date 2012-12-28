//////////////////////////////////////////////////////////////////////////////////////////
//	InitLightTextures.cpp
//	Function to prepare attenuation textures for per-pixel attenuation
//	Downloaded from: www.paulsprojects.net
//	Created:	19th November 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glext.h>
#include <GL\wglext.h>
#include "LOG.h"
#include "WINDOW.h"
#include "extensions/EXT_texture3D_extension.h"
#include "Maths/Maths.h"
#include "InitLightTextures.h"

extern LOG errorLog;

bool InitLightTextures(GLuint & atten1DTexture, GLuint & atten2DTexture, GLuint & atten3DTexture,
					   GLuint & gaussian1DTexture, GLuint & gaussian2DTexture)
{
	//Create a strip of data for our 1d texture
	const int attenSize=32;
	GLubyte * attenData=new GLubyte[attenSize];
	if(!attenData)
	{
		errorLog.OutputError("Unable to create space to hold attenuation texture data");
		return false;
	}

	for(int i=0; i<attenSize; ++i)
	{
		//Get distance from centre to this point
		float dist=(float)i;
		dist-=(float)attenSize/2-0.5f;
		dist/=(float)attenSize/2-0.5f;

		//square and Clamp to [0,1]
		dist=dist*dist;

		if(dist>1.0f)
			dist=1.0f;

		if(dist<0.0f)
			dist=0.0f;

		//Fill this in as color
		attenData[i]=GLubyte(dist*255);

		//Make sure the color is 255 at the edges
		attenData[0]=255;
		attenData[attenSize-1]=255;
	}

	//Make 1d attenuation texture
	glGenTextures(1, &atten1DTexture);
	glBindTexture(GL_TEXTURE_1D, atten1DTexture);
	glTexImage1D(	GL_TEXTURE_1D, 0, GL_INTENSITY8, attenSize,
					0, GL_LUMINANCE, GL_UNSIGNED_BYTE, attenData);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	//Create data for our 2d texture by adding the 1d strips
	//since the attenuation function is separable by addition
	GLubyte * attenData2D=new GLubyte[attenSize*attenSize];
	if(!attenData2D)
	{
		errorLog.OutputError("Unable to create space to hold attenuation texture data");
		return false;
	}
	
	int currentByte=0;
	GLubyte dataI, dataJ;

	for(int i=0; i<attenSize; ++i)
	{
		dataI=attenData[i];
	
		for(int j=0; j<attenSize; ++j)
		{
			GLuint newData;

			dataJ=attenData[j];

			newData=dataI+dataJ;
			if(newData>255)
				newData=255;
			
			attenData2D[currentByte]=newData;
			++currentByte;
		}
	}		

	//2d attenuation
	glGenTextures(1, &atten2DTexture);
	glBindTexture(GL_TEXTURE_2D, atten2DTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_INTENSITY8, attenSize, attenSize,
					0, GL_LUMINANCE, GL_UNSIGNED_BYTE, attenData2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);



	//Create the 3d attenuation texture if supported
	//do this by adding 3 of the 1d strips
	if(EXT_texture3D_supported)
	{
		GLubyte * attenData3D=new GLubyte[attenSize*attenSize*attenSize];
		if(!attenData3D)
		{
			errorLog.OutputError("Unable to create space to hold attenuation texture data");
			return false;
		}
	
		int currentByte=0;
		GLubyte dataI, dataJ, dataK;

		for(int i=0; i<attenSize; ++i)
		{
			dataI=attenData[i];
		
			for(int j=0; j<attenSize; ++j)
			{
				dataJ=attenData[j];

				for(int k=0; k<attenSize; ++k)
				{
					dataK=attenData[k];
					GLuint newData;
	
					newData=dataI+dataJ+dataK;
					if(newData>255)
						newData=255;

					//Invert data as there is no need to do any more summing
					//Thus the invert need not wait until the register combiners
					newData=255-newData;
			
					attenData3D[currentByte]=newData;
					++currentByte;
				}
			}
		}		
		
		glGenTextures(1, &atten3DTexture);
		glBindTexture(GL_TEXTURE_3D_EXT, atten3DTexture);
		glTexImage3DEXT(	GL_TEXTURE_3D_EXT, 0, GL_INTENSITY8, attenSize, attenSize, attenSize,
							0, GL_LUMINANCE, GL_UNSIGNED_BYTE, attenData3D);
		glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		//delete 3d attenuation data
		if(attenData3D)
			delete [] attenData3D;
		attenData3D=NULL;
	}




	//Now make the Gaussian attenuation textures
	for(int i=0; i<attenSize; ++i)
	{
		//Get distance from centre to this point
		float dist=(float)i;
		dist-=(float)attenSize/2-0.5f;
		dist/=(float)attenSize/2-0.5f;

		//Calculate brightness
		float brightness=(float)exp(-(2.5*dist*dist));

		//Fill this in as color
		attenData[i]=GLubyte(brightness*255);

		//Make sure the color is 0 at the edges
		attenData[0]=0;
		attenData[attenSize-1]=0;
	}

	//Make 1d attenuation texture
	glGenTextures(1, &gaussian1DTexture);
	glBindTexture(GL_TEXTURE_1D, gaussian1DTexture);
	glTexImage1D(	GL_TEXTURE_1D, 0, GL_INTENSITY8, attenSize,
					0, GL_LUMINANCE, GL_UNSIGNED_BYTE, attenData);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	//Create data for our 2d texture by multiplying the 1d strips
	//since the attenuation function is separable by multiplication
	currentByte=0;

	for(int i=0; i<attenSize; ++i)
	{
		dataI=attenData[i];
	
		for(int j=0; j<attenSize; ++j)
		{
			dataJ=attenData[j];

			float data1=(float)dataI/255;
			float data2=(float)dataJ/255;
						
			attenData2D[currentByte]=GLubyte(255*data1*data2);
			++currentByte;
		}
	}		

	//2d attenuation
	glGenTextures(1, &gaussian2DTexture);
	glBindTexture(GL_TEXTURE_2D, gaussian2DTexture);
	glTexImage2D(	GL_TEXTURE_2D, 0, GL_INTENSITY8, attenSize, attenSize,
					0, GL_LUMINANCE, GL_UNSIGNED_BYTE, attenData2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);



	//delete 1d and 2d attenuation data
	if(attenData)
		delete [] attenData;
	attenData=NULL;

	if(attenData2D)
		delete [] attenData2D;
	attenData2D=NULL;

	return true;
}