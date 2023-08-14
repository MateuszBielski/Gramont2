#version 120
attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec3 aTangent;
attribute vec3 aBitangent;
attribute vec2 aTexCoords;

//uniform mat4 model;
uniform mat4 mMVP;
uniform mat4 mToViewSpace;

uniform vec3 lightPos;
uniform vec3 viewPos;

varying vec3 theNormal;
varying vec3 pointPos;
varying vec2 textCoord;
//varying mat3 TBN3;
varying mat4 transform;

void main()
{
    textCoord = aTexCoords;
    vec4 temp4 = mToViewSpace * vec4(aPos, 1.0);
    pointPos = temp4.xyz;
    temp4 = mToViewSpace * vec4(aNormal, 0.0);
    theNormal = normalize(temp4.xyz);

    mat4 TBN4 = mat4(mat3(aTangent, aBitangent, aNormal));
    transform = mToViewSpace * TBN4;// ;

    gl_Position = mMVP * vec4(aPos, 1.0);
}