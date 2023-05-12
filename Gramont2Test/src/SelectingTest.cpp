#include <gtest/gtest.h>
#include "selectingtestaccess.h"
#include "selecting.h"
#include "glshadersmock.h"
#include "glFunctionsMock.h"
#include "shadersPath.h"
#include <stdio.h> //rename
#include "textfile.h" //AccessFileForTest
#include "selectable.h"

using namespace std;

TEST(Selecting,initializeShaderOnInit)
{
    AccessFileForTest enableAcc;
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.ConfigureShadersAndLocations();
    ASSERT_TRUE(shad_mock->InitUsed());
}
TEST(Selecting,addUniformsForShaderOnInit)
{
    AccessFileForTest enableAcc;
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.ConfigureShadersAndLocations();
    ASSERT_TRUE(shad_mock->hasUnif("modelUniqueId"));
    ASSERT_TRUE(shad_mock->hasUnif("mMVP"));
}
TEST(Selecting,addAttribForShaderOnInit)
{
    AccessFileForTest enableAcc;
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.ConfigureShadersAndLocations();
    ASSERT_TRUE(shad_mock->hasAttrib("position"));
}
TEST(Selecting,addCodeForShaderOnInit)
{
    AccessFileForTest enableAcc;
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.ConfigureShadersAndLocations();
    ASSERT_TRUE(shad_mock->hasCodeOfType(GL_VERTEX_SHADER));
    ASSERT_TRUE(shad_mock->hasCodeOfType(GL_FRAGMENT_SHADER));
}
TEST(Selecting,NoInitializeShaderIfShaderFilesNotExist)
{
    string pathVertexFile = d_vertexPickingShaderPath;
    string pathFragmentFile = d_fragmentPickingShaderPath;
    string newPathVertexFile = pathVertexFile+".noExist";
    string newPathFragmentFile = pathFragmentFile+".noExist";
    rename(pathVertexFile.c_str(),newPathVertexFile.c_str());
    rename(pathFragmentFile.c_str(),newPathFragmentFile.c_str());

    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.ConfigureShadersAndLocations();

    rename(newPathVertexFile.c_str(),pathVertexFile.c_str());
    rename(newPathFragmentFile.c_str(),pathFragmentFile.c_str());

    ASSERT_FALSE(shad_mock->InitUsed());
}
TEST(Selecting,LoadingShaders_incorrectPath)
{
    GlFunctionsMock funMock;
    funMock.Define();

    Selecting select;
    select.SetVertexShaderPath("vs");
    select.SetFragmentShaderPath("fs");
    select.ConfigureShadersAndLocations();
    SelectingTestAccess access(select);
    ASSERT_FALSE(access.ShadersLoaded());
}

TEST(Selecting,noCompilingShaders_ifNoShadersCode)
{
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.ConfigureShadersAndLocations();
    ASSERT_FALSE(shad_mock->UsingCompile());
}

