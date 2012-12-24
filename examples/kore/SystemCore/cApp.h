/*
 *  cApp.h
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/15/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#include "../globals.h"

class cApp
{
	public:
			cApp();
			virtual ~cApp();

			virtual void Init() = 0;
			virtual void Run() = 0;
			virtual void Shutdown() = 0;
			virtual void ResizeWindow(int width, int height) = 0;
			virtual void ToggleFullscreen() = 0;

	protected:
				int m_Width;
				int m_Height;
				int m_Bpp;
				std::string m_WindowName;
				bool m_FullScreen;

				SDL_Surface* m_Surface;
				Uint32 m_SdlFlags;
};
