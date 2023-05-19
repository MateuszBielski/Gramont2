/* %PATH% %FILE% */
#ifndef BufferLoader_H
#define BufferLoader_H
#include <memory>
#include "oglstuff.h"
#include "modeldata.h"
#include "textureformodel.h"
#include "onemodel.h"

enum class BufferLoaderProgress
{
    Completed,
    Checked,
    VaoNotInited,
    VertexError,
    ColorNormalsError,
    IndexError,
    TextureError,
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
    Locations m_loc;
    BufferLoaderProgress CreateBuffersForModelGeometry(ModelData& d);
    BufferLoaderProgress CreateBufferForTextureCoord(TextureForModel& tex);
    bool CreateVao(unsigned int& vao);
#ifdef TESTOWANIE_F
    virtual
#endif
    void RecreateVao(unsigned int& vao);
    virtual BufferLoaderProgress LoadBuffersForModelGeometry(ModelData& d,const int vao);
    virtual BufferLoaderProgress LoadBufferForTexture(TextureForModel& tex,const int vao);

    virtual void LoadBuffers(spOneModel model) {};
    virtual void setLocationsFrom(spMyOGLShaders);
    void ClearBuffersForSingleModelEntry(ModelData& d);
    unsigned int CreateBuffersCheckedCount();
    unsigned int LoadTextureSuccessCount();
    unsigned int LoadTextureFailsCount();

    bool LoadTextureBuffersForSingleModelEntry(TextureForModel& tex, ModelData& d);
protected:
    unsigned loadTextureSuccessCount = 0, loadTextureFailsCount = 0, createBuffersCheckedCount = 0;
};

using spBufferLoader = std::shared_ptr<BufferLoader>;
using upBufferLoader = std::unique_ptr<BufferLoader>;

#endif // BufferLoader_H
