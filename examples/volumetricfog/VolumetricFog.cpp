#include "stdafx.h"
#include <GL/glut.h>
#include <GL/glaux.h>
#include <GL/glext.h>

void OnDisplay();
void OnReshape(int widht, int height);
void OnIdle();
void Init();

PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;

GLuint g_nWallTex;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Volumetric fog");

	Init();

	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(OnReshape);
	glutIdleFunc(OnIdle);

	glutMainLoop();

	return 1;
}
void LoadTexture(const char* filename, GLenum eWrap, GLuint nTex)
{
	AUX_RGBImageRec* pImg = auxDIBImageLoad(filename);

	glBindTexture(GL_TEXTURE_2D, nTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, eWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, eWrap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, pImg->sizeX, pImg->sizeY, 
		0, GL_RGB, GL_UNSIGNED_BYTE, pImg->data);
	
	free(pImg->data);
	free(pImg);

}

void Init()
{
	float fogColor[4] = {0.5, 0.5, 1.0, 1.0};

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FOG);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

	glHint(GL_FOG_HINT, GL_NICEST);

	glFogi(GL_FOG_MODE, GL_EXP2);
	//glFogi(GL_FOG_COORDINATE_SOURCE_EXT,GL_FRAGMENT_DEPTH_EXT);
	glFogi(GL_FOG_COORDINATE_SOURCE_EXT,GL_FOG_COORDINATE_EXT);
	glFogf(GL_FOG_DENSITY, 0.35);
	glFogfv(GL_FOG_COLOR, fogColor);


	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glGenTextures(1, &g_nWallTex);
	LoadTexture("Walls.bmp", GL_REPEAT, g_nWallTex);

	glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC)wglGetProcAddress("glFogCoordfEXT");
}

