#include "rendersystemmanager.h"
#include "paralaxoclusionmaprendersystem.h"
#include "normalmaprendersystem.h"

using namespace std;

template<typename RsType>
unsigned RenderSystemManager::AddRenderSystem()
{
	upRenderSystem rs = make_unique<RsType>();
    unsigned pos = m_rss.size();
//    size_t positionInContainer = rs->getType();
//    if( < positionInContainer)m_rss.resize(positionInContainer);
//    m_rss[rsType] = rs;
    m_rss.emplace_back(move(rs));
    return pos;
}
void RenderSystemManager::DisableExternalRenderSystem()
{
	externalRenderSystemEnabled = false;
}
void RenderSystemManager::EnableExternalRenderSystem(spRenderSystem ers)
{
	externalRenderSystemEnabled = true;
    externalRs = ers;
}
void RenderSystemManager::ConnectModelWithRenderSystem(unsigned uniqueId, unsigned rsId)
{
    if(whichSystemForModel.size() <= uniqueId + 1)whichSystemForModel.resize(uniqueId + 1);
    auto s = whichSystemForModel.size();
    whichSystemForModel[uniqueId] = rsId;
}
spOglRenderer RenderSystemManager::ActiveRenderer()
{
    if(externalRenderSystemEnabled)return externalRs->getRenderer();
    auto whichRs = whichSystemForModel[currentModelId];
    auto rs = m_rss[whichRs];
    auto rend = rs->getRenderer();
    return rend;
    return m_rss[whichSystemForModel[currentModelId]]->getRenderer();
}
spMyOGLShaders RenderSystemManager::ActiveShader()
{
	if(externalRenderSystemEnabled)return externalRs->getShader();
    return m_rss[whichSystemForModel[currentModelId]]->getShader();
}

//for linker
template unsigned RenderSystemManager::AddRenderSystem<ParalaxOclusionMapRenderSystem>();
template unsigned RenderSystemManager::AddRenderSystem<NormalMapRenderSystem>();