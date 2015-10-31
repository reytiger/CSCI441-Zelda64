# Assignment 5
Author: Chris Butler

Bugs:
    Controls of the hero are wonky. Heading isn't taken into account, so they're
    a little counter-intuitive.

Features:
    I have four shaders under glsl:
        The 2 standard pass-through shaders
        A vertex shader which passes normals as the color
        A fragment shader which reverses the depth of things, which I have
            affectionately named "Wallhax".

    Any pairing of the above (1 vertex, 1 fragment) should work.

    I tried to get the Z-buffer to render, but it needs to be linearized
        apparently, and that's hard to do without uniforms.
