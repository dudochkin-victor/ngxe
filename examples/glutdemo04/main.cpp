//-----------------------------------------------------------------------------
// "Always share your knowledge"
//
// File: main.cpp
//
// Author : JeGX - jegx2002@yahoo.fr - blox.e9h@caramail.com
//
// Date : 13.03.2002
//
// PURPOSE : Win32/Linux OpenGL test. Benchmark on SGI Fuel Platform.
// 
// Copyright (c) 2002 JeGX - BloX.E9h - http://jegx.free.fr
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//
//-----------------------------------------------------------------------------

#include "demo.h"

/////////////////////////////////////////////////////////////////////////////////
// Big Edian routines convzertion
//
Word_ getWord( Word_ w )
{

#ifdef _HOST_LITTLE_EDIAN  
	return( w );
#endif

#ifdef _HOST_BIG_EDIAN  
	Word_ t;
	
	t = w;
	return( (Word_)((w<<8)+(t>>8)) );
#endif

}

Dword_ getDword( Dword_ dw )
{

#ifdef _HOST_LITTLE_EDIAN  
	return( dw );
#endif

#ifdef _HOST_BIG_EDIAN  
	Dword_ t0, t1, t2, t3;
	
	t0 = dw & 0xff000000;
	t1 = dw & 0x00ff0000;
	t2 = dw & 0x0000ff00;
	t3 = dw & 0x000000ff;

	return( (Dword_)(t3<<24) + (Dword_)(t2<<8) + (Dword_)(t1>>8) + (Dword_)(t0>>24) );
#endif

}


/////////////////////////////////////////////////////////////////////////////////
// Pixel Class implementation
//

cRGB_Byte_Pixel::cRGB_Byte_Pixel( Byte_ r_, Byte_ g_, Byte_ b_ )
{

}

void cRGB_Byte_Pixel::set( Byte_ r, Byte_ g, Byte_ b )
{


}

void cRGB_Byte_Pixel::set( cRGB_Byte_Pixel *pixel )
{


}


/////////////////////////////////////////////////////////////////////////////////
// Font management.
// Thanks to NeHe (http://nehe.gamedev.net) for his great site...
//

