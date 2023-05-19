#ifndef BufferLoaderMock_H
#define BufferLoaderMock_H

#include <iostream>
#include <memory>
#include <list>
#include "bufferloader.h"

class BufferLoaderMock : public BufferLoader
{
private:
    std::list<ModelData *> adressesOfModelDataLoaded;
    std::list<TextureForModel *> adressesOfTextureLoaded;
public:
    virtual BufferLoaderProgress LoadBuffersForModelGeometry(ModelData& d,const int vao);
    virtual BufferLoaderProgress LoadBufferForTexture(TextureForModel& tex,const int vao);
    
    bool LoadedBufferForModelGeometry(ModelData& d);
    bool LoadedBufferForTexture(TextureForModel& tex);
    virtual void RecreateVao(unsigned int& vao) override;
    unsigned short recreateVaoUsed = 0;
protected:

};
using spBufferLoaderMock = std::shared_ptr<BufferLoaderMock>;
using upBufferLoaderMock = std::unique_ptr<BufferLoaderMock>;
#endif // BufferLoaderMock_H
