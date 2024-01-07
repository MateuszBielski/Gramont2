#include "rendersystem.h"
#include "rspreproc.h"

using namespace std;

RenderSystem::RenderSystem()
{
    m_shader = make_shared<myOGLShaders>();
    m_BufferLoader = make_unique<BufferLoader>();
}

void RenderSystem::setBufferLoader(spBufferLoader bl)
{
    m_BufferLoader = bl;
}
void RenderSystem::setShader(spMyOGLShaders sha)
{
    m_shader = sha;
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
void RenderSystem::InitShadersAndReadLocations(string& nameOfFunction, const vector<string>& uniformNames, unsigned int* loc)
{
    for (auto& unif : uniformNames)m_shader->AddUnif(unif);
    m_shader->Init(nameOfFunction);
    short u{ 0 };
    for (auto& unif : uniformNames)loc[u++] = m_shader->GetUnifLoc(unif);
}
void RenderSystem::DrawIndicesAndFinish(ModelData& d)
{
	glDrawElements(d.primitiveMode, d.nuIndices, GL_UNSIGNED_INT, (GLvoid *)0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
}

