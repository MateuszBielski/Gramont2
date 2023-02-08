#include <gtest/gtest.h>
#include "matrixstack.h"
#include "cameratrial.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include "funkcje.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



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

