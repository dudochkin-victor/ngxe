/*
 *  cGraphics.h
 *  Kore
 *
 *  Created by Sean Chapel on 11/21/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#ifndef GRAPHICS
#define GRAPHICS

#include "../globals.h"

class cGraphics
{
	public:
			cGraphics();
			~cGraphics();

			bool Initialize(int Width, int Height);
			void Shutdown();

            uint CurrentTexture;

    protected:
            bool m_Loaded;
};

#endif
