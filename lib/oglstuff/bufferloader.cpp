#include "bufferloader.h"
#include "oglstuff.h"

void BufferLoader::ClearBuffersForSingleModelEntry(ModelData& d)
{
    //    if ( m_triangShaders )
//    m_ModelShader.DisableGenericVAA();

    // Clear graphics card memory
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//    auto& d = model->GetModelData();
    if ( d.bufIndexId )
        glDeleteBuffers(1, &d.bufIndexId);
    if ( d.bufColNorId )
        glDeleteBuffers(1, &d.bufColNorId);
    if ( d.bufVertId )
        glDeleteBuffers(1, &d.bufVertId);

    // Unbind from context
    glBindVertexArray(0);
    if ( d.modelVAO )
        glDeleteVertexArrays(1, &d.modelVAO);

    glFlush(); //Tell GL to execute those commands now, but we don't wait for them

//    m_triangShaders = NULL;
    d.modelVAO = d.bufIndexId = d.bufColNorId = d.bufVertId = 0;
}
BufferLoaderProgress BufferLoader::CreateBuffersForSingleModelEntry(ModelData& d)
{
    bool ok = true;
    ok &= (bool)d.bufColNorId;
    ok &= (bool)d.bufIndexId;
    ok &= (bool)d.bufVertId;
    ++createBuffersCheckedCount;
    if(ok) return BufferLoaderProgress::Checked;

    MyOnGLError(myoglERR_CLEAR);
    GLsizeiptr nBytes;
    if(!d.bufVertId) {

        glGenBuffers(1, &d.bufVertId);
        glBindBuffer(GL_ARRAY_BUFFER, d.bufVertId);
        // Populate the buffer with the array "vert"
        nBytes = d.nuPoints * 3 * sizeof(GLfloat); //3 components {x,y,z}
        glBufferData(GL_ARRAY_BUFFER, nBytes, d.verts, GL_STATIC_DRAW);

        if ( ! MyOnGLError(myoglERR_BUFFER) ) {
            // Likely the GPU got out of memory
            ClearBuffersForSingleModelEntry(d);
            return BufferLoaderProgress::VertexError;
        }
    }
//    GLsizeiptr bufoffset;
    if(!d.bufColNorId) {


        // Graphics card buffer for colours and normals.
        glGenBuffers(1, &d.bufColNorId);
        glBindBuffer(GL_ARRAY_BUFFER, d.bufColNorId);
        // Allocate space for both arrays
        nBytes = (d.nuColours * 4 + d.nuNormals * 3) * sizeof(GLfloat);
        glBufferData(GL_ARRAY_BUFFER, nBytes, NULL, GL_STATIC_DRAW);
        if ( ! MyOnGLError(myoglERR_BUFFER) ) {
            // Likely the GPU got out of memory
            ClearBuffersForSingleModelEntry(d);
            return BufferLoaderProgress::ColorNormalsError;
        }
        // Populate part of the buffer with the array "colo"
        nBytes = d.nuColours * 4 * sizeof(GLfloat); // rgba components
        glBufferSubData(GL_ARRAY_BUFFER, 0, nBytes, d.colours);
        // Add the array "norm" to the buffer
        d.offsetForNormalSubBuf = nBytes;
        nBytes = d.nuNormals * 3 * sizeof(GLfloat);
        glBufferSubData(GL_ARRAY_BUFFER, d.offsetForNormalSubBuf, nBytes, d.normals);
    }
    if(!d.bufIndexId) {

        // Graphics card buffer for indices.
        glGenBuffers(1, &d.bufIndexId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.bufIndexId);
        // Populate the buffer with the array "indices"
        // We use "triangle strip". An index for each additional vertex.
        nBytes = d.nuIndices * sizeof(GLuint); //Each triangle needs 3 indices
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, nBytes, d.indices, GL_STATIC_DRAW);

        if ( ! MyOnGLError(myoglERR_BUFFER) ) {
            // Likely the GPU got out of memory
            ClearBuffersForSingleModelEntry(d);
            return BufferLoaderProgress::IndexError;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return BufferLoaderProgress::Completed;
}

void BufferLoader::LoadBuffersForSingleModelEntry(ModelData& d)
{
    MyOnGLError(myoglERR_CLEAR); //clear error stack

    // Part 2: VAO - - - - - - - - - - - - - - - - - - -

    // Vertex Array Object (VAO) that stores the relationship between the
    // buffers and the shader input attributes
    glGenVertexArrays(1, &d.modelVAO);
    glBindVertexArray(d.modelVAO);
    // Set the way of reading (blocks of n floats each) from the current bound
    // buffer and passing data to the shader (through the index of an attribute).
    // Vertices positions
    glBindBuffer(GL_ARRAY_BUFFER, d.bufVertId);
//    GLuint loc = m_ModelShader.GetAttribLoc("in_Position");
    glEnableVertexAttribArray(m_loc.position);
    glVertexAttribPointer(m_loc.position, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    // Colours
    glBindBuffer(GL_ARRAY_BUFFER, d.bufColNorId);
//    loc = m_ModelShader.GetAttribLoc("in_Colour");
    glEnableVertexAttribArray(m_loc.colour);
    glVertexAttribPointer(m_loc.colour, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    // Normals. Their position in buffer starts at d.offsetForNormalSubBuf
//    loc = m_ModelShader.GetAttribLoc("in_Normal");
    glEnableVertexAttribArray(m_loc.normal);
    glVertexAttribPointer(m_loc.normal, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)d.offsetForNormalSubBuf);
    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.bufIndexId);

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // Some log
    MyOnGLError(myoglERR_JUSTLOG, "Triangles data loaded into GPU.");
}
bool BufferLoader::LoadTextureBuffersForSingleModelEntry(TextureForModel& tex, ModelData& d)
{
    ++loadTextureFailsCount;
    bool ok = true;

    ok &= (bool)tex.width;
    ok &= (bool)tex.height;
    ok &= (bool)tex.TextureData();
    ok &= (bool)tex.texCoord;
    ok &= (bool)tex.nuTexCoord;
    ok &= (bool)d.bufIndexId;
    ok &= (bool)d.bufColNorId;
    ok &= (bool)d.nuColours;
    ok &= (bool)d.bufVertId;
//    ok &= (bool)m_loc.position_tex;
//    ok &= (bool)m_loc.textureCoord;
//    ok &= (bool)m_loc.normal_tex;
    if(!ok)return false;
    GLsizeiptr nBytes = 2 * tex.nuTexCoord * sizeof(GLfloat);

    glGenBuffers(1, &tex.bufTexCoordId);
    glBindBuffer(GL_ARRAY_BUFFER, tex.bufTexCoordId);
    // Populate the buffer with the array "vert"
    glBufferData(GL_ARRAY_BUFFER, nBytes, tex.texCoord, GL_STATIC_DRAW);


    MyOnGLError(myoglERR_CLEAR); //clear error stack

//    auto& d = model->GetModelData();
    glGenVertexArrays(1, &tex.textureVAO);
    glBindVertexArray(tex.textureVAO);

    glBindBuffer(GL_ARRAY_BUFFER, d.bufVertId);
//    GLuint loc = m_TextureShader.GetAttribLoc("in_sPosition");
    glEnableVertexAttribArray(m_loc.position_tex);
    glVertexAttribPointer(m_loc.position_tex, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    // Normals. Their position in buffer starts at bufoffset
//        bufoffset = 12 * sizeof(GLfloat);
//        if ( norm ) {
    // Just for string on face, not immutable string
    glBindBuffer(GL_ARRAY_BUFFER, d.bufColNorId);
//    loc = m_TextureShader.GetAttribLoc("in_sNormal");
    glEnableVertexAttribArray(m_loc.normal_tex);
    GLsizeiptr bufoffset = d.nuColours * 4 *sizeof(GLfloat);
    glVertexAttribPointer(m_loc.normal_tex, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)bufoffset);
    glBindBuffer(GL_ARRAY_BUFFER, tex.bufTexCoordId);
//    loc = m_TextureShader.GetAttribLoc("in_TextPos");
    glEnableVertexAttribArray(m_loc.textureCoord);
    glVertexAttribPointer(m_loc.textureCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.bufIndexId);

    tex.textureUnit = 1;
    glActiveTexture(GL_TEXTURE0 + tex.textureUnit);
    glGenTextures(1, &tex.textureId);
    glBindTexture(GL_TEXTURE_2D, tex.textureId);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,(GLsizei)tex.width, (GLsizei)tex.height, 0,GL_RGB, GL_UNSIGNED_BYTE, tex.TextureData());
    if ( ! MyOnGLError(myoglERR_TEXTIMAGE) ) {
        // Likely the GPU got out of memory
//            ClearTexture();
        return false;
    }

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);

    // Some log
    MyOnGLError(myoglERR_JUSTLOG, "Texture loaded into GPU.");
    ++loadTextureSuccessCount;
    return true;
}
unsigned int BufferLoader::LoadTextureSuccessCount()
{
    return loadTextureSuccessCount;
}
unsigned int BufferLoader::LoadTextureFailsCount()
{
    return loadTextureFailsCount;
}
unsigned int BufferLoader::CreateBuffersCheckedCount()
{
    return createBuffersCheckedCount;
}
