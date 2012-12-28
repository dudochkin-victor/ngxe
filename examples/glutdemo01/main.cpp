//-----------------------------------------------------------------------------
// "Always share your knowledge"
//
// File: main.h
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

#include "demo.h"



cRGB_Byte_Pixel::cRGB_Byte_Pixel( Byte_ r_, Byte_ g_, Byte_ b_ )
{

}

void cRGB_Byte_Pixel::set( Byte_ r, Byte_ g, Byte_ b )
{


}

void cRGB_Byte_Pixel::set( cRGB_Byte_Pixel *pixel )
{


}


////////////////////////////////////////////////////////////////////////////////////
// The drawString function
//
void drawString( char *text, float x, float y )
{

	char *c;
	int font=(int)GLUT_BITMAP_8_BY_13;

	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	
	// Set the font color /////////////////////////////////////////
	glColor3f( 1.0, 1.0, 0.0 );
	
	// setOrthographicProjection ////////////////////////////////////
	
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D( 0, g_width, 0, g_height );
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -g_height, 0);
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
	glLoadIdentity();
	
	// draw string //////////////////////////////////////////
	
	// set position to start drawing fonts
	glRasterPos2f( x, y );
	
	// loop all the characters in the string
	
	for( c=text; *c != '\0'; c++)
	{
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *c);
	}
	
	glPopMatrix();
	
	// resetPerspectiveProjection /////////////////////////////
	
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
	
	
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	
}


////////////////////////////////////////////////////////////////////////////////////
// The BMP loader function
//
cRGB_Byte_Pixel *BMP_Loader( char *filename )
{
	FILE *fp;
	sBMP_header *header;
	sBMP_info *info;
	Dword_ alignedBytesPerScanline;
	Dword_ fileSize;
	Byte_ *fileBuffer;
	Byte_ *data;
	Dword_ i, x, y;
	cRGB_Byte_Pixel *pixel;
	cRGB_Byte_Pixel *pixmap;



	if( (fp = fopen(filename, "rb")) == NULL )
	{
		return(NULL);
	}

	// Move fp to the end.
	fseek( fp, 0 ,SEEK_END );
	
	// get the file size.
	fileSize = (Dword_)ftell( fp );

	// Move fp to the beginning.
	fseek( fp, 0 ,SEEK_SET );

	// Alloc a buffer to store the whole file
	fileBuffer = (Byte_ *)malloc( fileSize * sizeof(Byte_) );
	if( fileBuffer==NULL )
	{
		return(NULL);
	}

	// Read file entirely.
	fread( fileBuffer, fileSize * sizeof(Byte_), 1, fp );

	// Ok, the is in buffer, so we can close the file.
	fclose( fp );

	// Retrieval of bmp file header structure.
	header = (sBMP_header *)fileBuffer;

	// Retrieval of bmp file info structure.
	info = (sBMP_info *)(fileBuffer + sizeof(sBMP_header) );


	// Check right format.

	if( info->BitParPixel != 24 ) 
	{
		free( fileBuffer );
		return(NULL);
	}

	if( info->FormatCompressImage != 0 )
	{
		free( fileBuffer );
		return(NULL);
	}

	// Retrieval bmp data.
	data = (Byte_ *)(fileBuffer + sizeof(sBMP_header) + sizeof(sBMP_info) );

	// Allocate a buffer to store the bitmap data.
	pixmap = new cRGB_Byte_Pixel[ sizeof(cRGB_Byte_Pixel)*info->LargeurImage*info->HauteurImage ];
	if( pixmap==NULL )
	{
		free( fileBuffer );
		return(NULL);
	}


	// Compute the aligned bytes per scanline.
	alignedBytesPerScanline = (((info->LargeurImage * 24) + 31) & ~31) >> 3;

	i=0;
	for( y=0; y<info->HauteurImage; y++ )
	{
		pixel = (cRGB_Byte_Pixel *)(data + (y * alignedBytesPerScanline));

		for( x=0; x<info->LargeurImage; x++ )
		{
			( (cRGB_Byte_Pixel*)pixmap + i )->r = pixel->b; 
			( (cRGB_Byte_Pixel*)pixmap + i )->g = pixel->g; 
			( (cRGB_Byte_Pixel*)pixmap + i )->b = pixel->r; 
			i++;
			pixel++;
		}
	}

	g_texture_width = info->LargeurImage;
	g_texture_height = info->HauteurImage;

	free( fileBuffer );

	return(pixmap);
}




