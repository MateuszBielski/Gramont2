#include <gtest/gtest.h>
#include "onemodelmanager.h"
#include "triangle.h"
//#include <glad/glad.h>
#include <stack>
#include "glFunctionsMock.h"
#include "onemodelmock.h"
#include "multimodelmanager.h"
#include "MultiModelManagerAccess.h"
#include "glshadersmock.h"
#include "OglRendererMock.h"
#include "matrixstackmock.h"
#include "selectingtestaccess.h"
#include "bufferloadermock.h"
#include "textfile.h" //AccessFileForTest
#include "rendersystemmock.h"


TEST(GlFunctionsMock,StaticDefine_FailureOnNextDefine)
{
    GlFunctionsMock funMock;
    ASSERT_FALSE(funMock.Define());
}


TEST(GlFunctionsMock,glGetError_EmptyFunction)
{
    GlFunctionsMock funMock;
    funMock.Define();
    ASSERT_EQ(glGetError,&glGetError_Mock);
}
TEST(OneModelManager,funMock_defined)
{
    GlFunctionsMock funMock;
//    OneModelManager man;
    OneModelManager& man = OneModelManager::CreateAndInitStatic();
    ASSERT_TRUE(funMock.IsDefined());
}

TEST(OneModelManager,SetTextureBuffers_FalseIfNoTexture)
{
//    OneModelManager man;
//    OneModelManager& man = OneModelManager::CreateAndInitStatic();
//    man.SetModel(make_unique<OneModel>());
//    ASSERT_FALSE(man.SetTextureBuffers());//function deleted
}
TEST(OneModelManager,SetTextureBuffers_True)
{
//    OneModelManager man;
//    OneModelManager& man = OneModelManager::CreateAndInitStatic();
//    man.SetModel(make_unique<Triangle>());
//    ASSERT_TRUE(man.SetTextureBuffers());
}

TEST(OneModelManager,UsingTexCoordBuffer)
{
//    OneModelManager& man = OneModelManager::CreateAndInitStatic();
//    spOneModel triang = make_shared<Triangle>();
//    man.SetModel(triang);
//    man.SetTextureBuffers();
//    ASSERT_GT(triang->MyTexture()->bufTexCoordId,0);
}
TEST(MultiModelManager,SetShadersAndGeometry_HasModels)
{
    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();
    MultiModelManager man(nullptr);
    man.setModels(vector<spOneModel> {model_1,model_2});
    man.SetShadersAndGeometry();
    ASSERT_TRUE(model_1->GetModelDataUsed() && model_2->GetModelDataUsed());
}

TEST(MultiModelManager,SetShadersAndGeometry_LightSettedAndBindToRenderMatrix)
{
    myLight l;
    int i;
    for(i = 0; i < 4 ; i++)
        ASSERT_FLOAT_EQ(0.0f, l.GetFLightPos()[i]);
    MultiModelManager man(nullptr);
    man.SetShadersAndGeometry();
    auto renderer = man.getActiveRenderer();
    auto position = renderer->m_viewParamsfv.light_position;
    ASSERT_NE(nullptr,position);
    auto colour = renderer->m_viewParamsfv.light_colour;
    ASSERT_NE(nullptr,colour);
    ASSERT_NE(0.0f, position[3]);//only intensity greater than 0
}
TEST(MultiModelManager,TextureAddShadersCode)
{

    AccessFileForTest enableAcc;
    MultiModelManager man(nullptr);
    spShadersMock shader = make_shared<glShadersMock>();
    spRenderSystem rs = make_shared<OneTextureRenderSystem>();
    rs->setShader(shader);
    ASSERT_EQ(0,shader->getShaCode().size());
    man.setAndConfigureRenderSystem(rs);

    ASSERT_TRUE(shader->hasCodeOfType(GL_VERTEX_SHADER));
    ASSERT_TRUE(shader->hasCodeOfType(GL_FRAGMENT_SHADER));
}
TEST(MultiModelManager,TextureShadersAddUnif)
{
    MultiModelManager man(nullptr);
    spShadersMock shader = make_shared<glShadersMock>();
    spRenderSystem rs = make_shared<OneTextureRenderSystem>();
    rs->setShader(shader);
    ASSERT_EQ(0,shader->getShaUnif().size());
    man.setAndConfigureRenderSystem(rs);

    ASSERT_NE(0,shader->getShaUnif().size());
    ASSERT_NE(0,shader->hasUnif("mMVP"));
    ASSERT_NE(0,shader->hasUnif("mToViewSpace"));
    ASSERT_NE(0,shader->hasUnif("lightProps"));
    ASSERT_NE(0,shader->hasUnif("lightColour"));
    ASSERT_NE(0,shader->hasUnif("stringTexture"));
}
TEST(MultiModelManager,TextureShadersAddAttrib)
{
    MultiModelManager man(nullptr);
    spShadersMock shader = make_shared<glShadersMock>();
    spRenderSystem rs = make_shared<OneTextureRenderSystem>();
    rs->setShader(shader);
    ASSERT_EQ(0,shader->getShaAttrib().size());
    man.setAndConfigureRenderSystem(rs);

    ASSERT_NE(0,shader->getShaAttrib().size());
    ASSERT_NE(0,shader->hasAttrib("in_sPosition"));
    ASSERT_NE(0,shader->hasAttrib("in_sNormal"));
    ASSERT_NE(0,shader->hasAttrib("in_TextPos"));
}

