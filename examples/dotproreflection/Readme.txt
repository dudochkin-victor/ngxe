**************************************************************************
**	Dot Product Reflect
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This demo draws a torus, reflecting a cubic environment map. The reflection calculations are applied per pixel using the "Dot Product Reflect Cube Map" option of NV_texture_shader. A normal map in tangent space is applied to the torus. The "texel matrix" is then set up to convert the normals to eye space, and the reflection calculations are applied, giving a reflection vector, also in eye space. This is used to look up in the cube map.


Requirements:

ARB_multitexture
EXT_texture_edge_clamp
NV_register_combiners
NV_texture_shader
ARB_texture_cube_map
NV_vertex_program
EXT_compiled_vertex_array


References:

"Inside the Dot Product Reflect Demos", Cass Everitt. From developer.nvidia.com.
"Texture Shaders", Sebastien Domine and John Spitzer. From developer.nvidia.com

Keys:
F1	-	Take a screenshot
Escape	-	Quit
