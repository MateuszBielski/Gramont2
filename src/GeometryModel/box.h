#ifndef Box_H
#define Box_H
#include "onemodel.h"
using namespace std;

class Box : public OneModel
{
    private:
    const GLfloat gVerts[3*8];
    const GLfloat gColours[4*12];
    const GLfloat gNormals[3*12];
    const GLuint gIndices[16];
    public:
   Box();
    protected:
           
};

#endif // Box_H
