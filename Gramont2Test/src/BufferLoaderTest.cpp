#include <gtest/gtest.h>
#include "bufferloader.h"
#include "bufferloadermock.h"
//#include <memory>
//#include "textureformodel.h"
//#include "modeldata.h"
//#include "glshadersmock.h"
//#include "glFunctionsMock.h"

class BufferLoader_Te : public ::testing::Test
{

protected:
    static TextureForModel textureWithData;
    static spTextureInMemory sptextureInMemoryWithData;
    float textureCoord;
    static bool textureLoaded;
    void setDummyDataForTexture(TextureForModel& tex) {

        if(!textureLoaded) {
            textureCoord = 34.8f;
            sptextureInMemoryWithData = std::make_shared<TextureInMemory>("Gramont2Test/Resources/10x10image.jpg");
            tex.setTextureInMemory(sptextureInMemoryWithData);
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
spTextureInMemory BufferLoader_Te::sptextureInMemoryWithData;
bool BufferLoader_Te::textureLoaded= false;
TEST_F(BufferLoader_Te,CompleteCreateBuffers_ifNumbersNotZero)
{
    BufferLoader buf;
    ModelData d;
    d.nuColours = 4;
    d.nuIndices = 6;
    d.nuNormals = 8;
    d.nuPoints = 10;
    ASSERT_EQ(BufferLoaderProgress::Completed,buf.CreateBuffersForModelGeometry(d));
}
TEST_F(BufferLoader_Te,StartedCreateBuffers_ifNumbersZero)
{
    BufferLoader buf;
    ModelData d;
    d.nuColours = 0;
    d.nuIndices = 0;
    d.nuNormals = 0;
    d.nuPoints = 0;
    ASSERT_EQ(BufferLoaderProgress::Checked,buf.CreateBuffersForModelGeometry(d));
}
TEST_F(BufferLoader_Te,NotCreateGeometryBuffersIfCreated)
{
    BufferLoader buf;
    ModelData d;
    d.nuColours = 4;
    d.nuIndices = 6;
    d.nuNormals = 8;
    d.nuPoints = 10;
    d.bufColNorId = 2;
    d.bufIndexId = 5;
    d.bufVertId = 3;
    
    ASSERT_EQ(BufferLoaderProgress::Completed,buf.CreateBuffersForModelGeometry(d));
    ASSERT_EQ(d.bufColNorId,2);
    ASSERT_EQ(d.bufIndexId,5);
    ASSERT_EQ(d.bufVertId,3);
}
TEST_F(BufferLoader_Te,CreateGeometryBuffersIf_InitialValue_NotCreated)
{
    BufferLoader buf;
    ModelData d;
    d.nuColours = 4;
    d.nuIndices = 6;
    d.nuNormals = 8;
    d.nuPoints = 10;

    unsigned int colNor = d.bufColNorId, index = d.bufIndexId, vert = d.bufVertId;
    
    ASSERT_EQ(BufferLoaderProgress::Completed,buf.CreateBuffersForModelGeometry(d));
    ASSERT_GT(d.bufColNorId,0);
    ASSERT_NE(d.bufColNorId,colNor);
    
    ASSERT_GT(d.bufIndexId,0);
    ASSERT_NE(d.bufIndexId,index);
    
    ASSERT_GT(d.bufVertId,0);
    ASSERT_NE(d.bufVertId,vert);
}
TEST_F(BufferLoader_Te,NotCreateTextureBuffersIfCreated)
{
    BufferLoader buf;
    TextureForModel t;
    t.nuTexCoord = 2;
    t.bufTexCoordId = 3;
    
    ASSERT_EQ(BufferLoaderProgress::Completed,buf.CreateBufferForTextureCoord(t));
    ASSERT_EQ(t.bufTexCoordId,3);
}
TEST_F(BufferLoader_Te,CreateTextureBuffersIf_InitialValue_NotCreated)
{
    BufferLoader buf;
    TextureForModel t;
    t.nuTexCoord = 2;
    
    unsigned int texCoord = t.bufTexCoordId;
    
    ASSERT_EQ(BufferLoaderProgress::Completed,buf.CreateBufferForTextureCoord(t));
    ASSERT_GT(t.bufTexCoordId,0);
    ASSERT_NE(t.bufTexCoordId,texCoord);
}
TEST_F(BufferLoader_Te,CreateBuffersCheckedCount)
{
    BufferLoader buf;
    ModelData d;
    buf.CreateBuffersForModelGeometry(d);
    buf.CreateBuffersForModelGeometry(d);
    
    ASSERT_EQ(2,buf.Counter(BufferLoaderCounterType::CreateBuffersForModelGeometryStart));
}

TEST_F(BufferLoader_Te,LoadTextureSuccessCounting_Zero)
{
    BufferLoader buf;
    
    ASSERT_EQ(0,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureCompleted));
//    buf.LoadTextureBuffersForSingleModelEntry();
}

TEST_F(BufferLoader_Te,LoadTextureFailsCounting_Zero)
{
    BufferLoader buf;
    ASSERT_EQ(0,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureStart));
//    buf.LoadTextureBuffersForSingleModelEntry();
}

TEST_F(BufferLoader_Te,LoadTextureStartsCounting)
{
    BufferLoader buf;
//    ModelData d;
    TextureForModel tex;
    buf.LoadBufferForTexture(tex, 2);
    buf.LoadBufferForTexture(tex, 4);
    ASSERT_EQ(2,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureStart));
}

