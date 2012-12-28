/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Author: Damiano Vitulli <info@spacesimulator.net>
 *
 * ALL RIGHTS RESERVED
 *
 *
 * Tutorial 2: 3d engine - Start to draw using OpenGL!
 * 
 *
 * To compile this project you must include the following libraries:
 * opengl32.lib,glu32.lib,sdl.lib
 *
 */

/*
	port to SDL(www.libsdl.org) by Afrasinei Alexandru alex-toranaga@home.ro
*/
#include "main.h"        
                         
#define MAX_VERTICES 2000 // Max number of vertices (for each object)
#define MAX_POLYGONS 2000 // Max number of polygons (for each object)
			        
/**********************************************************
 *
 * TYPES DECLARATION
 *
 *********************************************************/

/*** Our vertex type ***/
typedef struct{
    float x,y,z;
}vertex_type;

/*** The polygon (triangle), 3 numbers that aim 3 vertices ***/
typedef struct{
    int a,b,c;
}polygon_type;

/*** The object type ***/
typedef struct { 
    vertex_type vertex[MAX_VERTICES]; 
    polygon_type polygon[MAX_POLYGONS];
} obj_type, *obj_type_ptr;



/**********************************************************
 *
 * VARIABLES DECLARATION
 *
 *********************************************************/


/*** Absolute rotation values (0-359 degrees) and rotiation increments for each frame ***/
double rotation_x=0, rotation_x_increment=0.1;
double rotation_y=0, rotation_y_increment=0.05;
double rotation_z=0, rotation_z_increment=0.03;
 
/*** Flag for rendering as lines or filled polygons ***/
int filling=1; //0=OFF 1=ON

/*** And, finally our first object! ***/
obj_type cube = 
{
    {
        -10, -10, 10,   // vertex v0
        10,  -10, 10,   // vertex v1
        10,  -10, -10,  // vertex v2
        -10, -10, -10,  // vertex v3
        -10, 10,  10,   // vertex v4
        10,  10,  10,   // vertex v5
        10,  10,  -10,  // vertex v6 
        -10, 10,  -10   // vertex v7
    }, 
    {
        0, 1, 4,  // polygon v0,v1,v4
        1, 5, 4,  // polygon v1,v5,v4
        1, 2, 5,  // polygon v1,v2,v5
        2, 6, 5,  // polygon v2,v6,v5
        2, 3, 6,  // polygon v2,v3,v6
        3, 7, 6,  // polygon v3,v7,v6
        3, 0, 7,  // polygon v3,v0,v7
        0, 4, 7,  // polygon v0,v4,v7
        4, 5, 7,  // polygon v4,v5,v7
        5, 6, 7,  // polygon v5,v6,v7
        3, 2, 0,  // polygon v3,v2,v0
        2, 1, 0,  // polygon v2,v1,v0
    }
};


/**********************************************************
 *
 * SUBROUTINE init()
 *
 * Used to initialize OpenGL and to setup our world
 *
 *********************************************************/
void Init()
{
    InitializeOpenGL(SCREEN_WIDTH, SCREEN_HEIGHT);    
    
    if ( SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL) )	 
    {
        std::cout << "Failed enablaing key repeat :" << SDL_GetError() << std::endl;
        Quit(1);
    }  
    
}


