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

//****************************************************************************
// See http://www.gametutorials.com for original code. Great site!
//****************************************************************************


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


	meshId=-1;

	angle = 0.0;

	m_scale.set( 0.5, 0.5, 0.5 );
	position.x=0.0;
	position.y=100.0;
	position.z=0.0;

}


cMesh::~cMesh()
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

			// Calcul des composantes de 2 vecteurs (u et v) du triangle
			u.set( x1-x0, y1-y0, z1-z0 );
			v.set( x2-x0, y2-y0, z2-z0 );
			// Calcul du produit vectoriel de w = u cross v
			w = u.cross_v2( v );
			w.normalize();		

			// Face normal //////////////
			glNormal3fv( w.v );

			glNormal3fv( m_verticesList[a].n.v );
			glVertex3f(x0, y0, z0);
		
			glNormal3fv( m_verticesList[b].n.v );
			glVertex3f(x1, y1, z1);
		
			glNormal3fv( m_verticesList[c].n.v );
			glVertex3f(x2, y2, z2);

		glEnd();
	} // For ...

	glEndList();
}
    

void cMesh::update()
{
	angle +=0.8;
}


void cMesh::draw()
{
	glPushMatrix() ;

	
	glEnable(GL_LIGHTING);
	glColor4f(0.2f, 0.2f, 0.9f, 0.7); // Alpha a 50%					

	glTranslatef( position.x, position.y, position.z );
	glRotatef( angle, 1.0, 1.0, 1.0) ;
	
	glCallList( meshId );
	
	glPopMatrix() ;
}


void cMesh::readLine( Byte_ *line )
{
	int i=0;
	Byte_ b0;
	Byte_ b1;

	memset( line, 0, 256 );

	while( 1 )
	{	
		b0 = *(linePointer);
		b1 = *(linePointer+1);
		
		//if( *(linePointer)==0x0a && *(linePointer+1)==0x0d )
		if( *(linePointer)==0x0a )
		{
			// It's a line end.
			++linePointer;
			return;
		}

		*(line+i) = *(linePointer);
		++linePointer;
		++i;
	}
}


