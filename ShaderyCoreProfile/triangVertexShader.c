#version 150
in vec3 in_Normal;
in vec4 in_Colour;
in vec3 in_Position;
uniform mat4 mMVP;
uniform mat4 mToViewSpace;
//#flat out vec4 theColour;
out vec4 theColour;
//flat out vec3 theNormal;
out vec3 theNormal;
out vec3 pointPos;
void main(void)
{
    gl_Position = mMVP * vec4(in_Position, 1.0);
//    gl_Position = vec4(in_Position, 1.0);
    theColour = in_Colour;
    // Operations in View Space
    vec4 temp4 = mToViewSpace * vec4(in_Position, 1.0);
    pointPos = temp4.xyz;
    temp4 = mToViewSpace * vec4(in_Normal, 0.0);
    theNormal = normalize(temp4.xyz);
    /**/
//    theNormal = in_Normal;
}
