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
    VertexError,
    ColorNormalsError,
    IndexError,
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
    BufferLoaderProgress CreateBuffersForSingleModelEntry(ModelData& d);
    
    virtual void LoadBuffers(spOneModel model){};
    void LoadBuffersForSingleModelEntry(ModelData& d);
    void ClearBuffersForSingleModelEntry(ModelData& d);
    bool LoadTextureBuffersForSingleModelEntry(TextureForModel& tex, ModelData& d);
    unsigned int CreateBuffersCheckedCount();
    unsigned int LoadTextureSuccessCount();
    unsigned int LoadTextureFailsCount();
protected:
    unsigned loadTextureSuccessCount = 0, loadTextureFailsCount = 0, createBuffersCheckedCount = 0;
};

using spBufferLoader = std::shared_ptr<BufferLoader>;
using upBufferLoader = std::unique_ptr<BufferLoader>;

#endif // BufferLoader_H
