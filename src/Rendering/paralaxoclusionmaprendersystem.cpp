#include "paralaxoclusionmaprendersystem.h"
#include "shadersPath.h"
#include "textfile.h"

//https://cplusplus.com/articles/2wA0RXSz/
const vector<string> explode(const string& s, const char& c)
{
    string buff{ "" };
    vector<string> v;

    for (auto n : s)
    {
        if (n != c) buff += n; else
            if (n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if (buff != "") v.push_back(buff);

    return v;
}

bool ParalaxOclusionMapRenderSystem::ConfigureShadersAndLocations()
{
	const char * vertCode = textFileRead(d_pomVertexShaderPath);
//    const char * fragIlumCode = textFileRead(d_illuminationShaderPath);
    const char * fragCode = textFileRead(d_pomFragmentShaderPath);
    
    m_shader->AddCode(vertCode,GL_VERTEX_SHADER);
//    m_shader->AddCode(fragIlumCode,GL_FRAGMENT_SHADER);
    m_shader->AddCode(fragCode,GL_FRAGMENT_SHADER);
    MA_CreateStrings(attribs, POM_SH_ATTR);
    for (auto& attr : attribs)m_shader->AddAttrib(attr);
    
    MA_CreateStrings(uniforms, POM_SH_UNIF);
    for (auto& unif : uniforms)m_shader->AddUnif(unif);

    m_BufferLoader->shadAttribLocations.resize((size_t)pomShAttr::pomShAttrSize);
    m_renderer->shadUnifLocations.resize((size_t)pomShUnif::pomShUnifSize);
    
    bool ok = true;
    ok &= (bool)vertCode;
//    ok &= (bool)fragIlumCode;
    ok &= (bool)fragCode;
//    if(!ok)return false;

    string nameOfFunction = "ParalaxOclusionMapRenderSystem::ConfigureShadersAndLocations";
    if(ok)m_shader->Init(nameOfFunction);
    //locations are accessible after compile and link shader
    for(short a = 0 ; a < (short)pomShAttr::pomShAttrSize ; a++)
        m_BufferLoader->shadAttribLocations[a] = m_shader->GetAttribLoc(attribs[a]);
    for(short u = 0 ; u < (short)pomShUnif::pomShUnifSize; u++)
        m_renderer->shadUnifLocations[u] = m_shader->GetUnifLoc(uniforms[u]);
//    m_renderer->setLocationsFrom(m_shader);
//    m_BufferLoader->setLocationsFrom(m_shader);
    return true;
}
void ParalaxOclusionMapRenderSystem::CreateGraphicBuffers(spOneModel model)
{
	auto& tex = *model->MyTexture();
    auto& d = model->GetModelData();
    m_BufferLoader->CreateBuffersForModelGeometry(d);
    m_BufferLoader->CreateBufferForTextureCoord(tex);
    m_BufferLoader->CreateBufferForTextureCoord(*model->getTextureOfType(TextureForModel::TextureType::Height));
    m_BufferLoader->CreateBufferForTextureCoord(*model->getTextureOfType(TextureForModel::TextureType::Normal));
}
void ParalaxOclusionMapRenderSystem::LoadVAO(spOneModel model)
{
	auto& tex = *model->MyTexture();
    auto& d = model->GetModelData();
    auto& vao = model->getVao();
    m_BufferLoader->LoadBuffersForModelGeometry(d,vao);
    m_BufferLoader->LoadBufferForTexture(tex,vao);
}
