**************************************************************************
**	Marble
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This demo generates a solid marble texture in a fragment program.

A 3d texture is first created in which each texel is a gradient, a unit vector in 3 space. For each fragment drawn, the fragment program samples the 8 gradients on the integer coordinates surrounding the fragment. These are then used in the standard perlin noise method (see reference below) to generate a noise factor n1. The weighting factors used in doing this are sampled from a second 3d texture. This process is then repeated at twice the frequency to generate a second octave of noise, n2. To create the marble effect, these values are fed into the function sin(x+z+n1+0.5*n2). Finally, this sin value is range compressed into [0, 1] and used to look up in a color ramp texture.

The demo simply draws a teapot 'carved out' of this 3d texture.


Requirements:

OpenGL 1.3
ARB_fragment_program
ARB_vertex_program


References:

"The Perlin Noise Math FAQ", by Matt Zucker. From www.robo-murito.net/code/perlin-noise-math-faq.html


Keys:

F1	-	Take a screenshot
Escape	-	Quit
