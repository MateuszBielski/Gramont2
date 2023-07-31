#version 120
attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec2 aTexCoords;

uniform mat4 mMVP;
uniform mat4 mToViewSpace;

varying vec3 theNormal;
varying vec3 pointPos;
varying vec2 textCoord;
void main(void)
{
    gl_Position = mMVP * vec4(aPos, 1.0);
    textCoord = aTexCoords;
    vec4 temp4 = mToViewSpace * vec4(aPos, 1.0);
    pointPos = temp4.xyz;
    temp4 = mToViewSpace * vec4(aNormal, 0.0);
    theNormal = normalize(temp4.xyz);
}
