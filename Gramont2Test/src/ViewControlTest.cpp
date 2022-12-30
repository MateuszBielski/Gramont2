#include <gtest/gtest.h>
#include "matrixstack.h"
#include "cameratrial.h"
#include <sstream>
#include <iomanip>
#include <cmath>


TEST(MatrixStack,DefaultReturnIdentity)
{
    MatrixStack ms;
    const float * matrixResult = ms.getModelViewProjectionMatrixfv();
    float ident[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    for(short u = 0; u < 16 ; u++) {
        ASSERT_FLOAT_EQ(ident[u], matrixResult[u]);
    }
}
TEST(MatrixStack,Update_ReturnIdentity)
{
    MatrixStack ms;
    const float * matrixResult = ms.getModelViewProjectionMatrixfv();
    float ident[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    ms.UpdateMatrices();
    for(short u = 0; u < 16 ; u++) {
        ASSERT_FLOAT_EQ(ident[u], matrixResult[u]);
    }
}


float round_to(float r, int d)
{
    float decim = 10;
    for(short i = 1 ; i < d ; ++i)decim *= 10;
    return round(r*decim)/decim;
}

TEST(MatrixStack,Update_ModelViewProjectionMatrix_multiply)
{
    MatrixStack ms;
    double matrix_model[] = {0.9467833, -0.0096355, -0.3217274, 0.0000000, 0.0422298, 0.9946301, 0.0944857, 0.0000000, 0.3190893, -0.1030440, 0.9421061, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 1.0000000 };
    double matrix_view[] = {0.9659258, 0.0801606, 0.2460926, 0.0000000, 0.0000000, 0.9508289, -0.3097168, 0.0000000, -0.2588190, 0.2991634, 0.9184301, 0.0000000, 0.0000000, 0.0000000, -395.4403082, 1.0000000 };
    double matrix_projection[] = {1.8647538, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 2.7474774, 0.0000000, 0.0000000, 0.0000000, 0.0000000, -2.8301946, -1.0000000, 0.0000000, 0.0000000, -979.4510744, 0.0000000 };
    float matrixExpect[] = {1.8606356, -0.0810948, 0.1684064, 0.0595035, 0.0304628, 2.6853154, 0.5968390, 0.2108827, 0.1200559, 0.5754452, -2.7614169, -0.9756986, 0.0000000, 0.0000000, 139.7219543, 395.4403076};
    const float * matrixResult;

    
    bool needUpdateM = true;
    bool needUpdateV = true;
    bool needUpdateP = true;
    ms.setNeedUpdateModelMat(&needUpdateM);
    ms.setNeedUpdateViewMat(&needUpdateV);
    ms.setNeedUpdateProjectionMat(&needUpdateP);
    
    ms.setModelMatrixdv(matrix_model);
    ms.setViewMatrixdv(matrix_view);
    ms.setProjectionMatrixdv(matrix_projection);
    matrixResult = ms.getModelViewProjectionMatrixfv();
    ms.UpdateMatrices();

    float expect, result;

    for(short u = 0; u < 16 ; u++) {
        result = round_to(matrixResult[u],4);
        expect = round_to(matrixExpect[u],4);

        ASSERT_FLOAT_EQ(expect,result);
    }
}
TEST(MatrixStack,NeedUpdateTrueAfterSet_ModelMat)
{
    MatrixStack ms;
    double matrix_model[16];
    bool needUpdateM = false;

    ms.setNeedUpdateModelMat(&needUpdateM);
    ms.setModelMatrixdv(matrix_model);
    
    ASSERT_TRUE(needUpdateM);
}
TEST(MatrixStack,NeedUpdateTrueAfterSet_ViewMat)
{
    MatrixStack ms;

    bool needUpdateV = false;
    double matrix_view[16];
    
    ms.setNeedUpdateViewMat(&needUpdateV);
    ms.setViewMatrixdv(matrix_view);
    
    ASSERT_TRUE(needUpdateV);
}
TEST(MatrixStack,NeedUpdateTrueAfterSet_ProjectionMat)
{
     MatrixStack ms;

    bool needUpdateP = false;
    double matrix_projection[16];
    
    ms.setNeedUpdateProjectionMat(&needUpdateP);
    ms.setProjectionMatrixdv(matrix_projection);
    
    ASSERT_TRUE(needUpdateP);
}
TEST(MatrixStack,NeedUpdateFalse_afterUpdate)
{
    MatrixStack ms;
    bool needUpdateM = true;
    bool needUpdateV = true;
    bool needUpdateP = true;
    double matModel[16];
    double matView[16];
    double matProjection[16];
    ms.setNeedUpdateModelMat(&needUpdateM);
    ms.setNeedUpdateViewMat(&needUpdateV);
    ms.setNeedUpdateProjectionMat(&needUpdateP);
    ms.setModelMatrixdv(matModel);
    ms.setViewMatrixdv(matView);
    ms.setProjectionMatrixdv(matProjection);
    ms.UpdateMatrices();
    ASSERT_FALSE(needUpdateM);
    ASSERT_FALSE(needUpdateV);
    ASSERT_FALSE(needUpdateP);
}
TEST(MatrixStack,Multiply_NoNeedUpdate)
{
    MatrixStack ms;
    double matrix_model[] = {0.9467833, -0.0096355, -0.3217274, 0.0000000, 0.0422298, 0.9946301, 0.0944857, 0.0000000, 0.3190893, -0.1030440, 0.9421061, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 1.0000000 };
    double matrix_view[] = {0.9659258, 0.0801606, 0.2460926, 0.0000000, 0.0000000, 0.9508289, -0.3097168, 0.0000000, -0.2588190, 0.2991634, 0.9184301, 0.0000000, 0.0000000, 0.0000000, -395.4403082, 1.0000000 };
    double matrix_projection[] = {1.8647538, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 2.7474774, 0.0000000, 0.0000000, 0.0000000, 0.0000000, -2.8301946, -1.0000000, 0.0000000, 0.0000000, -979.4510744, 0.0000000 };
    float matrixExpect[] = {1.8606356, -0.0810948, 0.1684064, 0.0595035, 0.0304628, 2.6853154, 0.5968390, 0.2108827, 0.1200559, 0.5754452, -2.7614169, -0.9756986, 0.0000000, 0.0000000, 139.7219543, 395.4403076};
    const float * matrixResult;

    
    bool needUpdateM = true;
    bool needUpdateV = true;
    bool needUpdateP = true;
    ms.setNeedUpdateModelMat(&needUpdateM);
    ms.setNeedUpdateViewMat(&needUpdateV);
    ms.setNeedUpdateProjectionMat(&needUpdateP);
    
    ms.setModelMatrixdv(matrix_model);
    ms.setViewMatrixdv(matrix_view);
    ms.setProjectionMatrixdv(matrix_projection);
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
//TEST(MatrixStack,ChangeModel_Updates_MVP_VW)
//{
//    
//}
//TEST(MatrixStack,ChangeView_Updates_MVP_VW)
//TEST(MatrixStack,ChangeProjection_noUpdates_VW)
//TEST(MatrixStack,ChangeProjection_Updates_MVP)
