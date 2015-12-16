
varying vec2 st;

uniform sampler2D fbo;

void main() {
    vec4 texel = texture2D(fbo, st);
    gl_FragColor = texel * 0.005 * gl_FragCoord;
}
