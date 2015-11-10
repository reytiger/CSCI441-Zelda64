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

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // pass the texture coordinate through to the fragment processor
    gl_TexCoord[0] = gl_MultiTexCoord0;
}
