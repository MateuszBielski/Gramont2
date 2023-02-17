#version 120                                                                        

uniform int gDrawIndex;                                                            
uniform int gObjectIndex;                                                          

//varying vec3 FragColor;

void main()                                                                         
{                                                                                   
//   FragColor = vec3(float(gObjectIndex), float(gDrawIndex),float(TriangleID + 1)); 
   gl_FragColor = vec4(float(gObjectIndex), float(gDrawIndex),0.0,0.0);
}