#version 120
uniform vec4 lightProps;
uniform vec3 lightColour;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform int normalEnabled;

varying vec3 theNormal;
varying vec3 pointPos;
varying vec2 textCoord;

varying mat4 transform;

vec3 Illuminate(vec4 LiProps, vec3 LiColour, vec4 PColour,
                vec3 PNormal, vec3 PPos);
void main(void)
{
    vec3 normal;
    if (normalEnabled == 1) {
        normal = texture2D(normalMap, textCoord).rgb;
        // transform normal vector to range [-1,1]
        normal = normalize((normal * 2.0 - 1.0));
        vec4 temp4 = transform * vec4(normal, 0.0);
        normal = normalize(temp4.xyz);
    } else {
        normal = theNormal;
    }
    
    vec4 colo4 = texture2D(diffuseMap, textCoord);
    vec3 lightRes = Illuminate(lightProps, lightColour, colo4,
                               normal, pointPos);
    gl_FragColor = vec4(lightRes, colo4.a);
}