int cMesh::load( char *filename )
{
	FILE *fp;
	char strMessage[255] = {0};
	char map[10];

	Dword_ numVertices,
		   numFaces;

	Dword_ i, j, face,
		 k,
		 a,
		 b,
		 c;
	float x,
		  y,
		  z,
		  u,
		  v;
	float x0,y0,z0;
	float x1,y1,z1;
	float x2,y2,z2;
	float ux,uy,uz; // Vecteur u pour le calcul de la normale
	float wx,wy,wz; // Vecteur w pour le calcul de la normale
	float uwx,uwy,uwz; // Vecteur w pour le calcul de la normale
	float norme, root;
	//float nx,ny,nz;
	int mapping=0;

	cVector r;
	//float rx,ry,rz;
	int nbf;
	sFace *facePtr;
	sVertex *vertex;

	Dword_ fileSize, p;
	Byte_ *fileBuffer;
	Byte_ *line;
	
	float inv_nbf;


/*
01 	
02 Named object: "Sphere01"
03 Tri-mesh. Vertices: 482     Faces: 960
04 Mapped
05 Vertex list:
06 Vertex 0:  X:2.3235     Y:-11.7812     Z:41.8995     U:0.5000     V:0.5000
*/

	printf( "\nLoading mesh - stage 1 : reading file ...", filename );

	if((fp=fopen(filename, "r"))==NULL)
	{
		//printf( "Mesh Loader::load_ASC_file():Unable to find the file:%s", filename );
		return(1);
	}

	// Move fp to the end.
	fseek( fp, 0 ,SEEK_END );
	
	// get the file size.
	fileSize = (Dword_)ftell( fp );

	// Move fp to the beginning.
	fseek( fp, 0 ,SEEK_SET );

	// Alloc a buffer to store the whole file
	fileBuffer = new Byte_[fileSize];
	if( fileBuffer==NULL )
	{
		//sprintf( strMessage, "O3_dataLoader::load_ASC_file():Unable to allocate memory" );
		return(2);
	}

	// Read file entirely.
	fread( fileBuffer, fileSize * sizeof(Byte_), 1, fp );

	// Ok, the is in buffer, so we can close the file.
	fclose( fp );

	line = new Byte_[256];

	linePointer=fileBuffer; 

	readLine( line );
	readLine( line );
	readLine( line );
	//fgets(buffer,256,filePtr);
	//fgets(buffer,256,filePtr);

	sscanf( (char *)line, "Tri-mesh. Vertices: %ld     Faces: %ld", &numVertices, &numFaces);
   	//fscanf(filePtr,"Tri-mesh. Vertices: %ld     Faces: %ld",&numVertices,&numFaces);

	m_numVertices = numVertices;
	m_numFaces = numFaces;
	
	readLine( line );
	sscanf( (char *)line, "%s", map );
	//fgets(buffer,256,fp);
	//fscanf(fp,"%s", map );

	if( !strncmp( map, "Mapped", 6 ) )
	{
		// Il y a les coordonnees de mapping U et V ///////////////////////////
		mapping=1;
		//fgets(buffer,256,filePtr);
		readLine( line );
	}
	
	//fgets(buffer,256,filePtr);
	readLine( line );

	try
	{
		m_verticesList = new sVertex[numVertices];
		m_facesList = new sFace[numFaces];
	}
	catch( ... )
	{
		//sprintf( strMessage, "O3_dataLoader::load_ASC_file():Unable to allocate memory" );
		return(3);
	}

	for(i=0;i<numVertices;i++)
	{
		
		if( mapping )
		{
			//fscanf(filePtr,"Vertex %d: X:%f Y:%f Z:%f U:%f V:%f",&k,&x,&y,&z,&u,&v);
			sscanf( (char *)line, "Vertex %d: X:%f Y:%f Z:%f U:%f V:%f",&k,&x,&y,&z,&u,&v);

			m_verticesList[i].x=x;
			m_verticesList[i].y=y;
			m_verticesList[i].z=z;
			/*if( u>1.0f ) u=1.0f;
			if( u<0.0f ) u=0.0f;
			if( v>1.0f ) v=1.0f;
			if( v<0.0f ) v=0.0f;
			m->meshData.verticesList[i].u=u;
			m->meshData.verticesList[i].v=v;
			m->meshData.transformedVerticesList[i].u=u;
			m->meshData.transformedVerticesList[i].v=v;*/

		}
		else
		{
			//fscanf(filePtr,"Vertex %d: X:%f Y:%f Z:%f",&k,&x,&y,&z);
			sscanf( (char *)line, "Vertex %d: X:%f Y:%f Z:%f",&k,&x,&y,&z );
			m_verticesList[i].x=x;
			m_verticesList[i].y=y;
			m_verticesList[i].z=z;
		}

		//fgets(buffer,256,filePtr);
		readLine( line );

	}
	
	//fgets(buffer,256,filePtr);
	readLine( line );

	for(i=0;i<numFaces;i++)
	{
		//fscanf(filePtr,"Face %d:  A:%d  B:%d  C:%d",&k,&a,&b,&c);
		sscanf( (char *)line, "Face %d:  A:%d  B:%d  C:%d",&k,&a,&b,&c);

		m_facesList[i].a = a;
		m_facesList[i].b = b;
		m_facesList[i].c = c;

		//fgets(buffer,256,filePtr);
		//fgets(buffer,256,filePtr);
		readLine( line );
		readLine( line );
		readLine( line );

		if( mapping )
		{
			//fgets(buffer,256,filePtr);
			readLine( line );
		}

	}

	//fclose(filePtr);
	delete [] line;
	delete [] fileBuffer;


	printf( "\nLoading mesh - stage 2:compute the faces normals...");


	//
	// Precalcul des vecteurs normaux a chaque face
	//
	for(j=0;j<numFaces;j++)
	{
		i=m_facesList[j].a;
		x0=m_verticesList[i].x;
		y0=m_verticesList[i].y;
		z0=m_verticesList[i].z;
		
		i=m_facesList[j].b;
		x1=m_verticesList[i].x;
		y1=m_verticesList[i].y;
		z1=m_verticesList[i].z;
		
		i=m_facesList[j].c;
		x2=m_verticesList[i].x;
		y2=m_verticesList[i].y;
		z2=m_verticesList[i].z;
		
		ux=x1-x0;
		uy=y1-y0;
		uz=z1-z0;

		wx=x2-x0;
		wy=y2-y0;
		wz=z2-z0;

		// Calcul du produit vectoriel de u et w
		uwx = uy*wz - uz*wy;
		uwy = uz*wx - ux*wz;
		uwz = ux*wy - uy*wx;

		// Calcul de la norme du produit vectoriel ///
		norme=uwx*uwx + uwy*uwy + uwz*uwz ;
		root=(float)(1.0f/sqrt(norme+0.00001));

		// Normalisation du vecteur uw ////////////////
		m_facesList[j].n.x = uwx*root;
		m_facesList[j].n.y = uwy*root;
		m_facesList[j].n.z = uwz*root;
		m_facesList[j].n.w = 1.0f;
	}

	// Precalcul des vecteurs normaux a chaque vertex ////////////////////////////////////
	printf( "\nLoading mesh - stage 3:compute the vertices normals...");

	// Init des vecteurs reflexion a chaque sommet ///////////////////////
	vertex = m_verticesList;

	for( p=0; p<numVertices; p++, vertex++ )
	{
		//rx=ry=rz=0.0;
		r.set( 0.0f, 0.0f, 0.0f );
		nbf=0;
		facePtr = m_facesList;
		
		for( face=0; face<numFaces; face++ )
		{
			if( m_facesList[ face ].a==p || m_facesList[ face ].b==p || m_facesList[ face ].c==p )
			{
				r.x += m_facesList[ face ].n.x;
				r.y += m_facesList[ face ].n.y;
				r.z += m_facesList[ face ].n.z;
				nbf++;
			}
		}

		inv_nbf = 1.0/ (float)nbf;
		r.x=r.x*inv_nbf;
		r.y=r.y*inv_nbf;
		r.z=r.z*inv_nbf;
		r.normalize();
		vertex->n.set( r );
	}	

	printf( "\nLoading mesh completed.");
	return(0);
}



