#ifndef ModelData_H
#define ModelData_H
#include <glad/glad.h>

struct ModelData
{
    const GLfloat* verts;
    const GLfloat* colours;
    const GLfloat* normals;
    const GLfloat* tangents;
    const GLfloat* bitangents;
    const GLuint* indices;
    GLuint nuPoints;
    GLuint nuNormals;
    GLuint nuColours;
    GLuint nuIndices;
    GLuint nuTangents;
    GLuint nuBitangents;
    
    GLenum primitiveMode = GL_TRIANGLE_STRIP;
    GLuint bufVertId = 0;//genBuffers starts generate from number 1
    GLuint bufColNorId = 0;
    GLuint bufIndexId = 0;
    GLuint bufTangentId = 0;
    GLuint bufBitangentId = 0;
    GLsizeiptr offsetForNormalSubBuf;
    
    GLuint modelVAO = 0;
};

#endif // ModelData_H
