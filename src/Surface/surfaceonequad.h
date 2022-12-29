#ifndef SurfaceOneQuad_H
#define SurfaceOneQuad_H
#include "onemodel.h"

using namespace std;

class SurfaceOneQuad : public OneModel
{
private:
    const GLfloat gVerts[3* 4];
    const GLfloat gColours[4* 4];
    const GLfloat gNormals[3* 4];
    const GLuint gIndices[4 ];
    const GLfloat gTexCoord[8];
public:
    SurfaceOneQuad(float,float);
protected:

};

#endif // SurfaceOneQuad_H
