#include "normalmaprendersystem.h"
#include "shadersPath.h"
#include "textfile.h"


NormalMapRenderSystem::NormalMapRenderSystem():loc{0}
{
}
bool NormalMapRenderSystem::ConfigureShadersAndLocations()
{
    const char * vertCode = textFileRead(d_normalVertexShaderPath);
    const char * fragIlumCode = textFileRead(d_illuminationShaderPath);
    const char * fragCode = textFileRead(d_normalFragmentShaderPath);

    m_shader->AddCode(vertCode,GL_VERTEX_SHADER);
    m_shader->AddCode(fragIlumCode,GL_FRAGMENT_SHADER);
    m_shader->AddCode(fragCode,GL_FRAGMENT_SHADER);
    MA_CreateStrings(attribs, NORMAL_SH_ATTR);
    for (auto& attr : attribs)m_shader->AddAttrib(attr);

    m_BufferLoader->shadAttribLocations.resize((size_t)normalShAttr::normalShAttrSize);

    bool ok = true;
    ok &= (bool)vertCode;
    ok &= (bool)fragIlumCode;
    ok &= (bool)fragCode;
    

    string nameOfFunction = "NormalMapRenderSystem::ConfigureShadersAndLocations";
    MA_CreateStrings(uniforms, NORMAL_SH_UNIF);
//    for (auto& unif : uniforms)m_shader->AddUnif(unif);
//    if(ok)m_shader->Init(nameOfFunction);
    if(ok)InitShadersAndReadLocations(nameOfFunction, uniforms, loc);
        
    int atLoc[(size_t)normalShAttr::normalShAttrSize];

    for(short a = 0 ; a < (short)normalShAttr::normalShAttrSize ; a++) {
        m_BufferLoader->shadAttribLocations[a] = m_shader->GetAttribLoc(attribs[a]);
        atLoc[a] = m_BufferLoader->shadAttribLocations[a];//******DEBUG*
    }
    
//    short u{ 0 };
//    for (auto& unif : uniforms)loc[u++] = m_shader->GetUnifLoc(unif);
    
    return true;
}

void NormalMapRenderSystem::CreateGraphicBuffers(spOneModel model)
{
    auto& tex = *model->MyTexture();
    auto& d = model->GetModelData();
    m_BufferLoader->CreateBuffersForModelGeometry(d);
    m_BufferLoader->CreateBufferForTextureCoord(tex);
    m_BufferLoader->CreateBufferForTextureCoord(*model->getTextureOfType(TextureForModel::TextureType::Normal));

}

void NormalMapRenderSystem::LoadVAO(spOneModel model)
{
    auto& tex = *model->MyTexture();
    auto& d = model->GetModelData();
    auto& vao = model->getVao();
    m_BufferLoader->StartLoadingBuffersWith(vao);
    m_BufferLoader->LoadBufferOnLocation3f(d.bufVertId,(size_t)normalShAttr::aPos);
    m_BufferLoader->LoadBufferOnLocation3f(d.bufTangentId,(size_t)normalShAttr::aTangent);
    m_BufferLoader->LoadBufferOnLocation3f(d.bufBitangentId,(size_t)normalShAttr::aBitangent);
    m_BufferLoader->LoadSubBufferOnLocation3f(d.bufColNorId,(size_t)normalShAttr::aNormal,4,d.nuColours);
    m_BufferLoader->LoadBufferOnLocation2f(tex.bufTexCoordId,(size_t)normalShAttr::aTexCoords);
    m_BufferLoader->LoadIndicesAndFinish(d.bufIndexId);
}
void NormalMapRenderSystem::Draw(spOneModel model)
{
	auto vao = model->getVao();
    if(!vao) return;
    glUseProgram(getProgramId());
    glBindVertexArray(vao);
    glUniformMatrix4fv(loc[(size_t)normalShUnif::mMVP], 1, GL_FALSE, matrixStack->getModelViewProjectionMatrixfv());
    glUniformMatrix4fv(loc[(size_t)normalShUnif::mToViewSpace], 1, GL_FALSE, matrixStack->getViewMatrixfv());
    glUniform4fv(loc[(size_t)normalShUnif::lightProps], 1, light->GetFLightPos());
    glUniform3fv(loc[(size_t)normalShUnif::lightColour], 1, light->GetFLightColour());

    auto& tex = *model->MyTexture();
    auto d = model->GetModelData();

    glActiveTexture(GL_TEXTURE0 + tex.getTextureUnit());
    glBindTexture(GL_TEXTURE_2D, tex.getTextureId());
    glUniform1i(loc[(size_t)normalShUnif::diffuseMap], tex.getTextureUnit());
    
    int normalEnabled = 1;
    auto& texNormalMap = *model->getTextureOfType(TextureForModel::Normal);
    
    if(texNormalMap.bufTexCoordId != (unsigned)-1) {
        glActiveTexture(GL_TEXTURE0 + texNormalMap.getTextureUnit());
        glBindTexture(GL_TEXTURE_2D, texNormalMap.getTextureId());
        glUniform1i(loc[(size_t)normalShUnif::normalMap], texNormalMap.getTextureUnit());
    }else{normalEnabled = 0;}
//
    glUniform1i(loc[(size_t)normalShUnif::normalEnabled], (int)normalEnabled);
   
    DrawIndicesAndFinish(d);
    
}
