#version 150
in vec3 in_sPosition;
in vec2 in_TextPos;
uniform mat4 mMVP;
out vec2 textCoord;
void main(void)
{
    gl_Position = mMVP * vec4(in_sPosition, 1.0);
    textCoord = in_TextPos;
}
