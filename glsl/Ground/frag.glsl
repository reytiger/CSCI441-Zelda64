/*
 *   Fragment shader that implements Phong Shading with texturing.
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

varying vec3 normalVec;
varying vec3 lightVec;
varying vec3 halfwayVec;
varying float attenuation;

uniform sampler2D tex;

void main(void)
{

    /*****************************************/
    /********* Texture Calculations  *********/
    /*****************************************/

    // get the texel corresponding to the interpolated texture coordinates
    vec4 texel = texture2D( tex, gl_TexCoord[0].st );

    /*****************************************/
    /********* Lighting Calculations *********/
    /*****************************************/

    // compute the ambient component of lighting
    vec4 ambientComponent;
    if( gl_FrontFacing )
        ambientComponent = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    else
        ambientComponent = gl_BackMaterial.ambient * gl_LightSource[0].ambient;

    // compute the diffuse component of lighting
    vec4 diffuseComponent;
    if( gl_FrontFacing )
        diffuseComponent = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * max( 0.0, dot( normalVec, lightVec ) ) * attenuation;
    else
        diffuseComponent = gl_BackMaterial.diffuse * gl_LightSource[0].diffuse * max( 0.0, dot( normalVec, lightVec ) ) * attenuation;

    // compute the specular component of lighting
    vec4 specularComponent;
    if( gl_FrontFacing )
        specularComponent = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow( max( 0.0, dot( normalVec, halfwayVec ) ), gl_FrontMaterial.shininess ) * attenuation;
    else
        specularComponent = gl_BackMaterial.specular * gl_LightSource[0].specular * pow( max( 0.0, dot( normalVec, halfwayVec ) ), gl_FrontMaterial.shininess ) * attenuation;

    // compute the emission component of lighting
    vec4 emissionComponent;
    if( gl_FrontFacing )
        emissionComponent = gl_FrontMaterial.emission;
    else
        emissionComponent = gl_BackMaterial.emission;

    // set the lighting color
    vec4 theColor = ambientComponent + diffuseComponent + specularComponent + emissionComponent;

    /*****************************************/
    /******* Final Color Calculations ********/
    /*****************************************/

    // break out the texture colors and lighting colors
    vec3 ct = texel.rgb;
    vec3 cf = theColor.rgb;

    float at = texel.a;
    float af = theColor.a;

    // compute the modulated resulting color
    gl_FragColor = vec4( ct + cf, at * af );
}
