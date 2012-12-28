**************************************************************************
**	Cloth Simulation
**
**	www.paulsprojects.net
**
**	paul@paulsprojects.net
**************************************************************************

Description:

This demo displays a cloth modelled using a "ball-and-spring" model. A grid of balls is created and these are linked using springs. Each ball is connected to all 8 neighbours (up, down, left, right and diagonally) and to its next-but-one neighbour in the up, down, left and right directions.

The cloth begins suspended from its four corners above a sphere, above the floor. Pressing 1, 2, 3 or 4 will release a corner.

The physics for the model are updated every 10ms. For each ball, the overall force on it due to gravity and the springs is calculated. This is then used to calculate a new position and velocity. If the new position is within the sphere or below the floor, the ball is moved outside the object.

The cloth can be drawn in two ways. Either triangles can be drawn between the balls to create a simple triangle mesh, or each 4x4 subgrid of balls can be submitted to the OpenGL evaluators for interpolation and subdivision to create a smoother look. 

The physics must be updated on a regular timescale, as we multiply the velocities by a constant damping factor (1-e, e small) to simulate energy loss. If this is done per frame, higher framerates will cause the physics calculations to slow down as the damping factor is applied more times per frame.
The problem with calculating the physics every 10ms is that if your computer takes >10ms to calculate the new positions, the frame rate will suffer an exponential decay.

This is only a very simple cloth demonstration. Many things could be done to improve it, including:

1. Collision/Friction between parts of the cloth.

This would prevent the cloth from penetrating itself as can sometimes be seen if the cloth is hung from a single point, and prevent two surfaces of the cloth becoming coplanar when the cloth is on the floor. These coplanar surfaces cause obvious z-fighting. Calculating friction between the cloth and the floor would also prevent the large amount of slipping which occurs once the cloth is entirely on the floor.

2. Better calculation of interpolated vertices.

The sphere drawn in this demo is only 90% of the size of the sphere which the cloth "feels". Any larger, and the sphere penetrates through the cloth even though the control points all lie on the outside.
Instead of using OpenGL evaluators, the vertices for the cloth (when displayed in patches mode) could be calculated using a vertex program. This would enable each of those vertices to be pushed out if they lie within the sphere, rather than just doing this for the control points. Due to the finer calculations, the cloth could then lie more closely on the sphere.

3. Better lighting

The vertex program used to accomplish 2 could also calculate coefficients for a different form of lighting, for example Minnaert lighting to simulate velvet. This would help the cloth look more realistic and less like a sheet of simply diffuse reflecting material.


Requirements:

No Extensions Required.


References:

http://freespace.virgin.net/hugo.elias/models/m_cloth.htm
Interactive Cloth Simulation, by Matthias Wloka. http://developer.nvidia.com/docs/IO/1279/ATT/ClothSim.pdf


Keys:

F1	-	Take a screenshot
Escape	-	Quit

1,2,3,4	-	Release Corners of Cloth

<space>	-	Reset Cloth

P	-	Draw Cloth as Patches
T	-	Draw Cloth as Triangles

B	-	Draw balls
S	-	Draw Springs

Up Arrow-	Increase Patch Tesselation
Down Arrow-	Decrease Patch Tesselation