void OnDisplay()
{
	int nPatches = 10;
	float density = 5.0;
	float height = 2.0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_TRIANGLES);
	{
		glFogCoordfEXT(0.0);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 2.0, 0.0);
		glFogCoordfEXT(density);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(-1.0, 0.0, 0.0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(1.0, 0.0, 0.0);
	}
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, g_nWallTex);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	{
		// floor
		glFogCoordfEXT(density);
		for (int y=0;y<=nPatches;y++)
		{
			for (int x=0;x<nPatches;x++)
			{
				glTexCoord2f(float(x)/float(nPatches), 1.0 - float(y)/float(nPatches));
				glVertex3f(float(20*x)/float(nPatches)-10.0, 0.0, float(20*y)/float(nPatches)-10.0);
				glTexCoord2f(float(x)/float(nPatches), 1.0 - float(y+1)/float(nPatches));
				glVertex3f(float(20*x)/float(nPatches)-10.0, 0.0, float(20*(y+1))/float(nPatches)-10.0);
				glTexCoord2f(float(x+1)/float(nPatches), 1.0 - float(y+1)/float(nPatches));
				glVertex3f(float(20*(x+1))/float(nPatches)-10.0, 0.0, float(20*(y+1))/float(nPatches)-10.0);
				glTexCoord2f(float(x+1)/float(nPatches), 1.0 - float(y)/float(nPatches));
				glVertex3f(float(20*(x+1))/float(nPatches)-10.0, 0.0, float(20*y)/float(nPatches)-10.0);
			}
		}

		// ceiling
		glFogCoordfEXT(0.0);
		for (y=0;y<=nPatches;y++)
		{
			for (int x=0;x<nPatches;x++)
			{
				glTexCoord2f(float(x)/float(nPatches), 1.0 - float(y)/float(nPatches));
				glVertex3f(float(20*x)/float(nPatches)-10.0, 10.0, float(20*y)/float(nPatches)-10.0);
				glTexCoord2f(float(x+1)/float(nPatches), 1.0 - float(y)/float(nPatches));
				glVertex3f(float(20*(x+1))/float(nPatches)-10.0, 10.0, float(20*y)/float(nPatches)-10.0);
				glTexCoord2f(float(x+1)/float(nPatches), 1.0 - float(y+1)/float(nPatches));
				glVertex3f(float(20*(x+1))/float(nPatches)-10.0, 10.0, float(20*(y+1))/float(nPatches)-10.0);
				glTexCoord2f(float(x)/float(nPatches), 1.0 - float(y+1)/float(nPatches));
				glVertex3f(float(20*x)/float(nPatches)-10.0, 10.0, float(20*(y+1))/float(nPatches)-10.0);
			}
		}

		// back wall
		for (y=0;y<=nPatches;y++)
		{
			for (int x=0;x<nPatches;x++)
			{
				if (float(10*y)/float(nPatches) > height)
					glFogCoordfEXT(0);
				else
					glFogCoordfEXT(density * (1.0-(float(10*y)/float(nPatches))/height));

				glTexCoord2f(float(x)/float(nPatches), 1.0 - float(y)/float(nPatches));
				glVertex3f(float(20*x)/float(nPatches)-10.0, float(10*y)/float(nPatches), -10.0);
				glTexCoord2f(float(x+1)/float(nPatches), 1.0 - float(y)/float(nPatches));
				glVertex3f(float(20*(x+1))/float(nPatches)-10.0, float(10*y)/float(nPatches), -10.0);

				if (float(10*(y+1))/float(nPatches) > height)
					glFogCoordfEXT(0);
				else
					glFogCoordfEXT(density * (1.0-(float(10*(y+1))/float(nPatches))/height));

				glTexCoord2f(float(x+1)/float(nPatches), 1.0 - float(y+1)/float(nPatches));
				glVertex3f(float(20*(x+1))/float(nPatches)-10.0, float(10*(y+1))/float(nPatches), -10.0);
				glTexCoord2f(float(x)/float(nPatches), 1.0 - float(y+1)/float(nPatches));
				glVertex3f(float(20*x)/float(nPatches)-10.0, float(10*(y+1))/float(nPatches), -10.0);
			}
		}

		// right wall
		for (y=0;y<=nPatches;y++)
		{
			for (int x=0;x<nPatches;x++)
			{
				if (float(10*y)/float(nPatches) > height)
					glFogCoordfEXT(0);
				else
					glFogCoordfEXT(density * (1.0-(float(10*y)/float(nPatches))/height));

				glTexCoord2f(float(x)/float(nPatches), 1.0 - float(y)/float(nPatches));
				glVertex3f(10.0, float(10*y)/float(nPatches),float(20*x)/float(nPatches)-10.0);
				glTexCoord2f(float(x+1)/float(nPatches), 1.0 - float(y)/float(nPatches));
				glVertex3f(10.0, float(10*y)/float(nPatches), float(20*(x+1))/float(nPatches)-10.0);

				if (float(10*(y+1))/float(nPatches) > height)
					glFogCoordfEXT(0);
				else
					glFogCoordfEXT(density * (1.0-(float(10*(y+1))/float(nPatches))/height));

				glTexCoord2f(float(x+1)/float(nPatches), 1.0 - float(y+1)/float(nPatches));
				glVertex3f(10.0, float(10*(y+1))/float(nPatches), float(20*(x+1))/float(nPatches)-10.0);
				glTexCoord2f(float(x)/float(nPatches), 1.0 - float(y+1)/float(nPatches));
				glVertex3f(10.0, float(10*(y+1))/float(nPatches), float(20*x)/float(nPatches)-10.0);
			}
		}

		// left wall
		for (y=0;y<=nPatches;y++)
		{
			for (int x=0;x<nPatches;x++)
			{
				if (float(10*y)/float(nPatches) > height)
					glFogCoordfEXT(0);
				else
					glFogCoordfEXT(density * (1.0-(float(10*y)/float(nPatches))/height));

				glTexCoord2f(float(x)/float(nPatches), 1.0 - float(y)/float(nPatches));
				glVertex3f(-10.0, float(10*y)/float(nPatches),float(20*x)/float(nPatches)-10.0);

				if (float(10*(y+1))/float(nPatches) > height)
					glFogCoordfEXT(0);
				else
					glFogCoordfEXT(density * (1.0-(float(10*(y+1))/float(nPatches))/height));

				glTexCoord2f(float(x)/float(nPatches), 1.0 - float(y+1)/float(nPatches));
				glVertex3f(-10.0, float(10*(y+1))/float(nPatches), float(20*x)/float(nPatches)-10.0);
				glTexCoord2f(float(x+1)/float(nPatches), 1.0 - float(y+1)/float(nPatches));
				glVertex3f(-10.0, float(10*(y+1))/float(nPatches), float(20*(x+1))/float(nPatches)-10.0);

				if (float(10*y)/float(nPatches) > height)
					glFogCoordfEXT(0);
				else
					glFogCoordfEXT(density * (1.0-(float(10*y)/float(nPatches))/height));

				glTexCoord2f(float(x+1)/float(nPatches), 1.0 - float(y)/float(nPatches));
				glVertex3f(-10.0, float(10*y)/float(nPatches), float(20*(x+1))/float(nPatches)-10.0);
			}
		}
	}
	glEnd();
	glutSwapBuffers();
}

void OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, float(width)/float(height), 0.1, 25.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 3.0, 12.0, 0.0, 3.0, 0.0, 0.0, 1.0, 0.0);
	
	glutPostRedisplay();
}

void OnIdle()
{
}
