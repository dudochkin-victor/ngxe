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
}

void Application::Update(float deltaTime)
{
}

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	window.Swap();
}
