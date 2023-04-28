#include "rendersystem.h"

using namespace std;

RenderSystem::RenderSystem()
{
    m_shader = make_shared<myOGLShaders>();
    m_renderer = make_unique<OglRenderer>();
    m_BufferLoader = make_unique<BufferLoader>();
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

