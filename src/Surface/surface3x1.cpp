#include "surface3x1.h"
#include "iostream"
#include <string.h>


Surface3x1::Surface3x1():Surface(3,1,200,200)
{
    if(data.indices) delete data.indices;
    data.nuIndices = 8;
//    GLushort temp[]={0, 1, 4,5};//ok

//    GLushort temp[]={4, 0, 5,1};//zle
//    GLushort temp[]={4, 0, 1,5};//zle
    GLuint temp[]= {0,1,2,6,1,5,0,4};
    GLuint * m_indices = new GLuint[data.nuIndices];
    memcpy(m_indices,temp,sizeof(GLuint) * data.nuIndices);
    data.indices = m_indices;
    cout<<"\nnuIndices: "<<data.nuIndices<<endl;
    for(short i = 0 ; i < data.nuIndices ; i++)cout<<data.indices[i]<<" ";
    cout<<endl;
}
