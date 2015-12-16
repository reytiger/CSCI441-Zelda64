# CSCI441-Zelda64
Computer Graphics final project
Contributors: Chris Butler, Kevin Carbaugh, Nico Pampe
=======

This final was an open ended project where students can create a world through glut. We where to follow several requirements for the project. Different types of shaders where needed such as a fragment color shader or blur shader. In addition, a vertex shader is needed. With these shaders, the world needed several lights and an animated object that the user can control. 

For our final project, we created a theme around "The Legend of Zelda: Ocerian of Time." Our level that we load in are Hyrule fields, which is a simple obj file that is loaded into the scene. Using FMOD for sound, we added the theme Hyrule fields theme to the program. To finish it off, a model of Link is rigged, skinned, and animated and placed in the world. A user can either use the keyboard to control the world, or an Xbox controller can be used. 

To build the project, simply: type 'make' in the root directory. A command of Cmake files is excuted that should find and build all of the packages. If some of them don't build, fiddling with the make file to point to the packages, such as FMOD, so Cmake can locate the information. 
After it is built, simply run the executable!

========
Controls:
WASD = move the camera or hero around in the world, depending on which one is active. 
Q and E = move the camera or hero up or down. 

C = changes the active camera. The two cameras are either a free camera or an arcballcam. 

1-4 = Change the currently selected fragment shader. If an invalide option is choosen, the default shader is used.

ESP = closes the program. 

========
Ester eggs! 
Hit space for an inspirational quote!

After 45 seconds, the King of Red Lions moves somewhere random in the field. Can you find him before his vertex shader takes him over?
