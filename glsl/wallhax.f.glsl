varying vec4 color;

void main() {
    gl_FragColor = 0.5 * gl_FragCoord.z + 0.5;
    gl_FragDepth *= -1;
}
