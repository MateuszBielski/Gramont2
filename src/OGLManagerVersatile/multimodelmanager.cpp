#include "multimodelmanager.h"
#include "convexsurface.h"
#include "paralaxoclusionmaprendersystem.h"
//#include "normalmaprendersystem.h"

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
#define TEXTURE_IMAGE2 "../ResourcesGramont2/MB640x400.png"
//#define TEXTURE_IMAGE2 "../ResourcesGramont2/ksiezyc.jpg"
//#define TEXTURE_IMAGE2 "../ResourcesGramont2/andromeda1955.jpg"

//#define T_BASE_1 "../ResourcesGramont2/maleKamienie1024base.jpg"
#define T_BASE_1 "../ResourcesGramont2/MB640x400.png"
#define T_NORMAL_1 "../ResourcesGramont2/maleKamienie1024normal.jpg"
#define T_HEIGHT_1 "../ResourcesGramont2/maleKamienie1024height.jpg"
    auto model_1 = make_shared<ConvexSurface>(80,80,205,205,80);
    auto model_2 = make_shared<ConvexSurface>(80,80,200,200,80);
    setModels( {model_1,model_2});

    model_1->Rotate(60.0f, {0.0f,0.3f,0.8f});
    model_1->Translate( {60.0f,0.0f,0.0f});
    model_2->Rotate(60.0f, {0.0f,0.3f,0.8f});
    model_2->Translate( {-60.0f,0.0f,0.0f});

    spTextureInMemory texm_2, texm_base_1, texm_normal_1, texm_height_1;
    texm_base_1 = make_shared<TextureInMemory>(T_BASE_1);
    texm_normal_1 = make_shared<TextureInMemory>(T_NORMAL_1);
    texm_height_1 = make_shared<TextureInMemory>(T_HEIGHT_1);
    texm_2 = make_shared<TextureInMemory>(TEXTURE_IMAGE2);
    texms.push_back(texm_base_1);
    texms.push_back(texm_normal_1);
    texms.push_back(texm_height_1);
    texms.push_back(texm_2);

    model_1->MyTexture()->setTextureInMemory(texm_base_1);
    model_2->MyTexture()->setTextureInMemory(texm_2);
    model_1->CalculateTangentAndBitangentForAllPointsBasedOn(*model_1->MyTexture());

    spTextureForModel texHg, texNr;
    texHg = make_shared<TextureForModel>();
    texNr = make_shared<TextureForModel>();
    texHg->setTextureInMemory(texm_height_1);
    texHg->setCoordinates(model_1->MyTexture());
    texNr->setTextureInMemory(texm_normal_1);
    texNr->setCoordinates(model_1->MyTexture());
    model_1->AddTexture(texHg,TextureForModel::Height);
//    model_1->CopyCoordintatesFromMainTex(texHg,TextureForModel::Height);//brzydko wygląda
    model_1->AddTexture(texNr,TextureForModel::Normal);

    m_selecting->RegisterSelectable( {model_1,model_2});
#endif
}
void MultiModelManager::RenderSystemSetIfWant()
{
//    setAndConfigureRenderSystem(make_unique<c>());//move to configuration module (when it will be made)
    setAndConfigureRenderSystem(make_unique<OneTextureRenderSystem>());
}
void MultiModelManager::CallForMyRenderable(FunReSys_spOm FunToCall, spRenderSystem rs)
{
    for(auto& model : models) {
        ((*rs).*FunToCall)(model);
    }
}
void MultiModelManager::CallForMyTextures(FunReSys_Tim FunToCall, spRenderSystem rs)
{
    for(auto& textureInMemory : texms) {
        ((*rs).*FunToCall)(*textureInMemory);
    }
}
void MultiModelManager::ConfigureWithMyViewControl(spRenderSystem rs)
{
    rs->getRenderer()->setViewMatrices(m_ptrMatrixStack);
    rs->getRenderer()->m_viewParamsfv.viewPosition = cameraTrial->getPositonfv();
}

void MultiModelManager::SetShadersAndGeometry()
{

#ifdef TESTOWANIE_F
#define TEXTURE_IMAGE "nieistniejacyPlik.jpg"
//real path is timeconsume in tests
#endif

    m_Light.Set(myVec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0, 1.0);

    m_selecting->ConfigureShadersAndLocations();
    ConfigureWithMyViewControl(m_selecting);

    setAndConfigureRenderSystem(make_unique<ParalaxOclusionMapRenderSystem>());
//    setAndConfigureRenderSystem(make_unique<NormalMapRenderSystem>());
//    setAndConfigureRenderSystem(make_unique<OneTextureRenderSystem>());

    CallForMyRenderable(&RenderSystem::LoadVAO,m_selecting);

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
        /*******
        glm::dvec4 corner(d.verts[0],d.verts[1],d.verts[2],1.0);
        std::cout<<"\npunkt pierwszy:\n"<<corner.x<<" "<<corner.y<<" "<<corner.z;
        glm::dvec4 modelCorner = *model->getModelGlmMatrixdv() * corner;
        std::cout<<"\npunkt pierwszy macierz model:\n"<<modelCorner.x<<" "<<modelCorner.y<<" "<<modelCorner.z;
        glm::dmat4x4 * viewMatrix = m_ptrMatrixStack->getViewGlmMatrixdv();
        glm::dvec4 modelViewCorner = *viewMatrix * modelCorner;
        std::cout<<"\npunkt pierwszy macierz model widok:\n"<<modelViewCorner.x<<" "<<modelViewCorner.y<<" "<<modelViewCorner.z;
        glm::dmat4x4 * projMatrix = m_ptrMatrixStack->getProjGlmMatrixdv();
        glm::dvec4 mVPcorner = *projMatrix * modelViewCorner;
        std::cout<<"\npunkt pierwszy macierz mvp:\n"<<mVPcorner.x<<" "<<mVPcorner.y<<" "<<mVPcorner.z;
        *******/
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
