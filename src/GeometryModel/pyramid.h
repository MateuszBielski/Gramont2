#ifndef Pyramid_H
#define Pyramid_H
#include "onemodel.h"

using namespace std;

class Pyramid : public OneModel
{
private:
    const GLfloat gVerts[3*4];
    const GLfloat gColours[4*4];
    const GLfloat gNormals[3*4];
    const GLuint gIndices[6];
public:
    Pyramid();
protected:

};

#endif // Pyramid_H
