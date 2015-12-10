
varying vec2 st;

uniform sampler2D fbo;

void main() {
    vec4 texel = texture2D(fbo, st);
    texel.rgb = 1.0 - texel.rgb;
    gl_FragColor = texel;
}
