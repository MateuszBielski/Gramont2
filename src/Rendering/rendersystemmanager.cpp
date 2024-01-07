#include "rendersystemmanager.h"
#include "paralaxoclusionmaprendersystem.h"
#include "normalmaprendersystem.h"

using namespace std;

template<typename RsType>
unsigned RenderSystemManager::AddRenderSystem()
{
    upRenderSystem rs = make_unique<RsType>();
    unsigned pos = m_CollectionOfRendSystems.size();
//    size_t positionInContainer = rs->getType();
//    if( < positionInContainer)m_rss.resize(positionInContainer);
//    m_rss[rsType] = rs;
    m_CollectionOfRendSystems.emplace_back(move(rs));
    return pos;
}
void RenderSystemManager::ConnectModelWithRenderSystem(unsigned uniqueId, unsigned rsId)
{
    if(whichSystemForModel.size() <= uniqueId + 1)whichSystemForModel.resize(uniqueId + 1);
    auto s = whichSystemForModel.size();
    whichSystemForModel[uniqueId] = rsId;
}
void RenderSystemManager::CreateGraphicBuffers(spOneModel model)
{
    m_CollectionOfRendSystems[whichSystemForModel[model->getUniqueId()]]->CreateGraphicBuffers(model);
}
void RenderSystemManager::LoadVAO(spOneModel model)
{
    m_CollectionOfRendSystems[whichSystemForModel[model->getUniqueId()]]->LoadVAO(model);
}
bool RenderSystemManager::ConfigureShadersAndLocations()
{
    bool result = true;
    for(auto& rs : m_CollectionOfRendSystems) {
        result &= (bool)rs->ConfigureShadersAndLocations();

    }
    return result;
}
void RenderSystemManager::Draw(spOneModel model)
{
    m_CollectionOfRendSystems[whichSystemForModel[model->getUniqueId()]]->Draw(model);
}
//for linker
template unsigned RenderSystemManager::AddRenderSystem<ParalaxOclusionMapRenderSystem>();
template unsigned RenderSystemManager::AddRenderSystem<NormalMapRenderSystem>();
