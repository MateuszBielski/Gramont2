#include <gtest/gtest.h>
#include "matrixstack.h"
#include "cameratrial.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include "funkcje.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "textfile.h" //fileReadCount::statFileReadCount



TEST(MatrixStackTestNeedFix,Update_ModelViewProjectionMatrix_multiply)
{
    MatrixStack ms;
    double matrix_model[16];
    double matrix_view[16];
    double matrix_projection[16];
    float matrixExpect[16];
    FillMatricesWithRealData_1(matrix_model,matrix_view,matrix_projection,matrixExpect);
    const float * matrixResult;


    bool needUpdateM = true;
    bool needUpdateV = true;
    bool needUpdateP = true;

//    ms.setModelMatrixdv(matrix_model,&needUpdateM);
//    ms.setViewMatrixdv(matrix_view,&needUpdateV); //deleted
    ms.setProjectionMatrixdv(matrix_projection,&needUpdateP);
    matrixResult = ms.getModelViewProjectionMatrixfv();
    ms.UpdateMatrices();

    float expect, result;

    for(short u = 0; u < 16 ; u++) {
        result = round_to(matrixResult[u],4);
        expect = round_to(matrixExpect[u],4);

        ASSERT_FLOAT_EQ(expect,result);
    }
}

TEST(MatrixStackTestNeedFix,NeedUpdateTrueAfterSet_ViewMat)
{
    MatrixStack ms;

    bool needUpdateV = false;
    double matrix_view[16];

//    ms.setViewMatrixdv(matrix_view,&needUpdateV); //deleted

    ASSERT_TRUE(needUpdateV);
}

TEST(MatrixStackTestNeedFix,NeedUpdateFalse_afterUpdate)
{
    MatrixStack ms;
    bool needUpdateM = true;
    bool needUpdateV = true;
    bool needUpdateP = true;
    double matModel[16];
    double matView[16];
    double matProjection[16];
//    ms.setModelMatrixdv(matModel,&needUpdateM);
//    ms.setViewMatrixdv(matView,&needUpdateV); //deleted
    ms.setProjectionMatrixdv(matProjection,&needUpdateP);
    ms.UpdateMatrices();
    ASSERT_FALSE(needUpdateM);
    ASSERT_FALSE(needUpdateV);
    ASSERT_FALSE(needUpdateP);
}
TEST(MatrixStackTestNeedFix,Multiply_NoNeedUpdate)
{
    MatrixStack ms;
    double matrix_model[16];
    double matrix_view[16];
    double matrix_projection[16];
    float matrixExpect[16];
    FillMatricesWithRealData_1(matrix_model,matrix_view,matrix_projection,matrixExpect);
    const float * matrixResult;


    bool needUpdateM = true;
    bool needUpdateV = true;
    bool needUpdateP = true;

//    ms.setModelMatrixdv(matrix_model,&needUpdateM);
//    ms.setViewMatrixdv(matrix_view,&needUpdateV); //deleted
    ms.setProjectionMatrixdv(matrix_projection,&needUpdateP);
    matrixResult = ms.getModelViewProjectionMatrixfv();
    ms.UpdateMatrices();
    matrix_view[3] = 1.2;
    needUpdateV = false;

    ms.UpdateMatrices();
    float expect, result;

    for(short u = 0; u < 16 ; u++) {
        result = round_to(matrixResult[u],4);
        expect = round_to(matrixExpect[u],4);

        ASSERT_FLOAT_EQ(expect,result);
    }
}
TEST(MatrixStackTestNeedFix,ChangeProjection_noUpdates_VW)
{
    MatrixStack ms;
    double matrix_model[16];
    double matrix_view[16];
    double matrix_projection[16];
    float matrixExpect[16];
    FillMatricesWithRealData_1(matrix_model,matrix_view,matrix_projection,matrixExpect);
    const float * matrixResult;

    bool needUpdateM = true;
    bool needUpdateV = true;
    bool needUpdateP = true;

//    ms.setModelMatrixdv(matrix_model,&needUpdateM);
//    ms.setViewMatrixdv(matrix_view,&needUpdateV); //deleted
    ms.setProjectionMatrixdv(matrix_projection,&needUpdateP);
    matrixResult = ms.getModelViewProjectionMatrixfv();
    ms.UpdateMatrices();
    matrix_projection[3] = 1.3;
    needUpdateP = false;

    ms.UpdateMatrices();
    float expect, result;

    for(short u = 0; u < 16 ; u++) {
        result = round_to(matrixResult[u],4);
        expect = round_to(matrixExpect[u],4);
//        cout<<result<<", ";
        ASSERT_FLOAT_EQ(expect,result);
    }
}


