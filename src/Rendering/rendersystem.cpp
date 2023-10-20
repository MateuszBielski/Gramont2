#include "rendersystem.h"

using namespace std;

RenderSystem::RenderSystem()
{
    m_shader = make_shared<myOGLShaders>();
    m_renderer = make_unique<OglRenderer>();
    m_BufferLoader = make_unique<BufferLoader>();
}
//RenderSystem::RenderSystem(spMyOGLShaders shad, spBufferLoader bl, spOglRenderer rend)
//{
//	m_shader = shad;
//    m_renderer = rend;
//    m_BufferLoader = bl;
//}
void RenderSystem::setBufferLoader(spBufferLoader bl)
{
    m_BufferLoader = bl;
}
void RenderSystem::setShader(spMyOGLShaders sha)
{
    m_shader = sha;
}
spOglRenderer RenderSystem::getRenderer()
{
    return m_renderer;
}
spMyOGLShaders RenderSystem::getShader()
{
    return m_shader;
}
spBufferLoader RenderSystem::getBufferLoader()
{
    return m_BufferLoader;
}
void RenderSystem::ReloadVAO(spOneModel model)
{
    m_BufferLoader->RecreateVao(model->getVao());
    LoadVAO(model);
}

void RenderSystem::CreateGraphicBuffers(TextureInMemory& texm)
{
    m_BufferLoader->CreateBufferForTextureInMemory(texm);
}
void RenderSystem::CreateVAO(spOneModel model)
{
    m_BufferLoader->CreateVao(model->getVao());
}
