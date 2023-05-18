#include "bufferloadermock.h"



BufferLoaderProgress BufferLoaderMock::LoadBuffersForModelGeometry(ModelData& d, const int vao)
{
    auto adr = &d;
    adressesOfModelDataLoaded.push_back(adr);
    return BufferLoaderProgress::Completed;
}

BufferLoaderProgress BufferLoaderMock::LoadBufferForTexture(TextureForModel& tex, const int vao)
{
    auto adr = &tex;
    adressesOfTextureLoaded.push_back(adr);
    return BufferLoaderProgress::Completed;
}
bool BufferLoaderMock::LoadedBufferForModelGeometry(ModelData& d)
{	
	for(auto& adr : adressesOfModelDataLoaded)
    {
        if (adr == &d) return true;
    }
    return false;
}
bool BufferLoaderMock::LoadedBufferForTexture(TextureForModel& tex)
{
	for(auto& adr : adressesOfTextureLoaded)
    {
        if (adr == &tex) return true;
    }
    return false;
}
