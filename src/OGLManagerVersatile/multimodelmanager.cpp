#include "multimodelmanager.h"
#include "convexsurface.h"
#include "paralaxoclusionmaprendersystem.h"

using namespace std;

MultiModelManager::MultiModelManager(myOGLErrHandler* extErrHnd)
{
    if(extErrHnd)
        setErrHandler(extErrHnd);
//    ptr_TextureShader = make_shared<myOGLShaders>();
    ptr_TextureShader = m_renderSystem->getShader();
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
void MultiModelManager::RenderSystemSetIfWant()
{
//    setAndConfigureRenderSystem(make_unique<c>());//move to configuration module (when it will be made)
    setAndConfigureRenderSystem(make_unique<OneTextureRenderSystem>());
}
void MultiModelManager::CallForMyRenderable(FunReSys FunToCall, spRenderSystem rs)
{
    for(auto& model : models) {
        auto& tex = *model->MyTexture();
        auto& d = model->GetModelData();
        rs->setActiveVaoPtr(model->getVaoPtr());
        ((*rs).*FunToCall)(d,tex);
    }
}
void MultiModelManager::ConfigureWithMyViewControl(spRenderSystem rs)
{
    rs->getRenderer()->setViewMatrices(m_ptrMatrixStack);
}

void MultiModelManager::SetShadersAndGeometry()
{

#ifdef TESTOWANIE_F
#define TEXTURE_IMAGE "nieistniejacyPlik.jpg"
//real path is timeconsume in tests
#endif

    m_Light.Set(myVec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0, 1.0);

//    m_renderSystem->ConfigureShadersAndLocations();
    m_selecting->ConfigureShadersAndLocations();

    ConfigureWithMyViewControl(m_selecting);
    
    setAndConfigureRenderSystem(make_unique<ParalaxOclusionMapRenderSystem>());
    
    for(auto& model : models) {
//        auto& tex = *model->MyTexture();
//        auto& d = model->GetModelData();
#ifdef TESTOWANIE_F
//        tex.LoadImageFile(TEXTURE_IMAGE);
#endif
//        m_BufferLoader->CreateBuffersForModelGeometry(d);
//        m_BufferLoader->CreateBufferForTextureCoord(tex);
//        unsigned int& vao = tex.textureVAO;
//        m_BufferLoader->CreateVao(vao);
//        //dla renderera potrzbna będzie możliwość wybrania dla którego vao renderuje
//        m_BufferLoader->LoadBuffersForModelGeometry(d,vao);
//        m_BufferLoader->LoadBufferForTexture(tex,vao);
        m_selecting->getBufferLoader()->LoadBuffers(model);
    }

    /**zamiast powyższej pętli***/
//    CallForMyRenderable(&RenderSystem::CreateGraphicBuffers,m_renderSystem);// funkcja CallForMyRenderable już działa
//    CallForMyRenderable(&RenderSystem::ReloadVAO,m_renderSystem);
//    CallForMyRenderable(&RenderSystem::ReloadVAO,m_selecting); // to nie będzie działać bo reload powinno tworzyć vao dotyczące selectu a nie tekstury
    /*****/
    auto auccessBufferLoadedCount = m_BufferLoader->LoadTextureSuccessCount();

    ConfigureWithMyViewControl(m_renderSystem);
    ConfigureWithMyLightSystem(m_renderSystem);
    

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
    }
}
void MultiModelManager::OnMouseMiddleClick(int posX, int posY)
{
    if(doesCameraViewControl) {
        cameraTrial->MoveOnScreenPlane(m_mousePrevX, m_mousePrevY, posX, posY);
    } else {
//        models[0]->MoveOnScreenPlane(m_mousePrevX, m_mousePrevY, posX, posY,m_ptrMatrixStack->getViewGlmMatrixdv());
        selectedTransformable->MoveOnScreenPlane(m_mousePrevX, m_mousePrevY, posX, posY,m_ptrMatrixStack->getViewGlmMatrixdv());
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
        selectedTransformable->MouseRotation(cameraTrial->RotationFromScreenMove(move));
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
    m_selecting->setReadPosition(posX,posY);
    m_selecting->EnableWritingToFrameBuffer();
    auto previousActiveShader = activeShader;
    auto previousActiveRenderer = activeRenderer;
    activeRenderer = m_selecting->getRenderer();
    activeShader = m_selecting->getShader();
    Draw3d();
    m_selecting->DisableWritingToFrameBuffer();
    m_selecting->ReadInClickedPosition();
//    activeRenderer = m_TexRenderer;
//    activeShader = ptr_TextureShader;
    activeRenderer = previousActiveRenderer;
    activeShader = previousActiveShader;
    setSelectingResult(m_selecting->getResult());
}
void MultiModelManager::SetViewport(int x, int y, int width, int height)
{
    myOGLManager::SetViewport(x,y,width,height);
    m_selecting->setWindowSize(width,height);
}
void MultiModelManager::setSelectingResult(SelectingResult&& res)
{
    if(!res.selectingDone()) {
        doesCameraViewControl = true;
        return;
    }
    doesCameraViewControl = false;
    spOneModel mod =  static_pointer_cast<OneModel>(res.getSelected());
    selectedTransformable = static_pointer_cast<Transformable>(mod);
}
void MultiModelManager::SwitchViewControl()
{
    if(!selectedTransformable) {
        doesCameraViewControl = true;
        return;
    }
    myOGLManager::SwitchViewControl();
}
