#ifndef ModelData_H
#define ModelData_H
#include <glad/glad.h>

struct ModelData
{
    const GLfloat* verts = nullptr;
    const GLfloat* colours = nullptr;
    const GLfloat* normals = nullptr;
    const GLfloat* tangents = nullptr;
    const GLfloat* bitangents = nullptr;
    const GLuint* indices = nullptr;
    GLuint nuPoints;
    GLuint nuNormals;
    GLuint nuColours;
    GLuint nuIndices;
    GLuint nuTangents = 0;//info for CreateBuffers
    GLuint nuBitangents = 0;
    
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
