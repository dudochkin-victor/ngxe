//-----------------------------------------------------------------------------
// "Always share your knowledge"
//
// File: main.cpp
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

	m_scale.set( 0.9, 0.9, 0.9 );
	position.x=0.0;
	position.y=0.0;
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

}


void cMesh::draw()
{
	cVector u, v, w;

	glPushMatrix() ;

	glDisable(GL_LIGHTING);
	
	glColor4f(0.9f, 0.9f, 0.9f, 0.9); 

	glBegin(GL_POINTS);					
	for(Dword_ i=0;i<m_numVertices; i++)
	{
		glColor3f( 1.0, 1.0, 1.0 );
		glVertex3f(m_verticesList[i].x, m_verticesList[i].y, m_verticesList[i].z);
	}
	glEnd();	
	
	
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
		//readLine( line );

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


///////////////////////////////////////////////////////////////////////////////////
// Mesh morphing routines
//

sVertex calculate( Dword_ i)									// Calculates Movement Of Points During Morphing
{
	sVertex a;											// Temporary Vertex Called a
	Dword_ indexS, indexD;

	if( i<sour->m_numVertices ) indexS = i;
	else indexS = sour->m_numVertices-1;
	
	if( i<dest->m_numVertices ) indexD = i;
	else indexD = dest->m_numVertices-1;

	a.x=(sour->m_verticesList[indexS].x-dest->m_verticesList[indexD].x)/(float)morph_steps;	// a.x Value Equals Source x - Destination x Divided By Steps
	a.y=(sour->m_verticesList[indexS].y-dest->m_verticesList[indexD].y)/(float)morph_steps;	// a.y Value Equals Source y - Destination y Divided By Steps
	a.z=(sour->m_verticesList[indexS].z-dest->m_verticesList[indexD].z)/(float)morph_steps;	// a.z Value Equals Source z - Destination z Divided By Steps
	return a;											// Return The Results
}														// This Makes Points Move At A Speed So They All Get To Their

Dword_ max( Dword_ &s, Dword_ &d )
{
	if( s<d )
	{
		isSrcTaller=0;
		return( d );
	}
	else
	{
		isSrcTaller=1;
		return( s );
	}
}


void morphFunc()
{
	sVertex q;								
	Dword_ i;

	// Build vertices list ////////////////////////////////////
	for( i=0; i<helper.m_numVertices; i++ )			
	{									
		if(morph_in_progress) q=calculate(i); 
		else q.x=q.y=q.z=0;			

		helper.m_verticesList[i].x -= q.x;
		helper.m_verticesList[i].y -= q.y;
		helper.m_verticesList[i].z -= q.z;
	}


	if(morph_in_progress && step<=morph_steps) step++; 
	else
	{ 
		morph_in_progress=0;

		if(isSrcTaller)
		{
			for( i=dest->m_numVertices-1; i<sour->m_numVertices; i++ )
			{
				memcpy( &helper.m_verticesList[i], &dest->m_verticesList[dest->m_numVertices-1], sizeof(sVertex) * 1 );
			}
		}
		
		sour=dest;
		step=0;
	}
}


void copyMesh( cMesh *src, cMesh *dst, Dword_ nbMaxVert  )
{
	Dword_ i;

	if( dst->m_verticesList )
	{
		delete [] dst->m_verticesList;
		dst->m_verticesList=NULL;
	}

	dst->m_verticesList = new sVertex[nbMaxVert];

	memcpy( dst->m_verticesList, src->m_verticesList, sizeof(sVertex) * src->m_numVertices );
	dst->m_numVertices = nbMaxVert;

	for( i=src->m_numVertices; i<dst->m_numVertices; i++ )
	{
		memcpy( &dst->m_verticesList[i], &dst->m_verticesList[src->m_numVertices-1], sizeof(sVertex) * 1 );
	}
}

void startupDemo()
{

	time=0;
	timebase=0;
	frame=0;
	sprintf( fps_glut, "FPS:init in progress" );
	fpssss = 0.0;
	
	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	// Disable lighting //////////////
	glDisable(GL_LIGHTING);
	
	// Morphing init ////////////////////////////////////////////
	
	if( mesh1.load( "data/torus576.asc" ) )
	{
		shutdownDemo();
		exit(0);
	}
	
	//if( mesh2.load( "data/sphere550.asc" ) )
	if( mesh2.load( "data/teapot1024.asc" ) )
	{
		shutdownDemo();
		exit(0);
	}

	sour = &mesh1;
	dest = &mesh2;
	
	copyMesh( sour, &helper, max( sour->m_numVertices, dest->m_numVertices ) );
	
	morph_in_progress=1;
}



void shutdownDemo()
{

}


void draw()
{
	char buf[256];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;

	glPushMatrix();
	
	glRotatef(g_angleCamera, 0.0, 1.0, 0.0) ;

	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glPointSize( 2.0 );

	morphFunc();
	helper.draw();

	glPopMatrix() ;
	
	drawString( "Micro OpenGL Demo 8" , 20, 20 );
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
 
	gluLookAt(0.0, 0.0, 100.0, 
		      0, 0, 0,  
			  0, 1, 0); 

	
	glEnable(GL_DEPTH_TEST);
}



////////////////////////////////////////////////////////////////////////////////////
// Idle function for animation
//
void idleFunc(void)
{

	//g_angle += 1.8 ;
	g_angleCamera += 0.3;

	//mesh.update();
	
	//tex0 += 0.001; 
	//tex1 += 0.001;
	//tex2 += 0.001;
	//tex3 += 0.001;

   glutPostRedisplay();
}



static void keyboardFunc(unsigned char k, int x, int y)
{
	Dword_ nbVert;

	switch (k)
	{
		case 27:
		{
			shutdownDemo();
			exit(0);
			break;
		}

		case 'm':
		{
			if( meshSrc!= 2 )
			{
				sour = &mesh2;
				dest = &mesh1;
				nbVert = max( sour->m_numVertices, dest->m_numVertices );
				copyMesh( sour, &helper, nbVert );
				morph_in_progress=1;
				step=0;
				meshSrc=2;
			}
			break;
		}

		case 'l':
		{
			if( meshSrc!= 1 )
			{
				sour = &mesh1;
				dest = &mesh2;
				nbVert = max( sour->m_numVertices, dest->m_numVertices );
				copyMesh( sour, &helper, nbVert );
				morph_in_progress=1;
				step=0;
				meshSrc=1;
			}
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

