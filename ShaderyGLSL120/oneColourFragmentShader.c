#version 120

varying vec3 theNormal;
varying vec3 pointPos;

uniform vec4 lightProps;
uniform vec3 lightColour;

vec3 Illuminate(vec4 LiProps, vec3 LiColour, vec4 PColour,
                vec3 PNormal, vec3 PPos);
void main(void)
{
    vec4 colo4 = vec4(0.3, 1.0, 0.5, 0.8);
    vec3 lightRes = Illuminate(lightProps, lightColour, colo4,
                               theNormal, pointPos);
                               
    gl_FragColor = vec4(lightRes, colo4.a);
}
