#include <gtest/gtest.h>
#include "bufferloader.h"
//#include "textureformodel.h"
//#include "modeldata.h"
//#include "glshadersmock.h"
//#include "glFunctionsMock.h"

class BufferLoader_Te : public ::testing::Test
{

protected:
    static TextureForModel textureWithData;
    float textureCoord;
    static bool textureLoaded;
    void setDummyDataForTexture(TextureForModel& tex) {

        if(!textureLoaded) {
            textureCoord = 34.8f;
            tex.LoadImageFile("Gramont2Test/Resources/10x10image.jpg");
            tex.texCoord = &textureCoord;
            tex.nuTexCoord = 1;
            textureLoaded = true;
//        cout<<"\ntex.LoadImageFile";
        }
//    cout<<"\nsetDummyDataForTexture";
    }
    void setDummyDataForModelData(ModelData& md) {
        md.bufIndexId = 2;
        md.bufColNorId = 7;
        md.nuColours = 42;
        md.bufVertId = 98;
    }

};
TextureForModel BufferLoader_Te::textureWithData;
bool BufferLoader_Te::textureLoaded= false;
TEST_F(BufferLoader_Te,CompleteCreateBuffers_ifModelBuffersZero)
{
    BufferLoader buf;
    ModelData d;
    d.bufColNorId = 0;
    d.bufIndexId = 0;
    d.bufVertId = 0;
    ASSERT_EQ(BufferLoaderProgress::Completed,buf.CreateBuffersForSingleModelEntry(d));
}
TEST_F(BufferLoader_Te,StartedCreateBuffers_ifModelBuffersSetted)
{
    BufferLoader buf;
    ModelData d;
    d.bufColNorId = 1;
    d.bufIndexId = 5;
    d.bufVertId = 4;
    ASSERT_EQ(BufferLoaderProgress::Checked,buf.CreateBuffersForSingleModelEntry(d));
}
TEST_F(BufferLoader_Te,CreateBuffersCheckedCount)
{
    BufferLoader buf;
    ModelData d;
    buf.CreateBuffersForSingleModelEntry(d);
    buf.CreateBuffersForSingleModelEntry(d);
    ASSERT_EQ(2,buf.CreateBuffersCheckedCount());
}

TEST_F(BufferLoader_Te,LoadTextureSuccessCounting_Zero)
{
    BufferLoader buf;
    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
//    buf.LoadTextureBuffersForSingleModelEntry();
}

TEST_F(BufferLoader_Te,LoadTextureFailsCounting_Zero)
{
    BufferLoader buf;
    ASSERT_EQ(0,buf.LoadTextureFailsCount());
//    buf.LoadTextureBuffersForSingleModelEntry();
}

TEST_F(BufferLoader_Te,LoadTextureFailsCounting)
{
    BufferLoader buf;
    ModelData d;
    TextureForModel tex;
    buf.LoadTextureBuffersForSingleModelEntry(tex, d);
    buf.LoadTextureBuffersForSingleModelEntry(tex, d);
    ASSERT_EQ(2,buf.LoadTextureFailsCount());
}

TEST_F(BufferLoader_Te,LoadTextureSuccessCounting_ZeroIfRequirementsNotMet)
{
    BufferLoader buf;
    ModelData d;
    TextureForModel tex;
    buf.LoadTextureBuffersForSingleModelEntry(tex, d);
    buf.LoadTextureBuffersForSingleModelEntry(tex, d);
    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
}


TEST_F(BufferLoader_Te,LoadFail_bufIndexId)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    d.bufIndexId = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
}
TEST_F(BufferLoader_Te,LoadFail_bufColNorId)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    d.bufColNorId = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
}
TEST_F(BufferLoader_Te,LoadFail_nuColours)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    d.nuColours = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
}
TEST_F(BufferLoader_Te,LoadFail_bufVertId)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    d.bufVertId = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
}
TEST_F(BufferLoader_Te,LoadFail_TexWidth)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    auto temp = textureWithData.width;
    textureWithData.width = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    textureWithData.width = temp;
    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
}
TEST_F(BufferLoader_Te,LoadFail_TexHeight)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    auto temp = textureWithData.height;
    textureWithData.height = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    textureWithData.height = temp;
    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
}
TEST_F(BufferLoader_Te,LoadFail_TextureData)
{
    BufferLoader buf;
    ModelData d;
    TextureForModel tex;
    setDummyDataForModelData(d);
    float coord = 32.1f;
    tex.width = 4;
    tex.height = 6;
    tex.texCoord = &coord;
    tex.nuTexCoord = 1;
    buf.LoadTextureBuffersForSingleModelEntry(tex, d);
    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
}
TEST_F(BufferLoader_Te,LoadFail_TexCoord)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    auto temp = textureWithData.texCoord;
    textureWithData.texCoord = nullptr;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    textureWithData.texCoord = temp;
    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
}

TEST_F(BufferLoader_Te,LoadFail_nuTexCoord)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    auto temp = textureWithData.nuTexCoord;
    textureWithData.nuTexCoord = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    textureWithData.nuTexCoord = temp;
    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
}
void setDummyLocationsForBufLoad(BufferLoader& buf)
{
//        buf.m_loc.position = 3;
    buf.m_loc.position_tex = 4;
//        buf.m_loc.normal = 5;
    buf.m_loc.normal_tex = 6;
//        buf.m_loc.colour = 7;
    buf.m_loc.textureCoord = 9;
};
TEST_F(BufferLoader_Te,LoadFail_LocationsNotSetted)
{

    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    setDummyLocationsForBufLoad(buf);
    buf.m_loc.position_tex = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    setDummyLocationsForBufLoad(buf);
    buf.m_loc.normal_tex = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    setDummyLocationsForBufLoad(buf);
    buf.m_loc.textureCoord = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);

    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
}
TEST_F(BufferLoader_Te,LoadTextureSuccessCounting)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    setDummyLocationsForBufLoad(buf);
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    ASSERT_EQ(3,buf.LoadTextureSuccessCount());
}
