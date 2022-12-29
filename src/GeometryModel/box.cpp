#include "box.h"


Box::Box():
    gVerts { -50.0f,-50.0f,50.0f,
           50.0f,-50.0f,50.0f,
           50.0f,-50.0f,-50.0f,
           -50.0f,-50.0f,-50.0f,
           -50.0f,50.0f,50.0f,
           50.0f,50.0f,50.0f,
           50.0f,50.0f,-50.0f,
           -50.0f,50.0f,-50.0f,
},
gColours { 1.0f, 0.0f, 0.0f, 1.0f,
           1.0f, 0.0f, 0.0f, 1.0f,
           0.0f, 0.0f, 1.0f, 1.0f,
           0.0f, 0.0f, 1.0f, 1.0f,
           0.0f, 0.0f, 1.0f, 0.3f,
           0.0f, 0.0f, 1.0f, 0.3f,
           1.0f, 0.0f, 0.0f, 1.0f,
           1.0f, 0.0f, 0.0f, 1.0f,
           0.0f, 0.0f, 1.0f, 1.0f,
           0.0f, 0.0f, 1.0f, 1.0f,
           0.0f, 0.0f, 1.0f, 0.3f,
           0.0f, 0.0f, 1.0f, 0.3f
         },
gNormals { 0.0f,    -1.0f,      0.0f,
           0.0f,    -1.0f,      0.0f,
           0.0f,    0.0f,      1.0f,
           0.0f,    0.0f,      1.0f,
           0.0f,    1.0f,      0.0f,
           0.0f,    1.0f,      0.0f,
           1.0f,    0.0f,      0.0f,
           1.0f,    0.0f,      0.0f,
           -1.0f,    0.0f,      0.0f,
           -1.0f,    0.0f,      0.0f,
         },
gIndices {3,2,0,1,4,5,7,6,5,1,6,2,7,3,4,0} {
//gIndices {3,2,0,1} {//4,5,7,6,5,1,6,2,7,3,4,0} {
    data.verts = gVerts;
    data.colours = gColours;
    data.normals = gNormals;
    data.indices = gIndices;
    data.nuPoints = 8;
    data.nuNormals = 12;
    data.nuColours = 12;
    data.nuIndices = 16;
//    data.primitiveMode = GL_QUAD_STRIP;
}
