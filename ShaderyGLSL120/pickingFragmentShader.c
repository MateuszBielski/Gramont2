#version 120
uniform int modelUniqueId;

void main(void)
{
    gl_FragColor = vec4(111.0005,float(modelUniqueId),0.3004,1.0);
}