#include <gtest/gtest.h>
#include "oglstuff.h"
#include "glshadersmock.h"
//#include "glFunctionsMock.h"

TEST(ShaderTest,NotInitializedShaderGivesZeroProgramId)
{
    myOGLShaders shader;
    ASSERT_EQ(shader.getProgramId(),0);
}
TEST(ShaderTest,setAttribLoc)
{
    glShadersMock shader;
    shader.setAttribLoc("in_sPosition",43);
    ASSERT_EQ(shader.GetAttribLoc("in_sPosition"),43);
}
TEST(ShaderTest,setAttribLoc_other)
{
    glShadersMock shader;
    shader.setAttribLoc("other",41);
    ASSERT_EQ(shader.GetAttribLoc("other"),41);
}
TEST(ShaderTest,hasAttrib_No)
{
    glShadersMock shader;
    ASSERT_FALSE(shader.hasAttrib("anyAttrib"));
}
TEST(ShaderTest,hasAttrib)
{
    glShadersMock shader;
    shader.AddAttrib("anyAttrib");
    ASSERT_TRUE(shader.hasAttrib("anyAttrib"));
}
TEST(ShaderTest,hasUnif_No)
{
    glShadersMock shader;
    ASSERT_FALSE(shader.hasUnif("anyUnif"));
}
TEST(ShaderTest,hasUnif)
{
    glShadersMock shader;
    shader.AddUnif("anyUnif");
    ASSERT_TRUE(shader.hasUnif("anyUnif"));
}
TEST(ShaderTest,setUnifLoc)
{
    glShadersMock shader;
    shader.setUnifLoc("mMVP",9);
    ASSERT_EQ(shader.GetUnifLoc("mMVP"),9);
}
TEST(ShaderTest,HasCodeOfType_No)
{
    glShadersMock shader;
    ASSERT_FALSE(shader.hasCodeOfType(GL_VERTEX_SHADER));
}
TEST(ShaderTest,nullptrCharCodeNotAdded)
{
    glShadersMock shader;
    shader.AddCode(nullptr,GL_VERTEX_SHADER);
    ASSERT_EQ(shader.getShaCode().size(),0);
    ASSERT_FALSE(shader.hasCodeOfType(GL_VERTEX_SHADER));
}
TEST(ShaderTest,HasCodeOfType_otherType)
{
    glShadersMock shader;
    string code{"anyCode"};
    shader.AddCode(code.c_str(),GL_VERTEX_SHADER);
    ASSERT_FALSE(shader.hasCodeOfType(GL_FRAGMENT_SHADER));
}
TEST(ShaderTest,HasCodeOfType)
{
    glShadersMock shader;
    string code{"anyCode"};
    shader.AddCode(code.c_str(),GL_FRAGMENT_SHADER);
    ASSERT_TRUE(shader.hasCodeOfType(GL_FRAGMENT_SHADER));
}
TEST(ShaderTest,HasCodeOfTypes)
{
    glShadersMock shader;
    string code{"anyCode"};
    string code2{"anyCode2"};
    shader.AddCode(code.c_str(),GL_FRAGMENT_SHADER);
    shader.AddCode(code2.c_str(),GL_VERTEX_SHADER);
    ASSERT_TRUE(shader.hasCodeOfType(GL_FRAGMENT_SHADER));
    ASSERT_TRUE(shader.hasCodeOfType(GL_VERTEX_SHADER));
}

