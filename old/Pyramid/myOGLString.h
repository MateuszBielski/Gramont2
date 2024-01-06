#ifndef MYOGLSTRING_H
#define MYOGLSTRING_H
#include "../src/oglstuff.h"

class myOGLString
{
public:
    myOGLString();
    ~myOGLString();

    // Clean up
    void Clear();
    // Load data into the GPU
    void SetStringWithVerts(myOGLShaders* theShader,
                            const unsigned char* tImage, int tWidth, int tHeigh,
                            const GLfloat* vert, const GLfloat* norm);
    // Draw the string
    void Draw(const GLfloat* unifMvp, const GLfloat* unifToVw,
              const myLight* theLight);


private:
    GLuint m_bufPosId;    // Buffer id
    GLuint m_stringVAO;   // Vertex Arrays Object
    GLuint m_textureUnit; // The context unit
    GLuint m_textureId;   // Texture name
    // GPU Program used to draw the texture
    myOGLShaders* m_stringShaders;
};

#endif 