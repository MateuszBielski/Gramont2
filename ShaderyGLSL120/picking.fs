#version 120                                                                        
uniform uint gDrawIndex;                                                            
uniform uint gObjectIndex;                                                          

varying vec3 FragColor;

void main()                                                                         
{                                                                                   
   FragColor = vec3(float(gObjectIndex), float(gDrawIndex),float(gl_PrimitiveID + 1));
}