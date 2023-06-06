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
BufferLoaderProgress PickingBuffLoader::LoadBuffersForModelGeometry(ModelData& d,const int vao)
{
    if(!vao)return BufferLoaderProgress::VaoNotInited;
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, d.bufVertId);
    glEnableVertexAttribArray(m_loc.position);
    glVertexAttribPointer(m_loc.position, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.bufIndexId);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    return BufferLoaderProgress::Completed;
}
