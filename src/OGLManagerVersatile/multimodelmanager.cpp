#include "multimodelmanager.h"
#include "shadersPath.h"
#include "textfile.h"
#include "convexsurface.h"

using namespace std;

MultiModelManager::MultiModelManager(myOGLErrHandler* extErrHnd)
{
    setErrHandler(extErrHnd);
    ptr_TextureShader = make_shared<myOGLShaders>();
    int i = 7;
    cameraTrial = make_shared<CameraTrial>();
    m_Camera = cameraTrial;
    m_ptrMatrixStack = make_shared<MatrixStack>();
    m_selecting = make_shared<Selecting>();
    MakeAndSetCustomModels();
}

MultiModelManager::~MultiModelManager()
{
}
void MultiModelManager::setModels(vector<spOneModel>&& m)
{
    models = m;
}
void MultiModelManager::MakeAndSetCustomModels()
{
#ifndef TESTOWANIE_F
//#define TEXTURE_IMAGE "Resources/andromeda.jpg"
#define TEXTURE_IMAGE "../ResourcesGramont2/MB640x400.jpg"
#define TEXTURE_IMAGE2 "../ResourcesGramont2/ksiezyc.jpg"
    auto model_1 = make_shared<ConvexSurface>(80,80,200,200,50);
    auto model_2 = make_shared<ConvexSurface>(80,80,100,100,30);
    setModels(vector<spOneModel> {model_1,model_2});
    model_1->Translate( {60.0f,0.0f,0.0f});
    model_2->Rotate(60.0f, {0.0f,0.3f,0.8f});
    model_2->Translate( {-60.0f,0.0f,0.0f});
    model_1->MyTexture()->LoadImageFile(TEXTURE_IMAGE2);
    model_2->MyTexture()->LoadImageFile(TEXTURE_IMAGE);
#endif
}
void MultiModelManager::setMatricesForRender(spOglRenderer rend)
{
    //OLD VERSION
//myOGLManager::setMatricesForRender(rend);
    //NEW VERSION
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
#ifdef TESTOWANIE_F
        tex.LoadImageFile(TEXTURE_IMAGE);
#endif
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
    m_ptrMatrixStack->setViewGlmMatrixdv(cameraTrial->getViewGlmMatrixdv());
    m_ptrMatrixStack->setProjectionGlmMatrixdv(cameraTrial->getProjGlmMatrixdv());
}
void MultiModelManager::DrawModels(spOglRenderer renderer)
{
    for(auto& model : models) {
        auto& tex = *model->MyTexture();
        auto d = model->GetModelData();
        m_ptrMatrixStack->setModelGlmMatrixdv(model->getModelGlmMatrixdv());
        m_ptrMatrixStack->UpdateMatrices();
        renderer->DrawTextureForSingleModelEntry(tex, d,ptr_TextureShader->getProgramId());
    }
}
void MultiModelManager::Draw3d()
{
    DrawModels(m_TexRenderer);
}
void MultiModelManager::OnMouseMiddleClick(int posX, int posY)
{
    if(doesCameraViewControl) {
        cameraTrial->MoveOnScreenPlane(m_mousePrevX, m_mousePrevY, posX, posY);
    } else {
        models[0]->MoveOnScreenPlane(m_mousePrevX, m_mousePrevY, posX, posY,m_ptrMatrixStack->getViewGlmMatrixdv());
    }
    m_mousePrevX = posX;
    m_mousePrevY = posY;
}
void MultiModelManager::OnMouseRotDragging(int posX, int posY)
{
    if ( m_mousePrevX == posX && m_mousePrevY == posY )
        return;


    if(doesCameraViewControl) {
        m_Camera->MouseRotation(m_mousePrevX, m_mousePrevY, posX, posY);
    } else {
        ScreenMove move;
        move.fromX = m_mousePrevX;
        move.fromY = m_mousePrevY;
        move.toX = posX;
        move.toY = posY;
        models[0]->MouseRotation(cameraTrial->RotationFromScreenMove(move));
    }
    m_mousePrevX = posX;
    m_mousePrevY = posY;
}
void MultiModelManager::OnMouseWheel(int direction)
{
    cameraTrial->MoveBackForWard(direction);
}

void MultiModelManager::OnMouseLeftDClick(int posX, int posY)
{
    //selectedModel = m_picking->PickModelInPoint(models,m_ptrMatrixStack,posX,posY)); ver1

//    m_picking->setReadPosition(posX,posY); //ver 2
    DrawModels(m_selecting->getRenderer());
//    auto selected = m_picking->getSelectedFrom(models);

}
