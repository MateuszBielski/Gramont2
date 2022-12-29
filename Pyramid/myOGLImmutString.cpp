#include "myOGLImmutString.h"
#include "../src/oglstuff.h"

void myOGLImmutString::SetImmutString(myOGLShaders* theShader,
                                      const unsigned char* tImage, int tWidth, int tHeigh)
{
    // Make a rectangle of the same size as the image. Order of vertices matters.
    // Set a 2 pixels margin
    double imaVerts[12];
    imaVerts[0] = 2.0         ;
    imaVerts[1] = 2.0         ;
    imaVerts[2]  = -1.0;
    imaVerts[3] = 2.0         ;
    imaVerts[4] = 2.0 + tHeigh;
    imaVerts[5]  = -1.0;
    imaVerts[6] = 2.0 + tWidth;
    imaVerts[7] = 2.0         ;
    imaVerts[8]  = -1.0;
    imaVerts[9] = 2.0 + tWidth;
    imaVerts[10] = 2.0 + tHeigh;
    imaVerts[11] = -1.0;

    // GLFloat version
    GLfloat fimaVerts[12];
    SetAsGLFloat4x4(imaVerts, fimaVerts, 12);

    // Call the base class without normals, it will handle this case
    SetStringWithVerts(theShader, tImage, tWidth, tHeigh, fimaVerts, NULL);
}

void myOGLImmutString::SetOrtho(int winWidth, int winHeight)
{
    // We want an image always of the same size, regardless of window size.
    // The orthogonal projection with the whole window achieves it.
    MyOrtho(0.0, winWidth, 0.0, winHeight, -1.0, 1.0, m_dOrtho);
    // Store the 'float' matrix
    SetAsGLFloat4x4(m_dOrtho, m_fOrtho, 16);
}
