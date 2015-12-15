varying vec4 color;

uniform float time;

void main() {
    vec4 pos = gl_Vertex;
    pos.xyz *= 0.005;
    pos.x += 0.5 * sin(0.22 * time * pos.y);
    pos.z += 0.5 * cos(0.3 * time * pos.y);
    pos = gl_ModelViewProjectionMatrix * pos;

    gl_Position = pos;
    color       = vec4(gl_Normal, 1.0);
}
