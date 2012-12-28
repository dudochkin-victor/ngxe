/*
	SCENE_Metaballs.h
*/

#include "Billboard.h"

void scene_metaballs(float sceneTime)
{
	// Clear the depth buffer only, as the background covers all of the screen anyways...
	glClear(GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Draw the background
	glColor3f(0.5f, 0.5f, 0.5f);
	t_refl->Bind();
	bg::DrawZooming(sceneTime*0.015f);

	// Enable lighting
	float lightPos[] = {0, 0, 0, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glTranslatef(0, 0, -4);

	glColor3f(1, 1, 1);

	// Enable texture coordinate generation
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	t_refl->Bind();

	// Update the positions of the metaballs
	m_balls->SetBall(0, vec(sinf(sceneTime*1.0f), cosf(sceneTime*1.3f), sinf(sceneTime*2.0f)), 0.15f);
	m_balls->SetBall(1, vec(sinf(sceneTime*0.2f), cosf(sceneTime*1.4f), sinf(sceneTime*1.1f)), 0.15f);
	m_balls->SetBall(2, vec(sinf(sceneTime*1.4f), cosf(sceneTime*0.7f), sinf(sceneTime*1.3f)), 0.20f);
	m_balls->SetBall(3, vec(sinf(sceneTime*1.1f), cosf(sceneTime*2.1f), sinf(sceneTime*1.2f)), 0.15f);
	m_balls->SetBall(4, vec(sinf(sceneTime*0.6f), cosf(sceneTime*1.4f), sinf(sceneTime*0.5f)), 0.20f);

	// Recalculate the metaballs
	m_balls->RecalcGrid();

	// Setup some blending
	glColor4f(1, 1, 1, 0.2f);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// Draw the metaballs with two different layers
	m_balls->Draw(0.1f);
	m_balls->Draw(0.15f);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glDisable(GL_LIGHTING);

	// Draw the fade in
	if(sceneTime < 1)
	{
		glEnable(GL_BLEND);
		Texture::Enable(false);

		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(1, 1, 1, 1-sceneTime);
		
		bg::DrawStatic();

		glDisable(GL_BLEND);
		Texture::Enable(true);
	}

	// ... And the fade out
	if(sceneTime > 27)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1, 1, 1, sceneTime-27);
		Texture::Enable(false);
		bg::DrawStatic();
		Texture::Enable(true);
		glDisable(GL_BLEND);
	}
}
