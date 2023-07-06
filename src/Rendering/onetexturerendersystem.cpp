#include "onetexturerendersystem.h"
#include "shadersPath.h"
#include "textfile.h"

OneTextureRenderSystem::OneTextureRenderSystem()
{

}

OneTextureRenderSystem::~OneTextureRenderSystem()
{

}
bool OneTextureRenderSystem::ConfigureShadersAndLocations()
{
	const char * vertCode = textFileRead(d_textureVertexShaderPath);
    const char * fragIlumCode = textFileRead(d_illuminationShaderPath);
    const char * fragCode = textFileRead(d_textureFragmentShaderPath);
    
    m_shader->AddCode(vertCode,GL_VERTEX_SHADER);
    m_shader->AddCode(fragIlumCode,GL_FRAGMENT_SHADER);
    m_shader->AddCode(fragCode,GL_FRAGMENT_SHADER);

    m_shader->AddAttrib("in_sPosition");
    m_shader->AddAttrib("in_sNormal");
    m_shader->AddAttrib("in_TextPos");
    m_shader->AddUnif("mMVP");
    m_shader->AddUnif("mToViewSpace");
    m_shader->AddUnif("lightProps");
    m_shader->AddUnif("lightColour");
    m_shader->AddUnif("stringTexture");
    
    m_BufferLoader->shadAttribLocations.resize(3);
    
    bool ok = true;
    ok &= (bool)vertCode;
    ok &= (bool)fragIlumCode;
    ok &= (bool)fragCode;
    if(!ok)return false;

    string nameOfFunction = "OneTextureRenderSystem::ConfigureShadersAndLocations";
    m_shader->Init(nameOfFunction);
    m_renderer->setLocationsFrom(m_shader);
    
    m_BufferLoader->shadAttribLocations[0] = m_shader->GetAttribLoc("in_sPosition");
    m_BufferLoader->shadAttribLocations[1] = m_shader->GetAttribLoc("in_sNormal");
    m_BufferLoader->shadAttribLocations[2] = m_shader->GetAttribLoc("in_TextPos");
    
    return true;
}
void OneTextureRenderSystem::CreateGraphicBuffers(spOneModel model)
{
    auto& tex = *model->MyTexture();
    auto& d = model->GetModelData();
    m_BufferLoader->CreateBuffersForModelGeometry(d);
    m_BufferLoader->CreateBufferForTextureCoord(tex);
}
void OneTextureRenderSystem::LoadVAO(spOneModel model)
{
	auto& tex = *model->MyTexture();
    auto& d = model->GetModelData();
    auto& vao = model->getVao();
//    m_BufferLoader->LoadBuffersForModelGeometry(d,vao);
//    m_BufferLoader->LoadBufferForTexture(tex,vao);
     m_BufferLoader->StartLoadingBuffersWith(vao);
    m_BufferLoader->LoadBufferOnLocation3f(d.bufVertId,0);
    m_BufferLoader->LoadSubBufferOnLocation3f(d.bufColNorId,1,4,d.nuColours);
    m_BufferLoader->LoadBufferOnLocation2f(tex.bufTexCoordId,2);
    m_BufferLoader->LoadIndicesAndFinish(d.bufIndexId);
}