/**********************************************************
 *
 * SUBROUTINE display()
 *
 * This is our main rendering subroutine, called each frame
 * 
 *********************************************************/

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();									
	
	int l_index;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clear the background color to dark blue
	glMatrixMode(GL_MODELVIEW); // Modeling transformation
	glLoadIdentity(); // Initialize the model matrix as identity
    
	glTranslatef(0.0,0.0,-50); // We move the object 50 points forward (the model matrix is multiplied by the translation matrix)
 
	rotation_x = rotation_x + rotation_x_increment;
	rotation_y = rotation_y + rotation_y_increment;
	rotation_z = rotation_z + rotation_z_increment;

	if (rotation_x > 359) rotation_x = 0;
	if (rotation_y > 359) rotation_y = 0;
	if (rotation_z > 359) rotation_z = 0;

	glRotatef(rotation_x,1.0,0.0,0.0); // Rotations of the object (the model matrix is multiplied by the rotation matrices)
	glRotatef(rotation_y,0.0,1.0,0.0);
	glRotatef(rotation_z,0.0,0.0,1.0);

	glBegin(GL_TRIANGLES); // GlBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
	for (l_index=0;l_index<12;l_index++)
	{
		glColor3f(1.0,0.0,0.0); // Color for the vertex
		glVertex3f( cube.vertex[ cube.polygon[l_index].a ].x,    cube.vertex[ cube.polygon[l_index].a ].y,    cube.vertex[ cube.polygon[l_index].a ].z);//Vertex definition
		glColor3f(0.0,1.0,0.0);
		glVertex3f( cube.vertex[ cube.polygon[l_index].b ].x,    cube.vertex[ cube.polygon[l_index].b ].y,    cube.vertex[ cube.polygon[l_index].b ].z);
		glColor3f(0.0,0.0,1.0);
		glVertex3f( cube.vertex[ cube.polygon[l_index].c ].x,    cube.vertex[ cube.polygon[l_index].c ].y,    cube.vertex[ cube.polygon[l_index].c ].z);
	}
	glEnd();

	glFlush(); // This force the execution of OpenGL commands								

	SDL_GL_SwapBuffers();									
}


//---------HANDLE KEY PRESS EVENT---------
void HandleKeyPressEvent(SDL_keysym * keysym)
{
    switch(keysym -> sym)                                  
    {
        case SDLK_ESCAPE:                                 
            Quit(0);                                       
	
	case SDLK_UP:
            rotation_x_increment = rotation_x_increment +0.005;
        break;
        case SDLK_DOWN:
            rotation_x_increment = rotation_x_increment -0.005;
        break;
        case SDLK_LEFT:
            rotation_y_increment = rotation_y_increment +0.005;
        break;
        case SDLK_RIGHT:
            rotation_y_increment = rotation_y_increment -0.005;
        break;
    }
}

//---------HANDLE KEY RELEASE EVENT---------
void HandleKeyReleaseEvent(SDL_keysym * keysym)
{
    switch(keysym -> sym)                                 
    {
    
    	case ' ':
            rotation_x_increment=0;
            rotation_y_increment=0;
            rotation_z_increment=0;
        break;
	
        case 'r': case 'R':
            if (filling==0)
            {
                glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
                filling=1;
            }   
            else 
            {
                glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); // Polygon rasterization mode (polygon outlined)
                filling=0;
            }
        break;
                                           
    }
}

//-------------HANDLE MAIN LOOP---------
void MainLoop(void)
{
    bool done = false;                                     
    SDL_Event event;

    while(! done)                                          
    {
        while( SDL_PollEvent(& event) )                    
        {
            switch ( event.type )                          
            {
                case SDL_QUIT :                                        
                    done = true;                                        
                    break;

                case SDL_KEYDOWN :                                      
                    HandleKeyPressEvent( & event. key.keysym );         
                    break;
		
		case SDL_KEYUP :                                                  
                    HandleKeyReleaseEvent( & event. key.keysym );                
                    break;
		    
                case SDL_VIDEORESIZE :                                                 
                    MainWindow = SDL_SetVideoMode( event.resize.w, event.resize.h, SCREEN_DEPTH, VideoFlags );
                    SizeOpenGLScreen(event.resize.w, event.resize.h);   
    
                    if(MainWindow == NULL)                              
                    {
                        std::cout << " Failed resizing SDL window : " << SDL_GetError() << std::endl;   
                        Quit(0);
                    }
                    break;

                default:                                    
                    break;                                  
            } 
        } 
		display();								// Redraw the scene every frame
    } 
}
