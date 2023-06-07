#version 120
attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec2 aTexCoords;
attribute vec3 aTangent;
attribute vec3 aBitangent;

varying vec3 FragPos;
varying vec2 TexCoords;
varying vec3 TangentLightPos;
varying vec3 TangentViewPos;
varying vec3 TangentFragPos;

uniform mat4 mMVP;
//uniform mat4 projection;
//uniform mat4 view;
//uniform mat4 model;//tu może być problem

//uniform vec3 lightPos;
//uniform vec3 viewPos;

void main()
{
//    vs_out.FragPos = vec3(model * vec4(aPos, 1.0)); 
    TexCoords = aTexCoords;   
    
//    vec3 T = normalize(mat3(model) * aTangent);
//    vec3 B = normalize(mat3(model) * aBitangent);
//    vec3 N = normalize(mat3(model) * aNormal);
//    mat3 TBN = transpose(mat3(T, B, N));
//
//    vs_out.TangentLightPos = TBN * lightPos;
//    vs_out.TangentViewPos  = TBN * viewPos;
//    vs_out.TangentFragPos  = TBN * vs_out.FragPos;
    
    gl_Position = mMVP * vec4(aPos, 1.0);
}