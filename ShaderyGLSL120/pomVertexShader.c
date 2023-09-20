#version 120
attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec3 aTangent;
attribute vec3 aBitangent;
attribute vec2 aTexCoords;

//uniform mat4 model;
uniform mat4 mMVP;
uniform mat4 mToViewSpace;
uniform mat4 mInvModelView;

uniform vec3 lightPos;
uniform vec3 viewPos;

varying vec3 theNormal;
varying vec3 pointPos;
varying vec2 textCoord;
varying vec3 viewPosVary;
varying mat3 invTBN3;
varying mat4 transform;
varying mat4 invModelView4;

void main()
{
    textCoord = aTexCoords;
    viewPosVary = viewPos;
    invModelView4 = mInvModelView;
    vec4 temp4 = mToViewSpace * vec4(aPos, 1.0);
    pointPos = temp4.xyz;
    temp4 = mToViewSpace * vec4(aNormal, 0.0);
    theNormal = normalize(temp4.xyz);
    invTBN3 = transpose(mat3(aTangent, aBitangent, aNormal));
    mat4 TBN4 = mat4(mat3(aTangent, aBitangent, aNormal));
    transform = mToViewSpace * TBN4;// ;

    gl_Position = mMVP * vec4(aPos, 1.0);
}