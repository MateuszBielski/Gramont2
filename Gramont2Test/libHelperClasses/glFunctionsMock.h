#ifndef GLFUNCTIONS_MOCK_H
#define GLFUNCTIONS_MOCK_H
#include <glad/glad.h>

class GlFunctionsMock
{
private:
    static bool defined;
public:
    GlFunctionsMock();
    ~GlFunctionsMock();
    bool Define();
    bool IsDefined();
};

unsigned int glGetError_Mock();
GLuint glCreateProgram_Mock();
const GLubyte* glGetString_Mock(GLenum name);
GLuint glCreateShader_Mock(GLenum type);
GLint glGetUniformLocation_Mock(GLuint program, const GLchar *name);
void glCompileShader_Mock(GLuint shader);
void glLinkProgram_Mock(GLuint program);
void glDeleteProgram_Mock(GLuint program);
void glGenBuffers_Mock(GLsizei n, GLuint *buffers);
void glBindAttribLocation_Mock(GLuint program, GLuint index, const GLchar *name);
void glShaderSource_Mock(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
void glAttachShader_Mock(GLuint program, GLuint shader);
void glDetachShader_Mock(GLuint program, GLuint shader);
void glDeleteShader_Mock(GLuint shader);
void glGetShaderiv_Mock(GLuint shader, GLenum pname, GLint *params);
void glGetShaderInfoLog_Mock(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void glGetProgramiv_Mock(GLuint program, GLenum pname, GLint *params);
void glGetProgramInfoLog_Mock(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void glUseProgram_Mock(GLuint program);
void glDisableVertexAttribArray_Mock(GLuint index);
void glViewport_Mock(GLint x, GLint y, GLsizei width, GLsizei height);
void glDepthFunc_Mock(GLenum func);
void glEnable_Mock(GLenum cap);
void glBlendFunc_Mock(GLenum sfactor, GLenum dfactor);
void glClearColor_Mock(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void glClear_Mock(GLbitfield mask);
void glBindBuffer_Mock(GLenum target, GLuint buffer);
void glDeleteBuffers_Mock(GLsizei n, const GLuint *buffers);
void glBindVertexArray_Mock(GLuint array);
void glDeleteVertexArrays_Mock(GLsizei n, const GLuint *arrays);
void glBufferData_Mock(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
void glBufferSubData_Mock(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
void glGenVertexArrays_Mock(GLsizei n, GLuint *arrays);
void glEnableVertexAttribArray_Mock(GLuint index);
void glVertexAttribPointer_Mock(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
void glActiveTexture_Mock(GLenum texture);
void glGenTextures_Mock(GLsizei n, GLuint *textures);
void glBindTexture_Mock(GLenum target, GLuint texture);
void glTexParameteri_Mock(GLenum target, GLenum pname, GLint param);
void glTexImage2D_Mock(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
void glUniformMatrix4fv_Mock(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glUniform4fv_Mock(GLint location, GLsizei count, const GLfloat *value);
void glUniform3fv_Mock(GLint location, GLsizei count, const GLfloat *value);
void glUniform1i_Mock(GLint location, GLint v0);
void glDrawElements_Mock(GLenum mode, GLsizei count, GLenum type, const void *indices);
void glFlush_Mock();
void glFinish_Mock();
void glGenFramebuffers_Mock(GLsizei n, GLuint *framebuffers);
void glBindFramebuffer_Mock(GLenum target, GLuint framebuffer);
void glFramebufferTexture2_Mock(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
GLenum glCheckFramebufferStatus_Mock(GLenum target);
void glReadBuffer_Mock(GLenum src);
void glDrawBuffer_Mock(GLenum buf);
#endif
