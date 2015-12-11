
varying vec2 st;

uniform sampler2D fbo;

void main() {
    vec4 texel = texture2D(fbo, st);
    texel.rgb = 0.21 * texel.r + 0.72 * texel.g + 0.07 * texel.b;
    gl_FragColor = texel;
}
