**************************************************************************
**	Deferred Shading
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

 This project displays a torus knot lit by 8 point lights. The lights are diffuse only, and their contributions to the scene(including attenuation) are calculated per pixel using fragment programs. This is done in two ways.

In the "standard" shading method, a first pass is made to set the depth buffer to hold the depth of the nearest fragment. Then the mesh is drawn once per light, and each time, the lighting contribution is added to the framebuffer. The scissor test is used to restrict drawing to a small window rectangle containing the projection of the sphere of influence of the light.

This project's main feature however, is the "deferred" shading method. Here, the mesh is drawn once and the positions and normals at each fragment are saved into a floating point buffer. Then, for each light, this buffer is used as a texture and a flat quad is drawn covering the sphere of influence of each light. A fragment program unpacks the values from the buffer and, using the light's position, color etc as parameters, evaluates the lighting equation and adds it to the framebuffer.

The advantage of the deferred shading approach is that the mesh (32000 vertices) need only be drawn once. 


Requirements:

ARB_fragment_program
ARB_vertex_buffer_object
ARB_vertex_program
NV_float_buffer
NV_fragment_program
NV_texture_rectangle
WGL_ARB_extensions_string
WGL_ARB_pbuffer
WGL_ARB_pixel_format


References:

"Deferred Shading", Tom Nuydens. From http://www.delphi3d.net/articles/viewarticle.php?article=deferred.htm


Keys:

F1		-	Take a screenshot
Escape	-	Quit

1		-	Use deferred shading method
2		-	Use standard shading method

P		-	Pause
U		-	Unpause
