#include <gtest/gtest.h>
#include "oglrenderer.h"
#include "OglRendererMock.h"
#include "textureformodel.h"
#include "modeldata.h"
//#include "glshadersmock.h"
//#include "glFunctionsMock.h"

TEST(OglRenderer,ShaderNotInitialized)
{
    OglRenderer renderer;
    int notInitializedShader = 0;
    TextureForModel tex;
    ModelData d;

    ASSERT_EQ(OglRendererProgress::BeforeOgl,renderer.DrawTextureForSingleModelEntry(3,tex,d,notInitializedShader));

}
TEST(OglRenderer,MatricesNotInitialized_AtStartObject)
{
    OglRenderer renderer;
    ASSERT_EQ(renderer.m_matrices.matMVP,nullptr);
    ASSERT_EQ(renderer.m_matrices.matToVw,nullptr);
    ASSERT_EQ(renderer.m_matrices.light_position,nullptr);
    ASSERT_EQ(renderer.m_matrices.light_colour,nullptr);
}
TEST(OglRenderer,DrawTexture_MaricesNotInitialized)
{
    OglRenderer renderer;
    int InitializedShader = 3; // >0
    TextureForModel tex;
    ModelData d;
    ASSERT_EQ(OglRendererProgress::BeforeOgl,renderer.DrawTextureForSingleModelEntry(3,tex,d,InitializedShader));
}

TEST(OglRenderer,DrawTexture_LocationsNotInitialized)
{
    OglRenderer renderer;
    int InitializedShader = 3; // >0
    TextureForModel tex;
    ModelData d;
    float f;
    renderer.m_matrices.matMVP = &f;
    renderer.m_matrices.matToVw = &f;
    renderer.m_matrices.light_position = &f;
    renderer.m_matrices.light_colour = &f;

    ASSERT_EQ(OglRendererProgress::BeforeOgl,renderer.DrawTextureForSingleModelEntry(3,tex,d,InitializedShader));
}
TEST(OglRenderer,DrawStartCount_0)
{
    OglRenderer renderer;
    ASSERT_EQ(0,renderer.StartCallCount());
}
TEST(OglRenderer,DrawStartCount)
{
    OglRenderer renderer;
    int InitializedShader = 3; // >0
    TextureForModel tex;
    ModelData d;
    renderer.DrawTextureForSingleModelEntry(3,tex,d,InitializedShader);
    renderer.DrawTextureForSingleModelEntry(3,tex,d,InitializedShader);
    ASSERT_EQ(2,renderer.StartCallCount());
}
TEST(OglRenderer,Mock_DrawTexture_pushesOnStack_sumOfmatMVP)
{
    int InitializedShader = 3; // >0
    TextureForModel tex;
    ModelData d;
    float matMVP_1[] = {2.3,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0};
    float matMVP_2[] = {4.1,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0};
    float matMVP_3[] = {6.7,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
    OglRendererMock rend;
    rend.m_matrices.matMVP = matMVP_1;
    rend.DrawTextureForSingleModelEntry(3,tex,d,InitializedShader);
    rend.m_matrices.matMVP = matMVP_2;
    rend.DrawTextureForSingleModelEntry(3,tex,d,InitializedShader);
    rend.m_matrices.matMVP = matMVP_3;
    rend.DrawTextureForSingleModelEntry(3,tex,d,InitializedShader);
    auto st = rend.getSumsOfmatMVP();
    ASSERT_EQ(3,st.size());
    ASSERT_FLOAT_EQ(7.7,st.top());
    st.pop();
    ASSERT_FLOAT_EQ(7.1,st.top());
    st.pop();
    ASSERT_FLOAT_EQ(4.3,st.top());
}
