#version 120
attribute vec3 aPos;
attribute vec3 aNormal;

uniform mat4 mMVP;
uniform mat4 mToViewSpace;
uniform mat4 mInvModelView;

uniform vec3 lightPos;

varying vec3 theNormal;
varying vec3 pointPos;

void main()
{
	vec4 temp4 = mToViewSpace * vec4(aPos, 1.0);
    pointPos = temp4.xyz;
    
    temp4 = mToViewSpace * vec4(aNormal, 0.0);
    theNormal = normalize(temp4.xyz);
    gl_Position = mMVP * vec4(aPos, 1.0);
}