void buildFont()										// Build Our Font Display List
{
	float	cx;											// Holds Our X Character Coord
	float	cy;											// Holds Our Y Character Coord
	int loop;

	fontBaseIndex=glGenLists(256);								// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, fontTextureID);			// Select Our Font Texture
	for (loop=0; loop<256; loop++)						// Loop Through All 256 Lists
	{
		cx=float(loop%16)/16.0f;						// X Position Of Current Character
		cy=float(loop/16)/16.0f;						// Y Position Of Current Character

		glNewList(fontBaseIndex+loop,GL_COMPILE);				// Start Building A List
			glBegin(GL_QUADS);							// Use A Quad For Each Character
				glTexCoord2f(cx,1-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2i(0,0);						// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i(16,0);						// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,1-cy);			// Texture Coord (Top Right)
				glVertex2i(16,16);						// Vertex Coord (Top Right)
				glTexCoord2f(cx,1-cy);					// Texture Coord (Top Left)
				glVertex2i(0,16);						// Vertex Coord (Top Left)
			glEnd();									// Done Building Our Quad (Character)
			glTranslated(10,0,0);						// Move To The Right Of The Character
		glEndList();									// Done Building The Display List
	}													// Loop Until All 256 Are Built
}

void killFont()									// Delete The Font From Memory
{
	glDeleteLists(fontBaseIndex,256);							// Delete All 256 Display Lists
}

void glPrint(GLint x, GLint y, char *string, int set, float r, float g, float b, float a )	// Where The Printing Happens
{
	float env_color[4];
	
	env_color[0] = r; env_color[1] = g; env_color[2] = b; env_color[3] = 0.1;

	if (set>1)
	{
		set=1;
	}

	glEnable(GL_TEXTURE_2D) ;
	glEnable(GL_BLEND);	
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glBindTexture(GL_TEXTURE_2D, fontTextureID);		// Select Our Font Texture

	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
	glTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, env_color );
	glColor4f( 0.0, 0.0, 0.0, 1.0 ); 

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho( 0, g_width, 0, g_height, -1, 1 );			// Set Up An Ortho Screen
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	glTranslated(x,y,0);								// Position The Text (0,0 - Bottom Left)
	glScalef( 1.5, 1.5, 1.0 );
	glListBase(fontBaseIndex-32+(128*set));				// Choose The Font Set (0 or 1)
	glCallLists(strlen(string),GL_BYTE,string);			// Write The Text To The Screen
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDisable(GL_BLEND);	
	glDisable(GL_TEXTURE_2D) ;
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
	//info = (sBMP_info *)(fileBuffer + sizeof(sBMP_header) );
	info = (sBMP_info *)(fileBuffer + 14 );


	// Check right format. //////////////////////////////////////////////////////

	if( getDword(info->LargeurImage)!= 256 || getDword(info->HauteurImage)!=256 )
	{
		free( fileBuffer );
		printf( "\nLoading image failed. Must be 256x256." );
		return(NULL);
	}

	if( getWord(info->BitParPixel)!= 24 ) 
	{
		free( fileBuffer );
		printf( "\nLoading image failed. Must be 24 bits/pixels." );
		return(NULL);
	}

	if( getDword(info->FormatCompressImage) != 0 )
	{
		free( fileBuffer );
		return(NULL);
	}

	// Retrieval bmp data.
	//data = (Byte_ *)(fileBuffer + sizeof(sBMP_header) + sizeof(sBMP_info) );
	data = (Byte_ *)(fileBuffer + 14 + 40 );

	// Allocate a buffer to store the bitmap data.
	//pixmap = new cRGB_Byte_Pixel[ sizeof(cRGB_Byte_Pixel)*info->LargeurImage*info->HauteurImage ];
	pixmap = new cRGB_Byte_Pixel[ getDword(info->LargeurImage) * getDword(info->HauteurImage) ];
	//pixmap = new cRGB_Byte_Pixel[ 256*256 ];
	if( pixmap==NULL )
	{
		free( fileBuffer );
		return(NULL);
	}


	// Compute the aligned bytes per scanline.
	alignedBytesPerScanline = (((getDword(info->LargeurImage) * 24) + 31) & ~31) >> 3;

	i=0;
	for( y=0; y<getDword(info->HauteurImage); y++ )
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

	g_texture_width = getDword(info->LargeurImage);
	g_texture_height = getDword(info->HauteurImage);

	free( fileBuffer );

	return(pixmap);
}



////////////////////////////////////////////////////////////////////////////////////
// Vector methods
//

cVector::cVector( float dx, float dy, float dz, float dw )
{
	x=dx;
	y=dy;
	z=dz;
	w=dw;
}

void cVector::set( float dx, float dy, float dz, float dw )
{
	x=dx;
	y=dy;
	z=dz;
	w=dw;
}

void cVector::set( cVector &v )
{
	memcpy( this->v, v.v, 16 );
}

float cVector::dot( cVector &v )
{
	return( x*v.x + y*v.y + z*v.z );
}


cVector cVector::cross_v2( cVector &v )
{
	cVector r(	y*v.z - z*v.y, 
				z*v.x - x*v.z, 
				x*v.y - y*v.x   );
	return( r );
}

	
void cVector::normalize()
{
	float norme;
	float root;
	
	norme = x*x + y*y + z*z;
	if( norme<0.0000001 )
	{
		//MessageBox(NULL,"normalize==0","FireGL",MB_OK);
		return;
	}

	root =(float)( 1.0f/(float)sqrt(norme) );
	x *= root;
	y *= root;
	z *= root;
}


float cVector::mag()
{
	return( sqrt(x*x + y*y + z*z) );
}


////////////////////////////////////////////////////////////////////////////////////
// Mesh methods
//

cMesh::cMesh()
{
	m_verticesList=NULL;
    m_facesList=NULL;
    m_numVertices=0;
	m_numFaces=0;
	
	materialAmbient[0]=0.9;
	materialAmbient[1]=0.4;
	materialAmbient[2]=0.1;
	materialAmbient[3]=0.5;
	
	materialDiffuse[0]=0.5;
	materialDiffuse[1]=0.5;
	materialDiffuse[2]=0.5;
	materialDiffuse[3]=0.5;

	materialSpecular[0]=0.1;
	materialSpecular[1]=0.1;
	materialSpecular[2]=0.9;
	materialSpecular[3]=0.5;

 	shininess = 10;


	env_color[0]=0.8;
	env_color[1]=0.8;
	env_color[2]=0.8;
	env_color[3]=0.8;
	
	materialColor[0]=0.8;
	materialColor[1]=0.0;
	materialColor[2]=0.0;
	materialColor[3]=1.0;
	
	vertex=0;

	meshId=-1;

	angle = 0.0;

	m_scale.set( 1.0, 1.0, 1.0 );
	position.x=0.0;
	position.y=0.0;
	position.z=0.0;

}


