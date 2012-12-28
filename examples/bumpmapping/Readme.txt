**************************************************************************
**	Bump Mapping
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This demo draws a torus lit by a single light.

The following equation is evaluated per fragment in tangent space:

Color=(N'.L)*Decal + [(N'.H)^Power]*Gloss

where:

N'      is the normal from the normal map
L       is the vector to the light
Decal   is the texture color
H       is the halfway vector
Power   is the specular power
Gloss   allows per-fragment coloring of the specular component

This is performed using NV_register_combiners.

Method 1

To evaluate the equation in a single pass, all 4 texture units and 8 general combiners of a Geforce3 or above are used. The specular exponent is 16, and is calculated by successive squaring in the combiners.

Method 2

Improved specular highlights are acheived by using the Geforce 3 or above's NV_texture_shader functionality to do a dot-product dependent look up into a specular ramp texture, again with exponent 16. The diffuse and decal are applied in another pass. However, as normalisation of the half way vector is not performed, this technique is not as good for coarsely tesselated geometry.

Method 3

In order to evaluate the equation using only the 2 general combiners and 2 texture units of a Geforce 1/2, three passes are performed. 


Requirements:

EXT_compiled_vertex_array
ARB_multitexture
NV_register_combiners
NV_vertex_program

Required for Higher Quality bump mapping:
At least 4 general combiners for NV_register_combiners
At least 4 texture units
NV_texture_shader


References:

"Per-Pixel Lighting", Philip Taylor. From msdn.microsoft.com
"Texture Compositing with Register Combiners", John Spitzer. From developer.nvidia.com


Keys:

F1	-	Take a screenshot
Escape	-	Quit

F	-	Draw smooth torus
B	-	Use bump map

P	-	Pause
U	-	Unpause

1	-	Use method 1 (4 general combiners, single pass)
2	-	Use method 2 (NV_texture_shader)
3	-	Use method 3 (Geforce 2 fallback)
