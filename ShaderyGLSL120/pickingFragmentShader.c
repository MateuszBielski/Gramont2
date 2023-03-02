#version 120
uniform vec4 lightProps; // Position in View space, and intensity
uniform vec3 lightColour;
uniform sampler2D stringTexture;
//flat varying vec3 theNormal;// dzięki flat jest rozbłysk
varying vec3 theNormal;
//in vec3 theNormal; 
varying vec3 pointPos;
varying vec2 textCoord;
//out vec4 fragColour;
// Declare this function
vec3 Illuminate(vec4 LiProps, vec3 LiColour, vec4 PColour,
                vec3 PNormal, vec3 PPos);
void main(void)
{
    vec4 colo4 = texture2D(stringTexture, textCoord);
    vec3 lightRes = Illuminate(lightProps, lightColour, colo4,
                               theNormal, pointPos);
    gl_FragColor = vec4(lightRes, colo4.a);
}
