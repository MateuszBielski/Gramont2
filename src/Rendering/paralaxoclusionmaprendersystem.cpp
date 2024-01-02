#include "paralaxoclusionmaprendersystem.h"
#include "shadersPath.h"
#include "textfile.h"
#include "pomrenderer.h"

using namespace std;

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
vec_for_subbuf ParalaxOclusionMapRenderSystem::CalculateInversedMatricesForModel(spOneModel model)
{
    auto& d = model->GetModelData();
    if(!d.tangents || !d.bitangents)return vec_for_subbuf();
    
    unsigned i = 0, amountOfMatrices = d.nuNormals;
    float * inv_t = new float[3 * amountOfMatrices];
    float * inv_b = new float[3 * amountOfMatrices];
    float * inv_n = new float[3 * amountOfMatrices];

    for(i ; i < amountOfMatrices; i++) {
        glm::mat3x3 tbn(
            d.tangents[3*i],
            d.tangents[3*i + 1],
            d.tangents[3*i + 2],
            d.bitangents[3*i],
            d.bitangents[3*i + 1],
            d.bitangents[3*i + 2],
            d.normals[3*i],
            d.normals[3*i + 1],
            d.normals[3*i + 2]
        );
        glm::mat3x3 inv_mtbn(inverse(tbn));
        size_t siz_vec_f = 3 * sizeof(float);
        for(short j = 0 ; j < 3 ; j++) {
            inv_t[i * 3 + j] = inv_mtbn[0][j];
            inv_b[i * 3 + j] = inv_mtbn[1][j];
            inv_n[i * 3 + j] = inv_mtbn[2][j];
        }
    }
    return vec_for_subbuf {
        make_tuple(amountOfMatrices,3,inv_t),
        make_tuple(amountOfMatrices,3,inv_b),
        make_tuple(amountOfMatrices,3,inv_n),
    };
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
    if(!d.tangents || !d.bitangents)return;
    vec_for_subbuf tbnInversed = CalculateInversedMatricesForModel(model);
    /***********DEBUG************
    unsigned pointIndex = 2100;
//    unsigned pointIndex = 1;
    float t[3],b[3],n[3],ti[3],bi[3],ni[3];
    float  x[] = {0.0, 0.0, 0.0};
    size_t siz_v = 3 * sizeof(float);
    memcpy(t,d.tangents + pointIndex * 3, siz_v);
    memcpy(b,d.bitangents + pointIndex * 3, siz_v);
    memcpy(n,d.normals + pointIndex * 3, siz_v);
    memcpy(ti,get<2>(tbnInversed[0]) + pointIndex * 3, siz_v);
    memcpy(bi,get<2>(tbnInversed[1]) + pointIndex * 3, siz_v);
    memcpy(ni,get<2>(tbnInversed[2]) + pointIndex * 3, siz_v);
    auto Print = [](float * v) {
        cout<<"\n"<<v[0]<<", "<<v[1]<<", "<<v[2];
    };
    
    Print(t);
    Print(b);
    Print(n);
    Print(ti);
    Print(bi);
    Print(ni);
    
    Print(x);
    
    


    ***********************/
    tbnInversedBufferIdForModel[model->getUniqueId()] = 0;
    m_BufferLoader->CreateBufferWithSubs(tbnInversedBufferIdForModel[model->getUniqueId()],tbnInversed);
    for(auto& v : tbnInversed)delete [] get<2>(v);
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
void ParalaxOclusionMapRenderSystem::Draw(spOneModel)
{
	
}
