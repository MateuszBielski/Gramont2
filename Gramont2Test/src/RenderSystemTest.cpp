#include <gtest/gtest.h>
#include "rendersystemmock.h"

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