cMesh::~cMesh()
{
	clear();
}


void cMesh::clear()
{
	if( m_verticesList )
	{
		delete [] m_verticesList;
		m_verticesList=NULL;
	}

	if( m_facesList )
	{
		delete [] m_facesList;
		m_facesList=NULL;
	}
}

void cMesh::preProcess()
{
	Dword_ face, a, b, c;
	cVector u, v, w;
	float x0,y0,z0;
	float x1,y1,z1;
	float x2,y2,z2;

	// Destroy display list if already exists.
	if( glIsList(meshId) )
	{
		glDeleteLists(meshId,1);
	}
	
	// Ask OpenGL for a new display list number //////////////
	meshId = glGenLists(1);
	if( meshId == 0 )
	{
		//TODO : throw an exception
		return;
	}
	
	glNewList( meshId, GL_COMPILE );

	for( face=0; face<m_numFaces; face++ )
	{
		glBegin(GL_TRIANGLES);

			a=m_facesList[face].a;
			x0=m_verticesList[a].x * m_scale.x;
			y0=m_verticesList[a].y * m_scale.y;
			z0=m_verticesList[a].z * m_scale.z;

			b=m_facesList[face].b;
			x1=m_verticesList[b].x * m_scale.x;
			y1=m_verticesList[b].y * m_scale.y;
			z1=m_verticesList[b].z * m_scale.z;

			c=m_facesList[face].c;
			x2=m_verticesList[c].x * m_scale.x;
			y2=m_verticesList[c].y * m_scale.y;
			z2=m_verticesList[c].z * m_scale.x;

			/*// Calcul des composantes de 2 vecteurs (u et v) du triangle
			u.set( x1-x0, y1-y0, z1-z0 );
			v.set( x2-x0, y2-y0, z2-z0 );
			// Calcul du produit vectoriel de w = u cross v
			w = u.cross_v2( v );
			w.normalize();		
			
			// Face normal //////////////
			glNormal3fv( w.v );
			*/
			glNormal3fv( m_facesList[face].n.v );

			glNormal3fv( m_verticesList[a].n.v );
			glTexCoord2d( m_verticesList[a].u, m_verticesList[a].v ); 
			glVertex3f(x0, y0, z0);
		
			glNormal3fv( m_verticesList[b].n.v );
			glTexCoord2d( m_verticesList[b].u, m_verticesList[b].v ); 
			glVertex3f(x1, y1, z1);
		
			glNormal3fv( m_verticesList[c].n.v );
			glTexCoord2d( m_verticesList[c].u, m_verticesList[c].v ); 
			glVertex3f(x2, y2, z2);

		glEnd();
	} // For ...

	glEndList();
}
    
void cMesh::update()
{

}

void cMesh::draw()
{
	glPushMatrix() ;

	switch( polygonMode )
	{
		case MODE_POINT:
		{
			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glPointSize( 2.0 );
			glEnable( GL_POINT_SMOOTH );
			glColor4f(1.0, 1.0, 1.0, 1.0);
			break;
		}

		case MODE_LINE:
		{
			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColor4f(1.0, 1.0, 1.0, 1.0);
			break;
		}

		case MODE_GOURAUD_FILL:
		{
			glEnable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			
			// Enable Material Coloring //////
			glMaterialfv(GL_FRONT, GL_AMBIENT, mesh.materialAmbient );
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mesh.materialDiffuse );
			glMaterialfv(GL_FRONT, GL_SPECULAR, mesh.materialSpecular );
			glMaterialf(GL_FRONT, GL_SHININESS, mesh.shininess );
			break;
		}

		case MODE_TEXTURE_FILL:
		{
			glDisable(GL_LIGHTING);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_TEXTURE_2D) ;
			glBindTexture(GL_TEXTURE_2D, g_texID) ;
			glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
			break;
		}
	}

	glScalef( m_scale.x, m_scale.y, m_scale.z );
	glTranslatef( position.x, position.y, position.z );
	
	glCallList( meshId );

	glPopMatrix() ;
}


