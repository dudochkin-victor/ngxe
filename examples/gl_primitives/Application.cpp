#include "Global.h"
#include "Application.h"

Application::Application()
{
}

Application::~Application()
{
}

void Application::Initialize()
{
	window.Initialize(800, 600, false);	

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void Application::Update(float deltaTime)
{
}

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Points
	glBegin(GL_POINTS);
	glColor3f(1,0,0);	glVertex2f(-0.1f,-0.1f); // red point
	glColor3f(0,1,0);	glVertex2f( 0.1f,-0.1f); // green point
	glColor3f(0,0,1);	glVertex2f( 0.1f, 0.1f); // blue point
	glColor3f(1,1,1);	glVertex2f(-0.1f, 0.1f); // white point
	glEnd();

	// Lines
	glColor3f(0.1f,0.5f,0.1f);
	glBegin(GL_LINES);
	// Horziontal :
	glVertex2f(-1,0);
	glVertex2f(1,0);
	// Vertical :
	glVertex2f(0,-1);
	glVertex2f(0,1);
	glEnd();

	// Red Triangle
	glColor3f(1,0,0);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f,-0.2f);
	glVertex2f(-0.8f,-0.8f); 
	glVertex2f(-0.2f,-0.8f);
	glEnd();

	// Blue Quad
	glColor3f(0,0,1);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(0.2f,0.2f);
	glVertex2f(0.8f,0.2f);
	glVertex2f(0.2f,0.8f);
	glVertex2f(0.8f,0.8f);
	glEnd();

	// Colorful Quad
	glBegin(GL_QUADS);
	glColor3f(1,0,0);	glVertex2f(-0.8f,0.2f);
	glColor3f(0,1,0);	glVertex2f(-0.2f,0.2f);
	glColor3f(0,0,1);	glVertex2f(-0.2f,0.8f);
	glColor3f(1,0,1);	glVertex2f(-0.8f,0.8f);
	glEnd();

	// Five Sided Polygon
	glColor3f(0,1,0);
	glBegin(GL_POLYGON);
	glVertex2f(0.2f,-0.4f);
	glVertex2f(0.3f,-0.8f);
	glVertex2f(0.7f,-0.8f);
	glVertex2f(0.8f,-0.4f);
	glVertex2f(0.5f,-0.2f);
	glEnd();

	window.Swap();
}
