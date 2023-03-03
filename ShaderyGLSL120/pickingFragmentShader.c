#version 120
uniform sampler2D stringTexture;
varying vec2 textCoord;

void main(void)
{
//    gl_FragColor = texture2D(stringTexture, textCoord);
    vec4 tex = texture2D(stringTexture, textCoord);
    gl_FragColor = vec4(111.0005,-1420.0,0.3004,1.0);
}