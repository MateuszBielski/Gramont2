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
void RenderSystem::ReloadVAO(ModelData& d, TextureForModel& tex)
{
//    unsigned int& vao = tex.textureVAO;
    if(!activeVaoPtr)return;
    unsigned int& vao = *activeVaoPtr;
    m_BufferLoader->RecreateVao(vao);
    m_BufferLoader->LoadBuffersForModelGeometry(d,vao);
    m_BufferLoader->LoadBufferForTexture(tex,vao);
}
void RenderSystem::CreateGraphicBuffers(ModelData& d, TextureForModel& tex)
{
    m_BufferLoader->CreateBuffersForModelGeometry(d);
    m_BufferLoader->CreateBufferForTextureCoord(tex);
}
void RenderSystem::setActiveVaoPtr(unsigned int * vao)
{
	activeVaoPtr = vao;
}