TEST(MultiModelManager,TextureShadersInit)
{
    AccessFileForTest enableAcc;
    MultiModelManager man(nullptr);
    spShadersMock shader = make_shared<glShadersMock>();
    spRenderSystem rs = make_shared<OneTextureRenderSystem>();
    rs->setShader(shader);
    man.setAndConfigureRenderSystem(rs);
    ASSERT_TRUE(shader->InitUsed());
}
TEST(MultiModelManager,CountLoadTextureBuffers)
{
    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();
    MultiModelManager man(nullptr);
    man.setModels(vector<spOneModel> {model_1,model_2});
    spRenderSystem rs = make_shared<OneTextureRenderSystem>();
    man.setAndConfigureRenderSystem(rs);
    auto buffer = rs->getBufferLoader();
    ASSERT_EQ(2,buffer->Counter(BufferLoaderCounterType::LoadBufferForModelGeometryCompleted));
    //ASSERT_EQ(2,buffer->LoadTextureSuccessCount());//how do this?
}
TEST(MultiModelManager,CountCreateBuffers)
{
    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();
    MultiModelManager man(nullptr);
    man.setModels(vector<spOneModel> {model_1,model_2});
    man.SetShadersAndGeometry();
    auto buffer = man.getBufferLoaderForTest();
    ASSERT_EQ(2,buffer->Counter(BufferLoaderCounterType::CreateBuffersForModelGeometryStart));
}
TEST(MultiModelManager,DrawTexture_IfMatircesUpdates)
{
    MultiModelManager man(nullptr);
    auto& camera = man.getCameraRefForTest();
    ASSERT_TRUE(camera.NeedMVPUpdate());
    man.Draw3d();
    ASSERT_FALSE(camera.NeedMVPUpdate());
}
TEST(MultiModelManager,DrawTexture_)
{
    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();
    MultiModelManager man(nullptr);
    man.setModels(vector<spOneModel> {model_1,model_2});
    man.Draw3d();
    auto renderer = man.getTexRendererForTest();
    ASSERT_EQ(2,renderer->StartCallCount());
}
TEST(MultiModelManager,RendererKnowsMatricesFromMatrixStack)
{
    MultiModelManager man(nullptr);
    spMatrixStack ms = make_shared<MatrixStack>();
    MultiModelManagerAccess acc(man);
    acc.setMatrixStack(ms);
    man.SetShadersAndGeometry();
    auto renderer = man.getActiveRenderer();
    ASSERT_EQ(ms->getModelViewProjectionMatrixfv(),renderer->m_viewParamsfv.matMVP);
    ASSERT_EQ(ms->getViewMatrixfv(),renderer->m_viewParamsfv.matToVw);
}
TEST(MultiModelManager,MatrixStackKnowsCameraMatrices)
{
    MultiModelManager man(nullptr);
    spMatrixStack ms = make_shared<MatrixStack>();
    MultiModelManagerAccess acc(man);
    acc.setMatrixStack(ms);
    man.SetShadersAndGeometry();
    auto camera = acc.getPtrCameraForTest();
    auto camViewMatrix = camera->getViewGlmMatrixdv();
    auto camProjMatrix = camera->getProjGlmMatrixdv();
    ASSERT_EQ(camViewMatrix,ms->getViewGlmMatrixdv());
    ASSERT_EQ(camProjMatrix,ms->getProjGlmMatrixdv());
}
TEST(MultiModelManager,MatrixStackKnowsEachModelMatrix_inDraw3d)
{
    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();
    auto model_3 = make_shared<OneModelMock>();
    MultiModelManager man(nullptr);
    man.setModels(vector<spOneModel> {model_1,model_2,model_3});
    spMatrixStackMock ms = make_shared<MatrixStackMock>();
    MultiModelManagerAccess acc(man);
    acc.setMatrixStack(ms);
    man.Draw3d();
    stack<glm::dmat4 *> modelMatrices = ms->getStackGivenModelMatricesRef();
    ASSERT_EQ(model_3->getModelGlmMatrixdv(),modelMatrices.top());
    modelMatrices.pop();
    ASSERT_EQ(model_2->getModelGlmMatrixdv(),modelMatrices.top());
    modelMatrices.pop();
    ASSERT_EQ(model_1->getModelGlmMatrixdv(),modelMatrices.top());
}
TEST(MultiModelManager,UpdateMatricesMultiplingByEachModel)
{
    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();

    auto sum_mat = [](const double * m) {
        float s = 0;
        for(short i = 0; i < 16 ; i++)
            s += m[i];
        return s;
    };
    model_1->Rotate(23, {0,1,1});
    model_2->Rotate(22, {1,0,1});
    ASSERT_NE(sum_mat(model_1->getModelMatrixdv()),sum_mat(model_2->getModelMatrixdv()));
    spMatrixStack ms = make_shared<MatrixStack>();
    bool need = true;
    double matrix[] = {1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,0,1,
                      };
//    ms->setViewMatrixdv(matrix,&need); //deleted
    ms->setProjectionMatrixdv(matrix,&need);
    MultiModelManager man(nullptr);
    MultiModelManagerAccess acc(man);
    spOglRendererMock rend = make_shared<OglRendererMock>();
    acc.setTexRenderer(rend);
    acc.setMatrixStack(ms);
    acc.setMatricesForTexRender();
    man.setModels(vector<spOneModel> {model_1,model_2});

    man.Draw3d();

    auto sum = rend->getSumsOfmatMVP();
    ASSERT_EQ(2,sum.size());
    float sum2 = sum.top();
    sum.pop();
    float sum1 = sum.top();
    cout<<sum1<<", "<<sum2;
    ASSERT_NE(sum2,sum1);
}
TEST(MultiModelManager,updateVWMatrixInStack_OnMouseRotDragging)
{
    auto ms = make_shared<MatrixStack>();
    MultiModelManager man(nullptr);

    MultiModelManagerAccess acc(man);
    acc.setMatrixStack(ms);

    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();
    man.setModels(vector<spOneModel> {model_1,model_2});
    man.SetViewport(1,1,640,400);
    man.SetShadersAndGeometry();
    float msVW_1[16];
    float msVW_2[16];
    auto msVW = ms->getViewMatrixfv();

    man.OnMouseRotDragging(45,39);
    man.Draw3d();
    for(short i = 0 ; i < 16 ; i++)msVW_1[i] = msVW[i];

    man.OnMouseRotDragging(47,35);
    man.Draw3d();
    for(short i = 0 ; i < 16 ; i++)msVW_2[i] = msVW[i];

    bool notEqual = false;
    for(short i = 0 ; i < 16 ; i++) {
        if((msVW_1[i] - msVW_2[i]) != 0.0f) notEqual = true;
    }
    ASSERT_TRUE(notEqual);
}
TEST(MultiModelManager,contentOfMatrices)
{
    auto ms = make_shared<MatrixStack>();
    MultiModelManager man(nullptr);

    MultiModelManagerAccess acc(man);
    acc.setMatrixStack(ms);

    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();
    man.setModels(vector<spOneModel> {model_1,model_2});
    man.SetViewport(1,1,640,400);
    man.SetShadersAndGeometry();
    man.Draw3d();
    auto camMVP = acc.getPtrCameraForTest()->GetFloatMVP();
    bool camMVPnoZero = false;
    for(short i = 0 ; i < 16 ; i++) {
//        cout<<"\n"<<camMVP[i];
        if(camMVP[i] == 0.0f) camMVPnoZero = true;
    }
    ASSERT_FALSE(camMVPnoZero);

    auto msMVP = ms->getModelViewProjectionMatrixfv();
    bool msMVPnoZero = false;
    for(short i = 0 ; i < 16 ; i++) {
        if(msMVP[i] == 0.0f) msMVPnoZero = true;
    }
    ASSERT_FALSE(msMVPnoZero);

}
TEST(MultiModelManager,SwitchViewControl)
{
    MultiModelManager man(nullptr);
    MultiModelManagerAccess acc(man);
    spTransformable model = make_shared<Transformable>();
    acc.setSelectedTransformable(model);

    ASSERT_TRUE(acc.CameraDoesViewControl());
    man.SwitchViewControl();
    ASSERT_FALSE(acc.CameraDoesViewControl());
    man.SwitchViewControl();
    ASSERT_TRUE(acc.CameraDoesViewControl());
}
TEST(MultiModelManager,CameraViewControl_ifNoSelectedTransformable)
{
    MultiModelManager man(nullptr);
    MultiModelManagerAccess acc(man);
    spTransformable emptyModel;
    acc.setSelectedTransformable(emptyModel);
    acc.CameraDoesViewControl(true);
    man.SwitchViewControl();
    ASSERT_TRUE(acc.CameraDoesViewControl());
}
TEST(MultiModelManager,DLeftClickUsesDrawOfPickingRendererForEachModel)
{
    MultiModelManager man(nullptr);
    MultiModelManagerAccess acc(man);
    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();
    auto model_3 = make_shared<OneModelMock>();
    man.setModels(vector<spOneModel> {model_1,model_2,model_3});
    man.OnMouseLeftDClick(20, 40);
    auto renderer = acc.getModelSelecting()->getRenderer();

    ASSERT_EQ(3,renderer->StartCallCount());
}
TEST(MultiModelManager,SelectingMeetsPositionOfDLeftClick)
{
    MultiModelManager man(nullptr);
    MultiModelManagerAccess manAcc(man);
    SelectingTestAccess selAcc(*manAcc.getModelSelecting());
    man.OnMouseLeftDClick(21, 39);
    ASSERT_EQ(21,selAcc.getClickedPosX());
    ASSERT_EQ(39,selAcc.getClickedPosY());
}
TEST(MultiModelManager,SelectingKnowsCurrentViewDimension)
{
    MultiModelManager man(nullptr);
    MultiModelManagerAccess manAcc(man);
    SelectingTestAccess selAcc(*manAcc.getModelSelecting());
    man.SetViewport(0,0,356,239);
    ASSERT_EQ(356,selAcc.getWindowWidth());
    ASSERT_EQ(239,selAcc.getWindowHeight());
    man.SetViewport(0,0,346,219);
    ASSERT_EQ(346,selAcc.getWindowWidth());
    ASSERT_EQ(219,selAcc.getWindowHeight());
}
TEST(MultiModelManager,InitSelectingOnSetShaders)
{
    AccessFileForTest enableAcc;
    MultiModelManager man(nullptr);
    MultiModelManagerAccess manAcc(man);
    SelectingTestAccess selAcc(*manAcc.getModelSelecting());
    man.SetShadersAndGeometry();
    ASSERT_TRUE(selAcc.Inited());
}
TEST(MultiModelManager,setSelectingResult_Background)
{
    SelectingResult result;//empty
    MultiModelManager man(nullptr);
    man.setSelectingResult(move(result));
    MultiModelManagerAccess manAcc(man);
    ASSERT_TRUE(manAcc.CameraDoesViewControl());
}
TEST(MultiModelManager,setSelectingResult_Background_consecutively)
{
    spOneModel model1 = make_shared<OneModel>();
    SelectingResult resultEmpty1;
    SelectingResult resultEmpty2;
    SelectingResult resultNotEmpty(model1);

    MultiModelManager man(nullptr);
    man.setModels( {model1});
    man.setSelectingResult(move(resultEmpty1));
    man.setSelectingResult(move(resultNotEmpty));
    man.setSelectingResult(move(resultEmpty2));
    MultiModelManagerAccess manAcc(man);
    ASSERT_TRUE(manAcc.CameraDoesViewControl());
}
TEST(MultiModelManager,setSelectingResult_NoCameraViewControl)
{
    spOneModel model1 = make_shared<OneModel>();
    SelectingResult result(model1);

    MultiModelManager man(nullptr);
    man.setModels( {model1});
    man.setSelectingResult(move(result));
    MultiModelManagerAccess manAcc(man);
    ASSERT_FALSE(manAcc.CameraDoesViewControl());
}
TEST(MultiModelManager,setSelectingResult_NoCameraViewControl_consecutively)
{
    spOneModel model1 = make_shared<OneModel>();
    spOneModel model2 = make_shared<OneModel>();
    SelectingResult resultM1(model1);
    SelectingResult resultEmpty;
    SelectingResult resultM2(model2);

    MultiModelManager man(nullptr);
    man.setModels( {model1,model2});
    man.setSelectingResult(move(resultM1));
    man.setSelectingResult(move(resultEmpty));
    man.setSelectingResult(move(resultM2));
    MultiModelManagerAccess manAcc(man);
    ASSERT_FALSE(manAcc.CameraDoesViewControl());
}
TEST(MultiModelManager,setSelectingResult_SelectedModelViewControl)
{
    spOneModel model1 = make_shared<OneModel>();
    spOneModel model2 = make_shared<OneModel>();
    SelectingResult resultM1(model1);
    SelectingResult resultEmpty;
    SelectingResult resultM2(model2);

    MultiModelManager man(nullptr);
    man.setModels( {model1,model2});
    man.setSelectingResult(move(resultM1));
    man.setSelectingResult(move(resultEmpty));
    man.setSelectingResult(move(resultM2));

    MultiModelManagerAccess manAcc(man);
    spTransformable selTrans= manAcc.getSelectedTransformable();
    OneModel * selectedModel = static_cast<OneModel *>(selTrans.get());
    ASSERT_EQ(selectedModel,model2.get());
}
TEST(MultiModelManager,setAndConfigureRenderSystem_activeShader)
{
    MultiModelManager man(nullptr);
    spRenderSystem rs = make_shared<OneTextureRenderSystem>();
    man.setAndConfigureRenderSystem(rs);
    auto adr1 = rs->getShader().get();
    auto adr2 = man.getActiveShader().get();
    ASSERT_EQ(adr1,adr2);
}
TEST(MultiModelManager,OnMouseLeftDClick_restoreAsActiveShaderFromSettedSystem)
{
    MultiModelManager man(nullptr);
    spRenderSystem rs = make_shared<OneTextureRenderSystem>();
    man.setAndConfigureRenderSystem(rs);
    auto adr1 = rs->getShader().get();
    man.OnMouseLeftDClick(3,4);
    auto adr2 = man.getActiveShader().get();
    ASSERT_EQ(adr1,adr2);
}

