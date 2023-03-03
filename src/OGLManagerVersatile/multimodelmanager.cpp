#include "multimodelmanager.h"
#include "shadersPath.h"
#include "textfile.h"
#include "convexsurface.h"

using namespace std;

MultiModelManager::MultiModelManager(myOGLErrHandler* extErrHnd)
{
    if(extErrHnd)
        setErrHandler(extErrHnd);
    ptr_TextureShader = make_shared<myOGLShaders>();
    int i = 7;
    cameraTrial = make_shared<CameraTrial>();
    m_Camera = cameraTrial;
    m_ptrMatrixStack = make_shared<MatrixStack>();
    m_selecting = make_shared<Selecting>();
    MakeAndSetCustomModels();
    activeShader = ptr_TextureShader;
    activeRenderer = m_TexRenderer;
//    activeRenderer = m_selecting->getRenderer();
//    activeShader = m_selecting->getShader();
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
//    setModels(vector<spOneModel> {model_1,model_2});
    setModels( {model_1,model_2});
    model_1->Translate( {60.0f,0.0f,0.0f});
    model_2->Rotate(60.0f, {0.0f,0.3f,0.8f});
    model_2->Translate( {-60.0f,0.0f,0.0f});
    model_1->MyTexture()->LoadImageFile(TEXTURE_IMAGE2);
    model_2->MyTexture()->LoadImageFile(TEXTURE_IMAGE);
    m_selecting->RegisterSelectable( {model_1,model_2});
#endif
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

    string nameOfFunction = "MultiModelManager::SetShadersAndGeometry";
    ptr_TextureShader->Init(nameOfFunction);
    m_Light.Set(myVec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0, 1.0);
    vector<t_BLoc_str> locNamsTexBuff {
        {&BLoc::position_tex,"in_sPosition"},
        {&BLoc::normal_tex,"in_sNormal"},
        {&BLoc::textureCoord,"in_TextPos"},
    };

    m_selecting->Init();
    m_selecting->getRenderer()->setViewMatrices(m_ptrMatrixStack);
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
        m_selecting->getBufferLoader()->LoadBuffers(model);
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

    m_TexRenderer->setViewMatrices(m_ptrMatrixStack);
    m_TexRenderer->setLightMatrices(&m_Light);
    
    /*tymczasowe*/
    m_selecting->getRenderer()->setViewMatrices(m_ptrMatrixStack);
    m_selecting->getRenderer()->setLightMatrices(&m_Light);
    /*koniec tymczasowe*/
    
    m_ptrMatrixStack->setViewGlmMatrixdv(cameraTrial->getViewGlmMatrixdv());
    m_ptrMatrixStack->setProjectionGlmMatrixdv(cameraTrial->getProjGlmMatrixdv());
}

void MultiModelManager::Draw3d()
{

    for(auto& model : models) {
        auto& tex = *model->MyTexture();
        auto d = model->GetModelData();
        m_ptrMatrixStack->setModelGlmMatrixdv(model->getModelGlmMatrixdv());
        m_ptrMatrixStack->UpdateMatrices();
        activeRenderer->DrawModel(model,activeShader->getProgramId());
//        activeRenderer->DrawTextureForSingleModelEntry(tex, d,activeShader->getProgramId());
    }
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

    m_selecting->setReadPosition(posX,posY);
    m_selecting->EnableWritingToFrameBuffer();
    activeRenderer = m_selecting->getRenderer();
    activeShader = m_selecting->getShader();
    Draw3d();
    m_selecting->DisableWritingToFrameBuffer();
    m_selecting->ReadPixel(posX, posY);
    activeRenderer = m_TexRenderer;
    activeShader = ptr_TextureShader;

    Draw3d();
//    DrawModels(m_selecting->getRenderer());
//    auto selected = m_picking->getSelectedFrom(models);

}
void MultiModelManager::SetViewport(int x, int y, int width, int height)
{
    myOGLManager::SetViewport(x,y,width,height);
    m_selecting->setWindowSize(width,height);
}
