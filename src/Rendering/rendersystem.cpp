#include "rendersystem.h"
#include "rspreproc.h"

using namespace std;

RenderSystem::RenderSystem()
{
    m_shader = make_shared<myOGLShaders>();
//    m_renderer = make_unique<OglRenderer>();
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
void RenderSystem::InitShadersAndReadLocations(string& nameOfFunction,const char * definedUniformNames, unsigned int * loc)
{
//	MA_CreateStrings(uniforms, NORMAL_SH_UNIF);
	MA_CreateStrings(uniforms, definedUniformNames);
    for (auto& unif : uniforms)m_shader->AddUnif(unif);
//    string nameOfFunction = "NormalMapRenderSystem::ConfigureShadersAndLocations";
    m_shader->Init(nameOfFunction);
    short u{ 0 };
    for (auto& unif : uniforms)loc[u++] = m_shader->GetUnifLoc(unif);
}
void RenderSystem::DrawIndicesAndFinish(ModelData& d)
{
	glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
}

