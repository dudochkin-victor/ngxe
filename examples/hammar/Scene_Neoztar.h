/*
	Scene_neoztar.h
*/

void scene_neoztar(float sceneTime)
{
	// Clear the buffers and the matrix
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glDepthMask(GL_FALSE);

	glTranslatef(0, 0, -3);

	// Draw the multilayered text
	glColor4f(0.5f, 0.7f, 1, 0.2f * fade_io(0, 6));
	for(int a = 0; a < 10; a++)
	{
		glTranslatef(0, 0, 0.1f);
		glPushMatrix();
			glTranslatef(2-sceneTime*0.7f, 0, 0);
			
			glScalef(0.02f, 0.02f, 0.02f);
			glTranslatef(-65, -35, 0);
//			fnt->Print(0, 0, "neoztar");
		glPopMatrix();
	}

	glDepthMask(GL_TRUE);

	glDisable(GL_BLEND);
}
