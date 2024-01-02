#include "normalmaprendersystem.h"
#include "shadersPath.h"
#include "textfile.h"



NormalMapRenderSystem::NormalMapRenderSystem()
{
//    m_renderer = std::make_unique<NormalRenderer>();
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

    MA_CreateStrings(uniforms, NORMAL_SH_UNIF);
    for (auto& unif : uniforms)m_shader->AddUnif(unif);

    m_BufferLoader->shadAttribLocations.resize((size_t)normalShAttr::normalShAttrSize);
//    m_renderer->shadUnifLocations.resize((size_t)normalShUnif::normalShUnifSize);

    bool ok = true;
    ok &= (bool)vertCode;
    ok &= (bool)fragIlumCode;
    ok &= (bool)fragCode;

    string nameOfFunction = "NormalMapRenderSystem::ConfigureShadersAndLocations";
    if(ok)m_shader->Init(nameOfFunction);

    int atLoc[(size_t)normalShAttr::normalShAttrSize];//******DEBUG*
//    , unifLoc[(size_t)normalShUnif::normalShUnifSize]
    string unifNames[(size_t)normalShUnif::normalShUnifSize];

    for(short a = 0 ; a < (short)normalShAttr::normalShAttrSize ; a++) {
        m_BufferLoader->shadAttribLocations[a] = m_shader->GetAttribLoc(attribs[a]);
        atLoc[a] = m_BufferLoader->shadAttribLocations[a];//******DEBUG*
    }
    loc_mMVP = m_shader->GetUnifLoc("mMVP");
    loc_mToViewSpace = m_shader->GetUnifLoc("mToViewSpace");
    loc_lightProps = m_shader->GetUnifLoc("lightProps");
    loc_lightColour = m_shader->GetUnifLoc("lightColour");
    loc_diffuseMap = m_shader->GetUnifLoc("diffuseMap");
    loc_normalMap = m_shader->GetUnifLoc("normalMap");
    loc_normalEnabled = m_shader->GetUnifLoc("normalEnabled");
    
//    for(short u = 0 ; u < (short)normalShUnif::normalShUnifSize; u++) {
//        m_renderer->shadUnifLocations[u] = m_shader->GetUnifLoc(uniforms[u]);
//        unifNames[u] = uniforms[u];
//        unifLoc[u] = m_renderer->shadUnifLocations[u];//******DEBUG*
//    }
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
    glUniformMatrix4fv(loc_mMVP, 1, GL_FALSE, matrixStack->getModelViewProjectionMatrixfv());
    glUniformMatrix4fv(loc_mToViewSpace, 1, GL_FALSE, matrixStack->getViewMatrixfv());
    glUniform4fv(loc_lightProps, 1, light->GetFLightPos());
    glUniform3fv(loc_lightColour, 1, light->GetFLightColour());

    auto& tex = *model->MyTexture();
    auto d = model->GetModelData();

    glActiveTexture(GL_TEXTURE0 + tex.getTextureUnit());
    glBindTexture(GL_TEXTURE_2D, tex.getTextureId());
    glUniform1i(loc_diffuseMap, tex.getTextureUnit());
    
    int normalEnabled = 1;
    auto& texNormalMap = *model->getTextureOfType(TextureForModel::Normal);
    
    if(texNormalMap.bufTexCoordId != (unsigned)-1) {
        glActiveTexture(GL_TEXTURE0 + texNormalMap.getTextureUnit());
        glBindTexture(GL_TEXTURE_2D, texNormalMap.getTextureId());
        glUniform1i(loc_normalMap, texNormalMap.getTextureUnit());
    }else{normalEnabled = 0;}
//
    glUniform1i(loc_normalEnabled, (int)normalEnabled);
   
    DrawIndicesAndFinish(d);
    
}
