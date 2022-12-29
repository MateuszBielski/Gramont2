#ifndef MYOGLTRIANGLES_H
#define MYOGLTRIANGLES_H

#include "../src/oglstuff.h"

class myOGLTriangles
{
public:
    myOGLTriangles();
    ~myOGLTriangles();

    // Clean up
    void Clear();
    // Load data into the GPU
    void SetBuffers(myOGLShaders* theShader, GLsizei nuPoints, GLsizei nuTriangs,
                    const GLfloat* vert, const GLfloat* colo,
                    const GLfloat* norm, const GLushort* indices);

    //Draw the triangles
    void Draw(const GLfloat* unifMvp, const GLfloat* unifToVw,
              const myLight* theLight);

private:
    GLsizei m_nuTriangs;
    // Buffers ids
    GLuint m_bufVertId;
    GLuint m_bufColNorId;
    GLuint m_bufIndexId;
    // Vertex Arrays Object
    GLuint m_triangVAO;
    // GPU Program used to draw the triangles
    myOGLShaders* m_triangShaders;
};

#endif