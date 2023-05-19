#include <gtest/gtest.h>
#include "rendersystemmock.h"
#include "bufferloadermock.h"
#include "onetexturerendersystem.h"

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
    ModelData d;
    TextureForModel t;
    rsm.ReloadVAO(d,t);
    ASSERT_TRUE(rsm.ReloadedVAOforModelData(d));
}
TEST(RenderSystem,ReloadedVAOforTexture)
{
    RenderSystemMock rsm;
    ModelData d;
    TextureForModel t;
    rsm.ReloadVAO(d,t);
    ASSERT_TRUE(rsm.ReloadedVAOforTexture(t));
}
TEST(RenderSystem,ReloadVAO_RecreateVao)
{
    OneTextureRenderSystem rs;
    spBufferLoaderMock blm = make_shared<BufferLoaderMock>();
    rs.setBufferLoader(blm);
    
    ModelData d;
    TextureForModel t;
    rs.ReloadVAO(d,t);
    ASSERT_EQ(1,blm->recreateVaoUsed);
}

TEST(RenderSystem,ReloadVAO_LoadedBufferForTexture)
{
    OneTextureRenderSystem rs;
    spBufferLoaderMock blm = make_shared<BufferLoaderMock>();
    rs.setBufferLoader(blm);
    
    ModelData d;
    TextureForModel t1, t2;
    rs.ReloadVAO(d,t1);
    ASSERT_FALSE(blm->LoadedBufferForTexture(t2));
    ASSERT_TRUE(blm->LoadedBufferForTexture(t1));
}