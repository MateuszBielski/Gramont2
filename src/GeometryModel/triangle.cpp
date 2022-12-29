#include "triangle.h"

Triangle::Triangle():
    gVerts { -100.0f,0.0f,0.0f,
           100.0f,0.0f,0.0f,
           0.0f,100.0f,0.0f,
},
gColours { 0.0f, 1.0f, 0.0f, 1.0f,
           0.0f, 0.0f, 1.0f, 1.0f,
           1.0f, 0.0f, 0.0f, 1.0f,
         },
gNormals { 0.0f,    0.0f,      1.0f,
           0.3f,    0.0f,      0.8f,
           0.0f,    0.0f,      1.0f,
         },
gIndices { 0, 1, 2},
gTexCoord {
    0.0f, 0.0f,  // lower-left corner
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner};
} {
    data.verts = gVerts;
    data.colours = gColours;
    data.normals = gNormals;
    data.indices = gIndices;
    data.nuPoints = 3;
    data.nuColours = 3;
    data.nuNormals = 3;
    data.nuIndices = 3;

    m_texture = make_shared<TextureForModel>();
    m_texture->texCoord = gTexCoord;
    m_texture->nuTexCoord = 3;
}
