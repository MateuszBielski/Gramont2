#include <gtest/gtest.h>
#include "rendersystemmock.h"
#include "bufferloadermock.h"
#include "onetexturerendersystem.h"
#include "paralaxoclusionmaprendersystem.h"
#include "triangle.h"
#include <glshadersmock.h>


using namespace std;

TEST(RenderSystem,Not_ReloadedVAOforModelData)
{
    RenderSystemMock rsm;
    ModelData d;
    ASSERT_FALSE(rsm.ReloadedVAOforModelData(d));
}
TEST(RenderSystem,Not_ReloadedVAOforTexture)
{
    RenderSystemMock rsm;
    TextureForModel t;
    ASSERT_FALSE(rsm.ReloadedVAOforTexture(t));
}
TEST(RenderSystem,ReloadedVAOforModelData)
{
    RenderSystemMock rsm;
    auto triangle = make_shared<Triangle>();
    ModelData& d = triangle->GetModelData();
    rsm.ReloadVAO(triangle);
    ASSERT_TRUE(rsm.ReloadedVAOforModelData(d));
}
TEST(RenderSystem,ReloadedVAOforTexture)
{
    RenderSystemMock rsm;
    auto triangle = make_shared<Triangle>();
    TextureForModel& t = *triangle->MyTexture();
    rsm.ReloadVAO(triangle);
    ASSERT_TRUE(rsm.ReloadedVAOforTexture(t));
}
TEST(RenderSystem,ReloadVAO_RecreateVao)
{
    OneTextureRenderSystem rs;
    spBufferLoaderMock blm = make_shared<BufferLoaderMock>();
    rs.setBufferLoader(blm);

    auto triangle = make_shared<Triangle>();
    rs.ReloadVAO(triangle);
    ASSERT_EQ(1,blm->recreateVaoUsed);
}


