#include "onemodelmanagerOld.h"
#include "../textfile.h"
//#include "../funkcje.h"

#define d_path "ShaderyCoreProfile/"
#define d_vertexShaderPath d_path"triangVertexShader.c"
#define d_fragmentShaderPath d_path"triangFragmentShader.c"
#define d_illuminationShaderPath d_path"illuminationShader.c"
//
OneModelManager::OneModelManager(myOGLErrHandler* extErrHnd):myOGLManager(extErrHnd)
{
}
void OneModelManager::Clear()
{
    m_ModelShader.DisableGenericVAA();
//
//    // Clear graphics card memory
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    if ( modelIndexBufId )
        glDeleteBuffers(1, &modelIndexBufId);
    if ( modelColorNormalBufId )
        glDeleteBuffers(1, &modelColorNormalBufId);
    if ( modelVertBufId )
        glDeleteBuffers(1, &modelVertBufId);

    // Unbind from context
    glBindVertexArray(0);
    if ( modelVAO )
        glDeleteVertexArrays(1, &modelVAO);

    glFlush(); //Tell GL to execute those commands now, but we don't wait for them

//    m_ModelShader = NULL;
    modelVAO = modelIndexBufId = modelColorNormalBufId = modelVertBufId = 0;
}
void OneModelManager::OdczytajShaderyZplikow()
{
    modelVertexShader = textFileRead(d_vertexShaderPath);
    modelFragmentShader = textFileRead(d_fragmentShaderPath);
    illuminationShader = textFileRead(d_illuminationShaderPath);
}
void OneModelManager::SetShadersAndGeometry()
{
    OdczytajShaderyZplikow();
    m_ModelShader.AddAttrib("in_Position");
    m_ModelShader.AddAttrib("in_Colour");
    m_ModelShader.AddAttrib("in_Normal");
    m_ModelShader.AddUnif("mMVP");
    m_ModelShader.AddUnif("mToViewSpace");
    m_ModelShader.AddUnif("lightProps");
    m_ModelShader.AddUnif("lightColour");
    m_ModelShader.AddCode(modelVertexShader, GL_VERTEX_SHADER);
    m_ModelShader.AddCode(illuminationShader, GL_FRAGMENT_SHADER);
    m_ModelShader.AddCode(modelFragmentShader, GL_FRAGMENT_SHADER);
    m_ModelShader.Init();
    m_Light.Set(myVec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0, 1.0);
    //buffers
    GLuint modelVertBufId;
    GLuint modelColorNormalBufId;
    GLuint modelIndexBufId;
    GLuint modelVAO;

    GLsizei nuPoints = 4;
    GLsizei nuTriangs = 4;
    const GLfloat vert[] = { 100.0f, -40.8248f, -57.7350f,
                             0.0f, -40.8248f, 115.4704f,
                             -100.0f, -40.8248f, -57.7350f,
                             0.0f, 122.4745f,   0.0f
                           };
    const GLfloat colo[] = { 0.0f, 1.0f, 0.0f, 1.0f,
                             1.0f, 0.0f, 0.0f, 1.0f,
                             0.0f, 0.0f, 1.0f, 1.0f,
                             1.0f, 1.0f, 0.0f, 0.3f
                           };
    const GLfloat norm[] = { 0.0f,    -1.0f,      0.0f,    /* face 0 1 2 */
                             -0.81650f, 0.33333f,  0.47140f, /* face 1 2 3 */
                             0.0f,     0.33333f, -0.94281f, /* face 2 3 0 */
                             0.81650f, 0.33333f,  0.47140f  /* face 3 0 1 */
                           };
    const GLuint indices[] = { 0, 1, 2, 3, 0, 1 };
    /***************/
    //Jakich nazw używać?
//    auto model_buff = model.GetStructureForBuffers;
//    glGenBuffers(1, model_buff.p_VertId);
//    glBindBuffer(GL_ARRAY_BUFFER, model_buff.VertId);
//    GLsizeiptr nBytes = model_buff.nuPoints * 3 * sizeof(GLfloat);//Niech oblicza model.GetNbytes(GLfloat);
//    glBufferData(GL_ARRAY_BUFFER, model_buff.nBytes, model_buff.vert, GL_STATIC_DRAW);
    /***************/

    glGenBuffers(1, &modelVertBufId);
    glBindBuffer(GL_ARRAY_BUFFER, modelVertBufId);
    GLsizeiptr nBytes = nuPoints * 3 * sizeof(GLfloat); //3 components {x,y,z}
    glBufferData(GL_ARRAY_BUFFER, nBytes, vert, GL_STATIC_DRAW);

    if ( ! MyOnGLError(myoglERR_BUFFER) ) {
        // Likely the GPU got out of memory
        Clear();
        return;
    }

    glGenBuffers(1, &modelColorNormalBufId);
    glBindBuffer(GL_ARRAY_BUFFER, modelColorNormalBufId);
    // Allocate space for both arrays
    nBytes = (nuPoints * 4 + nuTriangs * 3) * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, nBytes, NULL, GL_STATIC_DRAW);

    if ( ! MyOnGLError(myoglERR_BUFFER) ) {
        // Likely the GPU got out of memory
        Clear();
        return;
    }

    nBytes = nuPoints * 4 * sizeof(GLfloat); // rgba components
    glBufferSubData(GL_ARRAY_BUFFER, 0, nBytes, colo);
    // Add the array "norm" to the buffer
    GLsizeiptr bufoffset = nBytes;
    nBytes = nuTriangs * 3 * sizeof(GLfloat);
    glBufferSubData(GL_ARRAY_BUFFER, bufoffset, nBytes, norm);

    if ( ! MyOnGLError(myoglERR_BUFFER) ) {
        // Likely the GPU got out of memory
        Clear();
        return;
    }

    // Unbind buffers. We will bind them one by one just now, at VAO creation
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    MyOnGLError(myoglERR_CLEAR);

    glGenBuffers(1, &modelIndexBufId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIndexBufId);
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