void startupDemo()
{
//	int x, y, z, i;

	time=0;
	timebase=0;
	frame=0;
	sprintf( fps_glut, "FPS:init in progress" );
	fpssss = 0.0;
	
	glClearColor(0.0, 0.0, 0.0, 0.9) ;
	//loadTextures();				

	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	// Enable lighting //////////////
	//glEnable(GL_LIGHTING);
	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION, g_lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, g_lightAmbient);		
	glLightfv(GL_LIGHT0, GL_DIFFUSE, g_lightDiffuse);		

	// Mesh texture loading ////////////////////////////////////////////////////////////
	if( g_texture_pixmap = BMP_Loader( "data/blue_dive.bmp" ) )
	{
		glGenTextures( 1, &mesh.textureId );

		// Creation d'un nouvel objetde texture //////////////
		glBindTexture(GL_TEXTURE_2D, mesh.textureId);

		// Stockage des data dans le nouvel objetde texture .
		glTexImage2D( GL_TEXTURE_2D, 0, 3, g_texture_width, g_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, g_texture_pixmap );
		printf( "\nLoading texture 1 on board ok." );

	}
	else
	{
		printf( "\nLoading texture 1 failed!" );
	}

	if( g_texture_pixmap )
	{
		delete [] g_texture_pixmap;
		g_texture_pixmap=NULL;
	}

	// Mesh texture loading ////////////////////////////////////////////////////////////
	if( g_texture_pixmap = BMP_Loader( "data/virus-128x128.bmp" ) )
	//if( g_texture_pixmap = BMP_Loader( "data/texture01.bmp" ) )
	{
		glGenTextures( 1, &g_texID );

		// Creation d'un nouvel objetde texture //////////////
		glBindTexture(GL_TEXTURE_2D, g_texID );

		// Stockage des data dans le nouvel objetde texture .
		glTexImage2D( GL_TEXTURE_2D, 0, 3, g_texture_width, g_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, g_texture_pixmap );
		printf( "\nLoading texture 2 on board ok." );

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
		printf( "\nLoading texture 2 failed!" );
	}

	if( g_texture_pixmap )
	{
		delete [] g_texture_pixmap;
		g_texture_pixmap=NULL;
	}

