#version 150
uniform sampler2D stringTexture;
in vec2 textCoord;
out vec4 fragColour;
void main(void)
{
    fragColour= texture(stringTexture, textCoord);
}
