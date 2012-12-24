/*
 *  example.cpp
 *  Kore-Engine
 *
 *  Created by Sean Chapel on 11/15/05.
 *  Copyright 2005 Seoushi Games. All rights reserved.
 *
 */

#include "example.h"

#include "example.h"
#include "tests.h"

using namespace std;

Example::Example()
{
}

Example::~Example()
{
}

void Example::Init()
{
	//initalize SDL
	if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		cout << "Unable to initialize SDL: " << SDL_GetError() << endl;
		exit(1);
	}

	//setup window and surface flags
	m_Width = 800;
	m_Height = 600;
	m_Bpp = 32;
	m_WindowName = "An example cApp";
	m_SdlFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE | SDL_RESIZABLE;
	m_FullScreen = false;

	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();

	if (videoInfo->hw_available)
		m_SdlFlags |= SDL_HWSURFACE;
	else
		m_SdlFlags |= SDL_SWSURFACE;

    // This checks if hardware blits can be done
    if ( videoInfo->blit_hw )
        m_SdlFlags |= SDL_HWACCEL;

    // Sets up OpenGL double buffering
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	atexit(SDL_Quit);

	//SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, m_Bpp);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//create the surface
	m_Surface = SDL_SetVideoMode(m_Width, m_Height, m_Bpp, m_SdlFlags);

	if ( m_Surface == NULL)
	{
		cout << "Video mode set failed: " << SDL_GetError() << endl;
		SDL_Quit();
	}

	//setup opengl
	glViewport(0, 0, (GLsizei)m_Width, (GLsizei) m_Height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	InitGl();

	//set the window title
	SDL_WM_SetCaption(m_WindowName.c_str(), NULL);
}

void Example::ResizeWindow(int width, int height)
{
    // Protect against a divide by zero
    if ( height == 0 )
    {
        height = 1;
    }

    //resize sdl surface
	m_Surface = SDL_SetVideoMode( width, height, m_Bpp, m_SdlFlags );

	if (m_Surface == NULL)
	{
		cout << "Could not get a surface after resize: " << SDL_GetError() << endl;
		exit(1);
	}

//commented out so the screen scales instead of making more pixels
	//update the width/height
	//m_Width = width;
	//m_Height = height;

	InitGl();

	g_Graphics->Initialize(m_Width, m_Height);
}

void Example::InitGl()
{
	glShadeModel( GL_SMOOTH );
	glClearColor( 0.5f, 0.5f, 0.5f, 0.0f );
	glClearDepth( 0.0f );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_ALWAYS );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}

void Example::Run()
{
	//do main game stuff here

	//TestProcess();
	//TestStates();
	g_Graphics->Initialize(m_Width, m_Height);

    uint texture = g_TextureManager->LoadTextureFromFile("../textures/picture.png");

    sSection section1;
    section1.uMax = 0.5f;
    section1.uMin = 0.0f;
    section1.vMax = 0.5f;
    section1.vMin = 0.0f;

    sSection section2;
    section2.uMax = 1.0f;
    section2.uMin = 0.5f;
    section2.vMax = 0.5f;
    section2.vMin = 0.0f;

    sSection section3;
    section3.uMax = 0.5f;
    section3.uMin = 0.0f;
    section3.vMax = 1.0f;
    section3.vMin = 0.5f;

    sSection section4;
    section4.uMax = 1.0f;
    section4.uMin = 0.5f;
    section4.vMax = 1.0f;
    section4.vMin = 0.5f;

    uint textureSect1 = g_TextureManager->CreateTextureSection(texture, section1);
    uint textureSect2 = g_TextureManager->CreateTextureSection(texture, section2);
    uint textureSect3 = g_TextureManager->CreateTextureSection(texture, section3);
    uint textureSect4 = g_TextureManager->CreateTextureSection(texture, section4);

    vector< uint > Frames;
    Frames.push_back(textureSect2);
    Frames.push_back(textureSect1);
    Frames.push_back(textureSect3);
    Frames.push_back(textureSect4);

    //draw
    uint anim1 = g_AnimationManager->CreateAnimation( Frames, 1000 );
    uint anim2 = g_AnimationManager->CreateAnimation( Frames, 500 );


    g_FontManager->SetFontPath("../data/Fonts/");
    string font = g_FontManager->LoadFont("franks");
    g_FontManager->SetLetterSpacing(font, 0.55f);


    SDL_Event event;
    bool quit = false;

    GLfloat Scale = 1.0;
    bool shrink = "true";

    GLfloat Rotation = 0;

    Uint32 LastTime = SDL_GetTicks();
    uint ShownFrames = 0;
    ostringstream FPS;

    //draw 20 frames
    while(!quit)
    {

        while ( SDL_PollEvent( &event ) )
		{
		    switch( event.type )
			{
                case SDL_VIDEORESIZE:
                    ResizeWindow( event.resize.w, event.resize.h );
                    break;
                case SDL_QUIT:
                    /* handle quit requests */
                    quit = true;
                    break;
                default:
                    break;
			}
		}

		ShownFrames++;

		if((SDL_GetTicks() - LastTime) >= 1000)
		{
		    FPS.str("");
		    FPS << ShownFrames;

		    ShownFrames = 0;
		    LastTime = SDL_GetTicks();
		}


		Rotation += 1;

		if(Rotation == 360)
		{
		    Rotation = 0;
		}

		if(Scale <= 0.5f)
        {
            shrink = false;
		}
		if(Scale >= 1.5f)
		{
		    shrink = true;
		}

		if(shrink)
		{
		    Scale -= 0.02f;
		}
		else
		{
		    Scale += 0.02f;
		}

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        g_Camera->Move(0.5f, 0.5f);

        g_AnimationManager->UpdateAll();
        g_AnimationManager->DrawAnimation(anim1, 20, 20);
        g_AnimationManager->DrawAnimation(anim2, 200, 200, Scale, Rotation, 1, 0, 0);

        g_FontManager->DrawText(font, "Testing", 300 - g_Camera->GetXposition(), 300);
        g_FontManager->DrawText(font, "The Quick Brown Fox Jumps Over The Lazy Dogs Back", -1000, 250 - g_Camera->GetYposition(), 2.0f, 0, 0, 1, 0, 0.5);
        g_FontManager->DrawText(font, "FPS: " + FPS.str(), 10 - g_Camera->GetXposition(), 560 - g_Camera->GetYposition(), 0.5, 0, 0, 1);

        SDL_GL_SwapBuffers();
		SDL_Delay(1);
    }

}

void Example::Shutdown()
{
	SDL_Quit();
}

void Example::ToggleFullscreen()
{
	m_FullScreen = !m_FullScreen;

	SDL_WM_ToggleFullScreen(m_Surface);

	if (!m_FullScreen)
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
	else
	{
		SDL_ShowCursor(SDL_DISABLE);
	}
}