/*
	if( mesh.load( "data/chrome_logo.asc" ) )
	{
		shutdownDemo();
		exit(0);
	}
	
	mesh.preProcess();

*/



	glEnable(GL_DEPTH_TEST);							// Turn depth testing on

	// This is where we initialize our empty texture to be rendered too.
	// We pass in the texture array to store it, the texture size,
	// the channels (3 for R G B), the type of texture (RGB) and texture ID.
	CreateBlankRenderTexture( &g_tex_0_ID, 512, 3, GL_RGB);

	// Create the texture map for the spinning cube and assign it to texture ID 1
	//CreateTexture(g_Texture, "Brick.bmp", 1);
	g_tex_1_ID = g_texID;
	
}



void shutdownDemo()
{

}


void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The matrix

	
	// 	       Position        View	     Up Vector
	gluLookAt(0, 0, 10,     0, 0, 0,     0, 1, 0);		// Move the camera back by 10 units

	// We want to have the cube rotating along all axis's so we can better see the motion
	// blur at different rotations.  You can press the space bar to stop/start the rotation.

	glRotatef(g_angle, 1.0f, 1.0f, 1.0f);				// Rotate the cubes around the X-axis

	// Below we render to the texture.  We can control how much we render to the texture
	// per second by the LEFT and RIGHT arrow keys.  The more we render to the texture, the
	// smoother the blur is.  

	// Render to the texture only a controlled amount of frames per second
	if( AnimateNextFrame(g_BlurRate) )
	{
		// Shrink the viewport by the current viewport size.  Hit 1, 2, or 3 to change the size.
		glViewport(0, 0, g_Viewport, g_Viewport);								
								
		// This is where we redraw the current rendered texture before we draw the spinning box.
		// This creates the recursive fading of the motion blur.  We pass in the texture ID
		// we are using for the rendered texture.
		//RenderMotionBlur(0);
		RenderMotionBlur( g_tex_0_ID );
	
		// Now we get to render the spinning box to the texture.  We just
		// need to create the box and it will be effected by the current rotation.
		// The parameters are the texture ID, the center (x,y,z), dimensions and uv scale.
		//CreateBox(1,	0, 0, 0,	1, 6, 1,	1, 3);
		CreateBox(g_tex_1_ID,	0, 0, 0,	1, 6, 1,	1, 3);

		// Before we copy the screen to a texture, we need to specify the current
		// texture to draw to by calling glBindTexture() with the appropriate texture 
		//glBindTexture(GL_TEXTURE_2D,g_Texture[0]);				
		glBindTexture(GL_TEXTURE_2D,g_tex_0_ID);				

		// Render the current screen to our texture
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, g_Viewport, g_Viewport, 0);

		// Here we clear the screen and depth bits of the small viewport
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			

		// Set our viewport back to it's normal size
		glViewport(0, 0, g_width, g_height);	
	}

	// Now that we have updated our rendered texture, let us display it to the screen
	// by blending it once again.  This should have all the blur trails rendered to it.
	RenderMotionBlur( g_tex_0_ID );

	// Redraw the box at the same position as it was drawn into the rendered texture
	CreateBox(g_tex_1_ID,	0, 0, 0,	1, 6, 1,	1, 3);


	// If we want rotation, increase the current rotations along each axises
	/*if(g_bRotate)
	{
		g_RotateX += 2; g_RotateY += 3; g_RotateZ += 2;
	}*/

	//drawString( "Fast Motion Blur" , 20, 80 );

	// Swap the backbuffers to the foreground
	glutSwapBuffers() ;



	return;





	char buf[256];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;

	glPushMatrix();

	glRotatef(g_angleCamera, 0.0, 1.0, 0.0) ;


	// Draw the mesh ////////////////////////////////////////////

	mesh.materialAmbient[0]=0.9;
	mesh.materialAmbient[1]=0.4;
	mesh.materialAmbient[2]=0.1;
	mesh.materialAmbient[3]=0.5;
	
	mesh.materialDiffuse[0]=0.5;
	mesh.materialDiffuse[1]=0.5;
	mesh.materialDiffuse[2]=0.5;
	mesh.materialDiffuse[3]=0.5;

	mesh.materialSpecular[0]=0.1;
	mesh.materialSpecular[1]=0.1;
	mesh.materialSpecular[2]=0.9;
	mesh.materialSpecular[3]=0.5;

 	mesh.shininess = 10;
    
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mesh.materialDiffuse );
    glMaterialfv(GL_FRONT, GL_SPECULAR, mesh.materialSpecular );
    glMaterialf(GL_FRONT, GL_SHININESS, mesh.shininess );
    glMaterialfv(GL_FRONT, GL_AMBIENT, mesh.materialAmbient );
	mesh.draw();

	
	glPushMatrix();
	glScalef( 1.0, -1.0, 1.0 );

	mesh.materialAmbient[0]=0.2;
	mesh.materialAmbient[1]=0.2;
	mesh.materialAmbient[2]=0.2;
	mesh.materialAmbient[3]=0.5;
	
	mesh.materialDiffuse[0]=0.2;
	mesh.materialDiffuse[1]=0.2;
	mesh.materialDiffuse[2]=0.2;
	mesh.materialDiffuse[3]=0.2;

	mesh.materialSpecular[0]=0.1;
	mesh.materialSpecular[1]=0.1;
	mesh.materialSpecular[2]=0.1;
	mesh.materialSpecular[3]=0.5;

	mesh.shininess = 5.0;

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mesh.materialDiffuse );
    glMaterialfv(GL_FRONT, GL_SPECULAR, mesh.materialSpecular );
    glMaterialf(GL_FRONT, GL_SHININESS, mesh.shininess );
    glMaterialfv(GL_FRONT, GL_AMBIENT, mesh.materialAmbient );
	mesh.draw();
	
	glPopMatrix() ;


	// Draw the plane ////////////////////////////////////////////

	glDepthMask(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // Set The Blending Function For Translucency

	glDisable(GL_LIGHTING);
	
	glEnable(GL_TEXTURE_2D) ;
	glBindTexture(GL_TEXTURE_2D, g_texID) ;
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glColor4f(0.9f, 0.9f, 0.9f, 0.6); // Alpha a 50%					

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
	
	drawString( "Micro OpenGL Demo 02" , 20, 20 );
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



void OrthoMode(int left, int top, int right, int bottom)
{
	// Switch to our projection matrix so that we can change it to ortho mode, not perspective.
	glMatrixMode(GL_PROJECTION);						

	// Push on a new matrix so that we can just pop it off to go back to perspective mode
	glPushMatrix();									
	
	// Reset the current matrix to our identify matrix
	glLoadIdentity();								

	//Pass in our 2D ortho screen coordinates like so (left, right, bottom, top).  The last
	// 2 parameters are the near and far planes.
	glOrtho( left, right, bottom, top, 0, 1 );	
	
	// Switch to model view so that we can render the scope image
	glMatrixMode(GL_MODELVIEW);								

	// Initialize the current model view matrix with the identity matrix
	glLoadIdentity();										
}

void PerspectiveMode()										// Set Up A Perspective View
{
	// Enter into our projection matrix mode
	glMatrixMode( GL_PROJECTION );							

	// Pop off the last matrix pushed on when in projection mode (Get rid of ortho mode)
	glPopMatrix();											

	// Go back to our model view matrix like normal
	glMatrixMode( GL_MODELVIEW );							

	// We should be in the normal 3D perspective mode now
}


void CreateBox(Dword_ textureID, float x, float y, float z, float width, 
			   float height,  float length, float uscale, float vscale)
{
	// Check if we have a valid texture ID, then bind that texture to our box
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glDisable(GL_LIGHTING);							// Enable Texture Mapping

	if(textureID >= 0) glBindTexture(GL_TEXTURE_2D, textureID);

	// This centers the box around (x, y, z)
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;
	
	
	// Start drawing the side as a QUAD
	glBegin(GL_QUADS);		
		
		// Assign the texture coordinates and vertices for the BACK Side
		glTexCoord2f(0.0f,     0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(0.0f,   vscale); glVertex3f(x,			y + height, z);
		glTexCoord2f(uscale, vscale); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(uscale,   0.0f); glVertex3f(x + width, y,			z);
	
		// Assign the texture coordinates and vertices for the FRONT Side
		glTexCoord2f(uscale,   0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(uscale, vscale); glVertex3f(x,			y + height, z + length);
		glTexCoord2f(0.0f,   vscale); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f,     0.0f); glVertex3f(x + width, y,			z + length);
	
		// Assign the texture coordinates and vertices for the BOTTOM Side
		glTexCoord2f(uscale,   0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(uscale, vscale); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f,   vscale); glVertex3f(x + width, y,			z + length); 
		glTexCoord2f(0.0f,     0.0f); glVertex3f(x + width, y,			z);

		// Assign the texture coordinates and vertices for the TOP Side
		glTexCoord2f(uscale, vscale); glVertex3f(x,			y + height,	z);
		glTexCoord2f(uscale,   0.0f); glVertex3f(x,			y + height,	z + length);
		glTexCoord2f(0.0f,     0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f,   vscale); glVertex3f(x + width, y + height, z);
	
		// Assign the texture coordinates and vertices for the LEFT Side
		glTexCoord2f(uscale,   0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(0.0f,     0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f,   vscale); glVertex3f(x,			y + height,	z + length); 
		glTexCoord2f(uscale, vscale); glVertex3f(x,			y + height,	z);

		// Assign the texture coordinates and vertices for the RIGHT Side
		glTexCoord2f(0.0f,     0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(uscale,   0.0f); glVertex3f(x + width, y,			z + length);
		glTexCoord2f(uscale, vscale); glVertex3f(x + width, y + height,	z + length); 
		glTexCoord2f(0.0f,   vscale); glVertex3f(x + width, y + height,	z);

	// Stop drawing quads
	glEnd();
}


//void CreateBlankRenderTexture( Dword_ textureArray[], int size, int channels, int type, int textureID )										
void CreateBlankRenderTexture( Dword_ *textureID, int size, int channels, int type )										
{
	// Create a pointer to store the blank image data
	Byte_ *pTexture = NULL;											

	// Allocate and init memory for the image array and point to it from pTexture
	pTexture = new Byte_[size * size * channels];
	memset(pTexture, 0, size * size * channels * sizeof(Byte_));	

	// Register the texture with OpenGL and bind it to the texture ID
	//glGenTextures(1, &textureArray[textureID]);								
	glGenTextures(1, textureID);								
	glBindTexture(GL_TEXTURE_2D, *textureID);					
	
	// Create the texture and store it on the video card
	//glTexImage2D(GL_TEXTURE_2D, 0, channels, size, size, 0, type, GL_UNSIGNED_INT, pTexture);						
	glTexImage2D(GL_TEXTURE_2D, 0, channels, size, size, 0, type, GL_UNSIGNED_BYTE, pTexture);						
	
	// Set the texture quality
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Since we stored the texture space with OpenGL, we can delete the image data
	delete [] pTexture;																					
}


//void RenderMotionBlur(int textureID)
void RenderMotionBlur(Dword_ textureID)
{
	// This function was created to blend the rendered texture over top of the screen
	// to create the recursive blur effect.  What happens is we start out by turning
	// off depth testing, setting our blending mode, then binding the texture of our 
	// rendered textured over the QUAD that is about to be created.  Next, we set our 
	// alpha level to %90 of full strength.  This makes it so it will slowly disappear.
	// Before rendering the QUAD, we want to go into ortho mode.  This makes it easier
	// to render a QUAD over the full screen without having to deal with vertices, but
	// 2D coordinates.  Once we render the QUAD, we want to go back to perspective mode.
	// We can then turn depth testing back on and turn off texturing.

	// Push on a new stack so that we do not interfere with the current matrix
	glPushMatrix();

		// Turn off depth testing so that we can blend over the screen
		glDisable(GL_DEPTH_TEST);			

		// Set our blend method and enable blending
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
		//glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);	
		glEnable(GL_BLEND);					

		// Bind the rendered texture to our QUAD
		//glBindTexture(GL_TEXTURE_2D, g_Texture[textureID]);			
		glBindTexture( GL_TEXTURE_2D, textureID );			

		// Decrease the alpha value of the blend by %10 so it will fade nicely
		glColor4f(1, 1, 1, 0.88);
		//glColor4f(0, 0, 0, 0.9f);

		// Switch to 2D mode (Ortho mode)
		//OrthoMode(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		OrthoMode( 0, 0, g_width, g_height );

		// Display a 2D quad with our blended texture
		glBegin(GL_QUADS);

			// Display the top left point of the 2D image
			glTexCoord2f(0.0f, 1.0f);	glVertex2f(0, 0);

			// Display the bottom left point of the 2D image
			//glTexCoord2f(0.0f, 0.0f);	glVertex2f(0, SCREEN_HEIGHT);
			glTexCoord2f(0.0f, 0.0f);	glVertex2f(0, g_height);

			// Display the bottom right point of the 2D image
			//glTexCoord2f(1.0f, 0.0f);	glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
			glTexCoord2f(1.0f, 0.0f);	glVertex2f(g_width, g_height);

			// Display the top right point of the 2D image
			//glTexCoord2f(1.0f, 1.0f);	glVertex2f(SCREEN_WIDTH, 0);
			glTexCoord2f(1.0f, 1.0f);	glVertex2f(g_width, 0);

		// Stop drawing 
		glEnd();
		
		// Let's set our mode back to perspective 3D mode.
		PerspectiveMode();

		// Turn depth testing back on and texturing off.  If you do NOT do these 2 lines of 
		// code it produces a cool flash effect that might come in handy somewhere down the road.
		glEnable(GL_DEPTH_TEST);						
		glDisable(GL_BLEND);							

	// Go back to our original matrix
	glPopMatrix();
}



bool AnimateNextFrame(int desiredFrameRate)
{
	static float lastTime = 0.0f;
	float elapsedTime = 0.0;

	// Get current time in seconds  (milliseconds * .001 = seconds)
    float currentTime = GetTickCount() * 0.001f; 

	// Get the elapsed time by subtracting the current time from the last time
	elapsedTime = currentTime - lastTime;

	// Check if the time since we last checked is over (1 second / framesPerSecond)
    if( elapsedTime > (1.0f / desiredFrameRate) )
    {
		// Reset the last time
        lastTime = currentTime;	

		// Return TRUE, to animate the next frame of animation
        return true;
    }

	// We don't animate right now.
	return false;
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

	g_angle += 1.2 ;
	g_angleCamera += 0.3;

	mesh.update();
	
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

