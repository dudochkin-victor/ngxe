/*
	Billboard.h
*/

// I like namespaces
namespace bb
{
	// Draws a camera-oriented line given a set of points, a line width and
	// the camera's position
	void drawline(const vec *points, int numPoints, float width, const vec &campos)
	{
		vec lookat = campos - points[0];	// The direction from the point to the camera
		vec dir = points[1] - points[0];	// The direction of the line

		vec x, tmp;

		glBegin(GL_QUADS);
			for(int a = 0; a < numPoints-1; a++)
			{
				// Calculate the x vector
				x = lookat*dir;
				x.Normalize();

				// Send it to opengl
				tmp = points[a] + x * width;
				glTexCoord2f(0, a/(float)(numPoints-1));
				glVertex3fv(tmp.v);				

				// And the other side
				tmp = points[a] - x * width;
				glTexCoord2f(1, a/(float)(numPoints-1));
				glVertex3fv(tmp.v);
	
				// Calculate the next lookat, dir, and x vector
				lookat = campos - points[a+1];
				dir = points[a+1] - points[a];
				x = lookat * dir;
				x.Normalize();

				// And send it to opengl as well
				tmp = points[a+1] - x*width;
				glTexCoord2f(1, (a+1)/(float)(numPoints-1));
				glVertex3fv(tmp.v);

				// Including its opposite
				tmp = points[a+1] + x*width;
				glTexCoord2f(0, (a+1)/(float)(numPoints-1));
				glVertex3fv(tmp.v);			
			}
		glEnd();
	}
};