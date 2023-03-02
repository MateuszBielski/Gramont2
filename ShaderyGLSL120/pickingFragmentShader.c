#version 120
uniform sampler2D stringTexture;
varying vec2 textCoord;

void main(void)
{
    gl_FragColor = texture2D(stringTexture, textCoord);
}