#ifndef MYOGLIMMUT_STRING_H
#define MYOGLIMMUT_STRING_H
#include "myOGLString.h"

class myOGLImmutString : public myOGLString
{
public:
    myOGLImmutString() {}
    ~myOGLImmutString() {}

    // Load data into the GPU
    void SetImmutString(myOGLShaders* theShader,
                        const unsigned char* tImage, int tWidth, int tHeigh);
    // Update orthogonal projection matrix
    void SetOrtho(int winWidth, int winHeight);
    // The transformation matrix
    const GLfloat* GetFloatMVP() {
        return m_fOrtho;
    }

private:
    double m_dOrtho[16]; // The orthogonal projection matrix
    GLfloat m_fOrtho[16]; // Same as float
};

#endif