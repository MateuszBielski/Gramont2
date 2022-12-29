#include "pyramid.h"

Pyramid::Pyramid():
    gVerts { 100.0f, -40.8248f, -57.7350f,
           0.0f, -40.8248f, 115.4704f,
           -100.0f, -40.8248f, -57.7350f,
           0.0f, 122.4745f,   0.0f
},
gColours { 0.0f, 1.0f, 0.0f, 1.0f,
           1.0f, 0.0f, 0.0f, 1.0f,
           0.0f, 0.0f, 1.0f, 1.0f,
           1.0f, 1.0f, 0.0f, 0.3f
         },
gNormals { 0.0f,    -1.0f,      0.0f,    /* face 0 1 2 */
           -0.81650f, 0.33333f,  0.47140f, /* face 1 2 3 */
           0.0f,     0.33333f, -0.94281f, /* face 2 3 0 */
           0.81650f, 0.33333f,  0.47140f  /* face 3 0 1 */
         },
gIndices { 0, 1, 2, 3, 0, 1 } {
    data.verts = gVerts;
    data.colours = gColours;
    data.normals = gNormals;
    data.indices = gIndices;
    data.nuPoints = 4;
    data.nuNormals = 4;
    data.nuColours = 4;
    data.nuIndices = 6;
}
