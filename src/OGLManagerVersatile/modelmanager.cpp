#include "modelmanager.h"
#ifdef TESTOWANIE_F
#include "glFunctionsMock.h"
#endif

using namespace std;

ModelManager::ModelManager():myOGLManager()
{
#ifdef TESTOWANIE_F
    GlFunctionsMock functionsMock;
    functionsMock.Define();
#endif
    m_renderSystem = make_shared<OneTextureRenderSystem>(); //default
    m_BufferLoader = m_renderSystem->getBufferLoader();
    m_TexRenderer = m_renderSystem->getRenderer();

    m_Camera = make_unique<myOGLCamera>();
}

template<typename T>
void ModelManager::setLocations(shared_ptr<T> rend, vec_locations_T<T> vec, myOGLShaders& shader,  sha_FunGetStr FunGetByString)
{
    for(auto& name : vec) {
        rend->m_loc.*(get<0>(name)) = (shader.*FunGetByString)(get<1>(name));//ok
    }
}
//bool ModelManager::setAndConfigureRenderSystem(spRenderSystem rs)
//{
//    m_renderSystem = rs;
//    rs->ConfigureShadersAndLocations();
//    
//    CallForMyRenderable(&RenderSystem::CreateGraphicBuffers,rs);
//    CallForMyRenderable(&RenderSystem::ReloadVAO,rs);
//    CallForMyTextures(&RenderSystem::CreateGraphicBuffers,rs);
//    ConfigureWithMyViewControl(rs);
//    
//    ConnectWithMyLightSystem(rs);
//    activeShader = rs->getShader();
//    activeRenderer = rs->getRenderer();
//    m_BufferLoader = m_renderSystem->getBufferLoader();
//    m_TexRenderer = m_renderSystem->getRenderer();
//    return true;
//}
spMyOGLShaders ModelManager::getActiveShader()
{
    return activeShader;
}
spOglRenderer ModelManager::getActiveRenderer()
{
	return activeRenderer;
}
myLight* ModelManager::getLightPtr()
{
	return &m_Light;
}
void ModelManager::ConnectWithMyLightSystem(spRenderSystem rs)
{
    rs->light = &m_Light;
}




//for linker
template void ModelManager::setLocations<OglRenderer>(shared_ptr<OglRenderer>, vec_locations_T<OglRenderer>, myOGLShaders&, sha_FunGetStr);
template void ModelManager::setLocations<BufferLoader>(shared_ptr<BufferLoader>, vec_locations_T<BufferLoader>, myOGLShaders&, sha_FunGetStr);
