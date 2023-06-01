#include <gtest/gtest.h>
#include "rendersystemmock.h"
#include "bufferloadermock.h"
#include "onetexturerendersystem.h"
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
