**************************************************************************
**	Shadow Mapping
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This program displays a simple scene, lit by a single light. The scene is first rendered from the light's point of view, and the depth values are loaded into a texture. This texture is then projected onto the scene as drawn from the eye's point of view, and a comparison with distance from the light is made. This determines shadowed regions.
This demo implements 8-bit "Dual Texture" shadow mapping and a 16-bit version using register combiners. 24-bit hardware shadow mapping is also demonstrated.


Requirements:

ARB_multitexture
EXT_texture_env_combine	- the ARB version was not used so that the demo will work on a TNT.

Required for 16-bit shadow mapping:

NV_register_combiners

Required for hardware shadow mapping:

SGIX_depth_texture
SGIX_shadow
EXT_blend_minmax


References:

GDC 2000 "Shadow Mapping with Today's OpenGL Hardware", Mark Kilgard. From developer.nvidia.com

"Projective Texture Mapping" Cass Everitt. From developer.nvidia.com

"Hardware Shadow Mapping", Cass Everitt, Ashu Rege and Cem Cebenoyan. From developer.nvidia.com

"Shadowcast" demo. From developer.nvidia.com


Keys:

F1	-	Take a screenshot
Escape	-	Quit

Up Arrow-	Increase Shadow Map resolution
Down Arrow-	Decrease Shadow Map resolution

Space	-	Toggle drawing of light's frustum
Left Arrow-	Increase size of light's frustum
Right Arrow-	Decrease size of light's frustum

1	-	Use 8 bit shadow mapping
2	-	Use 16 bit shadow mapping
3	-	Use 24 bit hardware shadow mapping

C	-	Use mouse to move camera
L	-	Use mouse to move light

T	-	Draw Tori (Donuts)
B	-	Draw Spheres
