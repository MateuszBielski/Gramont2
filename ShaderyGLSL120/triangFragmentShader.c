#version 120
uniform vec4 lightProps; // Position in View space, and intensity
uniform vec3 lightColour;
//flat in vec4 theColour;
varying vec4 theColour;
//flat in vec3 theNormal;
varying vec3 theNormal;
varying vec3 pointPos;
//out vec4 fragColour;
// Declare this function
vec3 Illuminate(vec4 LiProps, vec3 LiColour, vec4 PColour,
                vec3 PNormal, vec3 PPos);
void main(void)
{
    vec3 lightRes = Illuminate(lightProps, lightColour, theColour,
                               theNormal, pointPos);
    gl_FragColor = vec4(lightRes, theColour.a);
}
