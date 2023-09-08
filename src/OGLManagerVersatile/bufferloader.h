/* %PATH% %FILE% */
#ifndef BufferLoader_H
#define BufferLoader_H
#include <memory>
#include "oglstuff.h"
#include "modeldata.h"
#include "textureformodel.h"
#include "onemodel.h"

using std::tuple;
using vec_for_subbuf = vector<tuple<unsigned int, int,const float *>>;

enum class BufferLoaderProgress
{
    Completed,
    Checked,
    VaoNotInited,
    VaoReadyForLoad,
    VertexError,
    ColorNormalsError,
    IndexError,
    TextureError,
    TextureNotFound,
};
enum class BufferLoaderCounterType
{
    CreateBuffersForModelGeometryStart,
    CreateBufferForTextureCoordStart,
    CreateBufferForTextureInMemoryStart,
    LoadBufferForTextureStart,
    LoadBufferForModelGeometryStart,
    CreateBuffersForModelGeometryCompleted,
    CreateBufferForTextureCoordCompleted,
    CreateBufferForTextureInMemoryCompleted,
    LoadBufferForTextureCompleted,
    LoadBufferForModelGeometryCompleted,
    BufferLoaderCounterTypeSize
};

class BufferLoader
{
private:

public:
    struct Locations {
        int position;
        int position_tex;
        int normal;
        int normal_tex;
        int colour;
        int textureCoord;
    };
    BufferLoader();
    Locations m_loc;
    vector<unsigned> shadAttribLocations;//to replace with struct Locations
    BufferLoaderProgress CreateBufferWithSubs(unsigned int& bufId, vec_for_subbuf& data);
    BufferLoaderProgress CreateBuffersForModelGeometry(ModelData& d);
//    BufferLoaderProgress CreateBufferForTagentAndBitangent(ModelData& d);
    //BufferLoaderProgress CreateBufferForVerticesVectorsInSpace();//name to using
    BufferLoaderProgress CreateBufferForTextureCoord(TextureForModel& tex);
    BufferLoaderProgress CreateBufferForTextureInMemory(TextureInMemory& tex);
    bool CreateVao(unsigned int& vao);
#ifdef TESTOWANIE_F
    virtual
#endif
    void RecreateVao(unsigned int& vao);
    virtual BufferLoaderProgress LoadBuffersForModelGeometry(ModelData& d,const int vao);
    virtual BufferLoaderProgress LoadBufferForTexture(TextureForModel& tex,const int vao);
    BufferLoaderProgress StartLoadingBuffersWith(unsigned vao);
    void LoadBufferOnLocation3f(unsigned buffId,size_t loc);
    void LoadSubBufferOnLocation3f(unsigned buffId,size_t loc,size_t offsetStep,size_t nuSteps);
    void LoadBufferOnLocation2f(unsigned buffId,size_t loc);
    BufferLoaderProgress LoadIndicesAndFinish(unsigned buffId);

//    virtual void LoadBuffers(spOneModel model);
    virtual void setLocationsFrom(spMyOGLShaders);
    void ClearBuffersForSingleModelEntry(ModelData& d);
    const unsigned Counter(BufferLoaderCounterType couterType);

    bool LoadTextureBuffersForSingleModelEntry(TextureForModel& tex, ModelData& d);
protected:
//unsigned loadTextureSuccessCount = 0, loadTextureFailsCount = 0, createBuffersCheckedCount = 0;
    vector<unsigned> counter;

};

using spBufferLoader = std::shared_ptr<BufferLoader>;
using upBufferLoader = std::unique_ptr<BufferLoader>;

#endif // BufferLoader_H
