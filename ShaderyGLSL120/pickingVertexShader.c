#version 120

attribute vec3 in_sPosition;
attribute vec3 in_sNormal;
attribute vec2 in_TextPos;
uniform mat4 mMVP;

varying vec2 textCoord;
void main(void)
{
    gl_Position = mMVP * vec4(in_sPosition, 1.0);
    textCoord = in_TextPos;
}
