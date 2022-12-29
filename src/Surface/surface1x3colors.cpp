#include "surface1x3colors.h"
#include "iostream"
#include <string.h>

Surface1x3colors::Surface1x3colors():Surface(1,3,200,200)
{
    if(data.colours) delete [] data.colours;
//    data.nuColours = 2 * 1 * 3 + 2;
//    GLfloat temp[] = {
//        1.0f,0.0f,0.0f,1.0f,
//        0.0f,0.0f,0.0f,1.0f,
//        0.0f,1.0f,0.0f,1.0f,
//        1.0f,1.0f,1.0f,1.0f,//nieistotny
//        0.0f,0.0f,1.0f,1.0f,
//        1.0f,1.0f,1.0f,1.0f,//nieistotny
//        1.0f,1.0f,0.0f,1.0f,
//        1.0f,1.0f,1.0f,1.0f,//nieistotny
//    };
    data.nuColours = 8;
    GLfloat temp[] = {
        1.0f,0.0f,0.0f,1.0f,
        0.0f,0.0f,0.0f,1.0f,
        0.0f,0.0f,0.0f,1.0f,
        0.0f,0.0f,0.0f,1.0f,
        0.0f,0.0f,0.0f,1.0f,
        0.0f,0.0f,0.0f,1.0f,
        1.0f,1.0f,0.0f,1.0f,
        1.0f,1.0f,0.0f,1.0f,
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