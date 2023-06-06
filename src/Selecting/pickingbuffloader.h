#ifndef PickingBuffLoader_H
#define PickingBuffLoader_H
#include "bufferloader.h"
#include "selectable.h"


class PickingBuffLoader : public BufferLoader
{
  public:
    PickingBuffLoader();
    virtual ~PickingBuffLoader();
//    virtual void LoadBuffers(spOneModel model) override;
    virtual BufferLoaderProgress LoadBuffersForModelGeometry(ModelData& d,const int vao) override;
    virtual void setLocationsFrom(spMyOGLShaders) override;
protected:

private:
        
};
using spPickingBuffLoader = std::shared_ptr<PickingBuffLoader>;
using upPickingBuffLoader = std::unique_ptr<PickingBuffLoader>;
#endif // PickingBuffLoader_H