//    modelVAO
    glGenVertexArrays(1, &modelVAO);
    glBindVertexArray(modelVAO);

    // Set the way of reading (blocks of n floats each) from the current bound
    // buffer and passing data to the shader (through the index of an attribute).
    // Vertices positions
    glBindBuffer(GL_ARRAY_BUFFER, modelVertBufId);
    GLuint loc =m_ModelShader.GetAttribLoc("in_Position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    // Colours
    glBindBuffer(GL_ARRAY_BUFFER, modelColorNormalBufId);
    loc =m_ModelShader.GetAttribLoc("in_Colour");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    // Normals. Their position in buffer starts at bufoffset
    loc =m_ModelShader.GetAttribLoc("in_Normal");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)bufoffset);
    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIndexBufId);

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Some log
    MyOnGLError(myoglERR_JUSTLOG, "Triangles data loaded into GPU.");
}
void OneModelManager::Draw3d()
{
    MyOnGLError(myoglERR_CLEAR); //clear error stack

    if ( ! m_ModelShader.Use() )
        return;

    // Bind the source data for the shader
    glBindVertexArray(modelVAO);

    // Pass matrices to the shader in column-major order
    glUniformMatrix4fv(m_ModelShader.GetUnifLoc("mMVP"), 1, GL_FALSE, m_Camera.GetFloatMVP());
    glUniformMatrix4fv(m_ModelShader.GetUnifLoc("mToViewSpace"), 1, GL_FALSE, m_Camera.GetFloatToVw());
    // Pass the light, in View coordinates in this sample
    glUniform4fv(m_ModelShader.GetUnifLoc("lightProps"), 1, m_Light.GetFLightPos());
    glUniform3fv(m_ModelShader.GetUnifLoc("lightColour"), 1, m_Light.GetFLightColour());

    // We have a flat shading, and we want the first vertex data as the flat value
    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

    // Indexed drawing the triangles in strip mode, using 6 indices
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, (GLvoid *)0);

    MyOnGLError(myoglERR_DRAWING_TRI);

    // Unbind
    glBindVertexArray(0);
    m_ModelShader.StopUse();
}