TEST(MatrixStackTestNeedFix,ChangeView_Updates_MVP_VW)
{
    MatrixStack ms;
    double matrix_model[16];
    double matrix_view[16];
    double matrix_projection[16];
    float matrixExpect[16];
    FillMatricesWithRealData_1(matrix_model,matrix_view,matrix_projection,matrixExpect);
    float matrixResultMVP_1[16];
    float matrixResultMVP_2[16];
    float matrixResultVW_1[16];
    float matrixResultVW_2[16];

    bool needUpdateM = true;
    bool needUpdateV = true;
    bool needUpdateP = true;

//    ms.setModelMatrixdv(matrix_model,&needUpdateM);
//    ms.setViewMatrixdv(matrix_view,&needUpdateV); //deleted
    ms.setProjectionMatrixdv(matrix_projection,&needUpdateP);

    ms.UpdateMatrices();
    memcpy(matrixResultMVP_1,ms.getModelViewProjectionMatrixfv(),16 * sizeof(float));
    memcpy(matrixResultVW_1, ms.getViewMatrixfv(),16 * sizeof(float));

    matrix_view[2] = 1.5;
    needUpdateV = true;

    ms.UpdateMatrices();
    memcpy(matrixResultMVP_2,ms.getModelViewProjectionMatrixfv(),16 * sizeof(float));
    memcpy(matrixResultVW_2, ms.getViewMatrixfv(),16 * sizeof(float));

    bool notEqual = false;
    for(short u = 0; u < 16 ; u++) {
        if(matrixResultMVP_1[u] != matrixResultMVP_2[u])notEqual = true;
//        cout<<matrixResultMVP_1[u]<<", "<<matrixResultMVP_2[u]<<"\n";
    }
    ASSERT_TRUE(notEqual);
    notEqual = false;
    for(short u = 0; u < 16 ; u++) {
        if(matrixResultVW_1[u] != matrixResultVW_2[u])notEqual = true;
//        cout<<matrixResultVW_1[u]<<", "<<matrixResultVW_2[u]<<"\n";
    }
    ASSERT_TRUE(notEqual);
}
TEST(MatrixStackTestNeedFix,ChangeProjection_Updates_MVP)
{
    MatrixStack ms;
    double l = 1.3, h = 83.4;
    double matrix_model[16];
    CreateRandomMatrixTv(matrix_model,l,h);
    double matrix_view[16];
    CreateRandomMatrixTv(matrix_view,l,h);
    double matrix_projection[16];
    CreateRandomMatrixTv(matrix_projection,l,h);
    float matrixExpect[16];
//    FillMatricesWithRealData_1(matrix_model,matrix_view,matrix_projection,matrixExpect);
    float matrixResultMVP_1[16];
    float matrixResultMVP_2[16];
    float matrixResultVW_1[16];
    float matrixResultVW_2[16];

    bool needUpdateM = true;
    bool needUpdateV = true;
    bool needUpdateP = true;

//    ms.setModelMatrixdv(matrix_model,&needUpdateM);
//    ms.setViewMatrixdv(matrix_view,&needUpdateV); //deleted
    ms.setProjectionMatrixdv(matrix_projection,&needUpdateP);

    ms.UpdateMatrices();
    memcpy(matrixResultMVP_1,ms.getModelViewProjectionMatrixfv(),16 * sizeof(float));
    memcpy(matrixResultVW_1, ms.getViewMatrixfv(),16 * sizeof(float));
    auto matrixExpectVW_1glm =glm::make_mat4x4(matrix_view) * glm::make_mat4x4(matrix_model);
    auto matrixExpectMVP_1glm =glm::make_mat4x4(matrix_projection) * matrixExpectVW_1glm;
    double * matrixExpectVW_1fv = glm::value_ptr(matrixExpectVW_1glm);
    double * matrixExpectMVP_1fv = glm::value_ptr(matrixExpectMVP_1glm);

    bool notEqual = false;
    for(short u = 0; u < 16 ; u++) {
        if(round_to(matrixExpectVW_1fv[u],4) != round_to(matrixResultVW_1[u],4)) {
            
            notEqual = true;
//            cout<<"notEqual "<<matrixResultVW_1[u] - matrixExpectVW_1fv[u]<<"  ";
        }
//        cout<<matrixExpectVW_1fv[u]<<", "<<matrixResultVW_1[u]<<"\n";
    }
    ASSERT_EQ(-1,fileReadCount::statFileReadCount);
    ASSERT_FALSE(notEqual);
    notEqual = false;
    for(short u = 0; u < 16 ; u++) {
        if(round_to(matrixExpectMVP_1fv[u],4) != round_to(matrixResultMVP_1[u],4)) {
            notEqual = true;
        }
    }
    ASSERT_FALSE(notEqual);
    
    matrix_projection[2] = 1.5;
    needUpdateP = true;

    ms.UpdateMatrices();
    memcpy(matrixResultMVP_2,ms.getModelViewProjectionMatrixfv(),16 * sizeof(float));
    memcpy(matrixResultVW_2, ms.getViewMatrixfv(),16 * sizeof(float));
    auto matrixExpectMVP_2glm =glm::make_mat4x4(matrix_projection) * glm::make_mat4x4(matrix_view) * glm::make_mat4x4(matrix_model);
    double * matrixExpectMVP_2fv = glm::value_ptr(matrixExpectMVP_2glm);
    for(short u = 0; u < 16 ; u++) {
        if(round_to(matrixExpectMVP_2fv[u],4) != round_to(matrixResultMVP_2[u],4)) {
            notEqual = true;
        }
//        cout<<"\n"<<matrixExpectMVP_2fv[u]<<", "<<matrixResultMVP_2[u];
    }
    ASSERT_FALSE(notEqual);
}
TEST(GlFunctionsMock,StaticDefine_SuccesOnFirstDefine)
{
    GlFunctionsMock funMock;
    ASSERT_TRUE(funMock.Define());//test failed if static OneModelManager is used;
}
//outdated logic of shader_mock loading 
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
TEST(RenderSystem,ReloadVAO_LoadedBufferForTexture)
{
    OneTextureRenderSystem rs;
    spBufferLoaderMock blm = make_shared<BufferLoaderMock>();
    rs.setBufferLoader(blm);
    auto triangle = make_shared<Triangle>();
    ModelData& d = triangle->GetModelData();
    TextureForModel& t1 = *triangle->MyTexture();
    TextureForModel t2;
    rs.ConfigureShadersAndLocations();
    rs.ReloadVAO(triangle);
    //outdated logic
    ASSERT_FALSE(blm->LoadedBufferForTexture(t2));
    ASSERT_TRUE(blm->LoadedBufferForTexture(t1));
}
TEST(MultiModelManager,BuffersLoadedForEachModel_Geometry)
{
    MultiModelManager man(nullptr);
    spOneModel model1 = make_shared<Triangle>();
    spOneModel model2 = make_shared<Triangle>();
    man.setModels( {model1,model2});

    spRenderSystem rs = make_shared<OneTextureRenderSystem>();
    spBufferLoaderMock bl = make_shared<BufferLoaderMock>();
    rs->setBufferLoader(bl);

    man.setAndConfigureRenderSystem(rs);
//outdated logic
    ASSERT_TRUE(bl->LoadedBufferForModelGeometry(model1->GetModelData()));
    ASSERT_TRUE(bl->LoadedBufferForModelGeometry(model2->GetModelData()));

//    LoadBuffersForModelGeometry(d,vao);
//    m_BufferLoader->LoadBufferForTexture(tex,vao);
}