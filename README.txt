Controls for GFX
-----------------

By Mark M. Miller. Matriculation - 10006179

For SET09115. 

Graphics demo using OpenGL and C++

A (basic) model of the solar system, as well as a piece of terrain generated from a height map, starfield using point sprites and other objects.

The application will auto detect your monitors native resolution and set that as a default.

The orbits of the planets have been scaled down in a simple fashion. The actual planets revolve around the sun in a fashion similar to their actual orbital revolutions. 

The sun has not been scaled but the rest of the planets as well as their moons should be close.

There is a plane and a few of the prototype planets still hanging around below the terrain if you care to go hunting for them...

Enjoy!

Installation Instructions
--------------------------
I have included a runnable .exe release build inside this folder which should be able to run assuming you have the correct DLLS

I have also included all the required DLLs in a folder

the OpenGL.props file needs to be placed in the correct directory which is usually :-

"C:\Users\[Your Username]\Documents\Libraries"

The property sheet will also request that the libraries be located in the same directory but this can be altered

I have included the required libraries just incase.

Required Libraries
------------------
GLM 0.9.3.4 
SDL 1.2.15 
Boost 1.51
DevIL SDK 1.7.8
glut 3.7
GLEW 1.9.0
GLFW 2.7.6

FPS Camera
-----------
WASD - Movement
Mouse - Rotations
Left/Right Click - X Rotation
LCTRL - move down
Space - move up

Chase Camera
-------------
IJKL - Movement
Num Pad '/' and '*' - zoom in and out

Arcball Camera
---------------
'P' ';' - up and down on Y axis

Target Camera
--------------
N/A

Miscellaneous
--------------
Numpad Nums (1,2,3,4,5,6,7,8,9,0) - Switch camera focus between planets

Numpad enter - stop orbit

Numpad '+' '-' - increase/decrease orbit speed

Arrow keys, move sun or other objects

'=' '-' - control dissolve effect

1,2,3,4,5 - switch camera effect

1 = normal
2 = greyscale
3 = blur
4 = motion blur
5 = sharpen
6 = sobel edge
7 = scharr edge
8 = multi camera
