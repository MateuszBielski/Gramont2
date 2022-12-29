#version 150
uniform vec4 lightProps; // Position in View space, and intensity
uniform vec3 lightColour;
//flat in vec4 theColour;
in vec4 theColour;
//flat in vec3 theNormal;
in vec3 theNormal;
in vec3 pointPos;
out vec4 fragColour;
// Declare this function
vec3 Illuminate(in vec4 LiProps, in vec3 LiColour, in vec4 PColour,
                in vec3 PNormal, in vec3 PPos);
void main(void)
{
    vec3 lightRes = Illuminate(lightProps, lightColour, theColour,
                               theNormal, pointPos);
    fragColour = vec4(lightRes, theColour.a);
}
