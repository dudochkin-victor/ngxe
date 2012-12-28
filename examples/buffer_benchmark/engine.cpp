#include "engine.h"

void SetWindowRasterPos(int x,int y) {
  if (!GL_ARB_window_pos) {
		PUSH_ORTHO_2D
		glRasterPos2i(x,y);
		POP_ORTHO_2D
  } else glWindowPos2iARB(x,y);
}
