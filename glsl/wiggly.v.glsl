varying vec4 color;

uniform float time;

void main() {
    vec4 pos = gl_ModelViewProjectionMatrix * gl_Vertex;
    pos.x += 0.5 * sin(0.23 * time * pos.z);
    pos.x += 0.5 * cos(0.17 * time * pos.z);

    gl_Position = pos;
    color = vec4(gl_Normal, 1.0);
}
