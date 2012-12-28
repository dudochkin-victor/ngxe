/*
	SCENE_INTRO3.h
*/

void scene_intro3(float sceneTime)
{
	// Clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Setup lighting
	float lightPos[] = {0, 0, 0, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	// Setup texture coordinate generation
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	// Some matrix manipulation
	glTranslatef(0, 0, -0.5f);
	glRotatef(sceneTime*6.0f, 0, 1, 0);
	glRotatef(sceneTime*3.9f, 1, 0, 1);

	// Setup blending
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// Draw the wireframe background
	Texture::Enable(false);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(0, 0.1f, 0.2f);
	m_distsphere->Draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Texture::Enable(true);

	// Draw the outer distorted sphere
	glRotatef(sceneTime*16.0f, 0, 1, 0);
	glRotatef(sceneTime*13.9f, 1, 0, 1);

	t_re2xt->Bind();
	glColor3f(1, 0.7f, 0.2f);
	m_weirdsphere->Draw();

	// Draw the inner distored sphere (or should I call it pyramid?)
	glRotatef(sceneTime*36.0f, 0, 1, 0);
	glRotatef(sceneTime*33.9f, 1, 0, 1);

	t_metal2->Bind();
	glColor3f(1, 1, 1);
	m_insidesphere->Draw();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	// Draw the fade out
	if(sceneTime > 40)
	{
		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1, 1, 1, sceneTime-40);
		Texture::Enable(false);
		bg::DrawStatic();
		Texture::Enable(true);
	}

	// ... And the fade in
	if(sceneTime < 1)
	{
		Texture::Enable(false);

		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(1, 1, 1, 1-sceneTime);
		
		bg::DrawStatic();

		Texture::Enable(true);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	glDisable(GL_LIGHTING);
}