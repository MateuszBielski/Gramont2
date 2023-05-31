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
    m_loc.position = shader->GetAttribLoc("position");
}
void PickingBuffLoader::LoadBuffers(spOneModel model)
{
    unsigned int& vaoSel = model->getVAOforSelect();
    
//    ++loadTextureFailsCount;
    ModelData& d = model->GetModelData();

    glGenVertexArrays(1, &vaoSel);
    glBindVertexArray(vaoSel);

    glBindBuffer(GL_ARRAY_BUFFER, d.bufVertId);
    glEnableVertexAttribArray(m_loc.position);
    glVertexAttribPointer(m_loc.position, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.bufIndexId);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    std::string str_log = "Texture buffers loaded into GPU for model Id = "+to_string(model->getUniqueId()) + " in PickingBuffLoader::LoadBuffers";
    MyOnGLError(myoglERR_JUSTLOG, str_log.c_str());
//    ++loadTextureSuccessCount;
}