////////////////////////////////////////////////////////////////////////////////////
// Build terrain from image.
//

void createTerrain( int nbSubDiv, float echelleVerticale )
{

	int i, j;
	float pas = 20.0/(float)nbSubDiv;
	//float p1[3],p2[3],p3[3],p4[3];
	sVertex *p1, *p2, *p3, *p4;
	sVertex v1, v2, v3;
	Dword_ face;
	float heightMap;

	//float ux, uy, uz;
	//float vx, vy, vz;
	//float wx, wy, wz;
	float norme, root;
	float incx, incy, incz;

	mesh.m_numFaces = nbSubDiv*nbSubDiv*2;
	mesh.m_numVertices = (nbSubDiv+1)*(nbSubDiv+1);

	mesh.m_verticesList = new sVertex[mesh.m_numVertices];
	mesh.m_facesList = new sFace[mesh.m_numFaces];

	// Fill the vertices list /////////////////////////////
	for( j=0; j<nbSubDiv+1; j++ )
	{
		for( i=0; i<nbSubDiv+1; i++ )
		{
			mesh.m_verticesList[j*(nbSubDiv+1) + i].x = -10.0 + (float)i*pas;
			heightMap = elevation(i, j, (float)nbSubDiv, echelleVerticale );
			if( heightMap > g_threshold )
			{
				mesh.m_verticesList[j*(nbSubDiv+1) + i].y = heightMap;
			}
			else
			{
				mesh.m_verticesList[j*(nbSubDiv+1) + i].y = g_threshold;
			}

			mesh.m_verticesList[j*(nbSubDiv+1) + i].z = -10.0 + (float)j*pas;
			mesh.m_verticesList[j*(nbSubDiv+1) + i].u = (float)i/nbSubDiv;
			mesh.m_verticesList[j*(nbSubDiv+1) + i].v = (float)j/nbSubDiv;
		}
	}

	// Fill the faces list /////////////////////////////
	face=0;
	for( j=0; j<nbSubDiv; j++ )
	{
		for( i=0; i<nbSubDiv; i++ )
		{
			
			// Triangle 1 du quad ////////////////////////////////
			p1 = &mesh.m_verticesList[j*(nbSubDiv+1) + i];
			p2 = &mesh.m_verticesList[j*(nbSubDiv+1) + (i+1)];
			p3 = &mesh.m_verticesList[(j+1)*(nbSubDiv+1) + (i+1)];

			v1.x = p2->x - p1->x;
			v1.y = p2->y - p1->y;
			v1.z = p2->z - p1->z;

			v2.x = p3->x - p1->x;
			v2.y = p3->y - p1->y;
			v2.z = p3->z - p1->z;

			incx = v2.y*v1.z - v2.z*v1.y;
			incy = v2.z*v1.x - v2.x*v1.z;
			incz = v2.x*v1.y - v2.y*v1.x;

			norme = incx*incx + incy*incy + incz*incz;
			root =(float)( 1.0f/(float)sqrt(norme+0.000001) );
			incx *= root;
			incy *= root;
			incz *= root;

			p1->n.x += incx;
			p1->n.y += incy;
			p1->n.z += incz;

			p2->n.x += incx;
			p2->n.y += incy;
			p2->n.z += incz;

			p3->n.x += incx;
			p3->n.y += incy;
			p3->n.z += incz;

			mesh.m_facesList[face].a = j*(nbSubDiv+1) + i; // p1
			mesh.m_facesList[face].c = j*(nbSubDiv+1) + (i+1); // p2
			mesh.m_facesList[face].b = (j+1)*(nbSubDiv+1) + (i+1); // p3
			mesh.m_facesList[face].n.x = incx;
			mesh.m_facesList[face].n.y = incy;
			mesh.m_facesList[face].n.z = incz;
			face++;



			// Triangle 2 du quad ////////////////////////////////

			p4 = &mesh.m_verticesList[(j+1)*(nbSubDiv+1) + i];
				
			v3.x = p4->x - p1->x;
			v3.y = p4->y - p1->y;
			v3.z = p4->z - p1->z;

			incx = v3.y*v1.z - v3.z*v1.y;
			incy = v3.z*v1.x - v3.x*v1.z;
			incz = v3.x*v1.y - v3.y*v1.x;

			norme = incx*incx + incy*incy + incz*incz;
			root =(float)( 1.0f/(float)sqrt(norme+0.000001) );
			incx *= root;
			incy *= root;
			incz *= root;

			p1->n.x += incx;
			p1->n.y += incy;
			p1->n.z += incz;

			p3->n.x += incx;
			p3->n.y += incy;
			p3->n.z += incz;

			p4->n.x += incx;
			p4->n.y += incy;
			p4->n.z += incz;

			mesh.m_facesList[face].a = j*(nbSubDiv+1) + i; // p1
			mesh.m_facesList[face].c = (j+1)*(nbSubDiv+1) + (i+1); // p3
			mesh.m_facesList[face].b = (j+1)*(nbSubDiv+1) + i; // p4
			mesh.m_facesList[face].n.x = incx;
			mesh.m_facesList[face].n.y = incy;
			mesh.m_facesList[face].n.z = incz;
			face++;

		}
	}

	// Normalisation des normales ///////////////////////////////////////
	for( j=0; j<nbSubDiv+1; j++ )
	{
		for( i=0; i<nbSubDiv+1; i++ )
		{
			mesh.m_verticesList[j*(nbSubDiv+1) + i].n.normalize();
		}
	}

}


