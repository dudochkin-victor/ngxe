#pragma once

#include "GLWindow.h"

class Application
{
public:
	Application();
	~Application();
	
	void Initialize();
	void Update(float deltaTime);
	void Render();

private:
	GLWindow window;
};