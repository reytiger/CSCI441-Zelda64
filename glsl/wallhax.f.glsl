varying vec4 color;

void main() {
    gl_FragColor = color;
    gl_FragDepth *= -1;
}
