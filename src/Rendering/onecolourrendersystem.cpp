#include "onecolourrendersystem.h"
#include "shadersPath.h"
#include "textfile.h"

using namespace std;

OneColourRenderSystem::OneColourRenderSystem():loc{0}
{
//    m_renderer = std::make_unique<PomRenderer>();
}

bool OneColourRenderSystem::ConfigureShadersAndLocations()
{
    const char * vertCode = textFileRead(d_oneColourVertexShaderPath);
    const char * fragIlumCode = textFileRead(d_illuminationShaderPath);
    const char * fragCode = textFileRead(d_oneColourFragmentShaderPath);

    m_shader->AddCode(vertCode,GL_VERTEX_SHADER);
    m_shader->AddCode(fragIlumCode,GL_FRAGMENT_SHADER);
    m_shader->AddCode(fragCode,GL_FRAGMENT_SHADER);
    MA_CreateStrings(attribs, ONECOL_SH_ATTR);
    for (auto& attr : attribs)m_shader->AddAttrib(attr);

    

    m_BufferLoader->shadAttribLocations.resize((size_t)oneColShAttr::oneColShAttrSize);

    bool ok = true;
    ok &= (bool)vertCode;
    ok &= (bool)fragCode;
    MA_CreateStrings(uniforms, ONECOL_SH_UNIF);
    string nameOfFunction = "OneColourRenderSystem::ConfigureShadersAndLocations";
    if(ok)InitShadersAndReadLocations(nameOfFunction, uniforms, loc);


    int atLoc[(size_t)oneColShAttr::oneColShAttrSize];
//    , unifLoc[(size_t)oneColShUnif::oneColShUnifSize];//******DEBUG*
//    string unifNames[(size_t)oneColShUnif::oneColShUnifSize];
    //locations are accessible after compile and link shader
    for(short a = 0 ; a < (short)oneColShAttr::oneColShAttrSize ; a++) {
        m_BufferLoader->shadAttribLocations[a] = m_shader->GetAttribLoc(attribs[a]);
        atLoc[a] = m_BufferLoader->shadAttribLocations[a];//******DEBUG*
    }

//    for(short u = 0 ; u < (short)oneColShUnif::oneColShUnifSize; u++) {
//        m_renderer->shadUnifLocations[u] = m_shader->GetUnifLoc(uniforms[u]);
//        unifNames[u] = uniforms[u];
//        unifLoc[u] = m_renderer->shadUnifLocations[u];//******DEBUG*
//    }
    return true;
}

void OneColourRenderSystem::CreateGraphicBuffers(spOneModel model)
{
    auto& d = model->GetModelData();
    string messageLoadNotCompleted;
    auto CheckStatus = [&](BufferLoaderProgress loadStatus, string&& mes) {
        if(loadStatus != BufferLoaderProgress::Completed)messageLoadNotCompleted += mes;
    };

    CheckStatus(m_BufferLoader->CreateBuffersForModelGeometry(d)," model");
    if(messageLoadNotCompleted.size()) {
        messageLoadNotCompleted = "Buffers not complete loaded for: " + messageLoadNotCompleted;
        MyOnGLError(myoglERR_OTHER_ERROR,messageLoadNotCompleted.c_str() );
    }

}
void OneColourRenderSystem::LoadVAO(spOneModel model)
{
    auto& tex = *model->MyTexture();
    auto& d = model->GetModelData();
    auto& vao = model->getVao();
    m_BufferLoader->StartLoadingBuffersWith(vao);
    m_BufferLoader->LoadBufferOnLocation3f(d.bufVertId,(size_t)oneColShAttr::aPos);
    m_BufferLoader->LoadSubBufferOnLocation3f(d.bufColNorId,(size_t)oneColShAttr::aNormal,4,d.nuColours);
    m_BufferLoader->LoadIndicesAndFinish(d.bufIndexId);
}
void OneColourRenderSystem::Draw(spOneModel model)
{
	auto vao = model->getVao();
    if(!vao)return;
    glUseProgram(getProgramId());
    glBindVertexArray(vao);
    
     
    glUniformMatrix4fv(loc[(size_t)oneColShUnif::mMVP], 1, GL_FALSE, 
                        matrixStack->getModelViewProjectionMatrixfv());
    glUniformMatrix4fv(loc[(size_t)oneColShUnif::mToViewSpace], 1, GL_FALSE, 
                        matrixStack->getViewMatrixfv());
    glUniform4fv(loc[(size_t)oneColShUnif::lightProps], 1, light->GetFLightPos());
    glUniform3fv(loc[(size_t)oneColShUnif::lightColour], 1, light->GetFLightColour());
    
    auto d = model->GetModelData();

    DrawIndicesAndFinish(d);
}
