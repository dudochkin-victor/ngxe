**************************************************************************
**	Refraction
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This project displays a floating sphere, torus or teapot which demonstrates reflection and refraction.

The eye-space position and normal are calculated in the vertex program, and reflected and refracted vectors are computed. These are then converted to world space (by multiplying by the inverse view matrix) and used to look up in a cube map.

The "Chromatic Aberration" paths calculate the refraction differently for each of red, green and blue, and the results are combined in the register combiners. This simulates wavelength-dependent refraction.

An approximate fresnel term is also computed and used to interpolate between the reflected and refracted colors based on the angle between the eye vector and the normal vector. 


Requirements:

ARB_multitexture
ARB_texture_cube_map
EXT_texture_edge_clamp
NV_register_combiners
NV_vertex_program

For single-pass chromatic aberration:

NV_register_combiners2
Four or more texture units


References:

"Chromatic Aberration", Mark Kilgard. From developer.nvidia.com


Keys:

F1	-	Take a screenshot
Escape	-	Quit

Space	-	Change object

1	-	Single-pass chromatic aberration
2	-	Single-pass simple refraction
3	-	Two-pass chromatic aberration
