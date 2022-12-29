#include <gtest/gtest.h>
#include "selectingtestaccess.h"
#include "selecting.h"
#include "glshadersmock.h"
#include "glFunctionsMock.h"

//
//TEST(Selecting,LoadingShaders_undefinedShaderPath)
//{
//    Selecting select();
//    select.Init();
//    SelectingTestAccess access(select);
//    ASSERT_FALSE(access.ShadersLoaded());
//}
TEST(Selecting,LoadingShaders_incorrectPath)
{
    GlFunctionsMock funMock;
    funMock.Define();
    
    Selecting select;
    select.SetVertexShaderPath("vs");
    select.SetFragmentShaderPath("fs");
    select.Init();
    SelectingTestAccess access(select);
    ASSERT_FALSE(access.ShadersLoaded());
}
TEST(Selecting,LoadingShaders_correctPath)
{
    Selecting select;
    select.SetVertexShaderPath("Gramont2Test/Resources/picking.vs");
    select.SetFragmentShaderPath("Gramont2Test/Resources/picking.fs");
    select.Init();
    SelectingTestAccess access(select);
    ASSERT_TRUE(access.ShadersLoaded());
}
TEST(Selecting,noCompilingShaders_ifNoShadersCode)
{
    Selecting select;
    SelectingTestAccess access(select);
    glShadersMock * shad_mock = new glShadersMock;
    access.setShadersMock(shad_mock);
    select.Init();
    ASSERT_FALSE(shad_mock->UsingCompile());
}
TEST(Selecting,CompilingShaders)
{
    Selecting select;
    SelectingTestAccess access(select);
    glShadersMock * shad_mock = new glShadersMock;
    access.setShadersMock(shad_mock);
    access.setShadersLoaded(true);
    select.Init();
    ASSERT_TRUE(shad_mock->UsingCompile());
}
//TEST(Selecting,LinkingShaders)
//TEST(Selecting,Uniforms)
//
//TEST(Selecting,Buffers)
//TEST(Selecting,Rendering)
TEST(Selecting,ReturnResult_SelectingNotDone)
{
    Selecting select;
    SelectingResult result = select.getResult();
    ASSERT_FALSE(result.selectingDone());
}
TEST(Selecting,ReturnResult_SelectingDone)
{
    Selecting select;
    SelectingTestAccess access(select);
    access.SetReadyForRendering(true);
    SelectingResult result = select.getResult();
    ASSERT_TRUE(result.selectingDone());
}
//select.ReadInPositionXY(3,6);

//#ifndef d_pickingVertexShaderPath
//#define d_pickingVertexShaderPath vspath
//#endif
//
//#ifndef d_pickingFragmentShaderPath
//#define d_pickingFragmentShaderPath fspath
//#endif
//    select.setVertexShaderPath("vs");
//    select.setFragmentShaderPath("fs");
//    select.
//#undef  d_pickingVertexShaderPath
//#undef  d_pickingFragmentShaderPath