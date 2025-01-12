# CollisionSimulation
My university thesis, a physics based collision simulation on procedurally generated terrain

The thesis aims to implement a real-time simulation software. This simulation
models the physics based movement and collision of convex bodies on a generated
terrain.

In the beginning of the simulation, the bodies have an initial physical state, which
is continuously changing as time is being simualted forward. During the simulation
collisions occur, these collisions exert a greater change on the bodies’ states. Moreover,
the terrain geometry can be customized, this also has an effect in changing the states.

The user can navigate through the scene with a camera, which can be controlled
with the keyboard. The simulation is controllable in the sense that time can be simulated
forward and backward. Also, the physical property of the bodies can be customized, and
the result can be viewed by replaying the simulation.

The software which handles the mentioned tasks is written in a C++/OpenGL
environment using a few supporting libraries: GLEW for linking the graphical functions,
GLFW for window/event handling, GLM for mathematical datatypes/functions and last
but not least Dear ImGui for a graphical user interface.

![vid1](https://github.com/user-attachments/assets/2a74426f-6665-47dd-884c-33e67a4fa5e3)

![vid4](https://github.com/user-attachments/assets/598698a7-a00d-4cb6-90a7-aa880a322c70)

