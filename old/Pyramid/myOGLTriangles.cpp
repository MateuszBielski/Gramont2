#include "myOGLTriangles.h"

// ----------------------------------------------------------------------------
// myOGLTriangles
// ----------------------------------------------------------------------------
myOGLTriangles::myOGLTriangles()
{
    m_triangVAO = m_bufVertId = m_bufColNorId = m_bufIndexId = 0;
    m_triangShaders = NULL;
}

myOGLTriangles::~myOGLTriangles()
{
    Clear();
}

void myOGLTriangles::Clear()
{
    if ( m_triangShaders )
        m_triangShaders->DisableGenericVAA();

    // Clear graphics card memory
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    if ( m_bufIndexId )
        glDeleteBuffers(1, &m_bufIndexId);
    if ( m_bufColNorId )
        glDeleteBuffers(1, &m_bufColNorId);
    if ( m_bufVertId )
        glDeleteBuffers(1, &m_bufVertId);

    // Unbind from context
    glBindVertexArray(0);
    if ( m_triangVAO )
        glDeleteVertexArrays(1, &m_triangVAO);

    glFlush(); //Tell GL to execute those commands now, but we don't wait for them

    m_triangShaders = NULL;
    m_triangVAO = m_bufIndexId = m_bufColNorId = m_bufVertId = 0;
}

void myOGLTriangles::SetBuffers(myOGLShaders* theShader,
                                GLsizei nuPoints, GLsizei nuTriangs,
                                const GLfloat* vert, const GLfloat* colo,
                                const GLfloat* norm, const GLushort* indices)
{
    MyOnGLError(myoglERR_CLEAR); //clear error stack

    // NOTE: have you realized that I fully trust on parameters being != 0 and != NULL?

    // Part 1: Buffers - - - - - - - - - - - - - - - - - - -

    // Graphics card buffer for vertices.
    // Not shared buffer with colours and normals, why not? Just for fun.
    glGenBuffers(1, &m_bufVertId);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufVertId);
    // Populate the buffer with the array "vert"
    GLsizeiptr nBytes = nuPoints * 3 * sizeof(GLfloat); //3 components {x,y,z}
    glBufferData(GL_ARRAY_BUFFER, nBytes, vert, GL_STATIC_DRAW);

    if ( ! MyOnGLError(myoglERR_BUFFER) ) {
        // Likely the GPU got out of memory
        Clear();
        return;
    }

    // Graphics card buffer for colours and normals.
    glGenBuffers(1, &m_bufColNorId);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufColNorId);
    // Allocate space for both arrays
    nBytes = (nuPoints * 4 + nuTriangs * 3) * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, nBytes, NULL, GL_STATIC_DRAW);
    if ( ! MyOnGLError(myoglERR_BUFFER) ) {
        // Likely the GPU got out of memory
        Clear();
        return;
    }
    // Populate part of the buffer with the array "colo"
    nBytes = nuPoints * 4 * sizeof(GLfloat); // rgba components
    glBufferSubData(GL_ARRAY_BUFFER, 0, nBytes, colo);
    // Add the array "norm" to the buffer
    GLsizeiptr bufoffset = nBytes;
    nBytes = nuTriangs * 3 * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, bufoffset, nBytes, norm);

    // Graphics card buffer for indices.
    glGenBuffers(1, &m_bufIndexId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIndexId);
    // Populate the buffer with the array "indices"
    // We use "triangle strip". An index for each additional vertex.
    nBytes = (3 + nuTriangs - 1) * sizeof(GLushort); //Each triangle needs 3 indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nBytes, indices, GL_STATIC_DRAW);

    if ( ! MyOnGLError(myoglERR_BUFFER) ) {
        // Likely the GPU got out of memory
        Clear();
        return;
    }

    // Unbind buffers. We will bind them one by one just now, at VAO creation
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_nuTriangs = nuTriangs;
    m_triangShaders = theShader;

    MyOnGLError(myoglERR_CLEAR); //clear error stack

    // Part 2: VAO - - - - - - - - - - - - - - - - - - -

    // Vertex Array Object (VAO) that stores the relationship between the
    // buffers and the shader input attributes
    glGenVertexArrays(1, &m_triangVAO);
    glBindVertexArray(m_triangVAO);

    // Set the way of reading (blocks of n floats each) from the current bound
    // buffer and passing data to the shader (through the index of an attribute).
    // Vertices positions
    glBindBuffer(GL_ARRAY_BUFFER, m_bufVertId);
    GLuint loc = m_triangShaders->GetAttribLoc("in_Position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    // Colours
    glBindBuffer(GL_ARRAY_BUFFER, m_bufColNorId);
    loc = m_triangShaders->GetAttribLoc("in_Colour");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    // Normals. Their position in buffer starts at bufoffset
    loc = m_triangShaders->GetAttribLoc("in_Normal");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)bufoffset);
    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufIndexId);

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Some log
    MyOnGLError(myoglERR_JUSTLOG, "Triangles data loaded into GPU.");
}

void myOGLTriangles::Draw(const GLfloat* unifMvp, const GLfloat* unifToVw,
                          const myLight* theLight)
{
    if ( !m_triangVAO )
        return;

    MyOnGLError(myoglERR_CLEAR); //clear error stack

    if ( ! m_triangShaders->Use() )
        return;

    // Bind the source data for the shader
    glBindVertexArray(m_triangVAO);

    // Pass matrices to the shader in column-major order
    glUniformMatrix4fv(m_triangShaders->GetUnifLoc("mMVP"), 1, GL_FALSE, unifMvp);
    glUniformMatrix4fv(m_triangShaders->GetUnifLoc("mToViewSpace"), 1, GL_FALSE, unifToVw);
    // Pass the light, in View coordinates in this sample
    glUniform4fv(m_triangShaders->GetUnifLoc("lightProps"), 1, theLight->GetFLightPos());
    glUniform3fv(m_triangShaders->GetUnifLoc("lightColour"), 1, theLight->GetFLightColour());

    // We have a flat shading, and we want the first vertex data as the flat value
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

    // Indexed drawing the triangles in strip mode, using 6 indices
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, (GLvoid *)0);

    MyOnGLError(myoglERR_DRAWING_TRI);

    // Unbind
    glBindVertexArray(0);
    m_triangShaders->StopUse();
}
