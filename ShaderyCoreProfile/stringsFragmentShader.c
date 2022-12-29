#version 150
uniform vec4 lightProps; // Position in View space, and intensity
uniform vec3 lightColour;
uniform sampler2D stringTexture;
flat in vec3 theNormal;
in vec3 pointPos;
in vec2 textCoord;
out vec4 fragColour;
// Declare this function
vec3 Illuminate(in vec4 LiProps, in vec3 LiColour, in vec4 PColour,
                in vec3 PNormal, in vec3 PPos);
void main(void)
{
    vec4 colo4 = texture(stringTexture, textCoord);
    vec3 lightRes = Illuminate(lightProps, lightColour, colo4,
                               theNormal, pointPos);
    fragColour = vec4(lightRes, colo4.a);
}