float elevation( int i, int j, float nbSubDiv, float echelleVerticale )
{
	float x_temp, y_temp;
	long x, y;
	long offset;
	float result, valeur;

	x_temp = (float)i/nbSubDiv;
	y_temp = (float)j/nbSubDiv;

	x = (long)(x_temp*255.0);
	y = (long)(y_temp*255.0);

	offset = y*256 + x;
	valeur = terrainMap[ offset ];
	result = (valeur/255.0) * echelleVerticale;
	return( result );
}


void convertToGrayScale( float *terrain, cRGB_Byte_Pixel *pixmap )
{
	long i, j;
	long offset;

	if( !terrain || !pixmap ) return;

	for( j=0; j<256; j++ )
	{
		for( i=0; i<256; i++ )
		{
			offset = j*256 + i;

			terrain[offset] = 0.3*(float)pixmap[offset].r + 0.59*(float)pixmap[offset].g + 0.11*(float)pixmap[offset].b;
		
			//terrain[offset] = (float)sin( i*2.0*3.1416*5000.0 )*255.0;
			//terrain[offset] = (float)( sqrt( (i-128)*(i-128) + (j-128)*(j-128) ) )*2.1;
			
		}
	}
}



void startupDemo()
{
	time=0;
	timebase=0;
	frame=0;
	sprintf( fps_glut, "FPS:init in progress" );
	fpssss = 0.0;
	
	glClearColor(0.9, 0.4, 0.4, 1.0);
	//glClearColor(0.0, 0.0, 0.0, 1.0);

	glDisable( GL_CULL_FACE );

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	// Enable lighting //////////////
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION, g_lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, g_lightAmbient);		
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_lightDiffuse);		

	g_texture_pixmap=NULL;
	
	glEnable(GL_TEXTURE_2D) ;

	// Load and build fonts //////////////////////////////////////////
	
	if( g_texture_pixmap = BMP_Loader( "data/font.bmp" ) )
	{
		glGenTextures( 1, &fontTextureID );

		// Creation d'un nouvel objet de texture //////////////
		glBindTexture(GL_TEXTURE_2D, fontTextureID);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		// Stockage des data dans le nouvel objetde texture .
		glTexImage2D( GL_TEXTURE_2D, 0, 3, g_texture_width, g_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, g_texture_pixmap );
		printf( "\nLoading font texture on board ok." );
	}
	else
	{
		printf( "\nLoading font texture failed!" );
	}

	if( g_texture_pixmap )
	{
		delete [] g_texture_pixmap;
		g_texture_pixmap=NULL;
	}
	
	buildFont();

	
	// Mesh texture loading ////////////////////////////////////////////////////////////

	if( g_texture_pixmap = BMP_Loader( "data/supernova.bmp" ) )
	{
		glGenTextures( 1, &g_texID );

		// Creation d'un nouvel objetde texture //////////////
		glBindTexture(GL_TEXTURE_2D, g_texID);

		// Stockage des data dans le nouvel objetde texture .
		glTexImage2D( GL_TEXTURE_2D, 0, 3, g_texture_width, g_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, g_texture_pixmap );
		printf( "\nLoading texture 1 on board ok." );

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}
	else
	{
		printf( "\nLoading texture 1 failed!" );
	}

	
	g_threshold	 = 0.0;
	convertToGrayScale( terrainMap, g_texture_pixmap );
	createTerrain( terrain_subdivision, g_verticalScale );
	mesh.preProcess();
	mesh.polygonMode = MODE_TEXTURE_FILL;


	// Fog setup ////////////////////////////////////////////////////////
	fogColor[0] = 0.9;
	fogColor[1] = 0.4;
	fogColor[2] = 0.4;
	fogColor[3] = 1.0;
	glFogi( GL_FOG_MODE, GL_LINEAR );
	glFogfv( GL_FOG_COLOR, fogColor );
	glFogf( GL_FOG_START, 12.0 );
	glFogf( GL_FOG_END, 20.0 );

}


