#include <gtest/gtest.h>
#include "matrixstack.h"
#include "cameratrial.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


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

void FillMatricesWithRealData_1(double * model,double * view, double * proj, float * expect)
{

    double matrix_model[] = {0.9467833, -0.0096355, -0.3217274, 0.0000000, 0.0422298, 0.9946301, 0.0944857, 0.0000000, 0.3190893, -0.1030440, 0.9421061, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 1.0000000 };
    double matrix_view[] = {0.9659258, 0.0801606, 0.2460926, 0.0000000, 0.0000000, 0.9508289, -0.3097168, 0.0000000, -0.2588190, 0.2991634, 0.9184301, 0.0000000, 0.0000000, 0.0000000, -395.4403082, 1.0000000 };
    double matrix_projection[] = {1.8647538, 0.0000000, 0.0000000, 0.0000000, 0.0000000, 2.7474774, 0.0000000, 0.0000000, 0.0000000, 0.0000000, -2.8301946, -1.0000000, 0.0000000, 0.0000000, -979.4510744, 0.0000000 };
    float matrixExpect[] = {1.8606356, -0.0810948, 0.1684064, 0.0595035, 0.0304628, 2.6853154, 0.5968390, 0.2108827, 0.1200559, 0.5754452, -2.7614169, -0.9756986, 0.0000000, 0.0000000, 139.7219543, 395.4403076};
    memcpy(model,matrix_model,sizeof(double)*16);
    memcpy(view,matrix_view,sizeof(double)*16);
    memcpy(proj,matrix_projection,sizeof(double)*16);
    memcpy(expect,matrixExpect,sizeof(float)*16);
}
TEST(MatrixStack,Update_ModelViewProjectionMatrix_multiply)
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

    ms.setModelMatrixdv(matrix_model,&needUpdateM);
    ms.setViewMatrixdv(matrix_view,&needUpdateV);
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
TEST(MatrixStack,NeedUpdateTrueAfterSet_ModelMat)
{
    MatrixStack ms;
    double matrix_model[16];
    bool needUpdateM = false;

    ms.setModelMatrixdv(matrix_model,&needUpdateM);

    ASSERT_TRUE(needUpdateM);
}
TEST(MatrixStack,NeedUpdateTrueAfterSet_ViewMat)
{
    MatrixStack ms;

    bool needUpdateV = false;
    double matrix_view[16];

    ms.setViewMatrixdv(matrix_view,&needUpdateV);

    ASSERT_TRUE(needUpdateV);
}
TEST(MatrixStack,NeedUpdateTrueAfterSet_ProjectionMat)
{
    MatrixStack ms;

    bool needUpdateP = false;
    double matrix_projection[16];

    ms.setProjectionMatrixdv(matrix_projection,&needUpdateP);

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
    ms.setModelMatrixdv(matModel,&needUpdateM);
    ms.setViewMatrixdv(matView,&needUpdateV);
    ms.setProjectionMatrixdv(matProjection,&needUpdateP);
    ms.UpdateMatrices();
    ASSERT_FALSE(needUpdateM);
    ASSERT_FALSE(needUpdateV);
    ASSERT_FALSE(needUpdateP);
}
TEST(MatrixStack,Multiply_NoNeedUpdate)
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

    ms.setModelMatrixdv(matrix_model,&needUpdateM);
    ms.setViewMatrixdv(matrix_view,&needUpdateV);
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
TEST(MatrixStack,ChangeProjection_noUpdates_VW)
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

    ms.setModelMatrixdv(matrix_model,&needUpdateM);
    ms.setViewMatrixdv(matrix_view,&needUpdateV);
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
TEST(MatrixStack,ChangeModel_Updates_MVP_VW)
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

    ms.setModelMatrixdv(matrix_model,&needUpdateM);
    ms.setViewMatrixdv(matrix_view,&needUpdateV);
    ms.setProjectionMatrixdv(matrix_projection,&needUpdateP);
    matrixResult = ms.getModelViewProjectionMatrixfv();
    ms.UpdateMatrices();
    matrix_model[3] = 1.3;
    needUpdateM = true;

    ms.UpdateMatrices();
    float expect, result;
    bool notEqual = false;
    for(short u = 0; u < 16 ; u++) {
        result = round_to(matrixResult[u],4);
        expect = round_to(matrixExpect[u],4);
        if(result != expect)notEqual = true;
//        cout<<result<<", "<<expect<<"\n";
//        ASSERT_NE(expect,result);
    }
    ASSERT_TRUE(notEqual);
}
template<typename T>
void CreateRandomMatrixTv(T * mat,T low,T high)
{
    int lowInt = (int)floor(low);
    int highInt = (int)ceil(high);
    T rangeInt = highInt - lowInt;
    T range = high - low;
    T ratio = range/rangeInt;
    for(short i = 0 ; i < 16 ; i++) {
        mat[i] = low + ratio * (rand() % (highInt - lowInt + 1 ));
//        cout<<"\n"<<mat[i];
    }
}
TEST(MatrixStack,CreateRandomMatrix)
{
    double matrix[16];
    CreateRandomMatrixTv(matrix,0.5d,240.0d);
    double first = matrix[0];
    bool outOfRange = false;
    bool equal = true;
    for(short i = 0 ; i < 16 ; i++) {
        outOfRange |= matrix[i] < 0.5;
        outOfRange |= matrix[i] > 240;
        equal &= first != matrix[i];
    }
    ASSERT_FALSE(outOfRange);
    ASSERT_FALSE(equal);
}

TEST(MatrixStack,MultiplicationGLM)
{
    double mat4dv_1[16];
    CreateRandomMatrixTv(mat4dv_1,-9.3d,21.43d);
    double mat4dv_2[16];
    CreateRandomMatrixTv(mat4dv_1,-8.3d,24.43d);
    auto mat1 = glm::make_mat4x4(mat4dv_1);
    auto mat2 = glm::make_mat4x4(mat4dv_2);
    auto matResult = mat1 * mat2;
    double * matResultdv = glm::value_ptr(matResult);
    double matExp[16];
    MyMatMul4x4(mat4dv_1, mat4dv_2, matExp);
    bool equal = true;
    for(short i = 0 ; i < 16 ; i++) {
        equal &= matResultdv[i] == matExp[i];
    }
    ASSERT_TRUE(equal);
}
TEST(MatrixStack,ChangeView_Updates_MVP_VW)
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

    ms.setModelMatrixdv(matrix_model,&needUpdateM);
    ms.setViewMatrixdv(matrix_view,&needUpdateV);
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
TEST(MatrixStack,ChangeProjection_Updates_MVP)
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

    ms.setModelMatrixdv(matrix_model,&needUpdateM);
    ms.setViewMatrixdv(matrix_view,&needUpdateV);
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
        cout<<"\n"<<matrixExpectMVP_2fv[u]<<", "<<matrixResultMVP_2[u];
    }
    ASSERT_FALSE(notEqual);
}
TEST(CameraTrial,needUpdateProjMat_After_ViewSizeChanged)
{
    CameraTrial cam;
    cam.needUpdateProjMat = false;
    cam.ViewSizeChanged(34,52);
    ASSERT_TRUE(cam.needUpdateProjMat);
}
