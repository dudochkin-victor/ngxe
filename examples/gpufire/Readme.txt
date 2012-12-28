**************************************************************************
**	GPU fire
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This project creates an animated fire texture on the GPU. A pbuffer is created, and wglShareLists is used so that the pbuffer and window contexts share textures. A blank texture is also initiated.

The following steps occur:

1.  Average the values in the texture neighbouring each pixel in the pbuffer, and scroll up by one. This is done in a single pass on the pbuffer, by binding the texture to all four texture units and using NV_register_combiners to take the average.

2. Subtract one of four "cooling" textures from the values in the pbuffer, to simulate the flames cooling over time.

3. Add fuel values (random 0.0-1.0) to the bottom of the pbuffer.

4. Use CopyTexSubImage to copy the pbuffer contents back into the texture.

5. Display the texture in the window, using NV_register_combiners to convert the texture's alpha values to realistic colors.

6. Repeat from step 1.

Note that all drawing to the pbuffer occurs in the alpha channel only. This is because this technique is adapted from one which used a paletted display. The texture is converted into color values only when it is drawn to the screen. 


Requirements:

ARB_multitexture with at least 4 texture units
ARB_texture_border_clamp
EXT_blend_minmax
EXT_blend_subtract
NV_register_combiners with at least 3 general combiners
WGL_ARB_extensions_string
WGL_ARB_pbuffer
WGL_ARB_pixel_format


References:

"Dynamic Texturing", Mark Harris. From developer.nvidia.com


Keys:

F1	-	Take a screenshot
Escape	-	Quit

P	-	Pause
U	-	Unpause
