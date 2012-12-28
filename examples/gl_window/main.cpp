#include "Global.h"
#include "Application.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application game;
	MSG msg;
	long prevTick, curTick = GetTickCount();

	game.Initialize();
	
	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message==WM_QUIT)break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		prevTick = curTick;
		curTick = GetTickCount();

		game.Update((curTick - prevTick)/1000.f);
		game.Render();
	}
		
	return 0;
}