TEST(MultiModelManager,setAndConfigureRenderSystem_activeRenderer)
{
    MultiModelManager man(nullptr);
    spRenderSystem rs = make_shared<OneTextureRenderSystem>();
    man.setAndConfigureRenderSystem(rs);
    auto adr1 = rs->getRenderer().get();
    auto adr2 = man.getActiveRenderer().get();
    ASSERT_EQ(adr1,adr2);
}
TEST(MultiModelManager,OnMouseLeftDClick_restoreAsActiveRendererFromSettedSystem)
{
    MultiModelManager man(nullptr);
    spRenderSystem rs = make_shared<OneTextureRenderSystem>();
    man.setAndConfigureRenderSystem(rs);
    auto adr1 = rs->getRenderer().get();
    man.OnMouseLeftDClick(3,4);
    auto adr2 = man.getActiveRenderer().get();
    ASSERT_EQ(adr1,adr2);
}


TEST(MultiModelManager,ReloadedVaoForEach_ModelDataAndTextures)
{
    MultiModelManager man(nullptr);
    spOneModel model1 = make_shared<Triangle>();
    spOneModel model2 = make_shared<Triangle>();
    man.setModels( {model1,model2});

    spRenderSystemMock rsm = make_shared<RenderSystemMock>();

    man.setAndConfigureRenderSystem(rsm);

    ASSERT_TRUE(rsm->ReloadedVAOforModelData(model1->GetModelData()));
    ASSERT_TRUE(rsm->ReloadedVAOforTexture(*model1->MyTexture()));
    ASSERT_TRUE(rsm->ReloadedVAOforModelData(model2->GetModelData()));
    ASSERT_TRUE(rsm->ReloadedVAOforTexture(*model2->MyTexture()));
}
TEST(MultiModelManager,setAndConfigureRenderSystem_knownMatrixStack)
{
    MultiModelManager man(nullptr);
    MultiModelManagerAccess manAcc(man);

    spRenderSystem rs = make_shared<OneTextureRenderSystem>();

    man.setAndConfigureRenderSystem(rs);

    auto rs_matMVP = rs->getRenderer()->m_viewParamsfv.matMVP;
    auto ms_matMVP = manAcc.getMatrixStack()->getModelViewProjectionMatrixfv();
    auto rs_matToVw = rs->getRenderer()->m_viewParamsfv.matToVw;
    auto ms_matToVw = manAcc.getMatrixStack()->getViewMatrixfv();

    ASSERT_EQ(rs_matMVP, ms_matMVP);
    ASSERT_EQ(rs_matToVw, ms_matToVw);
}
TEST(MultiModelManager,setAndConfigureRenderSystem_knownLightSystem)
{
    MultiModelManager man(nullptr);
//    MultiModelManagerAccess manAcc(man);

    spRenderSystem rs = make_shared<OneTextureRenderSystem>();

    man.setAndConfigureRenderSystem(rs);

    auto rs_matlightPositon = rs->getRenderer()->m_viewParamsfv.light_position;
    auto ms_matlightPositon = man.getLightPtr()->GetFLightPos();
    auto rs_matlight_colour = rs->getRenderer()->m_viewParamsfv.light_colour;
    auto ms_matlight_colour = man.getLightPtr()->GetFLightColour();
//    m_matrices.light_position = light->GetFLightPos();
//    m_matrices.light_colour = light->GetFLightColour();

    ASSERT_EQ(rs_matlightPositon, ms_matlightPositon);
    ASSERT_EQ(rs_matlight_colour, ms_matlight_colour);
}
TEST(MultiModelManager,setAndConfigureRenderSystem_BuffersCreatedForModels)
{
    MultiModelManager man(nullptr);
    spOneModel model1 = make_shared<Triangle>();
    spOneModel model2 = make_shared<Triangle>();
    man.setModels( {model1,model2});

    spRenderSystem rs = make_shared<OneTextureRenderSystem>();

    man.setAndConfigureRenderSystem(rs);
    ASSERT_GT(model1->GetModelData().bufVertId,0);
    ASSERT_GT(model2->GetModelData().bufVertId,0);
}

stack<const float *> matricesFv_stack;
void glUniformMatrix4fv_ForTest(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    matricesFv_stack.push(value);
}
//useful
TEST(MultiModelManager,PointerToglUniformMatrix4fv)
{
//    auto glUniformMatrix4fv_Temp = glUniformMatrix4fv;
//    glUniformMatrix4fv = &glUniformMatrix4fv_ForTest;
//    auto& stack = matricesFv_stack;
//    auto model_1 = make_shared<OneModelMock>();
//    auto model_2 = make_shared<OneModelMock>();
//    MultiModelManager man(nullptr);
//    man.setModels(vector<spOneModel> {model_1,model_2});
//    man.Draw3d();
//    glUniformMatrix4fv = glUniformMatrix4fv_Temp;
//    auto sumOfMatrixElement = [](const float * mat){
//      float sum = 0;
//        for(short i = 0; i < 16 ; i++)
//      {
//          sum += mat[i];
//      }
//      return sum;
//    };
//    while(!stack.empty())
//    {
//        cout<<sumOfMatrixElement(stack.top())<<", ";
//        stack.pop();
//    }
//    ASSERT_EQ(2,3);
}
