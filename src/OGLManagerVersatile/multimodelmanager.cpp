#include "multimodelmanager.h"
#include "convexsurface.h"
#include "paralaxoclusionmaprendersystem.h"
#include "normalmaprendersystem.h"

using namespace std;

MultiModelManager::MultiModelManager(myOGLErrHandler* extErrHnd)
{
    if(extErrHnd)
        setErrHandler(extErrHnd);
    int i = 7;
    cameraTrial = make_shared<CameraTrial>();
    m_Camera = cameraTrial;
    m_ptrMatrixStack = make_shared<MatrixStack>();
    m_selecting = make_shared<Selecting>();
    m_rs_manager = make_shared<RenderSystemManager>();
    activeRenderSystem = m_rs_manager;
    
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
    auto model_1 = make_shared<ConvexSurface>(80,80,160,160,70);
    auto model_2 = make_shared<ConvexSurface>(80,80,160,160,70);
    setModels( {model_1,model_2});/**************/
//    setModels( {model_1});

//    model_1->Rotate(60.0f, {0.0f,0.3f,0.8f});
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
    model_2->CalculateTangentAndBitangentForAllPointsBasedOn(*model_2->MyTexture());

    spTextureForModel texHg, texNr;
    texHg = make_shared<TextureForModel>();
    texNr = make_shared<TextureForModel>();
    texHg->setTextureInMemory(texm_height_1);
    texHg->setCoordinates(model_1->MyTexture());
    texNr->setTextureInMemory(texm_normal_1);
    texNr->setCoordinates(model_1->MyTexture());
    model_1->AddTexture(texHg,TextureForModel::Height);
    model_1->AddTexture(texNr,TextureForModel::Normal);

    model_2->AddTexture(texNr,TextureForModel::Normal);

    m_selecting->RegisterSelectable( {model_1,model_2});
//    m_selecting->RegisterSelectable( {model_1});

    unsigned pomId = m_rs_manager->AddRenderSystem<ParalaxOclusionMapRenderSystem>();
    unsigned normalId = m_rs_manager->AddRenderSystem<NormalMapRenderSystem>();
    m_rs_manager->ConnectModelWithRenderSystem(model_1->getUniqueId(),pomId);
    m_rs_manager->ConnectModelWithRenderSystem(model_2->getUniqueId(),normalId);


#endif
}
//void MultiModelManager::RenderSystemSetIfWant()
//{
////    setAndConfigureRenderSystem(make_unique<c>());//move to configuration module (when it will be made)
//    setAndConfigureRenderSystem(make_unique<OneTextureRenderSystem>());
//}
void MultiModelManager::ConnectWithMyViewControl(spRenderSystem rs)
{
    rs->matrixStack = m_ptrMatrixStack;
    rs->camera = cameraTrial;//we don't know where it is used, so getter and setter would be better
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


void MultiModelManager::SetShadersAndGeometry()
{

#ifdef TESTOWANIE_F
#define TEXTURE_IMAGE "nieistniejacyPlik.jpg"
//real path is timeconsume in tests
#endif
    MakeAndSetCustomModels();
    m_Light.Set(myVec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0, 1.0);

    m_selecting->ConfigureShadersAndLocations();
    ConnectWithMyViewControl(m_selecting);

    m_rs_manager->ConfigureShadersAndLocations();
    CallForMyRenderable(&RenderSystem::CheckModelWasConnected,m_rs_manager);
    CallForMyRenderable(&RenderSystem::CreateGraphicBuffers,m_rs_manager);
    CallForMyRenderable(&RenderSystem::CreateVAO,m_rs_manager);
    CallForMyRenderable(&RenderSystem::LoadVAO,m_rs_manager);
    CallForMyTextures(&RenderSystem::CreateGraphicBuffers,m_rs_manager);
    for(auto& rs : m_rs_manager->getAllRenderSystems()) {
        ConnectWithMyViewControl(rs);
        ConnectWithMyLightSystem(rs);
    }
    
    CallForMyRenderable(&RenderSystem::LoadVAO,m_selecting); //may not be earlier

    m_ptrMatrixStack->setViewGlmMatrixdv(cameraTrial->getViewGlmMatrixdv());
    m_ptrMatrixStack->setProjectionGlmMatrixdv(cameraTrial->getProjGlmMatrixdv());
    m_ptrMatrixStack->setInvViewGlmMatrixdv(cameraTrial->getInvViewGlmMatrixdv());
}
void MultiModelManager::Draw3d()
{
    for(auto& model : models) {
        m_ptrMatrixStack->setModelGlmMatrixdv(model->getModelGlmMatrixdv());
        m_ptrMatrixStack->UpdateMatrices();
        activeRenderSystem->Draw(model);
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
    activeRenderSystem = m_selecting;
    Draw3d();
    m_selecting->DisableWritingToFrameBuffer();
    m_selecting->ReadInClickedPosition();
    activeRenderSystem = m_rs_manager;
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
