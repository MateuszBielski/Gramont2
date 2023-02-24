#include "glFunctionsMock.h"
#include <glad/glad.h>
#include <random>

bool GlFunctionsMock::defined = false;
unsigned int glGetError_Mock(){ return 0;}
GLuint glCreateProgram_Mock(){return 1;} //1?
const GLubyte* glGetString_Mock(GLenum name){ return nullptr;}
GLuint glCreateShader_Mock(GLenum type){return 1;} //1?
GLint glGetUniformLocation_Mock(GLuint program, const GLchar *name){return 1;} //1?
void glCompileShader_Mock(GLuint shader){}
void glLinkProgram_Mock(GLuint program){}
void glDeleteProgram_Mock(GLuint program){}
void glGenBuffers_Mock(GLsizei n, GLuint *buffers)
{
    *buffers = rand() % 10;
}
void glBindAttribLocation_Mock(GLuint program, GLuint index, const GLchar *name){}
void glShaderSource_Mock(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length){}
void glAttachShader_Mock(GLuint program, GLuint shader){}
void glDetachShader_Mock(GLuint program, GLuint shader){}
void glDeleteShader_Mock(GLuint shader){}
void glGetShaderiv_Mock(GLuint shader, GLenum pname, GLint *params){}
void glGetShaderInfoLog_Mock(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog){}
void glGetProgramiv_Mock(GLuint program, GLenum pname, GLint *params){}
void glGetProgramInfoLog_Mock(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog){}
void glUseProgram_Mock(GLuint program){}
void glDisableVertexAttribArray_Mock(GLuint index){}
void glViewport_Mock(GLint x, GLint y, GLsizei width, GLsizei height){}
void glDepthFunc_Mock(GLenum func){}
void glEnable_Mock(GLenum cap){}
void glBlendFunc_Mock(GLenum sfactor, GLenum dfactor){}
void glClearColor_Mock(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){}
void glClear_Mock(GLbitfield mask){}
void glBindBuffer_Mock(GLenum target, GLuint buffer){}
void glDeleteBuffers_Mock(GLsizei n, const GLuint *buffers){}
void glBindVertexArray_Mock(GLuint array){}
void glDeleteVertexArrays_Mock(GLsizei n, const GLuint *arrays){}
void glBufferData_Mock(GLenum target, GLsizeiptr size, const void *data, GLenum usage){}
void glBufferSubData_Mock(GLenum target, GLintptr offset, GLsizeiptr size, const void *data){}
void glGenVertexArrays_Mock(GLsizei n, GLuint *arrays){}
void glEnableVertexAttribArray_Mock(GLuint index){}
void glVertexAttribPointer_Mock(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer){}
void glActiveTexture_Mock(GLenum texture){}
void glGenTextures_Mock(GLsizei n, GLuint *textures){}
void glBindTexture_Mock(GLenum target, GLuint texture){}
void glTexParameteri_Mock(GLenum target, GLenum pname, GLint param){}
void glTexImage2D_Mock(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels){}
void glUniformMatrix4fv_Mock(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){}
void glUniform4fv_Mock(GLint location, GLsizei count, const GLfloat *value){}
void glUniform3fv_Mock(GLint location, GLsizei count, const GLfloat *value){}
void glUniform1i_Mock(GLint location, GLint v0){}
void glDrawElements_Mock(GLenum mode, GLsizei count, GLenum type, const void *indices){}
void glFlush_Mock(){}
void glFinish_Mock(){};
void glGenFramebuffers_Mock(GLsizei n, GLuint *framebuffers){};
void glBindFramebuffer_Mock(GLenum target, GLuint framebuffer){};
void glFramebufferTexture2_Mock(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){};
GLenum glCheckFramebufferStatus_Mock(GLenum target){return 1;};
void glReadBuffer_Mock(GLenum src){};
void glDrawBuffer_Mock(GLenum buf){};

GlFunctionsMock::GlFunctionsMock()
{
	
}
GlFunctionsMock::~GlFunctionsMock()
{
}
bool GlFunctionsMock::Define()
{
    if(defined) return false;
    glGetError = &glGetError_Mock;
    glFlush = &glFlush_Mock;
    glCreateProgram = &glCreateProgram_Mock;
    glGetString = &glGetString_Mock;
    glFinish = &glFinish_Mock;
    glCreateShader = &glCreateShader_Mock;
    glCompileShader = &glCompileShader_Mock;
    glLinkProgram = &glLinkProgram_Mock;
    glDeleteProgram = &glDeleteProgram_Mock;
    glGenBuffers = &glGenBuffers_Mock;
    glBindAttribLocation = &glBindAttribLocation_Mock;
    glGetUniformLocation = &glGetUniformLocation_Mock;
    glShaderSource = &glShaderSource_Mock;
    glAttachShader = &glAttachShader_Mock;
    glDetachShader = &glDetachShader_Mock;
    glDeleteShader = &glDeleteShader_Mock;
    glGetShaderiv = &glGetShaderiv_Mock;
    glGetShaderInfoLog = &glGetShaderInfoLog_Mock;
    glGetProgramiv = &glGetProgramiv_Mock;
    glGetProgramInfoLog = &glGetProgramInfoLog_Mock;
    glUseProgram = &glUseProgram_Mock;
    glDisableVertexAttribArray = &glDisableVertexAttribArray_Mock;
    glViewport = &glViewport_Mock;
    glDepthFunc = &glDepthFunc_Mock;
    glEnable = &glEnable_Mock;
    glBlendFunc = &glBlendFunc_Mock;
    glClearColor = &glClearColor_Mock;
    glClear = &glClear_Mock;
    glBindBuffer = &glBindBuffer_Mock;
    
    glDeleteBuffers = &glDeleteBuffers_Mock;
    glBindVertexArray = &glBindVertexArray_Mock;
    glDeleteVertexArrays = &glDeleteVertexArrays_Mock;
    glBufferData = &glBufferData_Mock;
    glBufferSubData = &glBufferSubData_Mock;
    glGenVertexArrays = &glGenVertexArrays_Mock;
    glEnableVertexAttribArray = &glEnableVertexAttribArray_Mock;
    glVertexAttribPointer = &glVertexAttribPointer_Mock;
    glActiveTexture = &glActiveTexture_Mock;
    glGenTextures = &glGenTextures_Mock;
    glBindTexture = &glBindTexture_Mock;
    glTexParameteri = &glTexParameteri_Mock;
    glTexImage2D = &glTexImage2D_Mock;
    glUniformMatrix4fv = &glUniformMatrix4fv_Mock;
    glUniform4fv = &glUniform4fv_Mock;
    glUniform3fv = &glUniform3fv_Mock;
    glUniform1i = &glUniform1i_Mock;
    glDrawElements = &glDrawElements_Mock;
    glGenFramebuffers = &glGenFramebuffers_Mock;
    glBindFramebuffer = &glBindFramebuffer_Mock;
    glFramebufferTexture2D = &glFramebufferTexture2_Mock;
    glCheckFramebufferStatus = &glCheckFramebufferStatus_Mock;
    glReadBuffer = &glReadBuffer_Mock;
    glDrawBuffer = &glDrawBuffer_Mock;
    
    defined = true;
    return true;
    // regex gl[A-Z][a-zA-Z]+\(.*\)
}
bool GlFunctionsMock::IsDefined()
{
	return defined;
}