void shutdownDemo()
{
	if( g_texture_pixmap )
	{
		delete [] g_texture_pixmap;
		g_texture_pixmap=NULL;
	}

	killFont();
}



void draw()
{
	char buf[256];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	gluLookAt(0.0, y_value, z_value, 
		      0, 0, 0,  
			  0, 1, 0); 


	glPushMatrix();
	
    glRotatef(spin_y, 1, 0, 0);
    glRotatef(spin_x, 0, 1, 0);
	glRotatef(g_angleCamera, 0.0, 1.0, 0.0) ;
	
	///////////////////////////////////////////////////////////////
	// Draw the terrain ///////////////////////////////////////////
	
	mesh.draw();
	

	//////////////////////////////////////////////////////////////
	// Draw the plane ////////////////////////////////////////////
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glDepthMask(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // Set The Blending Function For Translucency
	glColor4f( 0.5, 0.5, 0.5, 0.5 );

	glDisable(GL_LIGHTING);
	
	glEnable(GL_TEXTURE_2D) ;
	glBindTexture(GL_TEXTURE_2D, g_texID) ;
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

	
	glPushMatrix();
	
	glTranslatef(0.0, -1.0, 0.0 ) ;
	glRotatef(180.0, 0.0, 1.0, 0.0) ;

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
	
	glPopMatrix();

	glDisable(GL_TEXTURE_2D) ;
	glDisable(GL_BLEND);
	glDepthMask(1);

	glPopMatrix() ;

	// Print infos //////////////////////////////////////////////////////////////////////////////

	glPrint( 30, g_height-50, "Micro OpenGL Demo 04", 1, 0.0, 1.0, 0.0, 0.9  );
	glPrint( 30, g_height-70, "--------------------", 1, 0.0, 1.0, 0.0, 0.9  );
	
	sprintf( buf, "> Screen Resolution = %d x %d", g_width, g_height );
	glPrint( 30, g_height-90, buf, 0, 0.0, 1.0, 1.0, 0.9  );
	
	sprintf( buf, "> Number of triangles = %lu", mesh.m_numFaces );
	glPrint( 30, g_height-110, buf, 0, 0.0, 1.0, 1.0, 0.9  );

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000)
	{
		fpssss = (float)(frame*(1000.0/(time-timebase)));
		sprintf(fps_glut,"> Frames per second = %.2f", fpssss );
		timebase = time;		
		frame = 0;
	}
	
	glPrint( 30, g_height-130, fps_glut, 0, 0.0, 1.0, 1.0, 0.9  );

	glPrint( 30, g_height-170, "Click on the right button for contextual menu", 0, 0.0, 1.0, 1.0, 0.9  );
	glPrint( 30, g_height-190, "You can move the camera with UP and DOWN keys", 0, 0.0, 1.0, 1.0, 0.9  );

	glPrint( 30, g_height-210, "Visit my web site at http://jegx.free.fr", 0, 0.0, 1.0, 1.0, 0.9  );

	
	//glPrint( g_width/2, g_height/2, "O", 0, 1.0, 1.0, 1.0, 0.9  );


	// Send frame buffer to screen...
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
    gluPerspective(60.0, (GLfloat)width/height, 0.1, 256.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
/*	gluLookAt(0.0, 10.0, 15.0, 
		      0, 0, 0,  
			  0, 1, 0); 

*/	
	glEnable(GL_DEPTH_TEST);
}



////////////////////////////////////////////////////////////////////////////////////
// Idle function for animation
//
void idleFunc(void)
{
	g_angle += 0.2 ;
	g_angleCamera += 0.3; 
	mesh.update();

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

		case 'f':
		{
			fogActive = ~fogActive;
			if( fogActive ) glEnable( GL_FOG );
			else glDisable( GL_FOG );
			break;
		}

		case 'i':
		{
			terrain_subdivision *= 2;
			if( terrain_subdivision > 1024 ) terrain_subdivision = 1024;
			mesh.clear();
			createTerrain( terrain_subdivision, g_verticalScale );
			mesh.preProcess();
			break;
		}

		case 'd':
		{
			terrain_subdivision /= 2;
			if( terrain_subdivision < 2 ) terrain_subdivision = 2;
			mesh.clear();
			createTerrain( terrain_subdivision, g_verticalScale );
			mesh.preProcess();
			break;
		}

		case 'j':
		{
			g_verticalScale += 1.0;
			mesh.clear();
			createTerrain( terrain_subdivision, g_verticalScale );
			mesh.preProcess();
			break;
		}

		case 'k':
		{
			g_verticalScale -= 1.0;
			mesh.clear();
			createTerrain( terrain_subdivision, g_verticalScale );
			mesh.preProcess();
			break;
		}

		case 'w':
		{
			mesh.polygonMode = MODE_POINT;
			break;
		}

		case 'x':
		{
			mesh.polygonMode = MODE_LINE;
			break;
		}

		case 'v':
		{
			mesh.polygonMode = MODE_GOURAUD_FILL;
			break;
		}

		case 'b':
		{
			mesh.polygonMode = MODE_TEXTURE_FILL;
			break;
		}

		default:
			return;
	}
  
	glutPostRedisplay();
}

