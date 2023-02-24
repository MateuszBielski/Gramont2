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
    select.Init();
    ASSERT_TRUE(shad_mock->InitUsed());
}
TEST(Selecting,addUniformsForShaderOnInit)
{
    AccessFileForTest enableAcc;
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
    AccessFileForTest enableAcc;
    Selecting select;
    SelectingTestAccess access(select);
    spShadersMock shad_mock = make_shared<glShadersMock>();
    access.setShader(shad_mock);
    select.Init();
    ASSERT_TRUE(shad_mock->hasAttrib("Position"));
}
TEST(Selecting,addCodeForShaderOnInit)
{
    AccessFileForTest enableAcc;
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
    select.RegisterSelectable({sel1,sel2});
    ASSERT_NE(sel1->getUniqueId(),sel2->getUniqueId());
}

//select.ReadInPositionXY(3,6);

