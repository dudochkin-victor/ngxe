**************************************************************************
**	Quake 3 bsp
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This program loads in and displays a quake 3 level.

The following are implemented:

Polygon Faces
In-level meshes
Curved surfaces
Lightmapping
BSP & PVS

The program uses "config.txt" for a map name and initial camera position. It also reads in the curve tesselation (curved surfaces are tesselated once, on loading). These attributes can thus be changed without requiring a recompile.

I have created a small level using GtkRadiant to demonstreate the techniques. Note I am yet to implement billboards, which is why the torches do not have any flames above them. 

This software is based in part on the work of the Independent JPEG Group.
Lines 160 and 214 of jmorecfg.h have been altered from the original in order to prevent the redefinition of 'INT32' and 'FAR' on WIN32 platforms.


Requirements:

ARB_multitexture
EXT_texture_env_combine

Optional:

EXT_multi_draw_arrays
EXT_draw_range_elements


References:

Quake 3 BSP Format, Ben Humphrey. From www.gametutorials.com
Unofficial Quake 3 map specs, Kekoa Proudfoot. From graphics.stanford.edu


Controls:

F1	-	Take a screenshot
Escape	-	Quit

Arrow Keys -	Move around
W S A D

Mouse	-	Look around

P	-	Pause PVS/Frustum cull
U	-	Unpause

T	-	Show textures
L	-	Show lightmaps
M	-	Show lit textures
