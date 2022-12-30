#include "multimodelmanager.h"
#include "shadersPath.h"
#include "textfile.h"
#include "convexsurface.h"

MultiModelManager::MultiModelManager(myOGLErrHandler* extErrHnd):myOGLManager(extErrHnd)
{
    ptr_TextureShader = make_shared<myOGLShaders>();
    int i = 7;
    cameraTrial = make_shared<CameraTrial>(); 
    m_Camera = cameraTrial;
    m_ptrMatrixStack = make_shared<MatrixStack>();
#ifndef TESTOWANIE_F
#define TEXTURE_IMAGE "Resources/MB640x400.jpg"
    //move to separate private method
    auto model_1 = make_shared<ConvexSurface>(80,80,200,200,50);
//    auto model_2 = make_shared<OneModelMock>();
    setModels(vector<spOneModel> {model_1});
#endif
}

MultiModelManager::~MultiModelManager()
{
}
void MultiModelManager::setModels(vector<spOneModel>&& m)
{
    models = m;
}

void MultiModelManager::setMatricesForRender(upOglRenderer& rend)
{
    rend->m_matrices.matMVP = m_ptrMatrixStack->getModelViewProjectionMatrixfv();
    rend->m_matrices.matToVw = m_ptrMatrixStack->getViewMatrixfv();
    rend->m_matrices.light_position = m_Light.GetFLightPos();
    rend->m_matrices.light_colour = m_Light.GetFLightColour();
}
void MultiModelManager::SetShadersAndGeometry()
{

#ifdef TESTOWANIE_F
#define TEXTURE_IMAGE "nieistniejacyPlik.jpg"
//real path is timeconsume in tests
#endif

    char t[] = "shader code";
    textureShaderCode = t;
    ptr_TextureShader->AddCode(textFileRead(d_textureVertexShaderPath),GL_VERTEX_SHADER);
    ptr_TextureShader->AddCode(textFileRead(d_illuminationShaderPath),GL_FRAGMENT_SHADER);
    ptr_TextureShader->AddCode(textFileRead(d_textureFragmentShaderPath),GL_FRAGMENT_SHADER);
    ptr_TextureShader->AddAttrib("in_sPosition");
    ptr_TextureShader->AddAttrib("in_sNormal");
    ptr_TextureShader->AddAttrib("in_TextPos");
    ptr_TextureShader->AddUnif("mMVP");
    ptr_TextureShader->AddUnif("mToViewSpace");
    ptr_TextureShader->AddUnif("lightProps");
    ptr_TextureShader->AddUnif("lightColour");
    ptr_TextureShader->AddUnif("stringTexture");
    ptr_TextureShader->Init();
    m_Light.Set(myVec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0, 1.0);
    vector<t_BLoc_str> locNamsTexBuff {
        {&BLoc::position_tex,"in_sPosition"},
        {&BLoc::normal_tex,"in_sNormal"},
        {&BLoc::textureCoord,"in_TextPos"},
    };
    setLocations<BufferLoader>(m_BufferLoader,locNamsTexBuff,*ptr_TextureShader,&myOGLShaders::GetAttribLoc);
    int a = models.size();
    for(auto& model : models) {
        auto& tex = *model->MyTexture();// czy auto& ?
        auto& d = model->GetModelData();
        tex.LoadImageFile(TEXTURE_IMAGE);
        m_BufferLoader->CreateBuffersForSingleModelEntry(d);
        m_BufferLoader->LoadTextureBuffersForSingleModelEntry(tex, d);
//        model->GetModelData();
    }

    auto auccessBufferLoadedCount = m_BufferLoader->LoadTextureSuccessCount();
    vector<t_OLoc_str> tnames {
        {&OLoc::mMVP,"mMVP"},
        {&OLoc::mToViewSpace,"mToViewSpace"},
        {&OLoc::lightProps,"lightProps"},
        {&OLoc::lightColour,"lightColour"},
        {&OLoc::stringTexture,"stringTexture"}
    };
    setLocations<OglRenderer>(m_TexRenderer,tnames,*ptr_TextureShader,&myOGLShaders::GetUnifLoc);
    setMatricesForRender(m_TexRenderer);
    m_ptrMatrixStack->setNeedUpdateViewMat(&cameraTrial->needUpdateViewMat);
    m_ptrMatrixStack->setNeedUpdateProjectionMat(&cameraTrial->needUpdateProjMat);
    m_ptrMatrixStack->setViewMatrixdv(cameraTrial->getViewMatrixdv());
    m_ptrMatrixStack->setProjectionMatrixdv(cameraTrial->getProjMatrixdv());
}

void MultiModelManager::Draw3d()
{
    m_Camera->UpdateMatrices();
    for(auto& model : models) {
        auto& tex = *model->MyTexture();
        auto d = model->GetModelData();
        m_ptrMatrixStack->setNeedUpdateModelMat(&model->needUpdateModelMat);
        m_ptrMatrixStack->setModelMatrixdv(model->getModelMatrixdv());
        m_ptrMatrixStack->UpdateMatrices();
        m_TexRenderer->DrawTextureForSingleModelEntry(tex, d,ptr_TextureShader->getProgramId());
//        model->GetModelData();
    }
}

void MultiModelManager::OnMouseLeftDClick(int posX, int posY)
{
}

