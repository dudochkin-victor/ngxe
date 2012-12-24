/*
 *  example.h
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/15/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef EXAMPLE
#define EXAMPLE

#include "SystemCore/SystemCore.h"
#include "GraphicsCore/GraphicsCore.h"
#include "globals.h"

class Example : cApp
{
public:
	Example();
	~Example();

	void Init();
	void Run();
	void Shutdown();

	void ToggleFullscreen();
	void ResizeWindow(int WidthTable, int height);

	void InitGl();
};

#endif
