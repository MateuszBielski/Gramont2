#ifndef PickingBuffLoader_H
#define PickingBuffLoader_H
#include "bufferloader.h"
#include "selectable.h"


class PickingBuffLoader : public BufferLoader
{
  public:
    PickingBuffLoader();
    virtual ~PickingBuffLoader();
    virtual void LoadBuffers(spOneModel model) override;
    void LocationsFrom(spMyOGLShaders);
protected:

private:
        
};
using spPickingBuffLoader = std::shared_ptr<PickingBuffLoader>;
using upPickingBuffLoader = std::unique_ptr<PickingBuffLoader>;
#endif // PickingBuffLoader_H
