# Assignment 5
Author: Chris Butler

Bugs:
    Controls of the hero are wonky. Heading isn't taken into account, so they're
    a little counter-intuitive.

    Incallidus doesn't have a shader, or texture, so he inherits from whatever.

    The particles do not billboard and do not support transparent textures.

    The ground is still rendered as a single quad, without fragment shader
        lighting, lighting doesn't show up on it.

    Something is causing a GL_INVALID_OPERATION and I'm not sure what.
        Stuff works thought.

    For low particle spawn rates and high frame counts, the rate of particples
        per second can become 0 when converted to an integer.

    The version I'm turning in spawns 3500 particles a second when casting,
        ish. It runs smoothly on the semi-beefy computers in Alamode, but
        laptops may not fair as well.

    I rewrote the Makefile to be a frontend for CMake. I had issues getting it
        to play nice with environment variables to actually call CMake when I
        tried it in Marquez. It should work on any Linux or Mac set up without
        much issue. Especially if GLUT and GLEW are installed in default
        locations and CMake is installed, rather than drag-and-dropped.

    Apologies if this is hard to build! Please let me know if you have any issues.
        I can pull the Makefile out of git's version control and see if that works.
        Actually, I think I turn the .git folder in too. So you could do that
        too....

Features:
    The vulcano, the statue, and the temple all share a shader - an unmodified "Wiggly".
        This also means they ignore lighting.

    A vulcano! The 'embers', which are really flowers, darken as they
        'cool' and expire. They fall and bounce with gravity.

    Press the Space Bar to cast Incallidus's spell! The particles move with him,
        so if you press it while he's running, they will continue to move
        when you stop. They are short lived though!

