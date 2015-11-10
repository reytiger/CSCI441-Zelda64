#version 120

varying float v_lifespan;

uniform sampler2D u_tex;

void main(void) {
    vec4 u_tex = texture2D(u_tex, gl_TexCoord[0].st);

    gl_FragColor.rgb = (1.0 - pow(v_lifespan, 5.0) )* u_tex.rgb;
}
