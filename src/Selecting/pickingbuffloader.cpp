#include "pickingbuffloader.h"

PickingBuffLoader::PickingBuffLoader()
{

}

PickingBuffLoader::~PickingBuffLoader()
{

}
void PickingBuffLoader::LocationsFrom(spMyOGLShaders shader)
{
	m_loc.position = shader->GetAttribLoc("Position");
}
void PickingBuffLoader::LoadBuffers(spOneModel model)
{
	unsigned int& vaoSel = model->getVAOforSelect();
    
    glGenVertexArrays(1, &vaoSel);
    glBindVertexArray(vaoSel);
    glBindBuffer(GL_ARRAY_BUFFER, model->GetModelData().bufVertId);
    glEnableVertexAttribArray(m_loc.position);
    glVertexAttribPointer(m_loc.position, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
