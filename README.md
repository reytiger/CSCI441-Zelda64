# CSCI441-Zelda64
Computer Graphics final project
Contributors: Chris Butler, Kevin Carbaugh, Nico Pampe

=======

This final was an open ended project where students can create a world through GLUT, Glew, and OpenGL. We were to follow several requirements for the project. Different types of shaders were needed such as a fragment color shader or blur shader. In addition, a vertex shader was needed. With these shaders, the world needed several lights and an animated object that the user can control.

For our final project, we created a theme around "The Legend of Zelda: Ocarina of Time." We load Hyrule Field from an wavefront object (and mtl + textures) which we found online at http://www.models-resource.com/nintendo_64/legendofzeldaocarinaoftime/. Using FMOD for sound, we added Hyrule Field's triumphent overworld theme. To finish it off, a model of Link was rigged, skinned, animated and placed into the world. Link and a camera are controlled via the keyboard or, if it's plugged in, an Xbox controller. We tried to load other maps from the same site, but had issues with the files. Spaces in names were particularly problematic.

The project is built with cmake. To make life easier, there's a Makefile which calls cmake and *should* do everything for you, but will likely not work on the first try. You should be able to change the paths already set to make it work, or just run 'ccmake ..' or cmake-gui from the build-lab directory (which is made upon invokation of 'make'). This will allow you to manually specify paths to libraries and directories for included headers.

The FMOD thing works by assuming a directory structure that I copied from the zip file download. You can adjust this by setting FMOD_ROOT; e.g.
	FMOD_ROOT=/Users/jpaone/my_freaky_fmod_folder make
will adjust it. Again, opening ccmake or cmake-gui and manually adjusting any conflicting variables might be the easiest solution.

If you build this on Windows, you can use the xbox controller by setting USE_XINPUT to 'TRUE' in cmake. You might need to use Visual Studio to get this to work. If you have some other controller that uses XInput, it should also work.

Please don't hesitate to email us if there are issues building. We'd hate to lose points over that.

Also, Nico uplaoded the full source code.

Controls
========

WASD      Move the camera or hero around in the world, depending on which one is active.
Q, E      Move the camera or hero up or down.
C         Changes the active camera. The two cameras are either a free camera or an arcballcam.
1-5       Change the currently selected fragment shader. Any other number results in no shader passes being run.

Esc       Closes the program.

Ctrl + Left mouse button controls the zoom level in arcball camera mode

On the controller, we have:

Left stick                Move
Right stick               Look around
Left button, right stick  When in arcball, zoom in and out
y                         Shift through shaders
Start                     Switch camera
Back/Select               Closes the program.

Ester eggs!
========
Hit space/A for an inspirational quote!

Every 30 seconds, the King of Red Lions moves somewhere random in the field. Can you find him before his vertex shader takes him over?

Who did what?
=======

Kevin
-----

Nico
----

Chris
-----
	Multi-pass rendering
	Code architecture
	Fragment shaders for the post-processing
	Controller support
	Being unreasonably strict about code formatting etc.