void startupDemo()
{
	int x, y, z, i;

	time=0;
	timebase=0;
	frame=0;
	sprintf( fps_glut, "FPS:init in progress" );
	fpssss = 0.0;

	
	glClearColor(0.0,0.0,0.0,1.0) ;

	//loadTextures();				

	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	// Enable lighting //////////////
	glEnable(GL_LIGHTING);
	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION, g_lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, g_lightAmbient);		
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_lightDiffuse);		

	// Enable Material Coloring //////
    glMaterialfv(GL_FRONT, GL_AMBIENT, g_materialAmbient );
    glMaterialfv(GL_FRONT, GL_DIFFUSE, g_materialDiffuse );
    glMaterialfv(GL_FRONT, GL_SPECULAR, g_materialSpecular );
    glMaterialf(GL_FRONT, GL_SHININESS, g_shininess );




	// Loading de la texture pour la boite autour du tore //////////////////////////////
	if( g_texture_pixmap = BMP_Loader( "data/virus-128x128.bmp" ) )
	{
		glGenTextures( 1, &g_texID );

		// Creation d'un nouvel objetde texture //////////////
		glBindTexture(GL_TEXTURE_2D, g_texID);

		// Stockage des data dans le nouvel objetde texture /////////////////////////////////////////////////////////////////////////////////
		glTexImage2D( GL_TEXTURE_2D, 0, 3, g_texture_width, g_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, g_texture_pixmap );

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

		// S et T : coordonnes du texel (texture element) ////////////////
		// GL_REPEAT:repetition du motif dans le sens des S (GL_TEXTURE_WRAP_S)
		// ou des T (GL_TEXTURE_WRAP_T) ///////////////////////////////////////
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    
		// Positionne la fonction de texture active (cioe les valeurs des texels
		// peuvent moduler la couleur dans laquelle le polygone serait rendu en
		// l'absence de texture, ou plus simplement comment sont combinees les
		// valeurs des texels avec les valeurs chromatiques du fragment traite)... ouf!
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	else
	{
		printf( "\nLoading texture failed!" );
	}

	
	
	i=0;

	for( x=0; x<3; x++ )
	{
		for( y=0; y<3; y++ )
		{
			for( z=0; z<3; z++ )
			{

				torusCenters[i].x = -50.0 + x*DIST_BETWEEN_TORUS;
				torusCenters[i].y = 30.0 + y*DIST_BETWEEN_TORUS;
				torusCenters[i].z = -50.0 + z*DIST_BETWEEN_TORUS;
				i++;
			}
		}
	}

	displayListId = glGenLists(1);
	glNewList( displayListId, GL_COMPILE );
	glutSolidTorus( 9.0, 15.0, 20, 20 ) ;
	glEndList();

}



void shutdownDemo()
{

	if( g_texture_pixmap )
	{
		delete [] g_texture_pixmap;
		g_texture_pixmap=NULL;
	}

}


