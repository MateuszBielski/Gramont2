#version 120
uniform vec4 lightProps;
uniform vec3 lightColour;
uniform sampler2D diffuseMap;


varying vec3 theNormal;
varying vec3 pointPos;
varying vec2 textCoord;



vec3 Illuminate(vec4 LiProps, vec3 LiColour, vec4 PColour,
                vec3 PNormal, vec3 PPos);
void main(void)
{
    vec4 colo4 = texture2D(diffuseMap, textCoord);
    vec3 lightRes = Illuminate(lightProps, lightColour, colo4,
                               theNormal, pointPos);
    gl_FragColor = vec4(lightRes, colo4.a);
    
    
}
