#version 120
attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec3 aTangent;
attribute vec3 aBitangent;
attribute vec2 aTexCoords;

uniform mat4 model;
uniform mat4 mMVP;
uniform mat4 mToViewSpace;

uniform vec3 lightPos;
uniform vec3 viewPos;

varying vec3 theNormal;
varying vec3 pointPos;
varying vec2 textCoord;
varying vec3 FragPos;
varying vec3 TangentLightPos;
varying vec3 TangentViewPos;
 varying vec3 TangentFragPos;
void main(void)
{
    
    textCoord = aTexCoords;
    vec4 temp4 = mToViewSpace * vec4(aPos, 1.0);
    pointPos = temp4.xyz;
    temp4 = mToViewSpace * vec4(aNormal, 0.0);
    theNormal = normalize(temp4.xyz);

    /*normal*/
    FragPos = vec3(model * vec4(aPos, 1.0));
    /*mat3 model3 = mat3(model);
    mat3 normalMatrix = transpose(inverse(model3));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;*/


    /*vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));

    TangentLightPos = TBN * lightPos;
    TangentViewPos = TBN * viewPos;
    TangentFragPos = TBN * FragPos;*/

    gl_Position = mMVP * vec4(aPos, 1.0);
}
