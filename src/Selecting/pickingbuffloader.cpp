#include "pickingbuffloader.h"

using namespace std;

PickingBuffLoader::PickingBuffLoader()
{

}

PickingBuffLoader::~PickingBuffLoader()
{

}
void PickingBuffLoader::setLocationsFrom(spMyOGLShaders shader)
{
    /*
    m_loc.position = shader->GetAttribLoc("Position");
     */
    m_loc.position_tex = shader->GetAttribLoc("in_sPosition");
    m_loc.normal_tex = shader->GetAttribLoc("in_sNormal");
    m_loc.textureCoord = shader->GetAttribLoc("in_TextPos");
}
void PickingBuffLoader::LoadBuffers(spOneModel model)
{
    unsigned int& vaoSel = model->getVAOforSelect();
    
    ModelData& d = model->GetModelData();
    /*
    glGenVertexArrays(1, &vaoSel);
    glBindVertexArray(vaoSel);
    glBindBuffer(GL_ARRAY_BUFFER, d.bufVertId);
    glEnableVertexAttribArray(m_loc.position);
    glVertexAttribPointer(m_loc.position, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.bufIndexId);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
     */
    
    ++loadTextureFailsCount;
    
    TextureForModel& tex = *model->MyTexture();
//    ModelData& d = model->GetModelData();
    
    GLsizeiptr nBytes = 2 * tex.nuTexCoord * sizeof(GLfloat);
    
    unsigned int bufTexCoordId;//nie jest elementem modelu bo cała funkcja jest tymczasowa

    glGenBuffers(1, &bufTexCoordId);
    glBindBuffer(GL_ARRAY_BUFFER, bufTexCoordId);
    // Populate the buffer with the array "vert"
    glBufferData(GL_ARRAY_BUFFER, nBytes, tex.texCoord, GL_STATIC_DRAW);


    MyOnGLError(myoglERR_CLEAR); //clear error stack

    glGenVertexArrays(1, &vaoSel);
    glBindVertexArray(vaoSel);

    glBindBuffer(GL_ARRAY_BUFFER, d.bufVertId);
    glEnableVertexAttribArray(m_loc.position_tex);
    glVertexAttribPointer(m_loc.position_tex, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    glBindBuffer(GL_ARRAY_BUFFER, d.bufColNorId);
    glEnableVertexAttribArray(m_loc.normal_tex);
    GLsizeiptr bufoffset = d.nuColours * 4 *sizeof(GLfloat);
    glVertexAttribPointer(m_loc.normal_tex, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)bufoffset);
    
    glBindBuffer(GL_ARRAY_BUFFER, bufTexCoordId);
    glEnableVertexAttribArray(m_loc.textureCoord);
    glVertexAttribPointer(m_loc.textureCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.bufIndexId);

    //skorzystamy z tekstury przygotowanej w podstawowym buffloaderze
//    tex.textureUnit = 1;
//    glActiveTexture(GL_TEXTURE0 + tex.textureUnit);
//    glGenTextures(1, &tex.textureId);
//    glBindTexture(GL_TEXTURE_2D, tex.textureId);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,(GLsizei)tex.width, (GLsizei)tex.height, 0,GL_RGB, GL_UNSIGNED_BYTE, tex.TextureData());
    if ( ! MyOnGLError(myoglERR_TEXTIMAGE) ) {
        return;
    }

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    
    // Some log
    std::string str_log = "Texture buffers loaded into GPU for model Id = "+to_string(model->getUniqueId()) + " in PickingBuffLoader::LoadBuffers";
    MyOnGLError(myoglERR_JUSTLOG, str_log.c_str());
    ++loadTextureSuccessCount;
    
}
