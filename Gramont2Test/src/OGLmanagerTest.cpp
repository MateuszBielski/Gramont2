#include <gtest/gtest.h>
#include "onemodelmanager.h"
#include "triangle.h"
//#include <glad/glad.h>
#include "glFunctionsMock.h"
#include "onemodelmock.h"
#include "multimodelmanager.h"
#include "MultiModelManagerAccess.h"
#include "glshadersmock.h"
#include "OglRendererMock.h"
#include <stack>


TEST(GlFunctionsMock,StaticDefine_SuccesOnFirstDefine)
{
    GlFunctionsMock funMock;
    ASSERT_TRUE(funMock.Define());//test failed if static OneModelManager is used;
}
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
TEST(MultiModelManager,SetShadersAndGeometry_LocationsForBufferLoaderSetted)
{
    auto model_1 = make_shared<OneModelMock>();
    spShadersMock shader = make_shared<glShadersMock>();
    shader->setAttribLoc("in_sPosition",49);
    shader->setAttribLoc("in_sNormal",53);
    shader->setAttribLoc("in_TextPos",12);
    MultiModelManager man(nullptr);
    MultiModelManagerAccess acc(man);
    acc.setTextureShaderForTest(shader);
    man.SetShadersAndGeometry();

    auto bufloader =  man.getBufferLoaderForTest();
    ASSERT_EQ(49,bufloader->m_loc.position_tex);
    ASSERT_EQ(53,bufloader->m_loc.normal_tex);
    ASSERT_EQ(12,bufloader->m_loc.textureCoord);
}
TEST(MultiModelManager,SetShadersAndGeometry_LocationsForRendererSetted)
{
    auto model_1 = make_shared<OneModelMock>();
    spShadersMock shader = make_shared<glShadersMock>();
    shader->setUnifLoc("mMVP",49);
    shader->setUnifLoc("mToViewSpace",53);
    shader->setUnifLoc("lightProps",12);
    shader->setUnifLoc("lightColour",17);
    shader->setUnifLoc("stringTexture",19);
    MultiModelManager man(nullptr);
    MultiModelManagerAccess acc(man);
    acc.setTextureShaderForTest(shader);
    man.SetShadersAndGeometry();

    auto renderer =  man.getTexRendererForTest();
    ASSERT_EQ(49,renderer->m_loc.mMVP);
    ASSERT_EQ(53,renderer->m_loc.mToViewSpace);
    ASSERT_EQ(12,renderer->m_loc.lightProps);
    ASSERT_EQ(17,renderer->m_loc.lightColour);
    ASSERT_EQ(19,renderer->m_loc.stringTexture);

}
TEST(MultiModelManager,SetShadersAndGeometry_EachModelKnowsPathToTextureImage)
{
    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();
    MultiModelManager man(nullptr);
    man.setModels(vector<spOneModel> {model_1,model_2});
    man.SetShadersAndGeometry();
    ASSERT_TRUE(model_1->MyTexture()->hasImagePath() && model_2->MyTexture()->hasImagePath());
}
TEST(MultiModelManager,EachModelDoesNotKnowPathToTextureImage_beforeSetShadersAndGeom)
{
    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();
    MultiModelManager man(nullptr);
    man.setModels(vector<spOneModel> {model_1,model_2});
    ASSERT_FALSE(model_1->MyTexture()->hasImagePath() && model_2->MyTexture()->hasImagePath());
}
TEST(MultiModelManager,SetShadersAndGeometry_LightSettedAndBindToRenderMatrix)
{
    myLight l;
    int i;
    for(i = 0; i < 4 ; i++)
        ASSERT_FLOAT_EQ(0.0f, l.GetFLightPos()[i]);
    MultiModelManager man(nullptr);
    man.SetShadersAndGeometry();
    auto renderer = man.getTexRendererForTest();
    auto position = renderer->m_matrices.light_position;
    ASSERT_NE(nullptr,position);
    auto colour = renderer->m_matrices.light_colour;
    ASSERT_NE(nullptr,colour);
    ASSERT_NE(0.0f, position[3]);//only intensity greater than 0
}
TEST(MultiModelManager,TextureAddShadersCode)
{
    MultiModelManager man(nullptr);
    spShadersMock shader = make_shared<glShadersMock>();
    MultiModelManagerAccess acc(man);
    acc.setTextureShaderForTest(shader);
    ASSERT_EQ(nullptr,acc.getTextureShaderCode());
    ASSERT_EQ(0,shader->getShaCode().size());
    man.SetShadersAndGeometry();
    ASSERT_NE(nullptr,acc.getTextureShaderCode());
    ASSERT_TRUE(shader->hasCodeOfType(GL_VERTEX_SHADER));
    ASSERT_TRUE(shader->hasCodeOfType(GL_FRAGMENT_SHADER));
}
TEST(MultiModelManager,TextureShadersAddUnif)
{
    MultiModelManager man(nullptr);
    spShadersMock shader = make_shared<glShadersMock>();
    MultiModelManagerAccess acc(man);
    acc.setTextureShaderForTest(shader);
    ASSERT_EQ(0,shader->getShaUnif().size());
    man.SetShadersAndGeometry();
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
    MultiModelManagerAccess acc(man);
    acc.setTextureShaderForTest(shader);
    ASSERT_EQ(0,shader->getShaAttrib().size());
    man.SetShadersAndGeometry();
    ASSERT_NE(0,shader->getShaAttrib().size());
    ASSERT_NE(0,shader->hasAttrib("in_sPosition"));
    ASSERT_NE(0,shader->hasAttrib("in_sNormal"));
    ASSERT_NE(0,shader->hasAttrib("in_TextPos"));
}

