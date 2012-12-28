//-----------------------------------------------------------------------------
// "Always share your knowledge"
//
// File: demo.h
//
// Author : JeGX - jegx2002@yahoo.fr - blox.e9h@caramail.com
//
// Date : 08.03.2002
//
// PURPOSE : Win32/Linux OpenGL test.
// 
// Copyright (c) 2002 JeGX - BloX.E9h - http://jegx.free.fr
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//
//-----------------------------------------------------------------------------

#ifndef _DEMO_H
#define _DEMO_H


#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#pragma comment( lib, "glut32.lib" )



//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Basic types START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

typedef unsigned int Dword_;	// 32 bits //
typedef unsigned short Word_;   // 16 bits //
typedef unsigned char Byte_;	// 8 bits //

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Basic types END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< RGB Pixel class START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

class cRGB_Byte_Pixel
{
public:

	cRGB_Byte_Pixel( Byte_ r_=0, Byte_ g_=0, Byte_ b_=0 );
	void set( Byte_ r, Byte_ g, Byte_ b );
	void set( cRGB_Byte_Pixel *pixel );

	union
	{
		struct
		{
			Byte_ r;
			Byte_ g;
			Byte_ b;
		};

		Byte_ rgb[3];
	};
}; 

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< RGB Pixel class END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>





//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< BMP Structures START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

struct sBMP_header
{
	Word_ codmag;
	Dword_ taille;
	Word_ reserv1;
	Word_ reserv2;
	Dword_ offsetImage;
};


struct sBMP_info
{
	Dword_ TailleZoneInfo;
	Dword_ LargeurImage;
	Dword_ HauteurImage;
	Word_ Nb_plans;								// toujours 1
	Word_ BitParPixel;							// 1/4/8/24
	Dword_ FormatCompressImage;
	Dword_ TailleOctets;
	Dword_ ResolutionHorizon;					// pixel par mètres
	Dword_ ResolutionVertical;					// pixel par mètres
	Dword_ NbCouleurUtil;						// 0 = maximum  si 24 bits alors ce champ spécifie taille palette
	Dword_ NbIndexCouleur;						// 0 = toutes les couleur importantes						
};


struct sImageBMP
{
	sBMP_header header;
	sBMP_info info;
	cRGB_Byte_Pixel *data;
};

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< BMP Structures END >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


float g_lightPosition[]= {0.0, 30.0, 200.0, 0.9 };
float g_lightAmbient[]=  { 0.9f, 0.9f, 0.9f, 0.9 };
float g_lightDiffuse[]=  { 0.5f, 0.5f, 0.5f, 0.9 };

float g_materialAmbient[]= {0.10, 0.3, 0.10, 0.5};
float g_materialDiffuse[]= {0.5, 0.5, 0.5, 0.5};
float g_materialSpecular[]= {0.1, 0.1, 0.9, 0.5};
float g_shininess=50.0;


float v0[]={ -200.0, 0.0, -200.0, 1.0 }; //p.set( -half_x_size, 0.0f, -half_z_size );
float v1[]={ 200.0, 0.0, -200.0, 1.0 }; //	v1.p.set( half_x_size, 0.0f, -half_z_size );
float v2[]={ -200.0, 0.0, 200.0, 1.0 }; //	v2.p.set( -half_x_size, 0.0f, half_z_size );
float v3[]={ 200.0, 0.0, 200.0, 1.0 }; //	v3.p.set( half_x_size, 0.0f, half_z_size );


struct sVertex
{
	float x;
	float y;
	float z;
};

#define DIST_BETWEEN_TORUS 70.0
#define NUM_TORUS 3*3*3
sVertex torusCenters[NUM_TORUS];
int	displayListId;


float g_angle = 0.0;
float g_angleCamera = 0.0;


cRGB_Byte_Pixel *g_texture_pixmap=NULL;
int g_width;
int g_height;
Dword_ g_texID;

int g_texture_width;
int	g_texture_height;

void drawString( char *text, float x, float y );
cRGB_Byte_Pixel *BMP_Loader( char *filename );
void reshape(int width, int height);
void idleFunc(void);



float time;
float timebase;
int frame;
float frameRate;
char fps[256];
char fps_glut[256];
float fpssss;





#endif