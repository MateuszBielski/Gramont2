#include "surface1x3.h"
#include "iostream"
#include <string.h>

Surface1x3::Surface1x3():Surface(1,3,200,200)
{
//    if(data.indices) delete data.indices;
//    data.nuIndices = 8;
//
//    GLushort temp[]= {0,1,2,3,4,5,6,7};
//    GLushort * m_indices = new GLushort[data.nuIndices];
//    memcpy(m_indices,temp,sizeof(GLushort) * data.nuIndices);
//    data.indices = m_indices;

    if(data.colours) delete [] data.colours;
    data.nuColours = 2 * 1 * 3 + 2;//2 * 1 * 3 to jest za mało
    //order: r,g,b,a
//    GLfloat temp[] = {
//        1.0f,0.0f,0.0f,1.0f,
//        0.0f,1.0f,0.0f,1.0f,
//        0.0f,0.0f,1.0f,1.0f,
//        1.0f,0.0f,0.0f,1.0f,
//        0.0f,1.0f,0.0f,1.0f,
//        0.0f,0.0f,1.0f,1.0f,
//        1.0f,0.0f,0.0f,1.0f,
//        0.0f,1.0f,0.0f,1.0f,
//    };
    GLfloat temp[] = {
        1.0f,0.0f,0.0f,1.0f,
        0.0f,0.0f,0.0f,1.0f,//nieistotny
        0.0f,1.0f,0.0f,1.0f,
        1.0f,1.0f,1.0f,1.0f,//nieistotny
        0.0f,0.0f,1.0f,1.0f,
        1.0f,1.0f,1.0f,1.0f,//nieistotny
        1.0f,1.0f,0.0f,1.0f,
        1.0f,1.0f,1.0f,1.0f,//nieistotny
    };
    GLfloat * m_colours = new GLfloat[4 * data.nuColours];
    memcpy(m_colours,temp,sizeof(GLfloat) * data.nuColours * 4);
    
    
    data.colours =m_colours;
    cout<<"\nnuColours: "<<data.nuColours<<endl;
    for(short i = 0 ; i < data.nuColours ; i++)
    {
        cout<<i<<". ";
        for(short j = 0; j < 4 ; j++)
            cout<<data.colours[i * 4 + j]<<" ";
    cout<<endl;
    }
}
