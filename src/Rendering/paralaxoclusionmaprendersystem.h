#ifndef ParalaxOclusionMapRenderSystem_H
#define ParalaxOclusionMapRenderSystem_H

#include <iostream>
#include <memory>
#include "rendersystem.h"

class ParalaxOclusionMapRenderSystem : public RenderSystem
{
private:

public:
    virtual bool ConfigureShadersAndLocations();
    void CreateGraphicBuffers(spOneModel model) override;
    virtual void LoadVAO(spOneModel )  override;
protected:

};
using spParalaxOclusionMapRenderSystem = std::shared_ptr<ParalaxOclusionMapRenderSystem>;
using upParalaxOclusionMapRenderSystem = std::unique_ptr<ParalaxOclusionMapRenderSystem>;
#endif // ParalaxOclusionMapRenderSystem_H