TEST(MultiModelManager,TextureShadersInit)
{
    MultiModelManager man(nullptr);
    spShadersMock shader = make_shared<glShadersMock>();
    MultiModelManagerAccess acc(man);
    acc.setTextureShaderForTest(shader);
    man.SetShadersAndGeometry();
    ASSERT_TRUE(shader->InitUsed());
}
TEST(MultiModelManager,CountLoadTextureBuffers)
{
    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();
    MultiModelManager man(nullptr);
    man.setModels(vector<spOneModel> {model_1,model_2});
    man.SetShadersAndGeometry();
    auto buffer = man.getBufferLoaderForTest();
    ASSERT_EQ(2,buffer->LoadTextureFailsCount());
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
    ASSERT_EQ(2,buffer->CreateBuffersCheckedCount());
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
    auto renderer = man.getTexRendererForTest();
    ASSERT_EQ(ms->getModelViewProjectionMatrixfv(),renderer->m_matrices.matMVP);
    ASSERT_EQ(ms->getViewMatrixfv(),renderer->m_matrices.matToVw);
}
TEST(MultiModelManager,MatrixStackKnowsCameraMatrices)
{
    MultiModelManager man(nullptr);
    spMatrixStack ms = make_shared<MatrixStack>();
    MultiModelManagerAccess acc(man);
    acc.setMatrixStack(ms);
    man.SetShadersAndGeometry();
    auto camera = acc.getPtrCameraForTest();
    auto camViewMatrix = camera->getViewMatrixdv();
    auto camProjMatrix = camera->getProjMatrixdv();
    ASSERT_EQ(camViewMatrix,ms->getViewMatrixdv());
    ASSERT_EQ(camProjMatrix,ms->getProjMatrixdv());
}
TEST(MultiModelManager,UpdateMatricesMultiplingByEachModel)
{
    auto model_1 = make_shared<OneModelMock>();
    auto model_2 = make_shared<OneModelMock>();

    auto sum_mat = [](const float * m) {
        float s = 0;
        for(short i = 0; i < 16 ; i++)
            s += m[i];
        return s;
    };
    model_1->Rotate(23, {0,1,1});
    model_2->Rotate(22, {1,0,1});
    ASSERT_NE(sum_mat(model_1->getModelMatrixfv()),sum_mat(model_2->getModelMatrixfv()));
    spMatrixStack ms = make_shared<MatrixStack>();
    bool need = true;
    double matrix[] = {1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,0,1,
                      };
    ms->setViewMatrixdv(matrix,&need);
    ms->setProjectionMatrixdv(matrix,&need);
    MultiModelManager man(nullptr);
    MultiModelManagerAccess acc(man);

    acc.setTexRenderer(make_unique<OglRendererMock>());
    acc.setMatrixStack(ms);
    acc.setMatricesForTexRender();
    man.setModels(vector<spOneModel> {model_1,model_2});

    man.Draw3d();

    upOglRendererMock rend = acc.getTexRenderer<OglRendererMock>();
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
    for(short i = 0 ; i < 16 ; i++)
    {
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
    for(short i = 0 ; i < 16 ; i++)
    {
//        cout<<"\n"<<camMVP[i];
        if(camMVP[i] == 0.0f) camMVPnoZero = true;
    }
    ASSERT_FALSE(camMVPnoZero);
    
    auto msMVP = ms->getModelViewProjectionMatrixfv();
    bool msMVPnoZero = false;
    for(short i = 0 ; i < 16 ; i++)
    {
        if(msMVP[i] == 0.0f) msMVPnoZero = true;
    }
    ASSERT_FALSE(msMVPnoZero);
    
}
//class functorGlUniformMatrix4fv
//    {
//
//    };
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