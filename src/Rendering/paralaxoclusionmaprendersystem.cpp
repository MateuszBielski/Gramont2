#include "paralaxoclusionmaprendersystem.h"
#include "shadersPath.h"
#include "textfile.h"
#include "pomrenderer.h"

ParalaxOclusionMapRenderSystem::ParalaxOclusionMapRenderSystem()
{
    m_renderer = std::make_unique<PomRenderer>();
}

bool ParalaxOclusionMapRenderSystem::ConfigureShadersAndLocations()
{
    const char * vertCode = textFileRead(d_pomVertexShaderPath);
    const char * fragIlumCode = textFileRead(d_illuminationShaderPath);
    const char * fragCode = textFileRead(d_pomFragmentShaderPath);

    m_shader->AddCode(vertCode,GL_VERTEX_SHADER);
    m_shader->AddCode(fragIlumCode,GL_FRAGMENT_SHADER);
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


    int atLoc[(size_t)pomShAttr::pomShAttrSize], unifLoc[(size_t)pomShUnif::pomShUnifSize];//******DEBUG*
    string unifNames[(size_t)pomShUnif::pomShUnifSize];
    //locations are accessible after compile and link shader
    for(short a = 0 ; a < (short)pomShAttr::pomShAttrSize ; a++) {
        m_BufferLoader->shadAttribLocations[a] = m_shader->GetAttribLoc(attribs[a]);
        atLoc[a] = m_BufferLoader->shadAttribLocations[a];//******DEBUG*
    }

    for(short u = 0 ; u < (short)pomShUnif::pomShUnifSize; u++) {
        m_renderer->shadUnifLocations[u] = m_shader->GetUnifLoc(uniforms[u]);
        unifNames[u] = uniforms[u];
        unifLoc[u] = m_renderer->shadUnifLocations[u];//******DEBUG*
    }
    return true;
}
void ParalaxOclusionMapRenderSystem::CreateGraphicBuffers(spOneModel model)
{
    auto& tex = *model->MyTexture();
    auto& d = model->GetModelData();
//    auto loadStatus;
    string messageLoadNotCompleted;
    auto CheckStatus = [&](BufferLoaderProgress loadStatus, string&& mes) {
        if(loadStatus != BufferLoaderProgress::Completed)messageLoadNotCompleted += mes;
    };

    CheckStatus(m_BufferLoader->CreateBuffersForModelGeometry(d)," model");
    CheckStatus(m_BufferLoader->CreateBufferForTextureCoord(tex)," texDiffuse");
    CheckStatus(m_BufferLoader->CreateBufferForTextureCoord(*model->getTextureOfType(TextureForModel::TextureType::Height))," texHeight");
    CheckStatus(m_BufferLoader->CreateBufferForTextureCoord(*model->getTextureOfType(TextureForModel::TextureType::Normal))," texNormal");
    if(messageLoadNotCompleted.size()) {
        //całe to zabezpieczenie miało wyłapać, czy aby każda tekstura ma swoją mapę współrzędnych, co jak się okazuje nie ma sensu, bo i tak
        //shader przyjmuje tylko jeden rodzaj współrzednych i uzywa ich wspólnie.
        messageLoadNotCompleted = "Buffers not complete loaded for: " + messageLoadNotCompleted;
        MyOnGLError(myoglERR_OTHER_ERROR,messageLoadNotCompleted.c_str() );
    }
    vec_for_subbuf tbnInversed = CalculateInversedMatricesForModel(model);
    m_BufferLoader->CreateBufferWithSubs(tbnInversedBufferIdForModel(model->getModelId()),tbnInversed);
}
void ParalaxOclusionMapRenderSystem::LoadVAO(spOneModel model)
{
    auto& tex = *model->MyTexture();
    auto& d = model->GetModelData();
    auto& vao = model->getVao();
    m_BufferLoader->StartLoadingBuffersWith(vao);
    m_BufferLoader->LoadBufferOnLocation3f(d.bufVertId,(size_t)pomShAttr::aPos);
    m_BufferLoader->LoadBufferOnLocation3f(d.bufTangentId,(size_t)pomShAttr::aTangent);
    m_BufferLoader->LoadBufferOnLocation3f(d.bufBitangentId,(size_t)pomShAttr::aBitangent);
    m_BufferLoader->LoadSubBufferOnLocation3f(d.bufColNorId,(size_t)pomShAttr::aNormal,4,d.nuColours);
    m_BufferLoader->LoadBufferOnLocation2f(tex.bufTexCoordId,(size_t)pomShAttr::aTexCoords);
    m_BufferLoader->LoadIndicesAndFinish(d.bufIndexId);
}
