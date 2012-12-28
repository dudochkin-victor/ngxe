/*
	SCENE_Tentacles.h
*/

// Draws a single tentacle using GLU quadrics
void tentacle(float sceneTime)
{
	// Calculate the preferred level-of-detail
	int q = prefs::polylevel * 12 + 4;

	// Draw the tentacle
	glPushMatrix();
		glTranslatef(0, 0, 2);
		for(int a = 0; a < 10; a++)
		{
			gluCylinder(quadric, 2 - a*0.1f, 1 - a*0.1f, 4.0f, q, 1);
			gluDisk(quadric, 0, 2-a*0.1f, q, 1);
			glTranslatef(0, 0, 4);
			gluDisk(quadric, 0, 1-a*0.1f, q, 1);
			glTranslatef(0, 0, -1);
			glRotatef(sinf(sceneTime)*(3.0f + 5.0f*a), 0, 1, 0);
			glRotatef(sinf(sceneTime*1.4f)*(3.0f + 5.0f*a), 1, 0, 0);
		}
	glPopMatrix();
}

void scene_tentacles(float sceneTime)
{
	// Clear the buffers and the matrices
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Setup texture coordinate generation
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	// Setup lighting
	float lightPos[] = {0, 0, 0, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	
	// Do some stuff
	glColor3f(1, 1, 1);

	glTranslatef(0, 0, -22);
	glScalef(0.5f, 0.5f, 0.5f);

	glRotatef(sceneTime*60.0f, 0, 1, 0);
	glRotatef(sceneTime*34.0f, 1, 0, 0);

	// Calculate time since last frame
	static float lastTime = sceneTime;
	float deltaTime = sceneTime - lastTime;
	lastTime = sceneTime;

	// Save the current matrix
	glPushMatrix();

	float c = deltaTime;
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, 256, 256);
		// Draw the background
		glColor3f(1-c, 1-c, 1-c);
		t_blab->Bind();
		bg::DrawZooming(deltaTime*2);

		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);

		// Draw the entire tentacle monster
		glMatrixMode(GL_TEXTURE);
			glRotatef(sceneTime*27, 0, 0, 1);
			glRotatef(sceneTime*39, 0, 1, 0);
		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glColor3f(1, 1, 1);
		t_metal2->Bind();
		tentacle(sceneTime);
		glRotatef(90, 0, 1, 0);
		tentacle(sceneTime);
		glRotatef(90, 0, 1, 0);
		tentacle(sceneTime);
		glRotatef(90, 0, 1, 0);
		tentacle(sceneTime);
		glRotatef(-90, 1, 0, 0);
		tentacle(sceneTime);
		glRotatef(180, 1, 0, 0);
		tentacle(sceneTime);
		
		gluSphere(quadric, 3.82f, 32, 32);

		glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);

		// Copy the framebuffer to the background texture
		t_blab->Bind();
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 256, 256);
	glPopAttrib();

	// Restore the matrix
	glPopMatrix();

	// Draw the background
	glColor3f(1, 1, 1);
	bg::DrawStatic();

	// Draw the tentacle monster again
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glMatrixMode(GL_TEXTURE);
		glRotatef(sceneTime*27, 0, 0, 1);
		glRotatef(sceneTime*39, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glColor3f(1, 1, 1);
	t_metal2->Bind();
	tentacle(sceneTime);
	glRotatef(90, 0, 1, 0);
	tentacle(sceneTime);
	glRotatef(90, 0, 1, 0);
	tentacle(sceneTime);
	glRotatef(90, 0, 1, 0);
	tentacle(sceneTime);
	glRotatef(-90, 1, 0, 0);
	tentacle(sceneTime);
	glRotatef(180, 1, 0, 0);
	tentacle(sceneTime);
	
	gluSphere(quadric, 3.82f, 32, 32);

	glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	
	glDisable(GL_COLOR_MATERIAL);
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
	if(sceneTime > 51)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1, 1, 1, (sceneTime-51)*0.25f);
		Texture::Enable(false);
		bg::DrawStatic();
		Texture::Enable(true);
		glDisable(GL_BLEND);
	}

}