TEST(Selecting,SelectableNotDistinguishableAtConstruct)
{
    Selectable sel1,sel2;
    ASSERT_EQ(sel1.getUniqueId(),sel2.getUniqueId());
}
TEST(Selecting,SelectableIsdistinguishableAfterRegister)
{
    spSelectable sel1 = make_shared<Selectable>();
    spSelectable sel2 = make_shared<Selectable>();
    Selecting select;
    select.RegisterSelectable( {sel1,sel2});
    ASSERT_NE(sel1->getUniqueId(),sel2->getUniqueId());
}
TEST(Selecting,PickingBuffLoaderKnowsShaderLocationAfterInit)
{
    AccessFileForTest enableAcc;
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shader = make_shared<glShadersMock>();
    shader->setAttribLoc("position",12);
    access.setShader(shader);

    select.ConfigureShadersAndLocations();

    ASSERT_EQ(12,select.getBufferLoader()->m_loc.position);
}
TEST(Selecting,needUpdateFrameBuffer_FalseAfterUpdatingFrameBuffer)
{
    Selecting select;
    SelectingTestAccess access(select);
    access.setFBO(3);
    GlFunctionsMock::framebufferStatus = GL_FRAMEBUFFER_COMPLETE;
    access.UpdateFrameBuffer();
    ASSERT_FALSE(access.needUpdateFrameBuffer());
}
TEST(Selecting,needUpdateFrameBufferFalseIfWindowSizeNotChanged)
{
    Selecting select;
    SelectingTestAccess access(select);
    select.setWindowSize(100,100);
    access.setNeedUpdateFrameBuffer(true);
    access.setFramebufferUpdated(true);
    select.setWindowSize(100,100);
    ASSERT_FALSE(access.needUpdateFrameBuffer());
}
TEST(Selecting,needUpdateFrameBufferTrue_IfWindowSizeNotChangedAndFramBufNotUpdated)
{
    Selecting select;
    SelectingTestAccess access(select);
    select.setWindowSize(100,100);
    access.setNeedUpdateFrameBuffer(true);
    access.setFramebufferUpdated(false);
    select.setWindowSize(100,100);
    ASSERT_TRUE(access.needUpdateFrameBuffer());
}
TEST(Selecting,frameBufferUpdatedFalseAtStart)
{
    Selecting select;
    SelectingTestAccess access(select);
    ASSERT_FALSE(access.FrameBufferUpdated());
}
TEST(Selecting,needUpdateFrameBufferTrueIfWindowSizeChanged)
{
    Selecting select;
    SelectingTestAccess access(select);
    select.setWindowSize(100,100);
    access.setNeedUpdateFrameBuffer(false);
    select.setWindowSize(100,130);
    ASSERT_TRUE(access.needUpdateFrameBuffer());
}
//TEST(Selecting,FrameBuffer_Not_Updated)
//{
//    Selecting select;
//    SelectingTestAccess access(select);
//}
TEST(Selecting,NoUpdatedFrameBuffer_In_EnableWritingToFrameBuffer)
{
    Selecting select;
    SelectingTestAccess access(select);
    access.setNeedUpdateFrameBuffer(false);
    select.EnableWritingToFrameBuffer();
    ASSERT_FALSE(access.FrameBufferUpdated());
}
TEST(Selecting,UpdatingFrameBuffer_In_EnableWritingToFrameBuffer)
{
    Selecting select;
    SelectingTestAccess access(select);
    access.setNeedUpdateFrameBuffer(true);
    access.setFBO(3);
    GlFunctionsMock::framebufferStatus = GL_FRAMEBUFFER_COMPLETE;
    select.EnableWritingToFrameBuffer();
    ASSERT_TRUE(access.FrameBufferUpdated());
}
TEST(Selecting,ReturnResult_SelectingNotDone)
{
    SelectingResult result;
    ASSERT_FALSE(result.selectingDone());
}
TEST(Selecting,ReturnResult_SelectingDone)
{
    SelectingResult result(make_shared<Selectable>());
    ASSERT_TRUE(result.selectingDone());
}
TEST(Selecting,selectingDone_False_IfModelNotRegistered)
{
    Selecting select;
    SelectingTestAccess access(select);
    access.setSelectedModelId(1);
    ASSERT_FALSE(select.getResult().selectingDone());
}
TEST(Selecting,selectingDoneIfModelRegistered)
{
    Selecting select;
    spSelectable sel1 = make_shared<Selectable>();
    spSelectable sel2 = make_shared<Selectable>();
    spSelectable sel3 = make_shared<Selectable>();
    select.RegisterSelectable( {sel1,sel2,sel3});
    SelectingTestAccess access(select);
    unsigned int givenId = sel3->getUniqueId();
    access.setSelectedModelId(givenId);
    ASSERT_EQ(givenId,select.getResult().getSelected()->getUniqueId());
}
TEST(Selecting,UpdateSelectedModelId_notAffectIfPixelInfoNotInitialized)
{
    Selecting select;
    SelectingTestAccess access(select);
    access.setSelectedModelId(2549);
    Selecting::PixelInfo pxi;
    select.UpdateSelectedModelId(pxi);
    ASSERT_EQ(2549,access.SelectedModelId());
}
TEST(Selecting,SelectedModelId_OneNegative_HitBackground099)
{
    Selecting select;
    SelectingTestAccess access(select);
    access.setSelectedModelId(2549);
    Selecting::PixelInfo pxi;
    pxi.ObjectID = 0.99;
    select.UpdateSelectedModelId(pxi);
    ASSERT_EQ(-1,access.SelectedModelId());
}
TEST(Selecting,SelectedModelId_OneNegative_HitBackground1_0)
{
    Selecting select;
    SelectingTestAccess access(select);
    access.setSelectedModelId(2549);
    Selecting::PixelInfo pxi;
    pxi.ObjectID = 1.0;
    select.UpdateSelectedModelId(pxi);
    ASSERT_EQ(-1,access.SelectedModelId());
}
TEST(Selecting,SelectedModelId_IncludeBackgroundCompensation_nearZeroRead)
{
    Selecting select;
    SelectingTestAccess access(select);
    access.setSelectedModelId(2549);
    Selecting::PixelInfo pxi;
    int compensation = BACKGROUND_COMPENSATION;
    pxi.ObjectID = 0.0009995 + (float)compensation;
    select.UpdateSelectedModelId(pxi);
    ASSERT_EQ(0,access.SelectedModelId());
}
TEST(Selecting,SelectedModelId_IncludeBackgroundCompensation_nearOneRead)
{
    Selecting select;
    SelectingTestAccess access(select);
    access.setSelectedModelId(2549);
    Selecting::PixelInfo pxi;
    int compensation = BACKGROUND_COMPENSATION;
    pxi.ObjectID = 0.993 + (float)compensation;
    select.UpdateSelectedModelId(pxi);
    ASSERT_EQ(1,access.SelectedModelId());
}
TEST(Selecting,ReadInClickedPosition)
{
    Selecting select;
    select.ReadInClickedPosition();
    
}
