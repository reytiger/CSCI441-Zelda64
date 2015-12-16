
varying vec2 st;

uniform sampler2D fbo;

void main() {
    vec4 texel = texture2D(fbo, st);
    texel.rgb = vec3(max(texel.rgb) + min(texel.rgb)) / 2.0;
    gl_FragColor = texel;
}
