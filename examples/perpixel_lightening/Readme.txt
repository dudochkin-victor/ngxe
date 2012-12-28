**************************************************************************
**	Per Pixel Lighting
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This demo draws a torus lit by four colored lights. One pass is performed for each light, which is attenuated per-pixel using 2 textures. Diffuse tangent space bump mapping is also performed per light, using NV_register_combiners.


Requirements:

EXT_compiled_vertex_array
EXT_texture_edge_clamp
ARB_multitexture with at least 4 texture units
NV_register_combiners
NV_vertex_program


References:

"Per-Pixel Lighting", Philip Taylor. From msdn.microsoft.com
"Texture Compositing with Register Combiners", John Spitzer. From developer.nvidia.com
"Real-Time Per-Pixel Point Lights and Spot Lights", Ron Frazier. From www.ronfrazier.net/apparition
Also see demo at: www.cfxweb.net/~delphigl/projects.htm


Keys:

F1	-	Take a screenshot
Escape	-	Quit

Space	-	Draw sphere of influence for each light.

1	-	Disable light 1
2	-	Disable light 2
3	-	Disable light 3

Note: Light 0 cannot be disabled.
