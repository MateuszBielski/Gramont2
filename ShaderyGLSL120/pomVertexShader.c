#version 120
attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec2 aTexCoords;
attribute vec3 aTangent;
attribute vec3 aBitangent;

//varying vec3 FragPos;
varying vec2 TexCoords;
varying vec3 TangentLightPos;
varying vec3 TangentViewPos;
varying vec3 TangentFragPos;
varying vec3 theNormal; /*experiment*/
varying vec3 pointPos; /*experiment*/

uniform mat4 model;
uniform mat4 mMVP;
uniform mat4 mToViewSpace; /*experiment*/
//uniform mat4 projection;
//uniform mat4 view;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    /*textureShader start*/
    gl_Position = mMVP * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
    // Operations in View Space
    vec4 temp4 = mToViewSpace * vec4(aPos, 1.0);
    pointPos = temp4.xyz;
    temp4 = mToViewSpace * vec4(aNormal, 0.0);
    theNormal = normalize(temp4.xyz);
    /*textureShader finish*/
    
    vec3 FragPos = vec3(model * vec4(aPos, 1.0)); 
    TexCoords = aTexCoords;   
    
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
    mat3 TBN = transpose(mat3(T, B, N));

    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
//    
//    gl_Position = mMVP * vec4(aPos, 1.0);
}