void draw()
{
	int i;
	char buf[256];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;


	glColor4f(0.8f, 0.8f, 0.8f, 0.7); // Alpha a 50%					
	
	glPushMatrix();
	
	glRotatef(g_angleCamera, 0.0, 1.0, 0.0) ;


	// Rendu du tore dans la boite. Environment mapping //////////
	glEnable(GL_LIGHTING);

	g_materialAmbient[0] = 0.10;
	g_materialAmbient[0] = 0.3;
	g_materialAmbient[0]= 0.30;
	g_materialAmbient[0]= 0.5;
    glMaterialfv(GL_FRONT, GL_AMBIENT, g_materialAmbient );
	
	for( i=0; i<NUM_TORUS; i++ )
	{
		glPushMatrix() ;
				
		glTranslatef( torusCenters[i].x, torusCenters[i].y, torusCenters[i].z );
		glRotatef(g_angle*10.0,1.0,1.0,1.0) ;
		//glutSolidTorus( 9.0, 15.0, 20, 20 ) ;
		glCallList( displayListId );
				
		glPopMatrix() ;
	}

	
	
	glScalef( 1.0, -1.0, 1.0 );
	
	g_materialAmbient[0] = 0.20;
	g_materialAmbient[0] = 0.30;
	g_materialAmbient[0]= 0.20;
	g_materialAmbient[0]= 0.5;
    glMaterialfv(GL_FRONT, GL_AMBIENT, g_materialAmbient );
	
	for( i=0; i<NUM_TORUS; i++ )
	{
		glPushMatrix() ;
				
		glTranslatef( torusCenters[i].x, torusCenters[i].y, torusCenters[i].z );
		glRotatef(g_angle*10.0,1.0,1.0,1.0) ;
		//glutSolidTorus( 9.0, 15.0, 20, 20 ) ;
		glCallList( displayListId );
				
		glPopMatrix() ;
	}


	// Draw the plane ////////////////////////////////////////////

	glDepthMask(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // Set The Blending Function For Translucency

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D) ;
	glBindTexture(GL_TEXTURE_2D, g_texID) ;

	glBegin( GL_TRIANGLE_STRIP );

		glTexCoord2d( 1.0, 1.0 ); 
		glVertex4fv( v0 );
				
		glTexCoord2d( 0.0, 1.0 ); 
		glVertex4fv( v1 );
		
		glTexCoord2d( 1.0, 0.0 ); 
		glVertex4fv( v2 );
		
		glTexCoord2d( 0.0, 0.0 ); 
		glVertex4fv( v3 );

	glEnd();

	glDisable(GL_TEXTURE_2D) ;
	
	glDisable(GL_BLEND);
	glDepthMask(1);

	glPopMatrix() ;
	
	drawString( "Micro OpenGL Demo 01" , 20, 20 );
	drawString( "==================" , 20, 35 );

	sprintf( buf, "Screen Resolution = %d x %d", g_width, g_height );
	drawString( buf , 20, 60 );

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000)
	{
		fpssss = (float)(frame*(1000.0/(time-timebase)));
		sprintf(fps_glut,"Frames per second = %.2f", fpssss );
		timebase = time;		
		frame = 0;
	}
	
	//drawString( fps_glut , -0.9, -0.7 );
	drawString( fps_glut , 20, 80 );
	//drawString( "jerome" , 0, 0 );

	glutSwapBuffers() ;


}



////////////////////////////////////////////////////////////////////////////////////
// The reshape function
//
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	g_width = width;
	g_height = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width/height, 1.0, 1024.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
	gluLookAt(0.0, 220.0, 300.0, 
		      0, 0, 0,  
			  0, 1, 0); 

	
	glEnable(GL_DEPTH_TEST);
}



////////////////////////////////////////////////////////////////////////////////////
// Idle function for animation
//
void idleFunc(void)
{

	g_angle += 0.2 ;
	g_angleCamera += 0.3; 
	//tex0 += 0.001; 
	//tex1 += 0.001;
	//tex2 += 0.001;
	//tex3 += 0.001;

   glutPostRedisplay();
}



static void keyboardFunc(unsigned char k, int x, int y)
{
	switch (k)
	{
		case 27:
		{
			shutdownDemo();
			exit(0);
			break;
		}

		default:
			return;
	}
  
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	
	glutInit(&argc, argv) ;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH) ;   
	glutInitWindowSize(1024,768) ;

	glutCreateWindow("JeGX GLUT OpenGL Demo") ;
	
	startupDemo() ;
	
	glutDisplayFunc(draw) ;
	glutReshapeFunc(reshape) ;
	glutIdleFunc(idleFunc) ;
	glutKeyboardFunc(keyboardFunc);

	glutMainLoop() ;

	shutdownDemo();
	
	return 0;
}
