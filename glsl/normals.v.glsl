varying vec4 color;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    color       = vec4(gl_Normal, 1.0);
}
