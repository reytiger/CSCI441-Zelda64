/*
 *   Phong Shading vertex shader to simulate a single point light with texturing
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

varying float v_lifespan;

// The ratio of its life remaining.
attribute float a_lifespan;

void main() {
    v_lifespan = a_lifespan;

    // compute the camera vector in eye space
    vec3 cameraVec
        = normalize(vec3(gl_ModelViewMatrixInverse * vec4(0.0, 0.0, 0.0, 1.0))
                    - gl_Vertex.xyz);
    vec4 pos = gl_Vertex;
    // rotate them!

    gl_Position = gl_ModelViewProjectionMatrix * pos;

    // pass the texture coordinate through to the fragment processor
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
