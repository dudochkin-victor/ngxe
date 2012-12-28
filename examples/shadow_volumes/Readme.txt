**************************************************************************
**	Shadow Volumes
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This project displays four tori rotating around a point light source. Each casts shadows on the other tori and on the walls. The effect is acheived in three passes:

   1. Draw entire scene lit by ambient and a small amount of diffuse light.
      This pass also sets the depth values for the visible scene.
   2. Draw shadow volumes into stencil buffer.
      By default the shadow volumes use the "zFail" technique, with an infinite far clip plane, as described in the "Robust Stenciled Shadow Volumes" paper.
   3. Draw scene lit with full diffuse and specular, in unshadowed areas (where stencil == 0).

Several optimisations to the above method are also supported; see "technical.txt" for details.


Requirements for two sided stencil:

EXT_stencil_two_side
EXT_stencil_wrap

Requirements for vertex program extrusion:

ARB_vertex_program


References:

Practical & Robust Stenciled Shadow Volumes for Hardware-Accelerated Rendering, by Cass Everitt and Mark J Kilgard. From developer.nvidia.com
Nehe tutorials lesson 28 - stencil shadow volumes. From nehe.gamedev.net


Keys:

F1		-	Take a screenshot
Escape		-	Quit

1		-	Use "normal" 1 sided stencil testing
2		-	Use EXT_stencil_two_side, if supported

F		-	Always use zFail approach
G		-	Use zPass approach & uncapped volumes, when this will not produce artifacts.

P		-	Pause
U		-	Unpause

V		-	Use vertex program volume extrusion
S		-	Use standard shadow volume extrusion

Up Arrow	-	Move light up
Down Arrow	-	Move light down
