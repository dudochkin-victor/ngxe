//////////////////////////////////////////////////////////////////////////////////////////
//	box.cpp
//	Draw a simple box to enclose the scene
//	Downloaded from: www.paulsprojects.net
//	Created:	4th September 2002
//
//	Copyright (c) 2006, Paul Baker
//	Distributed under the New BSD Licence. (See accompanying file License.txt or copy at
//	http://www.paulsprojects.net/NewBSDLicense.txt)
//////////////////////////////////////////////////////////////////////////////////////////	
#include <windows.h>
#include <GL\gl.h>
#include "box.h"

void DrawBox()
{
	int quadsPerEdge=16;

	//Draw Far side
	glNormal3f(0.0f, 0.0f, 1.0f);

	//Loop through strips
	for(int i=0; i<quadsPerEdge; ++i)
	{
		glBegin(GL_TRIANGLE_STRIP);
		{
			for(int j=0; j<quadsPerEdge+1; ++j)
			{
				glVertex3f(-5.0f+10.0f*i/quadsPerEdge, -5.0f+10.0f*j/quadsPerEdge, -5.0f);
				glVertex3f(-5.0f+10.0f*(i+1)/quadsPerEdge, -5.0f+10.0f*j/quadsPerEdge, -5.0f);
			}
		}
		glEnd();
	}

	//Draw Near side
	glNormal3f(0.0f, 0.0f,-1.0f);

	//Loop through strips
	for(int i=0; i<quadsPerEdge; ++i)
	{
		glBegin(GL_TRIANGLE_STRIP);
		{
			for(int j=0; j<quadsPerEdge+1; ++j)
			{
				glVertex3f(-5.0f+10.0f*(i+1)/quadsPerEdge, -5.0f+10.0f*j/quadsPerEdge, 5.0f);
				glVertex3f(-5.0f+10.0f*i/quadsPerEdge, -5.0f+10.0f*j/quadsPerEdge, 5.0f);
			}
		}
		glEnd();
	}

	//Draw left side
	glNormal3f(1.0f, 0.0f, 0.0f);

	//Loop through strips
	for(int i=0; i<quadsPerEdge; ++i)
	{
		glBegin(GL_TRIANGLE_STRIP);
		{
			for(int j=0; j<quadsPerEdge+1; ++j)
			{
				glVertex3f(-5.0f, -5.0f+10.0f*i/quadsPerEdge, -5.0f+10.0f*j/quadsPerEdge);
				glVertex3f(-5.0f, -5.0f+10.0f*(i+1)/quadsPerEdge, -5.0f+10.0f*j/quadsPerEdge);
			}
		}
		glEnd();
	}

	//Draw right side
	glNormal3f(-1.0f, 0.0f, 0.0f);

	//Loop through strips
	for(int i=0; i<quadsPerEdge; ++i)
	{
		glBegin(GL_TRIANGLE_STRIP);
		{
			for(int j=0; j<quadsPerEdge+1; ++j)
			{
				glVertex3f(5.0f, -5.0f+10.0f*(i+1)/quadsPerEdge, -5.0f+10.0f*j/quadsPerEdge);
				glVertex3f(5.0f, -5.0f+10.0f*i/quadsPerEdge, -5.0f+10.0f*j/quadsPerEdge);
			}
		}
		glEnd();
	}

	//Draw top
	glNormal3f(0.0f,-1.0f, 0.0f);

	//Loop through strips
	for(int i=0; i<quadsPerEdge; ++i)
	{
		glBegin(GL_TRIANGLE_STRIP);
		{
			for(int j=0; j<quadsPerEdge+1; ++j)
			{
				glVertex3f(-5.0f+10.0f*i/quadsPerEdge, 5.0f, -5.0f+10.0f*j/quadsPerEdge);
				glVertex3f(-5.0f+10.0f*(i+1)/quadsPerEdge, 5.0f, -5.0f+10.0f*j/quadsPerEdge);
			}
		}
		glEnd();
	}

	//Draw bottom
	glNormal3f(0.0f, 1.0f, 0.0f);

	//Loop through strips
	for(int i=0; i<quadsPerEdge; ++i)
	{
		glBegin(GL_TRIANGLE_STRIP);
		{
			for(int j=0; j<quadsPerEdge+1; ++j)
			{
				glVertex3f(-5.0f+10.0f*(i+1)/quadsPerEdge,-5.0f, -5.0f+10.0f*j/quadsPerEdge);
				glVertex3f(-5.0f+10.0f*i/quadsPerEdge,-5.0f, -5.0f+10.0f*j/quadsPerEdge);
			}
		}
		glEnd();
	}
}
