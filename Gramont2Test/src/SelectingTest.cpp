#include <gtest/gtest.h>
#include "selectingtestaccess.h"
#include "selecting.h"
#include "glshadersmock.h"
#include "glFunctionsMock.h"
#include "shadersPath.h"
#include <stdio.h> //rename

using namespace std;

TEST(Selecting,initializeShaderOnInit)
{
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.Init();
    ASSERT_TRUE(shad_mock->InitUsed());
}
TEST(Selecting,addUniformsForShaderOnInit)
{
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.Init();
    ASSERT_TRUE(shad_mock->hasUnif("gDrawIndex"));
    ASSERT_TRUE(shad_mock->hasUnif("gObjectIndex"));
    ASSERT_TRUE(shad_mock->hasUnif("gWVP"));
}
TEST(Selecting,addAttribForShaderOnInit)
{
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.Init();
    ASSERT_TRUE(shad_mock->hasAttrib("Position"));
}
TEST(Selecting,addCodeForShaderOnInit)
{
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.Init();
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
    select.Init();
    
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
    select.Init();
    SelectingTestAccess access(select);
    ASSERT_FALSE(access.ShadersLoaded());
}

TEST(Selecting,noCompilingShaders_ifNoShadersCode)
{
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.Init();
    ASSERT_FALSE(shad_mock->UsingCompile());
}

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
