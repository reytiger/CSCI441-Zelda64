
varying vec2 st;

uniform sampler2D fbo;

void main() {
    vec4 texel = texture2D(fbo, st);
    texel.rgb = vec3(texel.r + texel.g + texel.b) / 3.0;
    gl_FragColor = texel;
}
