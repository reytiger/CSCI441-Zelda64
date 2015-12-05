varying vec4 color;

void main() {
    gl_FragColor.rgb = pow(gl_FragCoord.z, 100);
    gl_FragColor.a   = 1.0;
}
