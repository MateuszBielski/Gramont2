#include <gtest/gtest.h>
#include "matrixstack.h"
#include "cameratrial.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include "funkcje.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vec_swizzle.hpp>


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

TEST(MatrixStack,NeedUpdateTrueAfterSet_ProjectionMat)
{
    MatrixStack ms;

    bool needUpdateP = false;
    double matrix_projection[16];

    ms.setProjectionMatrixdv(matrix_projection,&needUpdateP);

    ASSERT_TRUE(needUpdateP);
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

//    ms.setModelMatrixdv(matrix_model,&needUpdateM);
//    ms.setViewMatrixdv(matrix_view,&needUpdateV); //deleted
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
TEST(MatrixStack,ModelGlmMatrixUsedInStack)
{
    glm::dmat4x4 modelMat(1.0);
    glm::dmat4x4 viewMat(1.0);
    glm::dmat4x4 projMat(1.0);
    MatrixStack ms;
    ms.setModelGlmMatrixdv(&modelMat);
    ms.setViewGlmMatrixdv(&viewMat);
    ms.setProjectionGlmMatrixdv(&projMat);
    ms.UpdateMatrices();
    const float * matrixToView = ms.getViewMatrixfv();
    ASSERT_EQ(1.0,matrixToView[0]);
    ASSERT_EQ(1.0,matrixToView[5]);
    ASSERT_EQ(1.0,matrixToView[10]);
    ASSERT_EQ(1.0,matrixToView[15]);
}
TEST(CameraTrial,needUpdateProjMat_After_ViewSizeChanged)
{
    CameraTrial cam;
    cam.needUpdateProjMat = false;
    cam.ViewSizeChanged(34,52);
    ASSERT_TRUE(cam.needUpdateProjMat);
}
//to replace with CameraScreenPlane later
TEST(CameraTrial,DistanceGreatherThanZeroAtBegin)
{
    CameraTrial cam;
    ASSERT_GT(cam.getDistance(),0);
}
TEST(CameraTrial,ViewMoveHorizontal)
{
    CameraTrial cam;
    dmat4x4 * cameraViewMatrixfv = cam.getViewGlmMatrixdv();
    dvec3 point(1.2,1.2,1.2);
    dvec3 firstPointPosition = xyz(*cameraViewMatrixfv * dvec4(point,1.0));
    cam.MoveOnScreenPlane(50,50,55,50);
    dvec3 secondPointPosition = xyz(*cameraViewMatrixfv * dvec4(point,1.0));
    ASSERT_EQ(round_to(firstPointPosition.y,5),round_to(secondPointPosition.y,5));
    ASSERT_EQ(round_to(firstPointPosition.z,5),round_to(secondPointPosition.z,5));
    ASSERT_NE(round_to(firstPointPosition.x,5),round_to(secondPointPosition.x,5));
}
TEST(CameraTrial,ViewMoveVertical)
{
    CameraTrial cam;
    dmat4x4 * cameraViewMatrixfv = cam.getViewGlmMatrixdv();
    dvec3 point(1.2,1.2,1.2);
    dvec3 firstPointPosition = xyz(*cameraViewMatrixfv * dvec4(point,1.0));
    cam.MoveOnScreenPlane(50,50,50,45);
    dvec3 secondPointPosition = xyz(*cameraViewMatrixfv * dvec4(point,1.0));
    ASSERT_EQ(round_to(firstPointPosition.x,5),round_to(secondPointPosition.x,5));
    ASSERT_EQ(round_to(firstPointPosition.z,5),round_to(secondPointPosition.z,5));
    ASSERT_NE(round_to(firstPointPosition.y,5),round_to(secondPointPosition.y,5));
}
TEST(CameraTrial,rotationQuatHasZeroesAtContruct)
{
    CameraTrial cam;
    glm::dquat expect(1.0,0.0,0.0,0.0);
    ASSERT_EQ(expect,cam.getQ_rotationVal());
}

