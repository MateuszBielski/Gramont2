#include "bufferloader.h"
#include "oglstuff.h"


using namespace std;

BufferLoader::BufferLoader():counter((size_t)BufferLoaderCounterType::BufferLoaderCounterTypeSize,0)
{
//(unsigned)
}
void BufferLoader::ClearBuffersForSingleModelEntry(ModelData& d)
{
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

    d.modelVAO = d.bufIndexId = d.bufColNorId = d.bufVertId = 0;
}
BufferLoaderProgress BufferLoader::CreateBufferWithSubs(unsigned int& bufId, vec_for_subbuf& data)//nazwa do potwierdzenia lub zmiany
{
    //consecutiveQuantities -> kolejneIlości
    if(bufId) return BufferLoaderProgress::Checked;
    glGenBuffers(1, &bufId);
    glBindBuffer(GL_ARRAY_BUFFER, bufId);
    int nBytes = 0;
    for(auto& sub : data)nBytes += get<0>(sub) * get<1>(sub) * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, nBytes, NULL, GL_STATIC_DRAW);

    int nuSubs = data.size();
    int offset = 0;
    for(short i = 0; i < nuSubs; i++) {
        nBytes = get<0>(data[i]) * get<1>(data[i]) * sizeof(GLfloat);
        glBufferSubData(GL_ARRAY_BUFFER, offset, nBytes, get<2>(data[i]));
        offset = nBytes;
    }
    return BufferLoaderProgress::Completed;
}
BufferLoaderProgress BufferLoader::CreateBuffersForModelGeometry(ModelData& d)
{
    bool ok = true;
    ok &= (bool)d.nuNormals;
    ok &= (bool)d.nuColours;
    ok &= (bool)d.nuIndices;
    ok &= (bool)d.nuPoints;
    ++counter[(size_t)BufferLoaderCounterType::CreateBuffersForModelGeometryStart];
    if(!ok) return BufferLoaderProgress::Checked;

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
//
    vec_for_subbuf data;
    data.push_back(make_tuple(d.nuColours, 4, d.colours));
    data.push_back(make_tuple(d.nuNormals, 3, d.normals));
    CreateBufferWithSubs(d.bufColNorId,data);

    if(!d.bufTangentId && d.nuTangents) {
        glGenBuffers(1, &d.bufTangentId);
        glBindBuffer(GL_ARRAY_BUFFER, d.bufTangentId);
        nBytes = d.nuTangents * 3 * sizeof(GLfloat); //3 components {x,y,z}
        glBufferData(GL_ARRAY_BUFFER, nBytes, d.tangents, GL_STATIC_DRAW);
    }
    if(!d.bufBitangentId && d.nuBitangents) {
        glGenBuffers(1, &d.bufBitangentId);
        glBindBuffer(GL_ARRAY_BUFFER, d.bufBitangentId);
        nBytes = d.nuBitangents * 3 * sizeof(GLfloat); //3 components {x,y,z}
        glBufferData(GL_ARRAY_BUFFER, nBytes, d.bitangents, GL_STATIC_DRAW);
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

BufferLoaderProgress BufferLoader::CreateBufferForTextureCoord(TextureForModel& tex)
{
    bool ok = true;
    ok &= (bool)tex.nuTexCoord;
    ok &= (bool)tex.texCoord;
//    ++createBuffersCheckedCount;

    if(!ok) return BufferLoaderProgress::Checked;

    GLsizeiptr nBytes = 2 * tex.nuTexCoord * sizeof(GLfloat);

    if(!tex.bufTexCoordId) {

        glGenBuffers(1, &tex.bufTexCoordId);
        glBindBuffer(GL_ARRAY_BUFFER, tex.bufTexCoordId);
        // Populate the buffer with the array "vert"
        glBufferData(GL_ARRAY_BUFFER, nBytes, tex.texCoord, GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return BufferLoaderProgress::Completed;
    //czy modele potrzebują na nowo mieć tworzone bufory na karcie graficznej, jesli tak to czy poprzednie kasować czy przechowywać.
    //https://gamedev.stackexchange.com/questions/75989/what-is-the-correct-way-to-reset-and-load-new-data-into-gl-array-buffer
    //glBufferData() overwrites the previous data, and is generally what you should use to load new data into a GL buffer. Conceptually it is similar to overwriting data in an array.
    //glDeleteBuffers() and glGenBuffers() destroy and recreate a buffer. Conceptually they are similar to free() and malloc()
}
BufferLoaderProgress BufferLoader::CreateBufferForTextureInMemory(TextureInMemory& texm)
{
//    texm.getTextureUnit() = 0;
//    glActiveTexture(GL_TEXTURE0 + texm.getTextureUnit());
    glGenTextures(1, &texm.getTextureId());
    glBindTexture(GL_TEXTURE_2D, texm.getTextureId());
    // Avoid some artifacts
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Do this before glTexImage2D because we only have 1 level, no mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    MyOnGLError(myoglERR_CLEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//potrzebne dla tekstur o nietypowych wymiarach, https://stackoverflow.com/questions/58925604/glteximage2d-crashing-program
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,(GLsizei)texm.width, (GLsizei)texm.height, 0,GL_RGB, GL_UNSIGNED_BYTE, texm.TextureData());

    if ( ! MyOnGLError(myoglERR_TEXTIMAGE) ) {
        // Likely the GPU got out of memory
//            ClearTexture();
        return BufferLoaderProgress::TextureError;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return BufferLoaderProgress::Completed;
}
bool BufferLoader::CreateVao(unsigned int& vao)
{
    if(vao) return false;
    glGenVertexArrays(1, &vao);
    return true;
}

void BufferLoader::RecreateVao(unsigned int& vao)
{
    //    sprawdzić rzeczywiste działanie: czy glDeleteVertexArray() zeruje przekazaną nazwę, i glGenVertexArrays() daje ten sam numer, czy następny wolny
    //ewentualnie zmodyfikować działanie funkcji glDeleteVertexArrays_Mock
    if(vao)
        glDeleteVertexArrays(1,&vao);

    glGenVertexArrays(1, &vao);
}
BufferLoaderProgress BufferLoader::LoadBuffersForModelGeometry(ModelData& d,const int vao)
{
    bool ok = true;
    ++counter[(size_t)BufferLoaderCounterType::LoadBufferForModelGeometryStart];
    if(!vao)return BufferLoaderProgress::VaoNotInited;
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, d.bufVertId);
    glEnableVertexAttribArray(m_loc.position_tex);
    glVertexAttribPointer(m_loc.position_tex, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    glBindBuffer(GL_ARRAY_BUFFER, d.bufColNorId);
    glEnableVertexAttribArray(m_loc.normal_tex);
    GLsizeiptr bufoffset = d.nuColours * 4 *sizeof(GLfloat);
    glVertexAttribPointer(m_loc.normal_tex, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)bufoffset);

//    if(d.bufTangentId) {
//        glBindBuffer(GL_ARRAY_BUFFER, d.bufTangentId);
//        glEnableVertexAttribArray(m_loc.position_tex???);
//        glVertexAttribPointer(m_loc.position_tex???, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
//    }
//    if(d.bufBitangentId) {
//        glBindBuffer(GL_ARRAY_BUFFER, d.bufBitangentId);
//        glEnableVertexAttribArray(m_loc.position_tex???);
//        glVertexAttribPointer(m_loc.position_tex???, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
//    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.bufIndexId);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    ++counter[(size_t)BufferLoaderCounterType::LoadBufferForModelGeometryCompleted];
    return BufferLoaderProgress::Completed;
}
BufferLoaderProgress BufferLoader::LoadBufferForTexture(TextureForModel& tex, const int vao)
{
    bool ok = true;
    ++counter[(size_t)BufferLoaderCounterType::LoadBufferForTextureStart];
    if(!vao)return BufferLoaderProgress::VaoNotInited;
    if((int)tex.bufTexCoordId == -1 )return BufferLoaderProgress::TextureNotFound;

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, tex.bufTexCoordId);
    glEnableVertexAttribArray(m_loc.textureCoord);
    glVertexAttribPointer(m_loc.textureCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ++counter[(size_t)BufferLoaderCounterType::LoadBufferForTextureCompleted];
    return BufferLoaderProgress::Completed;
}

BufferLoaderProgress BufferLoader::StartLoadingBuffersWith(unsigned vao)
{
    ++counter[(size_t)BufferLoaderCounterType::LoadBufferForModelGeometryStart];
    if(!vao)return BufferLoaderProgress::VaoNotInited;
    glBindVertexArray(vao);
    return BufferLoaderProgress::VaoReadyForLoad;
}

void BufferLoader::LoadBufferOnLocation3f(unsigned buffId,size_t loc)
{
    if(loc >= shadAttribLocations.size())return;
    glBindBuffer(GL_ARRAY_BUFFER, buffId);
    glEnableVertexAttribArray(shadAttribLocations[loc]);
    glVertexAttribPointer(shadAttribLocations[loc], 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
}
void BufferLoader::LoadSubBufferOnLocation3f(unsigned buffId,size_t loc,size_t offsetStep,size_t nuSteps)
{
    if(loc >= shadAttribLocations.size())return;
    glBindBuffer(GL_ARRAY_BUFFER, buffId);
    glEnableVertexAttribArray(shadAttribLocations[loc]);
    GLsizeiptr bufoffset = nuSteps * offsetStep *sizeof(GLfloat);
    glVertexAttribPointer(shadAttribLocations[loc], 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)bufoffset);
}
void BufferLoader::LoadBufferOnLocation2f(unsigned buffId,size_t loc)
{
    if(loc >= shadAttribLocations.size())return;
    glBindBuffer(GL_ARRAY_BUFFER, buffId);
    glEnableVertexAttribArray(shadAttribLocations[loc]);
    glVertexAttribPointer(shadAttribLocations[loc], 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
}
BufferLoaderProgress BufferLoader::LoadIndicesAndFinish(unsigned buffId)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffId);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    ++counter[(size_t)BufferLoaderCounterType::LoadBufferForModelGeometryCompleted];
    return BufferLoaderProgress::Completed;
}
const unsigned BufferLoader::Counter(BufferLoaderCounterType couterType)
{
    return counter[(size_t)couterType];
}
void BufferLoader::setLocationsFrom(spMyOGLShaders shader)
{
    m_loc.position_tex = shader->GetAttribLoc("in_sPosition");
    m_loc.normal_tex = shader->GetAttribLoc("in_sNormal");
    m_loc.textureCoord = shader->GetAttribLoc("in_TextPos");

}
//function used only in OneModelManager
bool BufferLoader::LoadTextureBuffersForSingleModelEntry(TextureForModel& tex, ModelData& d)
{

//    ++loadTextureFailsCount;
    bool ok = true;

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

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(tex.textureVAO);

    glBindBuffer(GL_ARRAY_BUFFER, tex.bufTexCoordId);
//    loc = m_TextureShader.GetAttribLoc("in_TextPos");
    glEnableVertexAttribArray(m_loc.textureCoord);
    glVertexAttribPointer(m_loc.textureCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.bufIndexId);

    /*poniższe nie działa po rozdzieleniu obiektu textury na TextureForModel i TextureInMemory
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
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
     */
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Some log
    std::string str_log = "Texture buffers loaded into GPU for model Id = ";
    MyOnGLError(myoglERR_JUSTLOG, str_log.c_str());
//    ++loadTextureSuccessCount;
    return true;
}
