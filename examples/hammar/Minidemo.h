/*
	MINIDEMO.h
*/

#include <stdio.h>
#include <math.h>
#include <unistd.h>

// LOTS OF INCLUDES
#include "TexGen.h"
#include "BG.h"
#include "Misc.h"
#include "Font.h"
#include "Metaballs.h"
#include "Progress.h"

#include "DemoTextures.h"
#include "DemoMeshes.h"
//#include "sound.h"

// Some common stuff
Font *fnt;
GLUquadricObj *quadric;

// The scenes
#include "Scene_Intro3.h"
#include "Scene_Neoztar.h"
#include "Scene_Tentacles.h"
#include "Scene_Metaballs.h"
#include "Scene_Electro.h"

// The demo text
#include "DemoText.h"

// Main demo cycle
void Cycle()
{
	// Calculate the current time
	static double lastTime = timeGetTime()/1000;
	double currTime = timeGetTime()/1000;
	double deltaTime = currTime - lastTime;
	lastTime = currTime;

	static double sceneTime = 0;
	
	float speed = 1;

#ifdef MYDEBUG
	// Debug stuff to allow fast-forwarding
	if(keys[VK_SHIFT])
		speed *= 2;

	if(keys[VK_CONTROL])
		speed *= 5;
#endif // MYDEBUG

	sceneTime += deltaTime * speed;

	fprintf(stderr, "%f\n", sceneTime);
	// Decides which scene to run
	if(sceneTime < 41)
		scene_intro3(sceneTime);
	else if(sceneTime < 47)
		scene_neoztar(sceneTime - 41);
	else if(sceneTime < 75.0f)
		scene_metaballs(sceneTime - 47);
	else if(sceneTime < 105.0f)
		scene_electro(sceneTime - 75);
	else if(sceneTime < 160.0f)
		scene_tentacles(sceneTime - 105.0f);
	else
		// The demo is over, quit
		exit(0);

	// Draw text
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Switch to 2D mode
	glDisable(GL_DEPTH_TEST);
	View::BeginOrtho2D(640, 480);

	// I want smaller text for the end scroller
	if(sceneTime >= 105.0f)
		glScalef(0.5f, 0.5f, 1);

	// Loop through all of the text
	for(int i = 0; i < numtext; i++)
	{
		// Draw the text if it's visible
		glPushMatrix();
		if(in_time(demotext[i].start, demotext[i].duration))
		{
			float c = fade_io(demotext[i].start, demotext[i].duration);
			
			glColor4f(1, 1, 1, c);

			glTranslatef(demotext[i].x + (sceneTime-demotext[i].start)*demotext[i].xspd,
						 demotext[i].y + (sceneTime-demotext[i].start)*demotext[i].yspd, 0);

			glScalef(1+demotext[i].xscl, 1+demotext[i].yscl, 1);

//			fnt->Print(0, 0, demotext[i].msg);
		}
		glPopMatrix();
	}

	// Return to 3D
	View::EndProjection();
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);
	glutSwapBuffers();
	usleep(1000);
	return;
}

void Init()
{
	progress(0);

	// Generate some textures and meshes
	loadTextures();
	loadMeshes();

	// And a font
	fnt = new Font("Arial", 42, true, false);

	progress(0.9f);

	// Enable texturing
	Texture::Enable(true);

	// Randomize
	srand(GetTime());

	// Get a quadric object
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);

	progress(1);

	// Start the music
//	startsound();
}

void Kill()
{
	// Stop the music
//	stopsound();

	// Delete the quadric object
	gluDeleteQuadric(quadric);

	// Delee the textures and meshes
	killTextures();
	killMeshes();

	// .. And the font
	delete fnt;
}