TEST(RenderSystem,CreateBuffersForEveryTexturesInModel)
{
    ParalaxOclusionMapRenderSystem rs;
    auto triangle = make_shared<Triangle>();
    triangle->CopyFromMainTextureAs(TextureForModel::TextureType::Height);
    triangle->CopyFromMainTextureAs(TextureForModel::TextureType::Normal);
    auto texHeigh = triangle->getTextureOfType(TextureForModel::TextureType::Height);
    auto texNormal = triangle->getTextureOfType(TextureForModel::TextureType::Normal);
    ASSERT_EQ(0,texHeigh->bufTexCoordId);
    ASSERT_EQ(0,texNormal->bufTexCoordId);
    rs.CreateGraphicBuffers(triangle);
    ASSERT_GT(texHeigh->bufTexCoordId,0);
    ASSERT_GT(texNormal->bufTexCoordId,0);
}
TEST(RenderSystem,OneTexRs_LoadVAO_LoadedBuffersForModelGeometry)
{
    OneTextureRenderSystem otrs;
    spOneModel model = make_shared<Triangle>();
    auto buff = otrs.getBufferLoader();
    buff->CreateVao(model->getVao());
    otrs.LoadVAO(model);
    ASSERT_EQ(1,buff->Counter(BufferLoaderCounterType::LoadBufferForModelGeometryCompleted));
}
TEST(RenderSystem,OneTexRs_LoadVAO_LoadedBuffersForTexture)
{
    OneTextureRenderSystem otrs;
    spOneModel model = make_shared<Triangle>();
    auto buff = otrs.getBufferLoader();
    buff->CreateVao(model->getVao());
    otrs.LoadVAO(model);
    ASSERT_EQ(1,buff->Counter(BufferLoaderCounterType::LoadBufferForModelGeometryCompleted));
}
TEST(RenderSystem,ParalOclTexRs_LoadVAO_LoadedBuffersForModelGeometry)
{
    ParalaxOclusionMapRenderSystem pors;
    spOneModel model = make_shared<Triangle>();
    auto buff = pors.getBufferLoader();
    buff->CreateVao(model->getVao());
    pors.LoadVAO(model);
    ASSERT_EQ(1,buff->Counter(BufferLoaderCounterType::LoadBufferForModelGeometryCompleted));
}
TEST(RenderSystem,ParalOclRs_LoadVAO_LoadedBuffersForTexture)
{
    ParalaxOclusionMapRenderSystem pors;
    spOneModel model = make_shared<Triangle>();
    auto buff = pors.getBufferLoader();
    buff->CreateVao(model->getVao());
    pors.LoadVAO(model);
    ASSERT_EQ(1,buff->Counter(BufferLoaderCounterType::LoadBufferForModelGeometryCompleted));
}
TEST(RenderSystem,ParalOclRs_shadAttribLocationsOfBuffLoaderHasCorrectSizeAfterConfigure)
{
    ParalaxOclusionMapRenderSystem pors;
    pors.ConfigureShadersAndLocations();
    auto buff = pors.getBufferLoader();
    int expect = (int)pomShAttr::pomShAttrSize;
    ASSERT_EQ(expect,buff->shadAttribLocations.size());
}
TEST(RenderSystem,ParalOclRs_shaderHasAttributesAfterConfigure)
{
    ParalaxOclusionMapRenderSystem pors;
    spShadersMock shader = make_shared<glShadersMock>();
    pors.setShader(shader);
    pors.ConfigureShadersAndLocations();
    MA_CreateStrings(expectedShaderAttribs, POM_SH_ATTR)
    for (auto attrib : expectedShaderAttribs) {
        ASSERT_TRUE(shader->hasAttrib(attrib));
    }
}
TEST(RenderSystem,ParalOclRs_buffLoadKnowsAttribLocationsAfterConfigure)
{
    ParalaxOclusionMapRenderSystem pors;
    spShadersMock shader = make_shared<glShadersMock>();
    pors.setShader(shader);
    
    MA_CreateStrings(shaderAttribNames, POM_SH_ATTR)
    constexpr short pomShAttrSize = (short)pomShAttr::pomShAttrSize;
    short expectLocations[pomShAttrSize], resultLocations[pomShAttrSize];
    for(short a = 0; a < pomShAttrSize ; a++) {
        expectLocations[a] = pomShAttrSize - a;
        shader->setAttribLoc(shaderAttribNames[a], pomShAttrSize - a);
    }
    
    pors.ConfigureShadersAndLocations();
    auto buffLoader = pors.getBufferLoader();
    for(short r = 0; r < pomShAttrSize ; r++) {
        resultLocations[r] = buffLoader->shadAttribLocations[r];
    }
    for(short r = 0; r < pomShAttrSize ; r++) {
        ASSERT_EQ(expectLocations[r],resultLocations[r]);
    }
}
TEST(RenderSystem,ParalOclRs_shadUnifLocationsOfRendererHasCorrectSizeAfterConfigure)
{
    ParalaxOclusionMapRenderSystem pors;
    pors.ConfigureShadersAndLocations();
    auto rend = pors.getRenderer();
    int expect = (int)pomShUnif::pomShUnifSize;
    ASSERT_EQ(expect,rend->shadUnifLocations.size());
}
TEST(RenderSystem,ParalOclRs_shaderHasUniformsAfterConfigure)
{
    ParalaxOclusionMapRenderSystem pors;
    spShadersMock shader = make_shared<glShadersMock>();
    pors.setShader(shader);
    pors.ConfigureShadersAndLocations();
    MA_CreateStrings(expectedShaderUniforms, POM_SH_UNIF)
    for (auto unif : expectedShaderUniforms) {
        ASSERT_TRUE(shader->hasUnif(unif));
    }
}
TEST(RenderSystem,ParalOclRs_rendererLoadKnowsUnifLocationsAfterConfigure)
{
    ParalaxOclusionMapRenderSystem pors;
    spShadersMock shader = make_shared<glShadersMock>();
    pors.setShader(shader);
    
    MA_CreateStrings(shaderUnifNames, POM_SH_UNIF)
    constexpr short pomShUnifSize = (short)pomShUnif::pomShUnifSize;
    short expectLocations[pomShUnifSize], resultLocations[pomShUnifSize];
    for(short a = 0; a < pomShUnifSize ; a++) {
        expectLocations[a] = pomShUnifSize - a;
        shader->setUnifLoc(shaderUnifNames[a], pomShUnifSize - a);
    }
    
    pors.ConfigureShadersAndLocations();
    auto rend = pors.getRenderer();
    for(short r = 0; r < pomShUnifSize ; r++) {
        resultLocations[r] = rend->shadUnifLocations[r];
    }
    for(short r = 0; r < pomShUnifSize ; r++) {
        ASSERT_EQ(expectLocations[r],resultLocations[r]);
    }
}