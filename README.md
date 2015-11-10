# Assignment 5
Author: Chris Butler

Bugs:
    Controls of the hero are wonky. Heading isn't taken into account, so they're
    a little counter-intuitive.

    Incallidus doesn't have a shader, or texture, so he inherits from whatever.

    The particles do not billboard and do not support transparent textures.

    The ground is still rendered as a single quad, without fragment shader
        lighting, lighting doesn't show up on it.

    TODO: control file

    TODO: Test build on Lab machines

Features:
    The vulcano, the statue, and the temple all share a shader - an unmodified "Wiggly".
        This also means they ignore lighting.

    A vulcano! The 'embers', which are really flowers, darken as they
        'cool' and expire. They fall and bounce with gravity.

    Press the Space Bar to cast Incallidus's spell! The particles move with him,
        so if you press it while he's running, they will continue to move
        when you stop.
        They are short lived and follow the same rules as the vulcano.
