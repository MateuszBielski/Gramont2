#include "surfaceonequad.h"

SurfaceOneQuad::SurfaceOneQuad(float sizeX,float sizeY):
    gVerts {-sizeX/2.0f,-sizeY/2.0f,0.0f,
           sizeX/2.0f,-sizeY/2.0f,0.0f,
           sizeX/2.0f,sizeY/2.0f,0.0f,
           -sizeX/2.0f,sizeY/2.0f,0.0f
},
gColours {1.0f,0.0f,0.0f,1.0f,
          0.0f,1.0f,0.0f,1.0f,
          1.0f,1.0f,1.0f,1.0f,
          1.0f,1.0f,1.0f,1.0f,
         },
gNormals {0.0f,0.0f,1.0f,
          0.0f,0.0f,1.0f,
          0.0f,0.0f,1.0f,
          0.0f,0.0f,1.0f,
         },
gIndices { 0,1,3,2},
gTexCoord {
    0.0f, 0.0f,  
    1.0f, 0.0f,  
    1.0f, 1.0f, 
    0.0f, 1.0f, 
  }
 {
    data.verts = gVerts;
    data.colours = gColours;
    data.normals = gNormals;
    data.indices = gIndices;
    data.nuPoints =  4;
    data.nuColours =  4;
    data.nuNormals =  4;
    data.nuIndices =  4;
    
    m_texture = make_shared<TextureForModel>();
    m_texture->texCoord = gTexCoord;
    m_texture->nuTexCoord = 4;
}
