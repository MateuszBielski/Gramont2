#include <gtest/gtest.h>
#include "rendersystemmock.h"
#include "bufferloadermock.h"
#include "onetexturerendersystem.h"
#include "paralaxoclusionmaprendersystem.h"
#include "triangle.h"

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

TEST(RenderSystem,ReloadVAO_LoadedBufferForTexture)
{
    OneTextureRenderSystem rs;
    spBufferLoaderMock blm = make_shared<BufferLoaderMock>();
    rs.setBufferLoader(blm);
    auto triangle = make_shared<Triangle>();
    ModelData& d = triangle->GetModelData();
    TextureForModel& t1 = *triangle->MyTexture();
    TextureForModel t2;

    rs.ReloadVAO(triangle);
    ASSERT_FALSE(blm->LoadedBufferForTexture(t2));
    ASSERT_TRUE(blm->LoadedBufferForTexture(t1));
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
    ASSERT_EQ(1,buff->Counter(BufferLoaderCounterType::LoadBufferForTextureCompleted));
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
    ASSERT_EQ(1,buff->Counter(BufferLoaderCounterType::LoadBufferForTextureCompleted));
}