#version 150
in vec3 in_sPosition;
in vec3 in_sNormal;
in vec2 in_TextPos;
uniform mat4 mMVP;
uniform mat4 mToViewSpace;
flat out vec3 theNormal;// dzięki flat jest rozbłysk
//out vec3 theNormal;
out vec3 pointPos;
out vec2 textCoord;
void main(void)
{
    gl_Position = mMVP * vec4(in_sPosition, 1.0);
    textCoord = in_TextPos;
    // Operations in View Space
    vec4 temp4 = mToViewSpace * vec4(in_sPosition, 1.0);
    pointPos = temp4.xyz;
    temp4 = mToViewSpace * vec4(in_sNormal, 0.0);
    theNormal = normalize(temp4.xyz);
}
