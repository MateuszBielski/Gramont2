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
bool ModelManager::setAndConfigureRenderSystem(spRenderSystem rs)
{
	/***pomysł*****/
//    rs->setRenderingResources(m_renderSystem->getRidOfRenderingResources());//(getRidOf - pozbądź się) jakie to mogą być zasoby?:
    //lokacje, wskaźniki do Light i matrixStack, 
    //dlaczego tworzyć kolejną klasę RenderingResources?
    /**/
    m_renderSystem = rs;
//    rs->ConfigureShadersAndLocations();
//skopiować i wyzerować od dotychczas aktywnego renderera: matrixStack, Light

    CallForMyRenderable(&RenderSystem::ReloadVAO,rs);
    activeShader = rs->getShader();
    activeRenderer = rs->getRenderer();
    
    return true;
}
spMyOGLShaders ModelManager::getActiveShader()
{
    return activeShader;
}
spOglRenderer ModelManager::getActiveRenderer()
{
	return activeRenderer;
}


//for linker
template void ModelManager::setLocations<OglRenderer>(shared_ptr<OglRenderer>, vec_locations_T<OglRenderer>, myOGLShaders&, sha_FunGetStr);
template void ModelManager::setLocations<BufferLoader>(shared_ptr<BufferLoader>, vec_locations_T<BufferLoader>, myOGLShaders&, sha_FunGetStr);
