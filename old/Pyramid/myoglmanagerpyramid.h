#ifndef myOGLManagerPyramid_H
#define myOGLManagerPyramid_H
#include "../src/oglstuff.h"
#include "myOGLImmutString.h"
#include "myOGLTriangles.h"

using namespace std;


class myOGLManagerPyramid : public myOGLManager
{
private:
    myOGLShaders   m_TriangShaders;
    myOGLShaders   m_StringShaders;
    myOGLShaders   m_ImmutStringSha;
    myOGLTriangles    m_Triangles;
    myOGLString       m_StringOnPyr;
    myOGLImmutString  m_ImmString;
    
    // Vertex shader for the triangles
    const GLchar* triangVertexShader;
    const GLchar* illuminationShader;
    const GLchar* triangFragmentShader;
    const GLchar* stringsVertexShader;
    const GLchar* stringsFragmentShader;
    const GLchar* stringsImmutableVS;
    const GLchar* stringsImmutableFS;
    
    const GLfloat gVerts[3*4];
    const GLfloat gColours[4*4];
    const GLfloat gNormals[3*4];
    const GLushort gIndices[6];
    
    // Load the texture for the string in the pyramid
    void SetStringOnPyr(const unsigned char* strImage, int iWidth, int iHeigh);
    // Load the texture for the immutable string
    void SetImmutableString(const unsigned char* strImage, int iWidth, int iHeigh);
public:
    myOGLManagerPyramid(myOGLErrHandler* extErrHnd = NULL);
    virtual void SetShadersAndGeometry() override;
    virtual void SetViewport(int x, int y, int width, int height) override;
    virtual void Draw3d() override;
    virtual void Draw2d() override;
    virtual void ZapiszShaderyDoPlikow() override;
    virtual void OdczytajShaderyZplikow() override;
  
};

#endif // myOGLManagerPyramid_H
