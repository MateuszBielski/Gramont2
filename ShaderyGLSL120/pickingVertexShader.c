#version 120

attribute vec3 in_sPosition;
uniform mat4 mMVP;

varying vec2 textCoord;
void main(void)
{
    gl_Position = mMVP * vec4(in_sPosition, 1.0);
}