TEST_F(BufferLoader_Te,LoadTextureSuccessCounting_ZeroIfRequirementsNotMet)
{
    BufferLoader buf;
    ModelData d;
    TextureForModel tex;
    buf.LoadTextureBuffersForSingleModelEntry(tex, d);
    buf.LoadTextureBuffersForSingleModelEntry(tex, d);
    ASSERT_EQ(0,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureCompleted));
}


TEST_F(BufferLoader_Te,LoadFail_bufIndexId)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    d.bufIndexId = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    ASSERT_EQ(0,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureStart));
}
TEST_F(BufferLoader_Te,LoadFail_bufColNorId)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    d.bufColNorId = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    ASSERT_EQ(0,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureCompleted));
}
TEST_F(BufferLoader_Te,LoadFail_nuColours)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    d.nuColours = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    ASSERT_EQ(0,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureCompleted));
}
TEST_F(BufferLoader_Te,LoadFail_bufVertId)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    d.bufVertId = 0;
    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    ASSERT_EQ(0,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureStart));
}
//LoadTextureBuffersForSingleModelEntry is function used only in OneModelManager
//TEST_F(BufferLoader_Te,LoadFail_TexWidth)
//{
//    BufferLoader buf;
//    ModelData d;
//    setDummyDataForModelData(d);
//    setDummyDataForTexture(textureWithData);
//    auto temp = sptextureInMemoryWithData->width;
//    sptextureInMemoryWithData->width = 0;
//    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
//    sptextureInMemoryWithData->width = temp;
//    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
//}
//TEST_F(BufferLoader_Te,LoadFail_TexHeight)
//{
//    BufferLoader buf;
//    ModelData d;
//    setDummyDataForModelData(d);
//    setDummyDataForTexture(textureWithData);
//    auto temp = sptextureInMemoryWithData->height;
//    sptextureInMemoryWithData->height = 0;
//    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
//    sptextureInMemoryWithData->height = temp;
//    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
//}
//TEST_F(BufferLoader_Te,LoadFail_TextureData)
//{
//    BufferLoader buf;
//    ModelData d;
//    TextureForModel tex;
//    spTextureInMemory texm = std::make_shared<TextureInMemory>("");
//    tex.setTextureInMemory(texm);
//    setDummyDataForModelData(d);
//    float coord = 32.1f;
//    texm->width = 4;
//    texm->height = 6;
//    tex.texCoord = &coord;
//    tex.nuTexCoord = 1;
//    buf.LoadTextureBuffersForSingleModelEntry(tex, d);
//    ASSERT_EQ(0,buf.LoadTextureSuccessCount());
//}
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
    ASSERT_EQ(0,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureStart));
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
    ASSERT_EQ(0,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureStart));
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

    ASSERT_EQ(0,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureCompleted));
}
TEST_F(BufferLoader_Te,LoadTextureSuccessCounting)
{
    BufferLoader buf;
    ModelData d;
    setDummyDataForModelData(d);
    setDummyDataForTexture(textureWithData);
    setDummyLocationsForBufLoad(buf);
    buf.LoadBufferForTexture(textureWithData, 2);
    buf.LoadBufferForTexture(textureWithData, 3);
    buf.LoadBufferForTexture(textureWithData, 4);
//    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
//    buf.LoadTextureBuffersForSingleModelEntry(textureWithData, d);
    ASSERT_EQ(3,buf.Counter(BufferLoaderCounterType::LoadBufferForTextureCompleted));
}
TEST_F(BufferLoader_Te,BufferForModelDataNotLoaded)
{
    BufferLoaderMock blm;
    ModelData d1;
    ASSERT_FALSE(blm.LoadedBufferForModelGeometry(d1));
}
TEST_F(BufferLoader_Te,BufferForModelDataLoaded)
{
    BufferLoaderMock blm;
    ModelData d1;
    blm.LoadBuffersForModelGeometry(d1,3);
    ASSERT_TRUE(blm.LoadedBufferForModelGeometry(d1));
}
TEST_F(BufferLoader_Te,BufferForModelDataLoaded_andNotLoaded)
{
    BufferLoaderMock blm;
    ModelData d1, d2;
    blm.LoadBuffersForModelGeometry(d1,3);
    ASSERT_TRUE(blm.LoadedBufferForModelGeometry(d1));
    ASSERT_FALSE(blm.LoadedBufferForModelGeometry(d2));
}
TEST_F(BufferLoader_Te,BufferForTextureNotLoaded)
{
    BufferLoaderMock blm;
    TextureForModel t;
    ASSERT_FALSE(blm.LoadedBufferForTexture(t));
}
TEST_F(BufferLoader_Te,BufferForTextureLoaded)
{
    BufferLoaderMock blm;
    TextureForModel t;
    blm.LoadBufferForTexture(t,4);
    ASSERT_TRUE(blm.LoadedBufferForTexture(t));
}
TEST_F(BufferLoader_Te,NotCreateVao_IfExist)
{
    BufferLoader bl;
    unsigned int vao = 343;
    bl.CreateVao(vao);
    ASSERT_EQ(343,vao);
}
TEST_F(BufferLoader_Te,CreateVao)
{
    BufferLoader bl;
    unsigned int vao = 0;
    bl.CreateVao(vao);
    ASSERT_NE(0,vao);
}
TEST_F(BufferLoader_Te,RecreateVao)
{
    BufferLoader bl;
    unsigned int vao = 33558;
    bl.RecreateVao(vao);

    ASSERT_NE(0,vao);
    ASSERT_NE(33558,vao);
}