static void specialFunc( int k, int x, int y)
{
	switch (k)
	{
		case GLUT_KEY_UP:
		{
			z_value -= 1.0;
			if(z_value<1.0 ) z_value = 1.0;
			y_value -= 0.5;
			if(y_value<1.0 ) y_value = 1.0;
			break;
		}

		case GLUT_KEY_DOWN:
		{
			z_value += 1.0;
			if(z_value>20.0 ) z_value = 20.0;
			y_value += 0.5;
			if(y_value>15.0 ) y_value = 15.0;

			break;
		}

		default:
			return;
	}
  
	glutPostRedisplay();
}


void mouseFunc(int button, int state, int x, int y)
{
    old_x = x - old_x;
    old_y = y - old_y;

    glutPostRedisplay();
}

void motionFunc(int x, int y)
{
    spin_x = x - old_x;
    spin_y = y - old_y;

    glutPostRedisplay();
}


void menu(int item)
{
    keyboardFunc((unsigned char)item, 0, 0);
}

void menustate(int state)
{

}


int main(int argc, char** argv)
{
	
	glutInit(&argc, argv) ;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH) ;   
	glutInitWindowSize(640,480) ;

	glutCreateWindow("JeGX's Micro OpenGL Demo - 2002 From Geneva") ;
	
	startupDemo() ;
	
	glutDisplayFunc(draw) ;
	glutReshapeFunc(reshape) ;
	glutIdleFunc(idleFunc) ;
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(specialFunc);
	glutMotionFunc(motionFunc);
	glutMouseFunc(mouseFunc);



    glutMenuStateFunc(menustate);
    glutCreateMenu(menu);
    glutAddMenuEntry("[f]   Fog on/off", 'f');
    glutAddMenuEntry("[i]   Increase terrain precision", 'i');
    glutAddMenuEntry("[d]   Decrease terrain precision", 'd');
    glutAddMenuEntry("[j]   Increase vertical scale", 'j');
    glutAddMenuEntry("[k]   Decrease vertical scale", 'k');
    glutAddMenuEntry("[w]   Polygion mode point", 'w');
    glutAddMenuEntry("[x]   Polygion mode line", 'x');
    glutAddMenuEntry("[v]   Rendering mode gouraud fill", 'v');
    glutAddMenuEntry("[b]   Rendering mode textured fill", 'b');
    glutAddMenuEntry("[Esc] Quit", 27);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop() ;

	shutdownDemo();
	
	return 0;
}



