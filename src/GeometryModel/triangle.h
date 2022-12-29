#ifndef triangle_H
#define triangle_H
#include "onemodel.h"

using namespace std;

class Triangle : public OneModel
{
private:
    const GLfloat gVerts[3*3];
    const GLfloat gColours[4*3];
    const GLfloat gNormals[3*3];
    const GLuint gIndices[3];
    const GLfloat gTexCoord[6];
    
    public:
   Triangle();
    protected:
           
};

#endif // triangle_H
