#include <gtest/gtest.h>
#include "surfaceonequad.h"
#include "surface.h"
#include "onemodelmock.h"

TEST(Surface,OneQuadNuPoints)
{
    shared_ptr<OneModel> quad1x1 = make_shared<SurfaceOneQuad>(100,100);
    ASSERT_EQ(4,quad1x1->GetModelData().nuPoints);
}
TEST(Surface,OneQuadPrimitive_GL_Triangle_strip)
{
    shared_ptr<OneModel> quad1x1 = make_shared<SurfaceOneQuad>(100,100);
    ASSERT_EQ(GL_TRIANGLE_STRIP,quad1x1->GetModelData().primitiveMode);
}

TEST(Surface,OneQuadDimensions)
{
    shared_ptr<OneModel> quad1x1 = make_shared<SurfaceOneQuad>(100,100);
    const float* verts = quad1x1->GetModelData().verts;
    ASSERT_EQ(-50.0,verts[0]);
    ASSERT_EQ(-50.0,verts[1]);
    ASSERT_EQ(50.0,verts[1*3]);
    ASSERT_EQ(-50.0,verts[1*3+1]);
    ASSERT_EQ(50.0,verts[2*3]);
    ASSERT_EQ(50.0,verts[2*3+1]);
    ASSERT_EQ(-50.0,verts[3*3]);
    ASSERT_EQ(50.0,verts[3*3+1]);
}
TEST(Surface,OneQuadNormals)
{
    shared_ptr<OneModel> quad1x1 = make_shared<SurfaceOneQuad>(100,100);
    const float* normals = quad1x1->GetModelData().normals;
    ASSERT_EQ(1.0,normals[0+2]);
    ASSERT_EQ(1.0,normals[1*3+2]);
    ASSERT_EQ(1.0,normals[2*3+2]);
    ASSERT_EQ(1.0,normals[3*3+2]);
}
//TEST(Surface,OneQuadColours)
//{
//    ASSERT_TRUE(false);
//}
TEST(Surface,OneQuadIndices)
{
    shared_ptr<OneModel> quad1x1 = make_shared<SurfaceOneQuad>(100,100);
    const GLuint* indices = quad1x1->GetModelData().indices;
    int expect[] = {0,1,3,2};
    for(short i = 0; i < 4 ; i++)ASSERT_EQ(expect[i],indices[i]);
}
TEST(Surface,NuPoints)
{
    shared_ptr<OneModel> quad1x1 = make_shared<Surface>(8,8,100,100);
    ASSERT_EQ(81,quad1x1->GetModelData().nuPoints);
}
TEST(Surface,DeletingVertDataOnDestructor)
{
    shared_ptr<OneModel> quad5x2 = make_shared<Surface>(5,2,100,100);
    const float*& verts = quad5x2->GetModelData().verts;
    quad5x2.reset();
    ASSERT_EQ(nullptr,verts);
}
TEST(Surface,DeletingNormalsOnDestructor)
{
    shared_ptr<OneModel> quad5x2 = make_shared<Surface>(5,2,100,100);
    const float*& normals = quad5x2->GetModelData().normals;
    quad5x2.reset();
    ASSERT_EQ(nullptr,normals);
}
TEST(Surface,DeletingColoursOnDestructor)
{
    shared_ptr<OneModel> quad50x20 = make_shared<Surface>(50,20,100,100);
    const float*& colours = quad50x20->GetModelData().colours;
    quad50x20.reset();
    ASSERT_EQ(nullptr,colours);
}
TEST(Surface,DeletingIndicesDataOnDestructor)
{
    shared_ptr<OneModel> quad50x20 = make_shared<Surface>(50,20,100,100);
    const GLuint*& indices = quad50x20->GetModelData().indices;
    quad50x20.reset();
    ASSERT_EQ(nullptr,indices);
}
TEST(Surface,CornerVerticesCoord)
{
    shared_ptr<OneModel> quad5x2 = make_shared<Surface>(5,2,100,100);
    const float* verts = quad5x2->GetModelData().verts;
    //corners num: points on bottom edge, from left. Next upper edge from left to right, next up to top edge.
    //0 left bottom
    ASSERT_EQ(-50.0,verts[0]);
    ASSERT_EQ(-50.0,verts[1]);
    //5 right bottom
    ASSERT_EQ(50.0,verts[5*3]);
    ASSERT_EQ(-50.0,verts[5*3+1]);
    //12 left top
    ASSERT_EQ(-50.0,verts[12*3]);
    ASSERT_EQ(50.0,verts[12*3+1]);
    //right top 17
    ASSERT_EQ(50.0,verts[17*3]);
    ASSERT_EQ(50.0,verts[17*3+1]);
}
TEST(Surface,IndicesOrder)
{
    shared_ptr<OneModel> quad5x2 = make_shared<Surface>(5,2,100,100);
    const GLuint* indices = quad5x2->GetModelData().indices;
    ASSERT_EQ(26,quad5x2->GetModelData().nuIndices);
    int expect[] = {0,6,1,7,2,8,3,9,4,10,5,11,11,6,6,12,7,13,8,14,9,15,10,16,11,17};
    for(short i = 0; i < 18 ; i++)ASSERT_EQ(expect[i],indices[i]);
}
TEST(Surface,IndicesOrder_1x1)
{
    shared_ptr<OneModel> quad1x1 = make_shared<Surface>(1,1,100,100);
    const GLuint* indices = quad1x1->GetModelData().indices;
    int expect[] = {0,2,1,3};
    for(short i = 0; i < 4 ; i++)ASSERT_EQ(expect[i],indices[i]);
}
TEST(Surface,NuNormals2x2)
{
    shared_ptr<OneModel> quad2x2 = make_shared<Surface>(2,2,100,100);
    ASSERT_EQ(9,quad2x2->GetModelData().nuNormals);
}
TEST(Surface,NuNormalsAlongZ_axis)
{
    shared_ptr<OneModel> quad2x2 = make_shared<Surface>(2,2,100,100);
    const float* normals = quad2x2->GetModelData().normals;
//    for(int i = 0 ; i < 3 * 4 ; i++) cout<<normals[i]<<" ";
    ASSERT_EQ(1,normals[3*0 + 2]);
    ASSERT_EQ(1,normals[3*1 + 2]);
    ASSERT_EQ(1,normals[3*2 + 2]);
    ASSERT_EQ(1,normals[3*3 + 2]);
}
TEST(Surface,NuColours)
{
    shared_ptr<OneModel> quad2x2 = make_shared<Surface>(1,1,100,100);
    ASSERT_EQ(4,quad2x2->GetModelData().nuColours);//two triangles on each quad
}
TEST(Surface,IndicesAdjacentToPoint_CornerLeftBottom_Size)
{
    Surface quad1x1(1,1,100,100);
    ASSERT_EQ(2,quad1x1.IndicesAdjacentToPoint(0).size());
}
TEST(Surface,IndicesAdjacentToPoint_CornerLeftBottom_Indices)
{
    Surface quad5x5(5,5,100,100);
    vector<GLuint> expected= {1,6};
    ASSERT_EQ(expected,quad5x5.IndicesAdjacentToPoint(0));
}
TEST(Surface,IndicesAdjacentToPoint_CornerRightBottom_Indices)
{
    Surface quad5x5(5,5,100,100);
    vector<GLuint> expected= {11,4};
    ASSERT_EQ(expected,quad5x5.IndicesAdjacentToPoint(5));
}
TEST(Surface,IndicesAdjacentToPoint_CornerLeftTop_Indices)
{
    Surface quad4x4(4,4,100,100);
    vector<GLuint> expected= {15,21};
    ASSERT_EQ(expected,quad4x4.IndicesAdjacentToPoint(20));
}
TEST(Surface,IndicesAdjacentToPoint_RightEdge_Indices)
{
    Surface quad3x2(3,2,100,100);
    vector<GLuint> expected= {11,6,3};
    ASSERT_EQ(expected,quad3x2.IndicesAdjacentToPoint(7));
}
TEST(Surface,IndicesAdjacentToPoint_Internal_Indices)
{
    Surface quad4x4(4,4,100,100);
    vector<GLuint> expected= { 8, 12, 6, 2 };
    ASSERT_EQ(expected,quad4x4.IndicesAdjacentToPoint(7));
}
TEST(Surface,ResultantNormalOnePoint_001)
{
    float verts[] = {
        0.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f
    };
    vector<GLuint> adjacent= { 1, 2};
    glm::vec3 expected(0.0f,0.0f,1.0f);
    Surface surf(1,1,100,100);
    ASSERT_EQ(expected,surf.ResultantNormalOnePoint(0,adjacent,verts));
}
TEST(Surface,ResultantNormalOnePoint_Ynegative)
{
    float verts[] = {
        0.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.5f
    };
    vector<GLuint> adjacent= { 1, 2};
//    glm::vec3 expected(0.0f,0.0f,1.0f);
    Surface surf(1,1,100,100);
    ASSERT_LT(surf.ResultantNormalOnePoint(0,adjacent,verts).y,0.0f);
}
TEST(Surface,ResultantNormalOnePoint_normalized)
{
    float verts[] = {
        0.0f,0.0f,13.0f,
        1.0f,0.0f,-2.0f,
        9.0f,1.0f,0.5f
    };
    vector<GLuint> adjacent= { 1, 2};
//    glm::vec3 expected(0.0f,0.0f,1.0f);
    Surface surf(1,1,100,100);
    ASSERT_EQ(1.0f,glm::length(surf.ResultantNormalOnePoint(0,adjacent,verts)));
}
TEST(Surface,ResultantNormalOnePoint_partialEqalOnFlat)
{
    float verts[] = {
        0.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        1.0f,1.0f,0.0f
    };
    vector<GLuint> adjacent13= { 1, 3};
    vector<GLuint> adjacent32= { 3, 2};
    vector<GLuint> adjacent132= { 1, 3, 2};
//    glm::vec3 expected(0.0f,0.0f,1.0f);
    Surface surf(1,1,100,100);
    auto norm13 = surf.ResultantNormalOnePoint(0,adjacent13,verts);
    auto norm32 = surf.ResultantNormalOnePoint(0,adjacent32,verts);
    auto norm132 = surf.ResultantNormalOnePoint(0,adjacent132,verts);
    ASSERT_EQ(norm13,norm32);
    ASSERT_EQ(norm132,norm32);
}
TEST(Surface,ResultantNormalOnePoint_partialNotEqalOnConvex)
{
    float verts[] = {
        0.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,
        1.0f,1.0f,0.6f // <---- Z
    };
    vector<GLuint> adjacent13= { 1, 3};
    vector<GLuint> adjacent32= { 3, 2};
    vector<GLuint> adjacent132= { 1, 3, 2};
//    glm::vec3 expected(0.0f,0.0f,1.0f);
    Surface surf(1,1,100,100);
    auto norm13 = surf.ResultantNormalOnePoint(0,adjacent13,verts);
    auto norm32 = surf.ResultantNormalOnePoint(0,adjacent32,verts);
    auto norm132 = surf.ResultantNormalOnePoint(0,adjacent132,verts);
    ASSERT_NE(norm13,norm32);
    ASSERT_NE(norm132,norm32);
}
TEST(Surface,SetVerticesForOnePoint)
{
    Surface surf(4,4,100,100);
    surf.SetZcoordinateForOnePoint(6,20.0f);
    ASSERT_EQ(20.0f,surf.GetModelData().verts[6 * 3 + 2]);
}
TEST(Surface,ResultantNormalAllPoints)
{
    Surface surf(3,3,100,100);
    surf.SetZcoordinateForOnePoint(6,20.0f);
    surf.CalculateResultantNormalForAllPoints();
    ASSERT_NE(1.0f,surf.GetModelData().normals[10 * 3 + 2]);
    int c = 1;
}
TEST(Surface,TexCoordCorners)
{
    Surface surf(3,3,100,100);
    const GLfloat * texCoord = surf.MyTexture()->texCoord;
    int cornInd[] = {0,3,12,15};
//    for(short i = 0 ; i < 16 ; i++)
//    {
//        cout<<"\n"<<texCoord[i * 2]<<", "<<texCoord[i * 2 + 1];
//    }
    GLfloat expCoord_x[] = {0,1,0,1};
    GLfloat expCoord_y[] = {0,0,1,1};
    for(short i = 0 ; i < 4 ; i++) {
        short n = cornInd[i];
        ASSERT_EQ(texCoord[n * 2],expCoord_x[i]);
        ASSERT_EQ(texCoord[n * 2 + 1],expCoord_y[i]);
    }

}
TEST(Surface,TexCoordExampleInner)
{
    Surface surf(4,4,100,100);
    const GLfloat * texCoord = surf.MyTexture()->texCoord;
    ASSERT_EQ(texCoord[18 * 2],0.75);
    ASSERT_EQ(texCoord[18 * 2 + 1],0.75);
}
TEST(Surface,nuTexCoord)
{
    Surface surf(7,3,100,100);
    ASSERT_EQ(surf.MyTexture()->nuTexCoord,32);
}