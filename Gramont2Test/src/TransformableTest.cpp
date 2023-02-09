#include <gtest/gtest.h>
#include "transformable.h"
#include "onemodelmock.h"

TEST(Transformable,TranslateByVector)
{
    Transformable tran;
    glm::vec3 translateVector(2.7f, 4.1f, -0.4f);
    tran.Translate(translateVector);
    const double * modelMatrix = tran.getModelMatrixdv();
    ASSERT_FLOAT_EQ(modelMatrix[12],2.7);
    ASSERT_FLOAT_EQ(modelMatrix[13],4.1);
    ASSERT_FLOAT_EQ(modelMatrix[14],-0.4);
}
TEST(Transformable,Rotate)
{
    Transformable tran;
    const double * modelMatrix = tran.getModelMatrixdv(); 
//    cout<<"\n";
//    for(int i = 0; i < 16 ; i++)cout<<modelMatrix[i]<<", ";
    ASSERT_EQ(0.0,modelMatrix[1]);
    glm::vec3 rotateAxis(1.0f, 1.0f, 1.0f);
    float angle = 20.0f;
    tran.Rotate(angle,rotateAxis); 
    ASSERT_NE(0.0,modelMatrix[1]);
}
TEST(Transformable,OneModelMockHasIdentityModelMatrixAtBegin)
{
    OneModelMock model;
    const double * modMat = model.getModelMatrixdv();
    float expect[] = {1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1,};
    for(short r = 0; r < 16 ; r++)
    {
        ASSERT_EQ(expect[r],modMat[r]);
    }
}
TEST(Transformable,OneModelMockHasIdentityModelMatrixAtBegin_Double)
{
    OneModelMock model;
    const double * modMat = model.getModelMatrixdv();
    float expect[] = {1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1,};
    for(short r = 0; r < 16 ; r++)
    {
        ASSERT_EQ(expect[r],modMat[r]);
    }
}
TEST(Transformable,rotationQuatHasZeroesAtContruct)
{
    Transformable tr;
    glm::dquat expect(1.0,0.0,0.0,0.0);
    ASSERT_EQ(expect,tr.getQ_rotationVal());
}
TEST(Transformable,positionIn000AtConstruct)
{
    Transformable tr;
    glm::dvec3 expect(0.0,0.0,0.0);
    ASSERT_EQ(expect,tr.getGlmVecPosition());
}
