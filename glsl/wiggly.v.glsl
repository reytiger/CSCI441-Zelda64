varying vec4 color;

uniform float time;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_Position.y *= 1 + 0.05 * sin(time);
    color = gl_Color;
}
