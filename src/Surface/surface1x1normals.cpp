#include "surface1x1normals.h"

#include "iostream"
#include <string.h>
#include <glm/glm.hpp>

Surface1x1normals::Surface1x1normals():Surface(5,5,200,200)
{
    glm::vec3 normal2 = glm::normalize(glm::vec3(0.0f, 0.05f, 1.0f));
    if(data.normals) delete [] data.normals;
    data.nuNormals = data.nuPoints;

//    GLfloat temp[] = {
//        0.0f,0.0f,1.0f,
//        0.0f,0.0f,1.0f,
//        normal2.x,normal2.y,normal2.z,
//        0.0f,0.0f,1.0f,
//    };
    GLfloat * m_normals = new GLfloat[3 * data.nuNormals];
    for(short i = 0 ; i < data.nuNormals ; i++) {
        m_normals[i * 3] = 0.0f;
        m_normals[i * 3 + 1] = 0.0f;
        m_normals[i * 3 + 2] = 1.0f;
    }
    m_normals[16 * 3] = normal2.x;
    m_normals[16 * 3 + 1] = normal2.y;
    m_normals[16 * 3 + 2] = normal2.z;
    m_normals[17 * 3] = normal2.x;
    m_normals[17 * 3 + 1] = normal2.y;
    m_normals[17 * 3 + 2] = normal2.z;
    data.normals = m_normals;
//    memcpy(m_normals,temp,sizeof(GLfloat) * data.nuNormals * 3);
    cout<<"\nnuNormals: "<<data.nuNormals<<endl;
    for(short i = 0 ; i < data.nuNormals ; i++) {
        cout<<i<<". ";
        for(short j = 0; j < 3 ; j++)
            cout<<data.normals[i * 3 + j]<<" ";
        cout<<endl;
    }
}
