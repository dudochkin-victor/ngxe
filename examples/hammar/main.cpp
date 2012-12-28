/*
	DEMO.cpp
*/

// Put windows on a diet
#define WIN32_LEAN_AND_MEAN 1

// Uncomment these for easier debugging
//#define MUTE
//#define NOCONFIG
//#define MYDEBUG

// Include stuff
#include "Globals.h"
#include "View.h"
//#include "glext.h"
#include "Minidemo.h"
#include "Config.h"

int main(int argc, char **argv) {
	View * v = new View(&argc, argv);
	// Open a window
	v->createWindow("Mini-Demo 2003", prefs::screenx, prefs::screeny); // , prefs::bitdepth, prefs::fullscreen

	// get window id
//	Window_ID = v->getWindowId();

	// Register the callback function to do the drawing.
	v->onDraw(&Cycle);

	// If there's nothing to do, draw.
	v->onIdle(&Cycle);

	// It's a good idea to know when our window's resized.
//	v->onResize(&cbResizeScene);

	// And let's get some keyboard input.
//	v->onKeyboard(&cbKeyPressed);
//	v->setSpecialHandler(&cbSpecialKeyPressed);

	// OK, OpenGL's ready to go.  Let's call our own init function.
	// Load stuff
	Init();

//#ifdef MYDEBUG
//			// Calculate fps and display it in the titlebar
//			static DWORD fpsTime = GetTickCount();
//			static int frames = 0;
//
//			frames ++;
//
//			if(frames == 30)
//			{
//				DWORD now = GetTickCount();
//				float fps = 30/((now-fpsTime)/1000.0f);
//				fpsTime = now;
//				frames = 0;
//
//				char buffer[20] = {0};
//				sprintf(buffer, "FPS: %7.2f", fps);
//				SetWindowText(pWnd->Handle(), buffer);
//			}
//#endif // MYDEBUG

	// Pass off control to OpenGL.
	// Above functions are called as appropriate.
	v->start();

	return 1;
}
