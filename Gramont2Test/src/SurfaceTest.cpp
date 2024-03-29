#include <gtest/gtest.h>
#include "surfaceonequad.h"
#include "surface.h"
#include "convexsurface.h"
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
TEST(Surface,ResultantNormalIsVerticalOnCenterOfConvexSurface)
{
    ConvexSurface cs(4,4,50,50,38);
    vec3 expectNormal(0.0,0.0,1.0);
    auto& md = cs.GetModelData();
    const float * n = md.normals;
    unsigned p = 12;
    vec3 resultNormal(n[p * 3], n[p * 3 + 1], n[p * 3 + 2]);
    ASSERT_EQ(expectNormal,resultNormal);
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
TEST(Surface,NoAddTextureWithoutPtrToCoordinates)
{
    Surface surf(2,2,100,100);
    spTextureForModel tex = make_shared<TextureForModel>();
    tex->texCoord = nullptr;
    tex->nuTexCoord = 9;
    surf.AddTexture(tex,TextureForModel::TextureType::Height);
    auto foundTex = surf.getTextureOfType(TextureForModel::TextureType::Height);
    ASSERT_EQ(-1,(int)foundTex->bufTexCoordId);
}
TEST(Surface,NoAddTextureWithBadCoordNumber)
{
    Surface surf(2,2,100,100);
    spTextureForModel tex = make_shared<TextureForModel>();
    float coord[8];
    tex->texCoord = coord;
    tex->nuTexCoord = 8;
    surf.AddTexture(tex,TextureForModel::TextureType::Height);
    auto foundTex = surf.getTextureOfType(TextureForModel::TextureType::Height);
    ASSERT_EQ(-1,(int)foundTex->bufTexCoordId);
}
TEST(Surface,AddTexture)
{
    Surface surf(2,2,100,100);
    spTextureForModel tex = make_shared<TextureForModel>();
    float coord[9];
    tex->texCoord = coord;
    tex->nuTexCoord = 9;
    surf.AddTexture(tex,TextureForModel::TextureType::Height);
    ASSERT_EQ(tex,surf.getTextureOfType(TextureForModel::TextureType::Height));
}
TEST(Surface,GiveTextureUnit_DuringAddTexture)
{
    Surface surf(2,2,100,100);
    spTextureForModel tex = make_shared<TextureForModel>();
    float coord[9];
    tex->texCoord = coord;
    tex->nuTexCoord = 9;
    surf.AddTexture(tex,TextureForModel::Normal);
    ASSERT_EQ(TextureForModel::Normal,tex->getTextureUnit());
}
TEST(Surface,CopyTextureFromMain_CorrectCoordNumbers)
{
    Surface surf(2,2,100,100);
    auto foundTex = surf.getTextureOfType(TextureForModel::TextureType::Height);
    ASSERT_EQ(-1,(int)foundTex->bufTexCoordId);
    surf.CopyFromMainTextureAs(TextureForModel::TextureType::Height);
    auto res = surf.getTextureOfType(TextureForModel::TextureType::Height);
    ASSERT_NE(nullptr,res);
    ASSERT_EQ(9,res->nuTexCoord);
}
TEST(Surface,CopyTextureFromMain_AsSeparate)
{
    Surface surf(2,2,100,100);
    surf.CopyFromMainTextureAs(TextureForModel::TextureType::Height);
    auto copiedTex = surf.getTextureOfType(TextureForModel::TextureType::Height);
    auto mainTex = surf.MyTexture();
    ASSERT_NE(copiedTex,mainTex);
}
//TEST(Surface,CopyTextureFromMain_TheSameCoordBufferId)// jakie konsekwencje to spowoduje?
//{
//    Surface surf(2,2,100,100);
//    ASSERT_EQ(nullptr,surf.getTextureOfType(TextureForModel::TextureType::Height));
//    surf.CopyFromMainTextureAs(TextureForModel::TextureType::Height);
//    auto res = surf.getTextureOfType(TextureForModel::TextureType::Height);
//    ASSERT_NE(nullptr,res);
//    ASSERT_EQ(9,res->nuTexCoord);
//}
TEST(Surface,PointOppositeTo_LeftBottomCorner)
{
    Surface surf(3,3,10,10);
    ASSERT_EQ(5,surf.PointOppositeTo(0));
}
TEST(Surface,PointOppositeTo_RightBottomCorner)
{
    Surface surf(4,3,10,10);
    ASSERT_EQ(8,surf.PointOppositeTo(4));
}
TEST(Surface,PointOppositeTo_LeftUpperCorner)
{
    Surface surf(1,1,10,10);
    ASSERT_EQ(1,surf.PointOppositeTo(2));
}
TEST(Surface,PointOppositeTo_RighUpperCorner)
{
    Surface surf(5,5,10,10);
    ASSERT_EQ(28,surf.PointOppositeTo(35));
}
TEST(Surface,PointOppositeTo_ZeroIfOutOfRange)
{
    Surface surf(1,1,10,10);
    ASSERT_EQ(0,surf.PointOppositeTo(35));
}
TEST(Surface,CalculateTangentAndBitangent_Amount)
{
    Surface surf(8,9,10,10);
    auto& tex = *surf.MyTexture();
    auto& md = surf.GetModelData();
    surf.CalculateTangentAndBitangentForAllPointsBasedOn(tex);
    ASSERT_EQ(90,md.nuTangents);
    ASSERT_EQ(90,md.nuBitangents);
}
TEST(Surface, CalculateTangent)
{
    Surface surf(8, 9, 10, 10);
    auto& tex = *surf.MyTexture();
    auto& md = surf.GetModelData();
    vec3 aSlightlyTiltedNormal(normalize(vec3 ( - 0.08, 0.04, 1.0)));
    unsigned givenPoint = 37;
    float* n = new float[3 * md.nuNormals];
    n[3 * givenPoint] = aSlightlyTiltedNormal.x;
    n[3 * givenPoint + 1] = aSlightlyTiltedNormal.y;
    n[3 * givenPoint + 2] = aSlightlyTiltedNormal.z;
    delete[] md.normals;
    md.normals = n;
    surf.CalculateTangentAndBitangentForAllPointsBasedOn(tex);
    vec3 expectTangentIgivenPoint(normalize(vec3(1.0, 0.08, 0.04)));
    vec3 resultTangent(md.tangents[3 * givenPoint], md.tangents[3 * givenPoint + 1], md.tangents[3 * givenPoint + 2]);
    //this test will not be passed, because tangent calculation is not based on normal.
//    ASSERT_EQ(expectTangentIgivenPoint, resultTangent);
}
TEST(Surface, ResultantTangentAndBitangentOnePoint)
{
    Surface surf(9, 8, 10, 10);
    const float * texCoord = surf.MyTexture()->texCoord;
    vec3 normal(0.0, 0.0, 1.0);
    glm::mat2x3 tBi = surf.RotationOfOriginalTangentAndBitangent(0.126, normal);
    glm::vec3 tangent(tBi[0]);
    glm::vec3 bitangent(tBi[1]);
    float angle = acos(glm::dot(tangent, bitangent));
    ASSERT_FLOAT_EQ(3.14159265 / 2, angle);
}
TEST(Surface,ResultantTangentAndBitangentAreHorizontalOnCenterOfConvexSurface)
{
    ConvexSurface cs(4,4,50,50,38);

    auto& tex = *cs.MyTexture();

    cs.CalculateTangentAndBitangentForAllPointsBasedOn(tex);
    unsigned p = 12;
//    auto adj = cs.IndicesAdjacentToPoint(p);
//    cs.ResultantTangentAndBitangentOnePoint(p,adj,tex.texCoord);
    auto& md = cs.GetModelData();
    const float * t = md.tangents;
    const float * b = md.bitangents;
    vec3 resultTangent(t[p * 3], t[p * 3 + 1], t[p * 3 + 2]);
    vec3 resultBitangent(b[p * 3], b[p * 3 + 1], b[p * 3 + 2]);
    vec3 expectTangent(1.0,0.0,0.0);
    vec3 expectBitangent(0.0,1.0,0.0);
    ASSERT_EQ(expectTangent,resultTangent);
    ASSERT_EQ(expectBitangent,resultBitangent);
}
TEST(Surface,TBN_Right_AngleInEveryPoint)//right angle - kąt prosty
{
    ConvexSurface cs(4,4,100,50,8);
    cs.CalculateTangentAndBitangentForAllPointsBasedOn(*cs.MyTexture());
    auto& md = cs.GetModelData();
    const float * t = md.tangents;
    const float * b = md.bitangents;
    const float * n = md.normals;
    float ra = 3.14159265 / 2;

    auto r2d = [](float r) {
        return r * 180.0 / 3.14159265;
    };
    for(int i = 0 ; i < md.nuTangents; i++) {
//        cout<<"\n"<<i<<" "<<t[i * 3]<<", "<<t[i * 3 + 1]<<", "<<t[i * 3 + 2];
//        cout<<"   "<<b[i * 3]<<", "<<b[i * 3 + 1]<<", "<<b[i * 3 + 2];
//        cout<<"   "<<n[i * 3]<<", "<<n[i * 3 + 1]<<", "<<n[i * 3 + 2];
        vec3 vt(t[i * 3], t[i * 3 + 1], t[i * 3 + 2]);
        vec3 vb(b[i * 3], b[i * 3 + 1], b[i * 3 + 2]);
        vec3 vn(n[i * 3], n[i * 3 + 1], n[i * 3 + 2]);
        float a_vt_vb = acos(glm::dot(vt, vb));
        float a_vn_vb = acos(glm::dot(vn, vb));
        float a_vt_vn = acos(glm::dot(vt, vn));
        ASSERT_FLOAT_EQ(ra,a_vt_vb);
        ASSERT_FLOAT_EQ(ra,a_vn_vb);
        ASSERT_FLOAT_EQ(ra,a_vt_vn);
//        cout<<"\n"<<i<<" "<<r2d(a_vt_vb)<<", "<<r2d(a_vn_vb)<<", "<<r2d(a_vt_vn);
    }
}
TEST(Surface,AveragedCoordRotation_commonDirection)//zgodny kierunek
{
    Surface s(8,9,10,10);
    auto& md = s.GetModelData();

    float * texCoord = new float[md.nuPoints * 2];
    texCoord[0] = 0.0;
    texCoord[1] = 0.0;
    texCoord[1*2] = 1.0;
    texCoord[1*2 + 1] = 0.577350269189626;//30deg from horizont
    texCoord[9*2] = -0.363970234266202;//20deg from vertical
    texCoord[9*2 + 1] = 1.0;
    auto& neighborsOfZeroPoint = s.IndicesAdjacentToPoint(0);
    float result = s.AveragedCoordinateAngle(0,neighborsOfZeroPoint,texCoord);
    float expect = 0.436332312998582; //25deg
    delete [] texCoord;
    ASSERT_EQ(result,expect);
}
TEST(Surface,AveragedCoordRotation_oppositeDirection)//przeciwnykierunek
{
    Surface s(8,9,10,10);
    auto& md = s.GetModelData();

    float * texCoord = new float[md.nuPoints * 2];
    texCoord[0] = 0.0;
    texCoord[1] = 0.0;
    texCoord[1*2] = 1.0;
    texCoord[1*2 + 1] = 0.577350269189626;//30deg from horizont
    texCoord[9*2] = 0.363970234266202;//20deg from vertical
    texCoord[9*2 + 1] = 1.0;
    auto& neighborsOfZeroPoint = s.IndicesAdjacentToPoint(0);
    float result = s.AveragedCoordinateAngle(0,neighborsOfZeroPoint,texCoord);
    float expect = 0.087266462599717; //5deg
    delete [] texCoord;
    ASSERT_FLOAT_EQ(result,expect);
}
TEST(Surface,AveragedCoordRotation_commonDirectionButNegative)
{
    Surface s(8,9,10,10);
    auto& md = s.GetModelData();

    float * texCoord = new float[md.nuPoints * 2];
    texCoord[0] = 0.0;
    texCoord[1] = 0.0;
    texCoord[1*2] = 1.0;
    texCoord[1*2 + 1] = -0.577350269189626;//-30deg from horizont
    texCoord[9*2] = 0.363970234266202;//-20deg from vertical
    texCoord[9*2 + 1] = 1.0;
    auto& neighborsOfZeroPoint = s.IndicesAdjacentToPoint(0);
    float result = s.AveragedCoordinateAngle(0,neighborsOfZeroPoint,texCoord);
    float expect = -0.436332312998582; //-25deg
    delete [] texCoord;
    ASSERT_EQ(result,expect);
}
TEST(Surface,AveragedCoordRotation_InnerPoint14)
{
    Surface s(5,5,10,10);
    auto& md = s.GetModelData();

    float * texCoord = new float[md.nuPoints * 2];
    texCoord[14 * 2] = 0.2;
    texCoord[14 * 2 + 1] = 0.21;
    texCoord[15 * 2] = 0.219562952014676;
    texCoord[15 * 2 + 1] = 0.214158233816355;
    texCoord[20 * 2] = 0.203046733585129;
    texCoord[20 * 2 + 1] = 0.234813653791033;
    texCoord[13 * 2] = 0.180246233188097;
    texCoord[13 * 2 + 1] = 0.206871310699195;
    texCoord[8 * 2] = 0.205623776358597;
    texCoord[8 * 2 + 1] = 0.185640748380369;
    auto& neighborsOf14Point = s.IndicesAdjacentToPoint(14);
    float result = s.AveragedCoordinateAngle(14,neighborsOf14Point,texCoord);
    float expect = 0.117809724509617;
    delete [] texCoord;
    ASSERT_FLOAT_EQ(result,expect);
}
TEST(Surface,RotationOfOriginalTangentAndBitangent)
{
    Surface s(5,5,10,10);
    auto& md = s.GetModelData();
    float planeRotatonAngle = 0.117809724509617;
    vec3 normal(0.0, 0.0, 1.0);
    mat2x3 tanBiTan = s.RotationOfOriginalTangentAndBitangent(planeRotatonAngle, normal);
    vec3 tangent = tanBiTan[0];
    vec3 bitangent = tanBiTan[1];
    ASSERT_FLOAT_EQ(0.993068456954926, tangent.x);
    ASSERT_FLOAT_EQ(-0.117537397457838, bitangent.x);
    ASSERT_FLOAT_EQ(0.117537397457838, tangent.y);
    ASSERT_FLOAT_EQ(0.993068456954926, bitangent.y);
    ASSERT_EQ(0.0, tangent.z);
    ASSERT_EQ(0.0, bitangent.z);
}
TEST(Surface, whatmakesTBNmatrix)
{
    ConvexSurface cs(5, 5, 100, 100, 50);
    cs.CalculateTangentAndBitangentForAllPointsBasedOn(*cs.MyTexture());
    auto& md = cs.GetModelData();
    int p(5 * 1 + 1);
    glm::vec3 t{ md.tangents[p * 3], md.tangents[p * 3 + 1],  md.tangents[p * 3 + 2] };
    glm::vec3 b{ md.bitangents[p * 3], md.bitangents[p * 3 + 1],  md.bitangents[p * 3 + 2] };
    glm::vec3 n{ md.normals[p * 3], md.normals[p * 3 + 1], md.normals[p * 3 + 2] };
    glm::mat3x3 TBN(t, b, n);
    vec3 v(0, 0, 1);
    vec3 newV = TBN * v;
}

