/*
	Scene_Electro.h
*/

void scene_electro(float sceneTime)
{
	// Clear the buffers & stuff
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Setup tex coord generation
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	// Setup lighting
	float lightPos[] = {0, 0, 0, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	// Move the camera
	vec campos;
	campos.x = sinf(sceneTime*0.21f)*5;
	campos.y = cosf(sceneTime*0.31f)*5;
	campos.z = 2 + sinf(sceneTime*1.01f+2)*4;

	// Apply the camera
	gluLookAt(campos.x, campos.y, campos.z, 0, 0, 0, 0, 1, 0);
	
	// Enable tex coord generation & lighting
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glEnable(GL_LIGHTING);

	// Draw the metal-like stuff at the ends
	glColor4f(1, 1, 1, 0.5f);
	t_silver->Bind();
	glPushMatrix();
	glTranslatef(-6, 0, 0);
		glRotatef(90, 0, 1, 0);
		gluCylinder(quadric, 1, 1, 2, 32, 6);
		gluDisk(quadric, 0, 1, 32, 1);
		glTranslatef(0, 0, 2);
		gluDisk(quadric, 0.7f, 1, 32, 1);

		glTranslatef(0, 0, 8);
		gluDisk(quadric, 0.7f, 1, 32, 1);
		gluCylinder(quadric, 1, 1, 2, 32, 6);
		glTranslatef(0, 0, 2);
		gluDisk(quadric, 0, 1, 32, 1);
	glPopMatrix();

	glDisable(GL_LIGHTING);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	
	// Setup some blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	glDepthMask(GL_FALSE);

	t_metal2->Bind();
	glColor3f(1, 1, 1);

	vec line[100];

	// Speed up a little
	sceneTime *= 2;

	// Draw some sparks inside the tube
	for(int b = 1; b < 4; b++)
	{
		int c = 0;
		for(int a = -50; a < 50; a++)
		{
			line[c].x = a*0.1f;
			line[c].y = sinf(a*0.2f+sceneTime*4*semirand(b))*0.2f + sinf(a*0.5f)*0.05f;
			line[c].z = sinf(a*0.1f+sceneTime*3*semirand(b))*0.2f;
			c++;
		}

		// Use my billboarded line function
		bb::drawline(line, 100, 0.2f, campos);
	}

	// Draw the tube
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glEnable(GL_LIGHTING);

	glColor4f(1, 1, 1, 1);
	t_refl->Bind();
	glTranslatef(-5, 0, 0);
	glRotatef(90, 0, 1, 0);
	gluCylinder(quadric, 0.7f, 0.7f, 10, 32, 6);
	gluCylinder(quadric, 0.71f, 0.71f, 10, 32, 6);

	glDisable(GL_LIGHTING);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	// Slow down again
	sceneTime /= 2.0f;

	// Draw fade in
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

	// .. And fade out
	if(sceneTime > 29)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1, 1, 1, sceneTime-29);
		Texture::Enable(false);
		bg::DrawStatic();
		Texture::Enable(true);
		glDisable(GL_BLEND);
	}
}