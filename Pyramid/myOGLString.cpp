#include "myOGLString.h"


// ----------------------------------------------------------------------------
// myOGLString
// ----------------------------------------------------------------------------
myOGLString::myOGLString()
{
    m_bufPosId = m_textureId = m_stringVAO = m_textureUnit = 0;
    m_stringShaders = NULL;
}

myOGLString::~myOGLString()
{
    Clear();
}

void myOGLString::Clear()
{
    if ( m_stringShaders )
        m_stringShaders->DisableGenericVAA();

    // Clear graphics card memory
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if ( m_bufPosId )
        glDeleteBuffers(1, &m_bufPosId);

    // Unbind from context
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_stringVAO);

    if ( m_textureUnit && m_textureId ) {
        glActiveTexture(GL_TEXTURE0 + m_textureUnit);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &m_textureId);
    }
    glActiveTexture(GL_TEXTURE0);

    glFlush(); //Tell GL to execute those commands now, but we don't wait for them

    m_bufPosId = m_textureId = m_stringVAO = m_textureUnit = 0;
    m_stringShaders = NULL;
}

void myOGLString::SetStringWithVerts(myOGLShaders* theShader,
                                     const unsigned char* tImage, int tWidth, int tHeigh,
                                     const GLfloat* vert, const GLfloat* norm)
{
    MyOnGLError(myoglERR_CLEAR); //clear error stack

    if ( !tImage )
        return;

    // Part 1: Buffers - - - - - - - - - - - - - - - - - - -

    // Graphics card buffer for vertices, normals, and texture coords
    glGenBuffers(1, &m_bufPosId);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufPosId);
    // (4+4) (vertices + normals) x 3 components + 4 text-vertices x 2 components
    GLsizeiptr nBytes = (8 * 3 + 4 * 2) * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, nBytes, NULL, GL_STATIC_DRAW);

    if ( ! MyOnGLError(myoglERR_BUFFER) ) {
        // Likely the GPU got out of memory
        Clear();
        return;
    }

    // Populate part of the buffer with the array "vert"
    nBytes = 12 * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nBytes, vert);
    // Add the array "norm" to the buffer
    GLsizeiptr bufoffset = nBytes;
    if ( norm ) {
        // Just for string on face, not immutable string
        glBufferSubData(GL_ARRAY_BUFFER, bufoffset, nBytes, norm);
    }

    // Add the array of texture coordinates to the buffer.
    // Order is set accordingly with the vertices
    // See myOGLManager::SetStringOnPyr()
    GLfloat texcoords[8] = { 0.0, 1.0,  0.0, 0.0,  1.0, 1.0,  1.0, 0.0 };
    bufoffset += nBytes;
    nBytes = 8 * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, bufoffset, nBytes, texcoords);

    m_stringShaders = theShader;

    MyOnGLError(myoglERR_CLEAR); //clear error stack

    // Part 2: VAO - - - - - - - - - - - - - - - - - - -

    // Vertex Array Object (VAO) that stores the relationship between the
    // buffers and the shader input attributes
    glGenVertexArrays(1, &m_stringVAO);
    glBindVertexArray(m_stringVAO);

    // Set the way of reading (blocks of n floats each) from the current bound
    // buffer and passing data to the shader (through the index of an attribute).
    // Vertices positions
    GLuint loc = m_stringShaders->GetAttribLoc("in_sPosition");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    // Normals. Their position in buffer starts at bufoffset
    bufoffset = 12 * sizeof(GLfloat);
    if ( norm ) {
        // Just for string on face, not immutable string
        loc = m_stringShaders->GetAttribLoc("in_sNormal");
        glEnableVertexAttribArray(loc);
        glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)bufoffset);
    }
    // Texture coordinates
    bufoffset *= 2; //Normals take same amount of space as vertices
    loc = m_stringShaders->GetAttribLoc("in_TextPos");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)bufoffset);

    // Part 3: The texture with the string as an image - - - - - - - -

    // Create the bind for the texture
    // Same unit for both textures (strings) since their characteristics are the same.
    m_textureUnit = 1;
    glActiveTexture(GL_TEXTURE0 + m_textureUnit);
    glGenTextures(1, &m_textureId); //"Name" of the texture object
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    // Avoid some artifacts
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Do this before glTexImage2D because we only have 1 level, no mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    // For RGBA default alignment (4) is good. In other circumstances, we may
    // need glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
    // Load texture into card. No mipmap, so 0-level
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 (GLsizei)tWidth, (GLsizei)tHeigh, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, tImage);
    if ( ! MyOnGLError(myoglERR_TEXTIMAGE) ) {
        // Likely the GPU got out of memory
        Clear();
        return;
    }

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);

    // Some log
    MyOnGLError(myoglERR_JUSTLOG, "Texture for string loaded into GPU.");
}

void myOGLString::Draw(const GLfloat* unifMvp, const GLfloat* unifToVw,
                       const myLight* theLight)
{
    if ( !m_stringVAO )
        return;

    MyOnGLError(myoglERR_CLEAR); //clear error stack

    if ( ! m_stringShaders->Use() )
        return;

    // Bind the source data for the shader
    glBindVertexArray(m_stringVAO);

    // Pass matrices to the shader in column-major order
    glUniformMatrix4fv(m_stringShaders->GetUnifLoc("mMVP"), 1, GL_FALSE, unifMvp);
    if ( unifToVw && theLight ) {
        // Just for string on face, not immutable string
        glUniformMatrix4fv(m_stringShaders->GetUnifLoc("mToViewSpace"), 1, GL_FALSE, unifToVw);
        // Pass the light, in View coordinates in this sample
        glUniform4fv(m_stringShaders->GetUnifLoc("lightProps"), 1, theLight->GetFLightPos());
        glUniform3fv(m_stringShaders->GetUnifLoc("lightColour"), 1, theLight->GetFLightColour());

        // We have a flat shading, and we want the first vertex normal as the flat value
        glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    }

    // Use our texture unit
    glActiveTexture(GL_TEXTURE0 + m_textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    // The fragment shader will read texture values (pixels) from the texture
    // currently active
    glUniform1i(m_stringShaders->GetUnifLoc("stringTexture"), m_textureUnit);

    // Draw the rectangle made up of two triangles
    glDrawArrays(GL_TRIANGLE_STRIP, 0,  4);

    MyOnGLError(myoglERR_DRAWING_STR);

    // Unbind
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);

    m_stringShaders->StopUse();
}